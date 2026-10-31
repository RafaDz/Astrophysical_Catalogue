// PHYS 30762 Programming in C++
/* Graphical interface (Dear ImGui) for the Astronomical Catalogue - implementation.
 * Features (mirroring the terminal menu):
 *  - Catalogue summary (sidebar)
 *  - One tab per object type with a searchable table; click a column header to sort
 *  - Details of the selected object, including all calculated properties
 *  - Compare two objects of the same type side by side
 *  - Export the selected object or the whole (sorted/filtered) list to 'Exports/' as txt or csv
 *  - Add and remove objects, save changes back to 'Astro_Objects/', reload from disk
 * The object classes, CSV loader/saver and txt/csv writers are the same ones used by
   the terminal program, so results and file formats are identical.
 */

#include "catalogue_gui.h"

#include "initialize_catalogue.h"
#include "save_catalogues.h"
#include "shared_functions.h"
#include "export_galaxy_data.h"
#include "export_star_data.h"
#include "export_exoplanet_data.h"

#include "imgui.h"

#include<algorithm> // for std::sort, std::find, std::count_if
#include<cctype> // for std::tolower
#include<cmath> // for std::isnan
#include<cstdio> // for std::snprintf
#include<filesystem> // for std::filesystem
#include<fstream> // for std::ofstream
#include<limits> // for std::numeric_limits
#include<sstream> // for std::ostringstream
#include<stdexcept> // for std::exception
#include<type_traits> // for std::is_same_v

namespace
{
  const ImVec4 colour_good(0.45f, 0.85f, 0.45f, 1.0f);
  const ImVec4 colour_bad(0.95f, 0.45f, 0.40f, 1.0f);
  const ImVec4 colour_muted(0.60f, 0.62f, 0.68f, 1.0f);
  const ImVec4 colour_accent(0.55f, 0.75f, 1.0f, 1.0f);

  const double not_available = std::numeric_limits<double>::quiet_NaN();

  // Format a number compactly (4 significant figures, scientific for very large/small values)
  std::string format_number(double value)
  {
    if(std::isnan(value)) return "N/A";
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%.4g", value);
    return buffer;
  }

