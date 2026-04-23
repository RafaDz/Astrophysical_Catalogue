// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Export exoplanet data header file. */

#ifndef EXPORT_EXOPLANET_DATA_H
#define EXPORT_EXOPLANET_DATA_H

#include "exoplanet.h"

#include<vector> // for std::vector
#include<iostream> // for std::ostream
#include<memory> // for std::shared_ptr

// Function prototypes for exporting exoplanet data
// These functions will handle the export of exoplanet data to different formats
void export_exoplanet_to_file(const Exoplanet &exoplanet);
void write_exoplanet_txt(std::ostream&, const Exoplanet&);
void write_exoplanet_csv(std::ostream&, const Exoplanet&);
void write_exoplanet_csv_header(std::ostream&); // For Exoplanet

#endif // EXPORT_EXOPLANET_DATA_H
