// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Sorting menu implementation file.
 * This file contains functions to sort galaxies and stars based on user input.
 * It includes sorting options for different properties of galaxies, stars and exoplanets.
 * The sorting options are displayed to the user, and the user can choose the property 
   and order (ascending/descending) for sorting.
 * The sorting is done using lambda functions and templates to handle different types of celestial objects.
 * It includes functions for displaying sorting options, handling user input, and sorting the catalogues.
 */

#include "sort_menu.h"

#include<iostream> // for std::cout, std::endl
#include<string> // for std::string
#include<algorithm> // for std::transform
#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<functional> // for std::function
#include<iomanip> // for std::setw
#include<stdexcept> // for std::invalid_argument

// Function to sort galaxies based on user input
void galaxy_sort_menu(std::vector<std::shared_ptr<Galaxy>>& galaxies) 
{
  // Display sorting options and handle user input for sorting
  std::string choice;
  SortOrder order = SortOrder::Ascending;

  while(true)
  {
    std::cout<<"\nSort galaxies by:\n"
      <<"  (1) Distance\n"
      <<"  (2) Apparent Magnitude\n"
      <<"  (3) Redshift\n"
      <<"Enter 1-3: ";
    std::getline(std::cin, choice);

    if(choice == "1" || choice == "2" || choice == "3") 
    {
      break;
    }
    else
    {
      std::cout<<"Invalid choice. Please enter a number between 1 and 3.\n";
    }
  }
  // Get sorting order from user
  std::string order_choice;
  // Loop until valid input is received
  while(true) 
  {
    std::cout<<"Sort order? (A = Ascending, D = Descending): ";
    std::getline(std::cin, order_choice);

    // Check for valid input
    if(order_choice == "A") 
    {
      order = SortOrder::Ascending;
      break;
    }
    else if(order_choice == "D") 
    {
      order = SortOrder::Descending;
      break;
    }
    else 
    {
      std::cout<<"Invalid input. Please enter 'A' or 'D'.\n";
    }
  }

  // Sort galaxies based on user choice (using lambda functions)
  if(choice == "1")
  {
    sort_catalogue<Galaxy>(galaxies, [](const auto& galaxy) { return galaxy->get_distance(); }, order);
  }
  else if(choice == "2")
  {
    sort_catalogue<Galaxy>(galaxies, [](const auto& galaxy) { return galaxy->get_apparent_magnitude(); }, order);
  }
  else if(choice == "3")
  {
    sort_catalogue<Galaxy>(galaxies, [](const auto& galaxy) { return galaxy->get_redshift(); }, order);
  }
  else // If invalid choice, display error message
  {
    std::cout<<"Invalid choice. No sorting applied.\n";
  }
}

// Function to sort stars based on user input
void star_sort_menu(std::vector<std::shared_ptr<Star>>& stars) 
{
  // Display sorting options and handle user input for sorting
  std::string choice;
  SortOrder order = SortOrder::Ascending;

  while(true)
  {
    std::cout<<"\nSort exoplanets by:\n"
      <<"  (1) Mass\n"
      <<"  (2) Radius\n"
      <<"  (3) Semi-Major Axis\n"
      <<"  (4) Eccentricity\n"
      <<"Enter 1-4: ";
    std::getline(std::cin, choice);

    if(choice == "1" || choice == "2" || choice == "3" || choice == "4") 
    {
      break;
    }
    else // If invalid choice, display error message
    {
      std::cout<<"Invalid choice. Please enter a number between 1 and 4.\n";
    }
  }

  // Get sorting order from user
  std::string order_choice;
  // Loop until valid input is received
  while(true) 
  {
    std::cout<<"Sort order? (A = Ascending, D = Descending): ";
    std::getline(std::cin, order_choice);

    // Check for valid input
    if(order_choice == "A") 
    {
      order = SortOrder::Ascending;
      break;
    } 
    else if(order_choice == "D") 
    {
      order = SortOrder::Descending;
      break;
    } 
    else // If invalid input, display error message
    {
      std::cout<<"Invalid input. Please enter 'A' or 'D'.\n";
    }
  }

  // Sort stars based on user choice (using lambda functions)
  if(choice == "1")
  {
    sort_catalogue<Star>(stars, [](const auto& star) { return star->get_mass(); }, order);
  }
  else if(choice == "2")
  {
    sort_catalogue<Star>(stars, [](const auto& star) { return star->get_radius(); }, order);
  }
  else if(choice == "3")
  {
    sort_catalogue<Star>(stars, [](const auto& star) { return star->get_temperature(); }, order);
  }
  else if(choice == "4")
  {
    sort_catalogue<Star>(stars, [](const auto& star) { return star->get_age(); }, order);
  }
  else if(choice == "5")
  {
    sort_catalogue<Star>(stars, [](const auto& star) { return star->get_distance(); }, order);
  }
  else
  {
    std::cout<<"Invalid choice. No sorting applied.\n";
  }
}

// Function to sort exoplanets based on user input
void exoplanet_sort_menu(std::vector<std::shared_ptr<Exoplanet>>& exoplanets) 
{
  // Display sorting options and handle user input for sorting
  std::string choice;
  SortOrder order = SortOrder::Ascending;

  while(true)
  {
    std::cout<<"\nSort exoplanets by:\n"
      <<"  (1) Mass\n"
      <<"  (2) Radius\n"
      <<"  (3) Semi-Major Axis\n"
      <<"  (4) Eccentricity\n"
      <<"Enter 1-4: ";
    std::getline(std::cin, choice);

    if(choice == "1" || choice == "2" || choice == "3" || choice == "4") 
    {
      break;
    }
    else // If invalid choice, display error message
    {
      std::cout<<"Invalid choice. Please enter a number between 1 and 4.\n";
    }
  }
  // Get sorting order from user
  std::string order_choice;
  // Loop until valid input is received
  while(true) 
  {
    std::cout<<"Sort order? (A = Ascending, D = Descending): ";
    std::getline(std::cin, order_choice);

    // Check for valid input
    if(order_choice == "A")
    {
      order = SortOrder::Ascending;
      break;
    } 
    else if(order_choice == "D")
    {
      order = SortOrder::Descending;
      break;
    } 
    else 
    {
      std::cout<<"Invalid input. Please enter 'A' or 'D'.\n";
    }
  }

  // Sort exoplanets based on user choice (using lambda functions)
  if(choice == "1")
  {
    sort_catalogue<Exoplanet>(exoplanets, [](const auto& exoplanet) { return exoplanet->get_planet_mass(); }, order);
  }
  else if(choice == "2")
  {
    sort_catalogue<Exoplanet>(exoplanets, [](const auto& exoplanet) { return exoplanet->get_planet_radius(); }, order);
  }
  else if(choice == "3")
  {
    sort_catalogue<Exoplanet>(exoplanets, [](const auto& exoplanet) { return exoplanet->get_semi_major_axis(); }, order);
  }
  else if(choice == "4")
  {
    sort_catalogue<Exoplanet>(exoplanets, [](const auto& exoplanet) { return exoplanet->get_eccentricity(); }, order);
  }
  else
  {
    std::cout<<"Invalid choice. No sorting applied.\n";
  }
}

