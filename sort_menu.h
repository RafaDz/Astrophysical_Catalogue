// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Sorting menu header file. */

#ifndef SORT_MENU_H
#define SORT_MENU_H

#include "sort_template.h"
#include "galaxy.h"
#include "star.h"
#include "exoplanet.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<string> // for std::string
#include<iostream> // for std::cout, std::endl
#include<algorithm> // for std::transform

// Function prototypes for sorting menus
// These functions will display sorting options and handle user input for sorting
void galaxy_sort_menu(std::vector<std::shared_ptr<Galaxy>>& galaxies);
void star_sort_menu(std::vector<std::shared_ptr<Star>>& stars);
void exoplanet_sort_menu(std::vector<std::shared_ptr<Exoplanet>>& planets);

#endif // SORT_MENU_H
