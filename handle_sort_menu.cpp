// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Implementation file to handle sorting menu.
 * Includes functions to handle sorting for galaxy, star, and exoplanet menus.
 * Each function displays the respective catalogue and provides sorting options.
 * The sorting options include sorting by various properties.
 * The functions also handle user input and validation for sorting choices.
 * The functions also provide options to export the sorted catalogue.
 */

#include "star.h"
#include "galaxy.h"
#include "exoplanet.h"
#include "handle_sort_menu.h"
#include "export_star_data.h"
#include "export_galaxy_data.h"
#include "export_exoplanet_data.h"
#include "shared_functions.h"
#include "export_catalogue_menu.h"
#include "sort_menu.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<iostream> // for std::cout, std::cin
#include<string> // for std::getline

// Function to handle the sort galaxy menu
void handle_sort_galaxy_menu(std::vector<std::shared_ptr<Galaxy>>& galaxy_catalogue)
{
  // Ask if the user wants to sort the galaxy list
  if(yes_no_handling("Would you like to sort the galaxy list? (yes/no): ")) 
  {
    bool keep_sorting = true;
    while (keep_sorting) 
    {
      galaxy_sort_menu(galaxy_catalogue);
      std::cout<<"\nSorted galaxy list:\n";
      for (const auto& galaxy : galaxy_catalogue) galaxy->print();
      keep_sorting = yes_no_handling("Would you like to sort by another property? (yes/no): ");
    }

    // After sorting is finished, ask if the user wants to export the sorted galaxy list
    if(yes_no_handling("Would you like to export the sorted galaxy list? (yes/no): ")) 
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
  }
  else // If the user chooses not to sort, return to the main menu
  {
    std::cout<<"Sorting skipped. Returning to main menu.\n";
  }
}

// Function to handle the sort star menu
void handle_sort_star_menu(std::vector<std::shared_ptr<Star>>& star_catalogue)
{
  // Ask if the user wants to sort the star list
  if(yes_no_handling("Would you like to sort the star list? (yes/no): ")) 
  {
    bool keep_sorting = true;
    while(keep_sorting) 
    {
      star_sort_menu(star_catalogue);
      std::cout<<"\nSorted star list:\n";
      for (const auto& star : star_catalogue) star->print();
      keep_sorting = yes_no_handling("Would you like to sort by another property? (yes/no): ");
    }

    // After sorting is finished, ask if the user wants to export the sorted star list
    if(yes_no_handling("Would you like to export the sorted star list? (yes/no): ")) 
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
  } 
  else // If the user chooses not to sort the star list
  {
    // Inform the user that sorting is skipped and return to the main menu
    std::cout<<"Sorting skipped. Returning to main menu.\n";
  }
}

// Function to handle the sort exoplanet menu
void handle_sort_exoplanet_menu(std::vector<std::shared_ptr<Exoplanet>>& exoplanet_catalogue)
{
  // Ask if the user wants to sort the exoplanet list
  if(yes_no_handling("Would you like to sort the exoplanet list? (yes/no): ")) 
  {
    bool keep_sorting = true;
    while(keep_sorting)
    {
      exoplanet_sort_menu(exoplanet_catalogue);
      std::cout<<"\nSorted exoplanet list:\n";
      for(const auto& planet : exoplanet_catalogue) planet->print();
      keep_sorting = yes_no_handling("Would you like to sort by another property? (yes/no): ");
    }

    // After sorting is finished, ask if the user wants to export the sorted exoplanet list
    if(yes_no_handling("Would you like to export the sorted exoplanet list? (yes/no): ")) 
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
  } 
  else
  {
    // If the user chooses not to sort, inform them and return to the main menu
    std::cout<<"Sorting skipped. Returning to main menu.\n";
  }
}