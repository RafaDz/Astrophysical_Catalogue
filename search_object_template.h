// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Search for an object template.
 * This template can be applied to all derived classes.
 * The function takes a vector of shared pointers to the object type and a label string.
 * It prompts the user to enter the name of the object to search for.
 * If the object is found, it prints its details.
 * If not found, it prints a list of available objects.
 * The function handles user input and provides feedback on the search process.
 * The function also allows for repeated attempts to search for objects until the user chooses to exit.
 * The function uses a lambda function to compare object names.
 * The function is designed to be reusable for different types of objects.
 */

#ifndef SEARCH_OBJECT_TEMPLATE_H
#define SEARCH_OBJECT_TEMPLATE_H

#include "shared_functions.h" // for yes_no_handling
#include "compare_objects_template.h" // for compare_objects_by_name

#include<iostream> // for std::cout, std::cin
#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<string> // for std::string
#include<algorithm> // for std::find_if

/// Function to search for an object by name in a catalogue
/// and print its details if found. If not found, it prints a list of available objects.
template <typename T>
void search_object_by_name(const std::vector<std::shared_ptr<T>>& catalogue, const std::string& label) 
{
  while(true) 
  {
    // Print prompt for user input
    std::cout<<"\n=== "<<label<<" Search/Compare Menu ===\n";
    std::cout<<"(1) Search for a "<<label<<"\n";
    std::cout<<"(2) Compare two "<<label<<"s\n";
    std::cout<<"(3) Return to main menu\n";
    std::cout<<"Enter your choice 1, 2, or 3: ";

    std::string choice; // Get user input
    std::getline(std::cin, choice); // Read the user input

    // Check if the user input is valid
    // If the user input is "1", search for an object by name
    if(choice == "1") 
    {
      std::cout<<"\n=== Case-sensitive inputs ===\n";
      std::cout<<"\nEnter the name of the "<<label<<" to search for: ";
      std::string search_name; // Variable to store the name of the object to search for
      std::getline(std::cin, search_name); // Read the user input

      // Lambda function to find an object by name in the catalogue
      // It uses std::find_if to search for the object in the catalogue
      auto iterator = std::find_if(catalogue.begin(), catalogue.end(), [&](const std::shared_ptr<T>& object) 
      {
        return object->get_name() == search_name;
      });

      // Check if the object is found in the catalogue
      // If found, print its details
      if(iterator != catalogue.end()) 
      {
        (*iterator)->print();
      } 
      else // If not found, print an error message and a list of available objects
      {
        std::cout<<"\n[!] "<<label<<" named \""<<search_name<<"\" not found.\n";
        std::cout<<"\nAvailable "<<label<<"s:\n";
        for(const auto& object : catalogue)
          std::cout<<" - "<<object->get_name()<<"\n";
      }
    }

    // If the user input is "2", compare two objects by name
    else if(choice == "2") 
    {
      compare_objects_by_name(catalogue, label);
    }

    // If the user input is "3", return to the main menu
    else if(choice == "3") 
    {
      break; // return to main menu
    }

    // If the user input is invalid, print an error message
    else 
    {
      std::cout<<"[!] Invalid choice. Please enter 1, 2, or 3.\n";
    }
  }
}

#endif // SEARCH_OBJECT_TEMPLATE_H
