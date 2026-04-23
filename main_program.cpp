// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Main program implementation file.
 * This file contains the main function that serves as the entry point for the program.
 * It initializes the catalogues, displays the main menu, and handles user input.
 * The program allows users to view, search, add, remove, and export celestial objects.
 * It also provides options for sorting the catalogues and saving changes.
 * The program uses a loop to continuously prompt the user for actions until they choose to quit.
 * The program is designed to be user-friendly and includes error handling for invalid inputs.
 * The program uses templates and lambda functions to handle different types of celestial objects.
 * The program is modular, with separate header and implementation files for different functionalities.
 * The program is designed to be extensible, allowing for easy addition of new celestial object types in the future.
 */

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"
#include "objects_loader.h"
#include "add_object.h"
#include "save_object.h"
#include "remove_object_template.h"
#include "search_object_template.h"
#include "type_choice_template.h"
#include "initialize_catalogue.h"
#include "save_catalogues.h"
#include "shared_functions.h"
#include "export_template.h"
#include "handle_sort_menu.h"
#include "handle_export_menu.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<iostream> // for std::cout, std::cin
#include<string> // for std::getline
#include<iomanip> // for std::setw

int main() 
{
  // === Initialize catalogues ===
  std::vector<std::shared_ptr<Galaxy>> galaxy_catalogue;
  std::vector<std::shared_ptr<Star>> star_catalogue;
  std::vector<std::shared_ptr<Exoplanet>> exoplanet_catalogue;

  load_catalogues(galaxy_catalogue, star_catalogue, exoplanet_catalogue);

  // === Main loop for the program ===
  std::cout<<"\nWelcome to the Astronomical Catalogue!\n";
  std::string view_choice;

  // === Loop until the user chooses to quit (loop will break on '7') ===
  while(true)
  {
    // === Main menu ===
    std::cout<< "\n--- Main Menu ---\n";
    std::cout<< "\nWhat would you like to do?\n"
      <<"  (0) Catalogue summary\n"
      <<"  (1) Search/Compare\n"
      <<"  (2) View/export\n"
      <<"  (3) Sort by property\n"
      <<"  (4) Add a new object\n"
      <<"  (5) Remove an object\n"
      <<"  (6) Save changes\n"
      <<"  (7) Quit\n"
      <<"Enter 0-7: ";
    std::getline(std::cin, view_choice);

    // === Choice 0: Catalogue summary ===
    // Total number of celestial objects in this catalogue
    if(view_choice == "0") 
    {
      std::cout<<"\n=== Catalogue Summary ===\n";
      std::cout<<"Galaxies:   "<<Galaxy::get_counter()<<"\n";
      std::cout<<"Stars:      "<<Star::get_counter()<<"\n";
      std::cout<<"Exoplanets: "<<Exoplanet::get_counter()<<"\n";
      std::cout<<"Total:      "<<CelestialObject::get_counter()<<"\n";
      std::cout<<"=========================\n";
      continue; // go back to the main menu
    }

    // === Choice 1: Search/Compare ===
    if(view_choice == "1") 
    {
      handle_type_choice(
        "What type of object do you want to search for? (G = Galaxy, S = Star, E = Exoplanet): ",
        [&] { search_object_by_name<Galaxy>(galaxy_catalogue, "Galaxy"); },
        [&] { search_object_by_name<Star>(star_catalogue, "Star"); },
        [&] { search_object_by_name<Exoplanet>(exoplanet_catalogue, "Exoplanet"); }
      );
    } 

    // === Choice 2: View/Export ===
    else if(view_choice == "2") 
    {
      handle_type_choice(
        "What type of object would you like to view/export? (G = Galaxy, S = Star, E = Exoplanet): ",
        [&] { handle_galaxy_menu(galaxy_catalogue); },
        [&] { handle_star_menu(star_catalogue); },
        [&] { handle_exoplanet_menu(exoplanet_catalogue); }
      );
    }

    // === Choice 3: Sort by property ===
    else if(view_choice == "3") 
    {
      handle_type_choice(
        "What type of object would you like to sort? (G = Galaxy, S = Star, E = Exoplanet): ",
        [&] { handle_sort_galaxy_menu(galaxy_catalogue); },
        [&] { handle_sort_star_menu(star_catalogue); },
        [&] { handle_sort_exoplanet_menu(exoplanet_catalogue); }
      );
    }

    // === Choice 4: Add a new object ===
    else if(view_choice == "4") 
    {
      handle_type_choice(
        "What type of object would you like to add? (G = Galaxy, S = Star, E = Exoplanet): ",
        [&] { add_galaxy_entry(galaxy_catalogue); },
        [&] { add_star_entry(star_catalogue); },
        [&] { add_exoplanet_entry(exoplanet_catalogue, star_catalogue); }
      );
    }

    // === Choice 5: Remove an object ===
    else if(view_choice == "5") 
    {
      handle_type_choice(
        "What type of object would you like to remove? (G = Galaxy, S = Star, E = Exoplanet): ",
        [&] { remove_object_by_name<Galaxy>(galaxy_catalogue, "Galaxy"); },
        [&] { remove_object_by_name<Star>(star_catalogue, "Star"); },
        [&] { remove_object_by_name<Exoplanet>(exoplanet_catalogue, "Exoplanet"); }
      );
    }

    // === Choice 6: Save changes ===
    else if(view_choice == "6")
    {
      if(yes_no_handling("Would you like to save the current catalogue? (yes/no): ")) 
      {
        save_all_catalogues(galaxy_catalogue, star_catalogue, exoplanet_catalogue);
      } 
      else 
      {
        std::cout<<"Changes not saved.\n";
      }
    }
    

    // === Choice 7: Quit ===
    else if(view_choice == "7")
    {
      if(yes_no_handling("All changes must be saved before exit. Would you like to save now? (yes/no): ")) 
      {
        save_all_catalogues(galaxy_catalogue, star_catalogue, exoplanet_catalogue);
        std::cout<<"Exiting...\n";
      } 
      else 
      {
        std::cout<<"Exiting without saving.\n";
      }
      break;
    }
    else 
    {
      std::cout<<"\nInvalid choice. Please enter a number between 0 and 7.\n";
    }
  }
  return 0;
}