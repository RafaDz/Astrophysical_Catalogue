// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Save objects to CSV files implementation file.
 * This file contains functions to save galaxies, stars, and exoplanets to CSV files.
 * It includes error handling for file operations and ensures the data folder exists.
 * The functions take a filename and a vector of shared pointers to the respective object types.
 * The CSV files are saved in a specific format, with headers for each type of object.
 * The functions also provide feedback on successful saving of data.
 * The CSV files are stored in a folder named "Astro_Objects".
 * The functions use the filesystem library to handle directory creation and file operations.
 */

#include "save_object.h"
#include<fstream> // for std::ofstream
#include<iostream> // for std::cerr, std::cout
#include <filesystem> // for std::filesystem

namespace fs = std::filesystem; // for filesystem operations

const std::string data_folder = "Astro_Objects/"; // Folder where CSV files are stored

// Function to save galaxies to CSV file
void save_galaxies_to_csv(const std::string& filename, const std::vector<std::shared_ptr<Galaxy>>& galaxies)
{
  fs::create_directories(data_folder); // Ensure the folder exists

  // Check if the file can be opened
  std::ofstream out(data_folder + filename);
  if(!out) 
  {
    std::cerr<<"Could not open file for writing: "<<filename<<"\n";
    return;
  }

  // Write the header
  out<<"name,constellation,distance,apparent_magnitude,redshift\n";

  // Write each galaxy's data
  for(const auto& galaxy : galaxies) 
  {
    out<<galaxy->get_name()<<","<<galaxy->get_constellation()<<","<<galaxy->get_distance()<<","<<galaxy->get_apparent_magnitude()<<","
      <<galaxy->get_redshift()<<"\n";
  }

  // Close the file
  std::cout<<"Galaxies saved to "<<data_folder + filename<<"\n";
}

// Function to save stars to CSV file
void save_stars_to_csv(const std::string& filename, const std::vector<std::shared_ptr<Star>>& stars)
{
  fs::create_directories(data_folder); // Ensure the folder exists

  // Check if the file can be opened
  std::ofstream out(data_folder + filename);
  if(!out) 
  {
    std::cerr<<"Could not open file for writing: "<<filename<<"\n";
    return;
  }

  // Write the header
  out<<"name,type,spectral_class,mass,radius,temperature,age,apparent_magnitude,distance\n";

  // Write each star's data
  for(const auto& star : stars) 
  {
    out<<star->get_name()<<","<<star->get_type()<<","<<star->get_spectral_class()<<","<<star->get_mass()<<","
      <<star->get_radius()<<","<<star->get_temperature()<<","<<star->get_age()<<","<<star->get_apparent_magnitude()<<","
      <<star->get_distance()<<"\n";
  }

  // Close the file
  std::cout<<"Stars saved to "<<data_folder + filename<<"\n";
}

// Function to save exoplanets to CSV file
void save_exoplanets_to_csv(const std::string& filename, const std::vector<std::shared_ptr<Exoplanet>>& exoplanets)
{
  fs::create_directories(data_folder); // Ensure the folder exists

  // Check if the file can be opened
  std::ofstream out(data_folder + filename);
  if(!out) 
  {
    std::cerr<<"Could not open file for writing: "<<filename<<"\n";
    return;
  }

  // Write the header
  out<<"name,type,host_star,mass,radius,semi_major_axis,eccentricity\n";

  // Write each exoplanet's data
  for(const auto& exoplanet : exoplanets) 
  {
    out<<exoplanet->get_name()<<","<<exoplanet->get_type()<<","<<exoplanet->get_host_star_name()<<","
      <<exoplanet->get_planet_mass()<<","<<exoplanet->get_planet_radius()<<","
      <<exoplanet->get_semi_major_axis()<<","<<exoplanet->get_eccentricity()<<"\n";
  }

  // Close the file
  std::cout<<"Exoplanets saved to "<<data_folder + filename<<"\n";
}