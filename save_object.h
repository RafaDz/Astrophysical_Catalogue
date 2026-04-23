// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Save objects header file. */

#ifndef SAVE_OBJECT_H
#define SAVE_OBJECT_H

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<string> // for std::string

// Function prototypes for saving objects to CSV files
// These functions take a filename and a vector of shared pointers to the respective object type
void save_galaxies_to_csv(const std::string& filename, const std::vector<std::shared_ptr<Galaxy>>& galaxies);
void save_stars_to_csv(const std::string& filename, const std::vector<std::shared_ptr<Star>>& stars);
void save_exoplanets_to_csv(const std::string& filename, const std::vector<std::shared_ptr<Exoplanet>>& exoplanets);

#endif // SAVE_OBJECT_H
