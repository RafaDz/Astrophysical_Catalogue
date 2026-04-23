// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Header file to load celestial objects. */

#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<string> // for std::string

// Function prototypes for loading objects from CSV files
// These functions take a filename and return a vector of shared pointers to the respective object type
std::vector<std::shared_ptr<Galaxy>> load_galaxies_from_csv(const std::string& filename);
std::vector<std::shared_ptr<Star>> load_stars_from_csv(const std::string& filename);
std::vector<std::shared_ptr<Exoplanet>> load_exoplanets_from_csv(const std::string& filename, const std::vector<std::shared_ptr<Star>>& star_catalogue);

#endif
