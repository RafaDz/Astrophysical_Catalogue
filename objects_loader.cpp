// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Load objects from CSV files implementation file.
 * This file contains functions to load galaxies, stars, and exoplanets from CSV files.
 * It includes error handling for file operations and data parsing.
 * The functions return vectors of shared pointers to the respective object types.
 * The CSV files are expected to be in a specific format, and the functions handle parsing accordingly.
 * The functions also ensure that the data folder exists before attempting to load files. 
 */

#include "objects_loader.h"

#include<fstream> // for std::ifstream
#include<sstream> // for std::istringstream
#include<stdexcept> // for std::runtime_error
#include<iostream> // for std::cerr
#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<string> // for std::string
#include<algorithm> // for std::find_if
#include<filesystem> // for std::filesystem

namespace fs = std::filesystem; // for filesystem operations

const std::string data_folder = "Astro_Objects/"; // Folder where CSV files are stored

// Function to load galaxies from CSV file
std::vector<std::shared_ptr<Galaxy>>load_galaxies_from_csv(const std::string& filename)
{
  fs::create_directories(data_folder); // Ensure the directory exists

  std::vector<std::shared_ptr<Galaxy>> galaxies;
  std::ifstream file(data_folder + filename);
  std::string line;

  std::getline(file, line); // skip header

  while(std::getline(file, line)) 
  {
    std::istringstream ss(line);
    std::string name, constellation;
    double distance, magnitude, redshift;

    std::getline(ss, name, ',');
    std::getline(ss, constellation, ',');
    ss>>distance; ss.ignore();
    ss>>magnitude; ss.ignore();
    ss>>redshift;

    galaxies.push_back(std::make_shared<Galaxy>(name, constellation, distance, magnitude, redshift));
  }
  return galaxies;
}

// Function to load stars from CSV file
std::vector<std::shared_ptr<Star>>load_stars_from_csv(const std::string& filename)
{
  fs::create_directories(data_folder); // Ensure the directory exists

  std::vector<std::shared_ptr<Star>> stars;
  std::ifstream file(data_folder + filename);
  std::string line;

  std::getline(file, line); // skip header

  while (std::getline(file, line)) 
  {
    std::istringstream ss(line);
    std::string name, type, spectral_class;
    double mass, radius, temperature, age, apparent_magnitude, distance;

    std::getline(ss, name, ',');
    std::getline(ss, type, ',');
    std::getline(ss, spectral_class, ',');
    ss>>mass; ss.ignore();
    ss>>radius; ss.ignore();
    ss>>temperature; ss.ignore();
    ss>>age; ss.ignore();
    ss>>apparent_magnitude; ss.ignore();
    ss>>distance;

    stars.push_back(std::make_shared<Star>(name, type, spectral_class, mass, radius, temperature, age, apparent_magnitude, distance));
  }
  return stars;
}

// Function to load exoplanets from CSV file
std::vector<std::shared_ptr<Exoplanet>>load_exoplanets_from_csv(
  const std::string& filename,
  const std::vector<std::shared_ptr<Star>>& star_catalogue)
{
  fs::create_directories(data_folder); // Ensure the directory exists
  
  std::vector<std::shared_ptr<Exoplanet>> exoplanets;
  std::ifstream file(data_folder + filename);
  std::string line;

  if(!file.is_open()) 
  {
    throw std::runtime_error("Could not open file: " + filename);
  }

  std::getline(file, line); // skip header

  while(std::getline(file, line)) 
  {
    std::istringstream ss(line);
    std::string name, type, host_star_name;
    double planet_mass, planet_radius, semi_major_axis, eccentricity;

    std::getline(ss, name, ',');
    std::getline(ss, type, ',');
    std::getline(ss, host_star_name, ',');
    ss>>planet_mass; ss.ignore();
    ss>>planet_radius; ss.ignore();
    ss>>semi_major_axis; ss.ignore();
    ss>>eccentricity;

    // Check if the host star exists in the catalogue
    auto match = std::find_if(star_catalogue.begin(), star_catalogue.end(),
      [&host_star_name](const std::shared_ptr<Star>& star) 
      {
        return star->get_name() == host_star_name;
      });

    // If the host star is found, create the exoplanet object
    std::shared_ptr<Star> host_ptr = nullptr;
    if (match != star_catalogue.end()) 
    {
      host_ptr = *match;
    }
    // Create the exoplanet object
    // Note: The constructor of Exoplanet should handle the case where host_ptr is nullptr
    auto exoplanet = std::make_shared<Exoplanet>(
      name, type, host_ptr, planet_mass, planet_radius, semi_major_axis, eccentricity);
    exoplanets.push_back(exoplanet);
  }
  return exoplanets;
}
