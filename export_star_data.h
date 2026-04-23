// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Export star data header file. */

#ifndef EXPORT_STAR_DATA_H
#define EXPORT_STAR_DATA_H

#include "star.h"

#include<vector> // For std::vector
#include<memory> // For std::shared_ptr
#include<iostream> // For std::ostream

// Function prototypes for exporting star data to file
// These functions will handle the export of stars data to different formats
void export_star_to_file(const Star &star);
void write_star_txt(std::ostream&, const Star&);
void write_star_csv(std::ostream&, const Star&);
void write_star_csv_header(std::ostream&); // For Star

#endif // EXPORT_STAR_DATA_H
