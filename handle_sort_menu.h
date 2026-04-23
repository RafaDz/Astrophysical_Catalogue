// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Header file to handle sorting menu. */

#ifndef HANDLE_SORT_MENU_H
#define HANDLE_SORT_MENU_H

#include "star.h"
#include "galaxy.h" 
#include "exoplanet.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr

// Declarations for sorting menu handlers
void handle_sort_star_menu(std::vector<std::shared_ptr<Star>>& star_catalogue);
void handle_sort_galaxy_menu(std::vector<std::shared_ptr<Galaxy>>& galaxy_catalogue);
void handle_sort_exoplanet_menu(std::vector<std::shared_ptr<Exoplanet>>& exoplanet_catalogue);

#endif // HANDLE_SORT_MENU_H