  std::string to_lower(std::string text)
  {
    std::transform(text.begin(), text.end(), text.begin(),
      [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
  }

  bool contains_case_insensitive(const std::string& text, const std::string& search)
  {
    return to_lower(text).find(to_lower(search)) != std::string::npos;
  }

  // Turn an object name into a valid export filename (e.g. "Messier 31" -> "Messier_31")
  std::string suggested_filename(const std::string& name)
  {
    std::string result;
    for(char c : name)
      result += (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_') ? c : '_';
    return result.empty() ? "export" : result;
  }

  // Copy a std::string into a fixed-size char buffer used by ImGui text inputs
  template <size_t N>
  void copy_to_buffer(char (&buffer)[N], const std::string& text)
  {
    std::snprintf(buffer, N, "%s", text.c_str());
  }

  // Habitable zone values throw if the host star is missing, so wrap them
  double habitable_inner(const Exoplanet& e) { try { return e.habitable_zone().first; } catch(...) { return not_available; } }
  double habitable_outer(const Exoplanet& e) { try { return e.habitable_zone().second; } catch(...) { return not_available; } }
  std::string habitable_text(const Exoplanet& e)
  {
    try { return e.is_in_habitable_zone() ? "Yes" : "No"; } catch(...) { return "N/A"; }
  }

  // Helpers to build table columns
  template <typename T>
  Column<T> text_column(const char* header, std::function<std::string(const T&)> text)
  {
    return Column<T>{header, std::move(text), nullptr};
  }

  template <typename T>
  Column<T> number_column(const char* header, std::function<double(const T&)> number)
  {
    return Column<T>{header, [number](const T& object) { return format_number(number(object)); }, number};
  }

  // Draw a cell's text, colouring Yes/No if requested
  template <typename T>
  void draw_cell(const Column<T>& column, const T& object)
  {
    std::string text = column.text(object);
    if(column.colour_yes_no && text == "Yes")
      ImGui::TextColored(colour_good, "%s", text.c_str());
    else if(column.colour_yes_no && text == "No")
      ImGui::TextColored(colour_bad, "%s", text.c_str());
    else
      ImGui::TextUnformatted(text.c_str());
  }

  // Check a text field before storing it in a CSV file
  bool valid_text_field(const char* text)
  {
    std::string value(text);
    return !value.empty() && value.find(',') == std::string::npos;
  }

  template <typename T>
  bool name_taken(const std::vector<std::shared_ptr<T>>& items, const std::string& name)
  {
    return std::any_of(items.begin(), items.end(),
      [&](const std::shared_ptr<T>& item) { return to_lower(item->get_name()) == to_lower(name); });
  }
}

// ============================================================================
// Construction, loading and saving
// ============================================================================

CatalogueApp::CatalogueApp()
{
  // === Galaxies ===
  galaxy_view.label = "Galaxy";
  galaxy_view.plural = "Galaxies";
  galaxy_view.items = &galaxies;
  galaxy_view.write_txt = write_galaxy_txt;
  galaxy_view.write_csv = write_galaxy_csv;
  galaxy_view.write_csv_header = ::write_csv_header;
  galaxy_view.columns = {
    text_column<Galaxy>("Name", [](const Galaxy& g) { return g.get_name(); }),
    text_column<Galaxy>("Constellation", [](const Galaxy& g) { return g.get_constellation(); }),
    number_column<Galaxy>("Distance [Mpc]", [](const Galaxy& g) { return g.get_distance(); }),
    number_column<Galaxy>("Apparent mag", [](const Galaxy& g) { return g.get_apparent_magnitude(); }),
    number_column<Galaxy>("Redshift z", [](const Galaxy& g) { return g.get_redshift(); }),
    number_column<Galaxy>("Absolute mag", [](const Galaxy& g) { return g.absolute_magnitude(); }),
    number_column<Galaxy>("Velocity [km/s]", [](const Galaxy& g) { return g.velocity(); }),
  };

  // === Stars ===
  star_view.label = "Star";
  star_view.plural = "Stars";
  star_view.items = &stars;
  star_view.write_txt = write_star_txt;
  star_view.write_csv = write_star_csv;
  star_view.write_csv_header = write_star_csv_header;
  star_view.columns = {
    text_column<Star>("Name", [](const Star& s) { return s.get_name(); }),
    text_column<Star>("Type", [](const Star& s) { return s.get_type(); }),
    text_column<Star>("Spectral class", [](const Star& s) { return s.get_spectral_class(); }),
    number_column<Star>("Mass [M_Sun]", [](const Star& s) { return s.get_mass(); }),
    number_column<Star>("Radius [R_Sun]", [](const Star& s) { return s.get_radius(); }),
    number_column<Star>("Temperature [K]", [](const Star& s) { return s.get_temperature(); }),
    number_column<Star>("Age [Gyr]", [](const Star& s) { return s.get_age(); }),
    number_column<Star>("Apparent mag", [](const Star& s) { return s.get_apparent_magnitude(); }),
    number_column<Star>("Distance [Mpc]", [](const Star& s) { return s.get_distance(); }),
    number_column<Star>("Luminosity [L_Sun]", [](const Star& s) { return s.luminosity(); }),
    number_column<Star>("Absolute mag", [](const Star& s) { return s.absolute_magnitude(); }),
    number_column<Star>("Surface gravity [m/s^2]", [](const Star& s) { return s.gravitational_strength(); }),
  };

  // === Exoplanets ===
  exoplanet_view.label = "Exoplanet";
  exoplanet_view.plural = "Exoplanets";
  exoplanet_view.items = &exoplanets;
  exoplanet_view.write_txt = write_exoplanet_txt;
  exoplanet_view.write_csv = write_exoplanet_csv;
  exoplanet_view.write_csv_header = write_exoplanet_csv_header;
  Column<Exoplanet> habitable_column = text_column<Exoplanet>("In habitable zone", habitable_text);
  habitable_column.colour_yes_no = true;
  exoplanet_view.columns = {
    text_column<Exoplanet>("Name", [](const Exoplanet& e) { return e.get_name(); }),
    text_column<Exoplanet>("Type", [](const Exoplanet& e) { return e.get_type(); }),
    text_column<Exoplanet>("Host star", [](const Exoplanet& e) { return e.get_host_star_name(); }),
    number_column<Exoplanet>("Mass [M_Earth]", [](const Exoplanet& e) { return e.get_planet_mass(); }),
    number_column<Exoplanet>("Radius [R_Earth]", [](const Exoplanet& e) { return e.get_planet_radius(); }),
    number_column<Exoplanet>("Semi-major axis [AU]", [](const Exoplanet& e) { return e.get_semi_major_axis(); }),
    number_column<Exoplanet>("Eccentricity", [](const Exoplanet& e) { return e.get_eccentricity(); }),
    number_column<Exoplanet>("Orbital period [yr]", [](const Exoplanet& e) { return e.orbital_period(); }),
    number_column<Exoplanet>("Surface gravity [m/s^2]", [](const Exoplanet& e) { return e.surface_gravity(); }),
    number_column<Exoplanet>("Density [kg/m^3]", [](const Exoplanet& e) { return e.density(); }),
    number_column<Exoplanet>("Escape velocity [m/s]", [](const Exoplanet& e) { return e.escape_velocity(); }),
    number_column<Exoplanet>("HZ inner [AU]", habitable_inner),
    number_column<Exoplanet>("HZ outer [AU]", habitable_outer),
    habitable_column,
  };

  load_from_disk();
}

void CatalogueApp::load_from_disk()
{
  galaxy_view.selected = galaxy_view.compare_a = galaxy_view.compare_b = nullptr;
  star_view.selected = star_view.compare_a = star_view.compare_b = nullptr;
  exoplanet_view.selected = exoplanet_view.compare_a = exoplanet_view.compare_b = nullptr;
  galaxies.clear();
  exoplanets.clear(); // Exoplanets hold pointers to stars, so clear them first
  stars.clear();

  load_catalogues(galaxies, stars, exoplanets); // Same loader as the terminal version
  galaxy_view.resort = star_view.resort = exoplanet_view.resort = true;
  dirty = false;

  set_status("Loaded " + std::to_string(galaxies.size()) + " galaxies, " + std::to_string(stars.size()) +
             " stars and " + std::to_string(exoplanets.size()) + " exoplanets from 'Astro_Objects/'.",
             galaxies.empty() && stars.empty() && exoplanets.empty());
}

void CatalogueApp::save_to_disk()
{
  save_all_catalogues(galaxies, stars, exoplanets); // Same saver as the terminal version
  dirty = false;
  set_status("Changes saved to 'Astro_Objects/'.");
}

void CatalogueApp::set_status(const std::string& text, bool is_error)
{
  status_text = text;
  status_is_error = is_error;
}

std::string CatalogueApp::window_title() const
{
  return std::string("Astronomical Catalogue") + (dirty ? " *" : "");
}

void CatalogueApp::request_quit()
{
  if(!dirty)
  {
    quit_confirmed = true;
    return;
  }
  ask_confirm("Quit", "You have unsaved changes. Quit without saving?", "Quit without saving",
              [this] { quit_confirmed = true; });
}

void CatalogueApp::ask_confirm(const std::string& title, const std::string& message,
                               const std::string& ok_label, std::function<void()> on_ok)
{
  confirm.title = title;
  confirm.message = message;
  confirm.ok_label = ok_label;
  confirm.on_ok = std::move(on_ok);
  confirm.open_requested = true;
}

void CatalogueApp::open_export(const std::string& description, const std::string& default_name,
                               std::function<void(std::ostream&, bool csv)> write)
{
  export_dialog.description = description;
  copy_to_buffer(export_dialog.filename, suggested_filename(default_name));
  export_dialog.write = std::move(write);
  export_dialog.open_requested = true;
}

void CatalogueApp::open_add(int type)
{
  add_form.type = type;
  add_form.error.clear();
  add_form.name[0] = '\0';
  add_form.open_requested = true;
}

template <typename T>
void CatalogueApp::remove_object(CatalogueView<T>& view, std::shared_ptr<T> object)
{
  auto& items = *view.items;
  items.erase(std::remove(items.begin(), items.end(), object), items.end());
  if(view.selected == object) view.selected = nullptr;
  if(view.compare_a == object) view.compare_a = nullptr;
  if(view.compare_b == object) view.compare_b = nullptr;
  view.resort = true;
  dirty = true;
  set_status("Removed " + to_lower(view.label) + " '" + object->get_name() + "'. Remember to save your changes.");
}

// ============================================================================
// Main frame
// ============================================================================

void CatalogueApp::draw()
{
  // Keyboard shortcut: Ctrl+S to save
  if(ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S, ImGuiInputFlags_RouteGlobal))
    save_to_disk();

  // One window filling the whole application window
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::Begin("Astronomical Catalogue", nullptr,
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

  draw_menu_bar();

  const float status_height = ImGui::GetFrameHeightWithSpacing();

  // === Left: summary and actions ===
  ImGui::BeginChild("Sidebar", ImVec2(250.0f, -status_height), ImGuiChildFlags_Borders);
  draw_sidebar();
  ImGui::EndChild();

  ImGui::SameLine();

  // === Right: tabs ===
  ImGui::BeginChild("Main", ImVec2(0.0f, -status_height));
  if(ImGui::BeginTabBar("CatalogueTabs"))
  {
    if(ImGui::BeginTabItem("Galaxies"))
    {
      draw_catalogue_tab(galaxy_view, 0);
      ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Stars"))
    {
      draw_catalogue_tab(star_view, 1);
      ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Exoplanets"))
    {
      draw_catalogue_tab(exoplanet_view, 2);
      ImGui::EndTabItem();
    }
    ImGuiTabItemFlags compare_flags = focus_compare_tab ? ImGuiTabItemFlags_SetSelected : 0;
    focus_compare_tab = false;
    if(ImGui::BeginTabItem("Compare", nullptr, compare_flags))
    {
      draw_compare_tab();
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::EndChild();

  // === Status bar ===
  ImGui::Separator();
  ImGui::TextColored(status_is_error ? colour_bad : colour_muted, "%s", status_text.c_str());

  // === Dialogs ===
  draw_confirm_popup();
  draw_export_popup();
  draw_add_popup();

  ImGui::End();
}

void CatalogueApp::draw_menu_bar()
{
  if(!ImGui::BeginMenuBar()) return;

  if(ImGui::BeginMenu("File"))
  {
    if(ImGui::MenuItem("Save changes", "Ctrl+S", false, true))
      save_to_disk();
    if(ImGui::MenuItem("Reload from disk"))
    {
      if(dirty)
        ask_confirm("Reload", "Discard unsaved changes and reload the CSV files?", "Reload",
                    [this] { load_from_disk(); });
      else
        load_from_disk();
    }
    ImGui::Separator();
    if(ImGui::MenuItem("Quit"))
      request_quit();
    ImGui::EndMenu();
  }

  if(ImGui::BeginMenu("Add"))
  {
    if(ImGui::MenuItem("Galaxy...")) open_add(0);
    if(ImGui::MenuItem("Star...")) open_add(1);
    if(ImGui::MenuItem("Exoplanet...")) open_add(2);
    ImGui::EndMenu();
  }

  ImGui::EndMenuBar();
}

void CatalogueApp::draw_sidebar()
{
  ImGui::TextColored(colour_accent, "Catalogue Summary");
  ImGui::Separator();

  if(ImGui::BeginTable("Summary", 2, ImGuiTableFlags_SizingStretchProp))
  {
    auto row = [](const char* label, size_t count)
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn(); ImGui::TextUnformatted(label);
      ImGui::TableNextColumn(); ImGui::Text("%zu", count);
    };
    row("Galaxies", galaxies.size());
    row("Stars", stars.size());
    row("Exoplanets", exoplanets.size());
    row("Total", galaxies.size() + stars.size() + exoplanets.size());
    ImGui::EndTable();
  }

  size_t habitable = static_cast<size_t>(std::count_if(exoplanets.begin(), exoplanets.end(),
    [](const std::shared_ptr<Exoplanet>& e) { return habitable_text(*e) == "Yes"; }));
  ImGui::Spacing();
  ImGui::PushStyleColor(ImGuiCol_Text, colour_muted);
  ImGui::TextWrapped("%zu of %zu exoplanets lie in their star's habitable zone.", habitable, exoplanets.size());
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::TextColored(colour_accent, "Actions");
  ImGui::Separator();

  const ImVec2 full_width(-FLT_MIN, 0.0f);
  if(ImGui::Button("Add galaxy...", full_width)) open_add(0);
  if(ImGui::Button("Add star...", full_width)) open_add(1);
  if(ImGui::Button("Add exoplanet...", full_width)) open_add(2);
  ImGui::Spacing();

  if(!dirty) ImGui::BeginDisabled();
  if(ImGui::Button("Save changes", full_width)) save_to_disk();
  if(!dirty) ImGui::EndDisabled();

  ImGui::Spacing();
  if(dirty)
    ImGui::TextColored(colour_bad, "Unsaved changes");
  else
    ImGui::TextColored(colour_good, "All changes saved");

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::TextColored(colour_accent, "Tips");
  ImGui::Separator();
  ImGui::PushStyleColor(ImGuiCol_Text, colour_muted);
  ImGui::TextWrapped("Click a column header to sort (click again to reverse, a third time to restore the file order).");
  ImGui::TextWrapped("Click a row to see full details, export it or compare it with another object.");
  ImGui::TextWrapped("Exports are written to the 'Exports' folder.");
  ImGui::PopStyleColor();
}

// ============================================================================
// Catalogue tabs (one template shared by galaxies, stars and exoplanets)
// ============================================================================

template <typename T>
void CatalogueApp::draw_catalogue_tab(CatalogueView<T>& view, int type_index)
{
  // === Toolbar ===
  ImGui::SetNextItemWidth(260.0f);
  ImGui::InputTextWithHint("##filter", ("Search " + to_lower(view.plural) + "...").c_str(),
                           view.filter, sizeof(view.filter));
  ImGui::SameLine();
  if(ImGui::Button(("Add " + to_lower(view.label) + "...").c_str()))
    open_add(type_index);
  ImGui::SameLine();
  if(ImGui::Button("Export list..."))
  {
    // Export exactly what is shown: current sort order and search filter
    std::string filter = view.filter;
    std::vector<std::shared_ptr<T>> shown;
    for(const auto& item : view.display)
    {
      bool match = filter.empty();
      for(const auto& column : view.columns)
        if(!column.number && contains_case_insensitive(column.text(*item), filter))
          match = true;
      if(match) shown.push_back(item);
    }
    auto write_txt = view.write_txt;
    auto write_csv = view.write_csv;
    auto write_header = view.write_csv_header;
    open_export(std::to_string(shown.size()) + " " + to_lower(view.plural) + " (as currently sorted/filtered)",
                to_lower(view.plural),
                [=](std::ostream& out, bool csv)
                {
                  if(csv && write_header) write_header(out);
                  for(const auto& item : shown)
                  {
                    if(csv) write_csv(out, *item);
                    else { write_txt(out, *item); out<<"\n"; }
                  }
                });
  }

  // === Table (top) and details (bottom) ===
  const float details_height = 280.0f;
  ImGui::BeginChild("TableArea", ImVec2(0.0f, -details_height));
  draw_table(view);
  ImGui::EndChild();

  ImGui::BeginChild("Details", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders);
  draw_details(view, type_index);
  ImGui::EndChild();
}

template <typename T>
void CatalogueApp::draw_table(CatalogueView<T>& view)
{
  auto& items = *view.items;
  const int column_count = static_cast<int>(view.columns.size());

  ImGuiTableFlags flags = ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate | ImGuiTableFlags_RowBg |
                          ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
                          ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollX |
                          ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit;

  if(!ImGui::BeginTable(view.plural.c_str(), column_count, flags))
    return;

  ImGui::TableSetupScrollFreeze(1, 1); // Keep the name column and header visible while scrolling
  for(int i = 0; i < column_count; ++i)
  {
    ImGuiTableColumnFlags column_flags = (i == 0) ? ImGuiTableColumnFlags_NoHide : 0;
    ImGui::TableSetupColumn(view.columns[i].header, column_flags);
  }
  ImGui::TableHeadersRow();

  // === Sorting: only changes the order shown, not the catalogue or the CSV files ===
  ImGuiTableSortSpecs* specs = ImGui::TableGetSortSpecs();
  if(view.resort || (specs && specs->SpecsDirty))
  {
    view.display = items; // Original (file) order
    if(specs && specs->SpecsCount > 0)
    {
      const ImGuiTableColumnSortSpecs& spec = specs->Specs[0];
      const Column<T>& column = view.columns[spec.ColumnIndex];
      const bool ascending = spec.SortDirection == ImGuiSortDirection_Ascending;

      std::stable_sort(view.display.begin(), view.display.end(),
        [&](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b)
        {
          if(column.number)
          {
            double x = column.number(*a), y = column.number(*b);
            if(std::isnan(x) || std::isnan(y)) return !std::isnan(x) && std::isnan(y); // N/A always last
            return ascending ? x < y : x > y;
          }
          std::string x = to_lower(column.text(*a)), y = to_lower(column.text(*b));
          return ascending ? x < y : x > y;
        });
    }
    if(specs) specs->SpecsDirty = false;
    view.resort = false;
  }

  // === Rows ===
  const std::string filter = view.filter;
  std::shared_ptr<T> clicked;
  for(const auto& item : view.display)
  {
    if(!filter.empty())
    {
      bool match = false;
      for(const auto& column : view.columns)
        if(!column.number && contains_case_insensitive(column.text(*item), filter))
          match = true;
      if(!match) continue;
    }

    ImGui::TableNextRow();
    ImGui::PushID(item.get());
    for(int i = 0; i < column_count; ++i)
    {
      if(!ImGui::TableSetColumnIndex(i)) continue;
      if(i == 0)
      {
        // The whole row is clickable
        if(ImGui::Selectable(view.columns[0].text(*item).c_str(), view.selected == item,
                             ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap))
          clicked = item;
      }
      else
      {
        draw_cell(view.columns[i], *item);
      }
    }
    ImGui::PopID();
  }
  if(clicked) view.selected = clicked;

  if(items.empty())
  {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextColored(colour_muted, "No %s in the catalogue.", to_lower(view.plural).c_str());
  }

  ImGui::EndTable();
}

template <typename T>
void CatalogueApp::draw_details(CatalogueView<T>& view, int type_index)
{
  if(!view.selected)
  {
    ImGui::TextColored(colour_muted, "Select a %s in the table above to see its details.",
                       to_lower(view.label).c_str());
    return;
  }

  std::shared_ptr<T> object = view.selected;

  // === Action buttons ===
  ImGui::TextColored(colour_accent, "%s", object->get_name().c_str());
  ImGui::SameLine();
  ImGui::TextColored(colour_muted, "(%s)", view.label.c_str());

  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 330.0f + ImGui::GetCursorPosX());
  if(ImGui::Button("Export..."))
  {
    auto write_txt = view.write_txt;
    auto write_csv = view.write_csv;
    auto write_header = view.write_csv_header;
    open_export(view.label + " '" + object->get_name() + "'", object->get_name(),
                [=](std::ostream& out, bool csv)
                {
                  if(csv) { if(write_header) write_header(out); write_csv(out, *object); }
                  else write_txt(out, *object);
                });
  }
  ImGui::SameLine();
  if(ImGui::Button("Compare with..."))
  {
    view.compare_a = object;
    if(view.compare_b == object) view.compare_b = nullptr;
    compare_type = type_index;
    focus_compare_tab = true;
  }
  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.18f, 0.18f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.22f, 0.22f, 1.0f));
  if(ImGui::Button("Remove..."))
  {
    std::string message = "Remove " + to_lower(view.label) + " '" + object->get_name() + "' from the catalogue?";
    if constexpr(std::is_same_v<T, Star>)
    {
      auto orbiting = std::count_if(exoplanets.begin(), exoplanets.end(),
        [&](const std::shared_ptr<Exoplanet>& e) { return e->get_host_star() == object; });
      if(orbiting > 0)
        message += "\n\n" + std::to_string(orbiting) + " exoplanet(s) orbit this star. They keep their data for now,"
                   "\nbut after saving and reloading their host star will show as N/A.";
    }
    ask_confirm("Remove " + to_lower(view.label), message, "Remove",
                [this, &view, object] { remove_object(view, object); });
  }
  ImGui::PopStyleColor(2);

  ImGui::Separator();

  // === Full details, using the same text as the txt export ===
  std::ostringstream details;
  view.write_txt(details, *object);
  ImGui::BeginChild("DetailsText");
  ImGui::TextUnformatted(details.str().c_str());
  ImGui::EndChild();
}

// ============================================================================
// Compare tab
// ============================================================================

void CatalogueApp::draw_compare_tab()
{
  ImGui::TextColored(colour_muted, "Compare two objects of the same type side by side.");
  ImGui::RadioButton("Galaxies", &compare_type, 0); ImGui::SameLine();
  ImGui::RadioButton("Stars", &compare_type, 1); ImGui::SameLine();
  ImGui::RadioButton("Exoplanets", &compare_type, 2);
  ImGui::Separator();

  if(compare_type == 0) draw_compare(galaxy_view);
  else if(compare_type == 1) draw_compare(star_view);
  else draw_compare(exoplanet_view);
}

template <typename T>
void CatalogueApp::draw_compare(CatalogueView<T>& view)
{
  auto& items = *view.items;
  if(items.size() < 2)
  {
    ImGui::TextColored(colour_muted, "At least two %s are needed to compare.", to_lower(view.plural).c_str());
    return;
  }

  // Drop references to objects that are no longer in the catalogue
  auto in_catalogue = [&](const std::shared_ptr<T>& p) { return std::find(items.begin(), items.end(), p) != items.end(); };
  if(view.compare_a && !in_catalogue(view.compare_a)) view.compare_a = nullptr;
  if(view.compare_b && !in_catalogue(view.compare_b)) view.compare_b = nullptr;

  auto picker = [&](const char* id, std::shared_ptr<T>& choice)
  {
    ImGui::SetNextItemWidth(260.0f);
    if(ImGui::BeginCombo(id, choice ? choice->get_name().c_str() : "Choose..."))
    {
      for(const auto& item : items)
      {
        ImGui::PushID(item.get());
        if(ImGui::Selectable(item->get_name().c_str(), choice == item))
          choice = item;
        ImGui::PopID();
      }
      ImGui::EndCombo();
    }
  };
  picker("First##compare", view.compare_a);
  ImGui::SameLine();
  picker("Second##compare", view.compare_b);
  ImGui::SameLine();
  if(ImGui::Button("Swap")) std::swap(view.compare_a, view.compare_b);

  if(!view.compare_a || !view.compare_b)
  {
    ImGui::TextColored(colour_muted, "Choose two %s.", to_lower(view.plural).c_str());
    return;
  }

  const T& a = *view.compare_a;
  const T& b = *view.compare_b;
  if(ImGui::BeginTable("CompareTable", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInner |
                       ImGuiTableFlags_BordersOuter | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp))
  {
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthStretch, 1.4f);
    ImGui::TableSetupColumn(a.get_name().c_str(), ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn(b.get_name().c_str(), ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn("Ratio (first / second)", ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableHeadersRow();

    for(size_t i = 1; i < view.columns.size(); ++i) // Skip "Name": the names are the column headers
    {
      const Column<T>& column = view.columns[i];
      ImGui::TableNextRow();
      ImGui::TableNextColumn(); ImGui::TextColored(colour_muted, "%s", column.header);
      ImGui::TableNextColumn(); draw_cell(column, a);
      ImGui::TableNextColumn(); draw_cell(column, b);
      ImGui::TableNextColumn();
      if(column.number)
      {
        double x = column.number(a), y = column.number(b);
        if(!std::isnan(x) && !std::isnan(y) && y != 0.0)
          ImGui::TextUnformatted(format_number(x / y).c_str());
        else
          ImGui::TextColored(colour_muted, "-");
      }
      else
      {
        ImGui::TextColored(colour_muted, "-");
      }
    }
    ImGui::EndTable();
  }
}

// ============================================================================
// Dialogs
// ============================================================================

void CatalogueApp::draw_confirm_popup()
{
  if(confirm.open_requested)
  {
    ImGui::OpenPopup("Confirm##popup");
    confirm.open_requested = false;
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if(ImGui::BeginPopupModal("Confirm##popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::TextColored(colour_accent, "%s", confirm.title.c_str());
    ImGui::Separator();
    ImGui::TextUnformatted(confirm.message.c_str());
    ImGui::Spacing();
    if(ImGui::Button(confirm.ok_label.c_str()))
    {
      if(confirm.on_ok) confirm.on_ok();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
      ImGui::CloseCurrentPopup();
    ImGui::EndPopup();
  }
}

void CatalogueApp::draw_export_popup()
{
  if(export_dialog.open_requested)
  {
    ImGui::OpenPopup("Export##popup");
    export_dialog.open_requested = false;
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if(ImGui::BeginPopupModal("Export##popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Export %s", export_dialog.description.c_str());
    ImGui::Separator();

    ImGui::SetNextItemWidth(260.0f);
    ImGui::InputText("File name", export_dialog.filename, sizeof(export_dialog.filename));
    ImGui::RadioButton("txt", &export_dialog.format, 0); ImGui::SameLine();
    ImGui::RadioButton("csv", &export_dialog.format, 1);

    const std::string extension = export_dialog.format == 0 ? "txt" : "csv";
    const std::string path = "Exports/" + std::string(export_dialog.filename) + "." + extension;
    const bool valid = is_valid_filename(export_dialog.filename);

    if(valid)
    {
      ImGui::TextColored(colour_muted, "Will be saved as %s", path.c_str());
      if(std::filesystem::exists(path))
        ImGui::TextColored(colour_bad, "A file with this name already exists and will be overwritten.");
    }
    else
    {
      ImGui::TextColored(colour_bad, "Invalid file name. Avoid special characters like . , ! ? : ; / \\");
    }

    ImGui::Spacing();
    if(!valid) ImGui::BeginDisabled();
    if(ImGui::Button("Export"))
    {
      std::filesystem::create_directories("Exports");
      std::ofstream out(path);
      if(out)
      {
        export_dialog.write(out, export_dialog.format == 1);
        set_status("Exported to " + path + ".");
      }
      else
      {
        set_status("Could not open " + path + " for writing.", true);
      }
      ImGui::CloseCurrentPopup();
    }
    if(!valid) ImGui::EndDisabled();
    ImGui::SameLine();
    if(ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
      ImGui::CloseCurrentPopup();
    ImGui::EndPopup();
  }
}

void CatalogueApp::draw_add_popup()
{
  if(add_form.open_requested)
  {
    ImGui::OpenPopup("Add object##popup");
    add_form.open_requested = false;
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if(!ImGui::BeginPopupModal("Add object##popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    return;

  AddForm& f = add_form;
  ImGui::RadioButton("Galaxy", &f.type, 0); ImGui::SameLine();
  ImGui::RadioButton("Star", &f.type, 1); ImGui::SameLine();
  ImGui::RadioButton("Exoplanet", &f.type, 2);
  ImGui::Separator();

  ImGui::PushItemWidth(220.0f);
  if(ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();
  ImGui::InputText("Name", f.name, sizeof(f.name));

  if(f.type == 0)
  {
    ImGui::InputText("Constellation", f.constellation, sizeof(f.constellation));
    ImGui::InputDouble("Distance [Mpc]", &f.g_distance, 0.0, 0.0, "%g");
    ImGui::InputDouble("Apparent magnitude", &f.g_magnitude, 0.0, 0.0, "%g");
    ImGui::InputDouble("Redshift z", &f.g_redshift, 0.0, 0.0, "%g");
  }
  else if(f.type == 1)
  {
    ImGui::InputText("Type", f.s_type, sizeof(f.s_type));
    ImGui::InputText("Spectral class", f.s_spectral, sizeof(f.s_spectral));
    ImGui::InputDouble("Mass [M_Sun]", &f.s_mass, 0.0, 0.0, "%g");
    ImGui::InputDouble("Radius [R_Sun]", &f.s_radius, 0.0, 0.0, "%g");
    ImGui::InputDouble("Temperature [K]", &f.s_temperature, 0.0, 0.0, "%g");
    ImGui::InputDouble("Age [Gyr]", &f.s_age, 0.0, 0.0, "%g");
    ImGui::InputDouble("Apparent magnitude", &f.s_magnitude, 0.0, 0.0, "%g");
    ImGui::InputDouble("Distance [Mpc]", &f.s_distance, 0.0, 0.0, "%g");
  }
  else
  {
    ImGui::InputText("Type", f.e_type, sizeof(f.e_type));
    if(stars.empty())
    {
      ImGui::TextColored(colour_bad, "Add a star first: exoplanets need a host star.");
    }
    else
    {
      f.e_host = std::clamp(f.e_host, 0, static_cast<int>(stars.size()) - 1);
      if(ImGui::BeginCombo("Host star", stars[f.e_host]->get_name().c_str()))
      {
        for(int i = 0; i < static_cast<int>(stars.size()); ++i)
        {
          ImGui::PushID(i);
          if(ImGui::Selectable(stars[i]->get_name().c_str(), f.e_host == i)) f.e_host = i;
          ImGui::PopID();
        }
        ImGui::EndCombo();
      }
    }
    ImGui::InputDouble("Mass [M_Earth]", &f.e_mass, 0.0, 0.0, "%g");
    ImGui::InputDouble("Radius [R_Earth]", &f.e_radius, 0.0, 0.0, "%g");
    ImGui::InputDouble("Semi-major axis [AU]", &f.e_axis, 0.0, 0.0, "%g");
    ImGui::InputDouble("Eccentricity", &f.e_eccentricity, 0.0, 0.0, "%g");
  }
  ImGui::PopItemWidth();

  if(!f.error.empty())
  {
    ImGui::Spacing();
    ImGui::TextColored(colour_bad, "%s", f.error.c_str());
  }

  ImGui::Spacing();
  if(ImGui::Button("Add"))
  {
    // Validate text fields here; the class constructors validate the numbers
    f.error.clear();
    const std::string name = f.name;
    bool texts_ok = valid_text_field(f.name);
    if(f.type == 0) texts_ok = texts_ok && valid_text_field(f.constellation);
    if(f.type == 1) texts_ok = texts_ok && valid_text_field(f.s_type) && valid_text_field(f.s_spectral);
    if(f.type == 2) texts_ok = texts_ok && valid_text_field(f.e_type);

    if(!texts_ok)
      f.error = "Text fields must not be empty or contain commas.";
    else if((f.type == 0 && name_taken(galaxies, name)) || (f.type == 1 && name_taken(stars, name)) ||
            (f.type == 2 && name_taken(exoplanets, name)))
      f.error = "An object with this name already exists.";
    else if(f.type == 2 && stars.empty())
      f.error = "Exoplanets need a host star.";
    else
    {
      try
      {
        if(f.type == 0)
        {
          auto galaxy = std::make_shared<Galaxy>(name, f.constellation, f.g_distance, f.g_magnitude, f.g_redshift);
          galaxies.push_back(galaxy);
          galaxy_view.selected = galaxy;
          galaxy_view.resort = true;
        }
        else if(f.type == 1)
        {
          auto star = std::make_shared<Star>(name, f.s_type, f.s_spectral, f.s_mass, f.s_radius, f.s_temperature,
                                             f.s_age, f.s_magnitude, f.s_distance);
          stars.push_back(star);
          star_view.selected = star;
          star_view.resort = true;
        }
        else
        {
          auto planet = std::make_shared<Exoplanet>(name, f.e_type, stars[f.e_host], f.e_mass, f.e_radius,
                                                    f.e_axis, f.e_eccentricity);
          exoplanets.push_back(planet);
          exoplanet_view.selected = planet;
          exoplanet_view.resort = true;
        }
        dirty = true;
        const char* type_names[] = {"galaxy", "star", "exoplanet"};
        set_status("Added " + std::string(type_names[f.type]) + " '" + name + "'. Remember to save your changes.");
        ImGui::CloseCurrentPopup();
      }
      catch(const std::exception& error)
      {
        f.error = error.what(); // e.g. "Mass must be positive."
      }
    }
  }
  ImGui::SameLine();
  if(ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
    ImGui::CloseCurrentPopup();

  ImGui::EndPopup();
}
