// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Adding an object header file. */

#ifndef ADD_OBJECT_H
#define ADD_OBJECT_H

#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr

// Function prototypes for adding objects to the catalogue
void add_galaxy_entry(std::vector<std::shared_ptr<Galaxy>>& catalogue);
void add_star_entry(std::vector<std::shared_ptr<Star>>& catalogue);
void add_exoplanet_entry(std::vector<std::shared_ptr<Exoplanet>>& catalogue, const std::vector<std::shared_ptr<Star>>& star_catalogue);

#endif // ADD_OBJECT_H
