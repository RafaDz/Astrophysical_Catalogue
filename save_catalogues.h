// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Header file to save changes in the catalogue. */

#ifndef SAVE_CATALOGUES_H
#define SAVE_CATALOGUES_H

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"
#include "save_object.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<iostream> // for std::cout

// Function to save all catalogues to CSV files
inline void save_all_catalogues(
  const std::vector<std::shared_ptr<Galaxy>>& galaxies,
  const std::vector<std::shared_ptr<Star>>& stars,
  const std::vector<std::shared_ptr<Exoplanet>>& exoplanets)
{
  save_galaxies_to_csv("galaxies.csv", galaxies);
  save_stars_to_csv("stars.csv", stars);
  save_exoplanets_to_csv("exoplanets.csv", exoplanets);
  std::cout<<"Changes saved successfully.\n";
}

#endif // SAVE_CATALOGUES_H
