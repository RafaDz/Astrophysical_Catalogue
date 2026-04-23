// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Export galaxy data header file. */

#ifndef EXPORT_GALAXY_DATA_H
#define EXPORT_GALAXY_DATA_H

#include "galaxy.h"

#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<iostream> // for std::ostream

// Function prototypes for exporting galaxy data
// These functions will handle the export of galaxy data to different formats
void export_galaxy_to_file(const Galaxy& galaxy);
void write_galaxy_txt(std::ostream&, const Galaxy&);
void write_galaxy_csv(std::ostream&, const Galaxy&);
void write_csv_header(std::ostream&);  // For Galaxy

#endif // EXPORT_GALAXY_DATA_H