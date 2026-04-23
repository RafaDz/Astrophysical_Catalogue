// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Implementation file to handle export menu.
 * Includes functions to handle galaxy, star, and exoplanet menus.
 * Each function displays the respective catalogue and provides export options.
 * The export options include exporting the entire catalogue or individual objects.
 * The functions also handle user input and validation for export choices.
 */

#include "exoplanet.h"
#include "galaxy.h"
#include "star.h"
#include "export_exoplanet_data.h"
#include "export_galaxy_data.h"
#include "export_star_data.h"
#include "shared_functions.h"
#include "export_catalogue_menu.h"
#include "sort_menu.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<iostream> // for std::cout, std::cin
#include<string> // for std::getline

// Function to handle the galaxy menu
void handle_galaxy_menu(std::vector<std::shared_ptr<Galaxy>>& galaxy_catalogue)
{
  // Display the galaxy list
  std::cout<<"\nGalaxy list:\n";
  for(const auto& galaxy : galaxy_catalogue) galaxy->print();

  // Ask if the user wants to export the current galaxy list
  if(yes_no_handling("Would you like to export the current galaxy list? (yes/no): ")) 
  {
    export_catalogue_menu<Galaxy>(
      galaxy_catalogue,
      "Galaxy",
      write_galaxy_txt,
      write_galaxy_csv,
      write_csv_header,
      export_galaxy_to_file,
      "Fields provided: name, constellation, distance, apparent magnitude, redshift.\n"
      "Fields calculated: absolute magnitude, velocity.\n");
  }
  else // If the user chooses not to export, return to the main menu
  {
    std::cout<<"Export skipped. Returning to main menu.\n";
  }
}

// Function to handle the sort star menu
// This function will display the star list, and export options
void handle_star_menu(std::vector<std::shared_ptr<Star>>& star_catalogue)
{
  // Display the star list
  std::cout<<"\nStar list:\n";
  for(const auto& star : star_catalogue) star->print();

  // Ask if the user wants to export the current star list
  if(yes_no_handling("Would you like to export the current star list? (yes/no): ")) 
  {
    export_catalogue_menu<Star>(
      star_catalogue,
      "Star",
      write_star_txt,
      write_star_csv,
      write_star_csv_header,
      export_star_to_file,
      "Fields provided: name, type, spectral class, mass, radius, temperature, age, apparent magnitude, distance.\n"
      "Fields calculated: absolute magnitude, luminosity, surface gravity.\n");
  }
  else // If the user chooses not to export the star list
  {
    // Inform the user that export is skipped and return to the main menu
    std::cout<<"Export skipped. Returning to main menu.\n";
  }
}

// Function to handle the exoplanet menu
// This function will display the exoplanet list, and export options
void handle_exoplanet_menu(std::vector<std::shared_ptr<Exoplanet>>& exoplanet_catalogue)
{
  // Display the exoplanet list
  std::cout<<"\nExoplanet list:\n";
  for(const auto& planet : exoplanet_catalogue) planet->print();

  // Ask if the user wants to export the current exoplanet list
  if(yes_no_handling("Would you like to export the current exoplanet list? (yes/no): ")) 
  {
    export_catalogue_menu<Exoplanet>(
      exoplanet_catalogue,
      "Exoplanet",
      write_exoplanet_txt,
      write_exoplanet_csv,
      write_exoplanet_csv_header,
      export_exoplanet_to_file,
      "Fields provided: name, type, host star, mass, radius, semi-major axis, eccentricity.\n"
      "Fields calculated: orbital period, surface gravity, density, escape velocity, habitable zone.\n");
  }
  else
  {
    // If the user chooses not to export, inform them and return to the main menu
    std::cout<<"Export skipped. Returning to main menu.\n";
  }
}