// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* A template designed to remove objects from the list.
 * The function takes a vector of shared pointers to the object type and a label string.
 * It prompts the user to enter the name of the object to remove.
 * If the object is found, it is removed from the catalogue.
 * The function handles user input and provides feedback on the removal process.
 * The function also allows for repeated attempts to remove objects until the user chooses to exit.
 * The function uses a lambda function to compare object names.
 * The function is designed to be reusable for different types of objects.
 */

#ifndef REMOVE_OBJECT_TEMPLATE_H
#define REMOVE_OBJECT_TEMPLATE_H

#include "shared_functions.h" // for yes_no_handling

#include<iostream> // for std::cout, std::cin
#include<string> // for std::string
#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<functional> // for std::function
#include<algorithm> // for std::remove_if

// Template function to remove an object by name from a catalogue
template <typename T>
void remove_object_by_name(
  std::vector<std::shared_ptr<T>>& catalogue,
  const std::string& label)
{
  // Check if the catalogue is empty
  if(catalogue.empty()) 
  {
    std::cout<<"No "<<label<<"s in the catalogue.\n";
    return;
  }

  while(true) 
  {
    std::cout<<"\n"<<label<<" catalogue:\n";
    for(const auto& object : catalogue)
      std::cout<<" - "<<object->get_name()<<"\n";

    bool removed = false;
    while (!removed) 
    {
      std::string name_to_remove;
      std::cout<<"Enter the name of the "<<label<<" to remove: ";
      std::getline(std::cin, name_to_remove);

      auto before = catalogue.size();
      catalogue.erase(std::remove_if(catalogue.begin(), catalogue.end(),
        [&name_to_remove](const std::shared_ptr<T>& object)
        {
          return object->get_name() == name_to_remove;
        }), catalogue.end());

      if(catalogue.size() < before) 
      {
        std::cout<<label<<" \""<<name_to_remove<<"\" removed.\n";
        removed = true;
      } 
      else
      {
        std::cout<<"No "<<label<<" with name \""<<name_to_remove<<"\" found.\n";
        if(!yes_no_handling("Would you like to try again? (yes/no): ")) 
        {
          break;
        }
      }
    }

    if(!yes_no_handling("Would you like to remove another " + label + "? (yes/no): ")) 
    {
      std::cout<<"Returning to main menu.\n";
      break;
    }
  }
}

#endif // REMOVE_OBJECT_TEMPLATE_H
