// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Header file to initialize (load objects) catalogue. */

#ifndef INITIALIZE_CATALOGUE_H
#define INITIALIZE_CATALOGUE_H

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"
#include "objects_loader.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<iostream> // for std::cout, std::cerr
#include<string> // for std::string
#include<stdexcept> // for std::exception

inline void load_catalogues(
  std::vector<std::shared_ptr<Galaxy>>& galaxies,
  std::vector<std::shared_ptr<Star>>& stars,
  std::vector<std::shared_ptr<Exoplanet>>& exoplanets)
{
  try 
  {
    galaxies = load_galaxies_from_csv("galaxies.csv");
  }
  catch(const std::exception& error_handling)
  {
    std::cerr<<"Error: "<<error_handling.what()<<"\n";
  }
  if(galaxies.empty()) 
  {
    std::cout<<"[!] Warning: No galaxies loaded from file.\n";
  }

  try 
  {
    stars = load_stars_from_csv("stars.csv");
  } 
  catch(const std::exception& error_handling) 
  {
    std::cerr<<"Error: "<<error_handling.what()<<"\n";
  }
  if(stars.empty()) 
  {
    std::cout<<"[!] Warning: No stars loaded from file.\n";
  }

  try 
  {
    exoplanets = load_exoplanets_from_csv("exoplanets.csv", stars);
  } 
  catch(const std::exception& error_handling) 
  {
    std::cerr << "Error: " << error_handling.what() << "\n";
  }
  if(exoplanets.empty()) 
  {
    std::cout<<"[!] Warning: No exoplanets loaded from file.\n";
  }
}

#endif // INITIALIZE_CATALOGUE_H
