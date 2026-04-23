// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Header file to handle export menu. */

#ifndef HANDLE_EXPORT_MENU_H
#define HANDLE_EXPORT_MENU_H

#include "star.h"
#include "galaxy.h" 
#include "exoplanet.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr

// Declarations for sorting menu handlers
void handle_star_menu(std::vector<std::shared_ptr<Star>>& star_catalogue);
void handle_galaxy_menu(std::vector<std::shared_ptr<Galaxy>>& galaxy_catalogue);
void handle_exoplanet_menu(std::vector<std::shared_ptr<Exoplanet>>& exoplanet_catalogue);

#endif // HANDLE_EXPORT_MENU_H
