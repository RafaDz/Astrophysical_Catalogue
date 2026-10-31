// PHYS 30762 Programming in C++
/* Graphical interface (Dear ImGui) for the Astronomical Catalogue - header.
 * The CatalogueApp class owns the three catalogues and draws the whole interface
   once per frame. It reuses the existing classes, CSV loader/saver and export
   writers, so all physics and file formats are shared with the terminal version.
 */

#ifndef CATALOGUE_GUI_H
#define CATALOGUE_GUI_H

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"

#include<functional> // for std::function
#include<memory> // for std::shared_ptr
#include<ostream> // for std::ostream
#include<string> // for std::string
#include<vector> // for std::vector

// One column of a catalogue table (also used as a row in the Compare view)
template <typename T>
struct Column
{
  const char* header; // Column title, including units
  std::function<std::string(const T&)> text; // Text shown in the cell
  std::function<double(const T&)> number; // Numeric value used for sorting (empty = sort by text)
  bool colour_yes_no = false; // Colour "Yes" green and "No" red
};

// Everything the interface needs to know about one type of object
template <typename T>
struct CatalogueView
{
  std::string label; // e.g. "Galaxy"
  std::string plural; // e.g. "Galaxies"
  std::vector<std::shared_ptr<T>>* items = nullptr; // The catalogue itself
  std::vector<std::shared_ptr<T>> display; // Sorted copy shown in the table (sorting never changes the files)
  std::vector<Column<T>> columns;
  std::function<void(std::ostream&, const T&)> write_txt;
  std::function<void(std::ostream&, const T&)> write_csv;
  std::function<void(std::ostream&)> write_csv_header;

  std::shared_ptr<T> selected; // Object highlighted in the table
  std::shared_ptr<T> compare_a; // Objects shown in the Compare tab
  std::shared_ptr<T> compare_b;
  char filter[128] = ""; // Search box text
  bool resort = true; // Rebuild the display order (after loading, adding or removing)
};

class CatalogueApp
{
  public:
    CatalogueApp();

    void draw(); // Draw the whole interface (call once per frame)

    bool has_unsaved_changes() const { return dirty; }
    void request_quit(); // Called when the window close button is pressed
    bool should_quit() const { return quit_confirmed; }
    std::string window_title() const;

  private:
    // === Data ===
    std::vector<std::shared_ptr<Galaxy>> galaxies;
    std::vector<std::shared_ptr<Star>> stars;
    std::vector<std::shared_ptr<Exoplanet>> exoplanets;

    CatalogueView<Galaxy> galaxy_view;
    CatalogueView<Star> star_view;
    CatalogueView<Exoplanet> exoplanet_view;

    bool dirty = false; // Unsaved additions/removals
    bool quit_confirmed = false;

    // === Status bar message ===
    std::string status_text;
    bool status_is_error = false;
    void set_status(const std::string& text, bool is_error = false);

    // === Tabs ===
    int compare_type = 0; // 0 = galaxies, 1 = stars, 2 = exoplanets
    bool focus_compare_tab = false;

    // === Generic yes/no confirmation dialog ===
    struct Confirm
    {
      bool open_requested = false;
      std::string title, message, ok_label;
      std::function<void()> on_ok;
    } confirm;
    void ask_confirm(const std::string& title, const std::string& message,
                     const std::string& ok_label, std::function<void()> on_ok);

    // === Export dialog ===
    struct Export
    {
      bool open_requested = false;
      std::string description; // What is being exported
      char filename[128] = "";
      int format = 0; // 0 = txt, 1 = csv
      std::function<void(std::ostream&, bool csv)> write;
    } export_dialog;
    void open_export(const std::string& description, const std::string& default_name,
                     std::function<void(std::ostream&, bool csv)> write);

    // === Add-object dialog ===
    struct AddForm
    {
      bool open_requested = false;
      int type = 0; // 0 = galaxy, 1 = star, 2 = exoplanet
      std::string error;
      char name[64] = "";
      // Galaxy
      char constellation[64] = "";
      double g_distance = 1.0, g_magnitude = 10.0, g_redshift = 0.001;
      // Star
      char s_type[64] = "Main Sequence";
      char s_spectral[16] = "G2V";
      double s_mass = 1.0, s_radius = 1.0, s_temperature = 5778.0, s_age = 4.6, s_magnitude = 5.0, s_distance = 10.0;
      // Exoplanet
      char e_type[64] = "Terrestrial";
      int e_host = 0;
      double e_mass = 1.0, e_radius = 1.0, e_axis = 1.0, e_eccentricity = 0.0;
    } add_form;
    void open_add(int type);

    // === Actions ===
    void load_from_disk();
    void save_to_disk();

    // === Drawing ===
    void draw_menu_bar();
    void draw_sidebar();
    void draw_compare_tab();
    void draw_confirm_popup();
    void draw_export_popup();
    void draw_add_popup();
    template <typename T> void draw_catalogue_tab(CatalogueView<T>& view, int type_index);
    template <typename T> void draw_table(CatalogueView<T>& view);
    template <typename T> void draw_details(CatalogueView<T>& view, int type_index);
    template <typename T> void draw_compare(CatalogueView<T>& view);
    template <typename T> void remove_object(CatalogueView<T>& view, std::shared_ptr<T> object);
};

#endif // CATALOGUE_GUI_H
