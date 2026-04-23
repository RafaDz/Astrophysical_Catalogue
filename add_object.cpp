// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Adding an object implementation: 
 * The function is used to add a new galaxy, star, or exoplanet entry to the catalogue.
 * It prompts the user for details, validates the inputs, and adds the entry to the catalogue.
 * The function also checks if the host star exists in the star catalogue when adding an exoplanet.
 * It uses shared pointers to manage memory and avoid memory leaks.
 * The function also provides feedback to the user on the success or failure of the addition.
 */

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"
#include "add_object.h"
#include "shared_functions.h"

#include<iostream> // for std::cout, std::cin
#include<memory> // for std::shared_ptr
#include<limits> // for std::numeric_limits
#include<string> // for std::string
#include<vector> // for std::vector
#include<algorithm> // for std::find_if
#include<stdexcept> // for std::invalid_argument

// Function to add a new galaxy entry to the catalogue
void add_galaxy_entry(std::vector<std::shared_ptr<Galaxy>>& catalogue)
{
  // Prompt user for galaxy details
  std::string name, constellation;
  double distance, apparent_magnitude, redshift;

  std::cout<<"\n--- Add New Galaxy ---\n";
  std::cout<<"Name: "; std::getline(std::cin, name);
  std::cout<<"Constellation: "; std::getline(std::cin, constellation);
  distance = get_validated_double("Distance [Mpc]: ");
  apparent_magnitude = get_validated_double("Apparent Magnitude: ");
  redshift = get_validated_double("Redshift: ");

  // Validate inputs
  try 
  {
    auto galaxy = std::make_shared<Galaxy>(name, constellation, distance, apparent_magnitude, redshift);
    catalogue.push_back(galaxy);
    std::cout<<"Galaxy added successfully. Don't forget to save it!\n";
  } 
  catch(const std::exception& error_handling) 
  {
    std::cerr<<"Error adding galaxy: "<<error_handling.what()<<"\n";
  }
}

// Function to add a new star entry to the catalogue
void add_star_entry(std::vector<std::shared_ptr<Star>>& catalogue)
{
  // Prompt user for star details
  std::string name, type, spectral_class;
  double mass, radius, temperature, age, apparent_magnitude, distance;

  std::cout<<"\n--- Add New Star ---\n";
  std::cout<<"Name: "; std::getline(std::cin, name);
  std::cout<<"Type: "; std::getline(std::cin, type);
  std::cout<<"Spectral Class: "; std::getline(std::cin, spectral_class);
  mass = get_validated_double("Mass [M_Sun]: ");
  radius = get_validated_double("Radius [R_Sun]: ");
  temperature = get_validated_double("Temperature [K]: ");
  age = get_validated_double("Age [Gyr]: ");
  apparent_magnitude = get_validated_double("Apparent Magnitude: ");
  distance = get_validated_double("Distance [Mpc]: ");

  // Validate inputs
  try 
  {
    // Create a new star object and add it to the catalogue
    auto star = std::make_shared<Star>(name, type, spectral_class, mass, radius, temperature, age, apparent_magnitude, distance);
    catalogue.push_back(star);
    std::cout<<"Star added successfully. Don't forget to save it!\n";
  } 
  catch(const std::exception& error_handling) // Catch any exceptions
  {
    std::cerr<<"Error adding star: "<<error_handling.what()<<"\n";
  }
}

// Function to add a new exoplanet entry to the catalogue
void add_exoplanet_entry(std::vector<std::shared_ptr<Exoplanet>>& catalogue,
                         const std::vector<std::shared_ptr<Star>>& star_catalogue)
{
  // Prompt user for exoplanet details
  std::string name, type, host_star_name;
  double planet_mass, planet_radius, semi_major_axis, eccentricity;

  std::cout<<"\n--- Add New Exoplanet ---\n";
  std::cout<<"Name: "; std::getline(std::cin, name);
  std::cout<<"Type: "; std::getline(std::cin, type);
  std::cout<<"Host Star Name: "; std::getline(std::cin, host_star_name);

  // Check if the host star exists in the star catalogue
  // If not, the exoplanet will be added without a link to a star
  std::shared_ptr<Star> host_star_ptr = nullptr;
  auto match = std::find_if(star_catalogue.begin(), star_catalogue.end(),
    [&host_star_name](const std::shared_ptr<Star>& star) 
    {
      const std::string& name = star->get_name();
      return name.size() == host_star_name.size() &&
        std::equal(name.begin(), name.end(), host_star_name.begin(),
        [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    });

  // If the star is found, assign it to host_star_ptr
  // Otherwise, host_star_ptr remains nullptr
  if(match == star_catalogue.end()) 
  {
    std::cout<<"Host star not found. Exoplanet will still be added without star link.\n";
  } 
  else 
  {
    host_star_ptr = *match;
  }

  planet_mass = get_validated_double("Planet Mass [M_Earth]: ");
  planet_radius = get_validated_double("Planet Radius [R_Earth]: ");
  semi_major_axis = get_validated_double("Semi-Major Axis [AU]: ");
  eccentricity = get_validated_double("Eccentricity: ");

  // Validate inputs
  try 
  {
    // Create a new exoplanet object and add it to the catalogue
    auto exoplanet = std::make_shared<Exoplanet>(
      name, type, host_star_ptr, planet_mass, planet_radius, semi_major_axis, eccentricity);
    catalogue.push_back(exoplanet);
    std::cout<<"Exoplanet added successfully. Don't forget to save it!\n";
  } 
  catch(const std::exception& error_handling) // Catch any exceptions
  {
    std::cerr<<"Error adding exoplanet: "<<error_handling.what()<<"\n";
  }
}
