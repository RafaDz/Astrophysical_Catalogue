// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Type choice template header file.
 * This template simplifies repeated object-type branching in main.
 * It takes a prompt string and three lambdas for Galaxy, Star, and Exoplanet logic.
 * The template is designed to be reusable for different types of objects.
 */

#ifndef TYPE_CHOICE_TEMPLATE_H
#define TYPE_CHOICE_TEMPLATE_H

#include<iostream> // for std::cout, std::cin
#include<string> // for std::string
#include<functional> // for std::function

// Template to simplify repeated object-type branching in main
// Takes a prompt string and three lambdas for Galaxy, Star, and Exoplanet logic
template <typename GalaxyFunc, typename StarFunc, typename ExoplanetFunc>
void handle_type_choice(
  const std::string& prompt,
  GalaxyFunc handle_galaxy,
  StarFunc handle_star,
  ExoplanetFunc handle_exoplanet)
{
  // Prompt the user for input
  std::string type_choice;
  std::cout << prompt;
  std::getline(std::cin, type_choice);

  // Check the input and call the appropriate function
  // Convert to lowercase for case-insensitive comparison
  if(type_choice == "G" || type_choice == "g") 
  {
    handle_galaxy();
  } 
  else if(type_choice == "S" || type_choice == "s") 
  {
    handle_star();
  } 
  else if(type_choice == "E" || type_choice == "e") 
  {
    handle_exoplanet();
  } 
  else 
  {
    std::cout<<"Invalid input. Please enter G, S, or E.\n";
  }
}

#endif // TYPE_CHOICE_TEMPLATE_H