// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Compare an objects template. 
 * This template can be applied to all derived classes. 
 * Defines lambda function 'find_by_name' to search for object
   in the list so both objects can be further compared. 
 * Prints error message if object(s) are not found.
 * Prints the details of both objects if they are found.
 * The function uses shared pointers to manage memory and avoid memory leaks.
 * It also provides feedback to the user on the success or failure of the comparison.
 */

#ifndef COMPARE_OBJECTS_TEMPLATE_H
#define COMPARE_OBJECTS_TEMPLATE_H

#include "shared_functions.h" 
#include "celestial_objects.h"

#include <iostream> // for std::cout, std::cin
#include <vector> // for std::vector
#include <memory> // for std::shared_ptr
#include <string> // for std::string
#include <algorithm> // for std::find_if
#include <iomanip> // for std::setw

/// Function to compare two objects by name and print both
template <typename T>
void compare_objects_by_name(const std::vector<std::shared_ptr<T>>& catalogue, const std::string& label)
{
  // Print prompt for user input
  std::cout<<"\n === Case-sensitive inputs ===\n";
  std::cout<<"\nEnter the name of the first "<<label<<": ";
  std::string first_name;
  std::getline(std::cin, first_name);

  std::cout<<"\nEnter the name of the second "<<label<<": ";
  std::string second_name;
  std::getline(std::cin, second_name);

  // Lambda function to find an object by name in the catalogue
  // It uses std::find_if to search for the object in the catalogue
  auto find_by_name = [&](const std::string& name)
  {
    return std::find_if(catalogue.begin(), catalogue.end(), [&](const std::shared_ptr<T>& object)
    {
      return object->get_name() == name;
    });
  };

  auto first = find_by_name(first_name); // Find the first object by name
  auto second = find_by_name(second_name); // Find the second object by name

  // Check if both objects are found in the catalogue
  // If both objects are found, print their details
  if(first != catalogue.end() && second != catalogue.end())
  {
    std::cout<<"\n=== Comparison of "<<label<<"s: ===\n";
    std::cout<<"------------------------------\n";
    (*first)->print();
    std::cout<<"------------------------------\n";
    (*second)->print();
    std::cout<<"------------------------------\n";
  }
  else
  {
    // If one or both objects are not found, print an error message
    std::cout<<"\n[!] One or both "<<label<<"s not found.\n";
    std::cout<<"\n === Available "<<label<<"s: ===\n";
    for(const auto& object : catalogue)
      std::cout<<" - "<<object->get_name()<<"\n";
  }
}

#endif // COMPARE_OBJECTS_TEMPLATE_H
