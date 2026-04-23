// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Shared functions header file. */

#ifndef SHARED_FUNCTIONS_H
#define SHARED_FUNCTIONS_H

#include<string> // for std::string
#include<fstream> // for std::ofstream

// Ask user yes/no with validation
bool yes_no_handling(const std::string &prompt);

// Open file and return std::ofstream with error message if failed
std::ofstream open_output_file(const std::string &filename);

// Check if the filename is valid (no invalid characters)
bool is_valid_filename(const std::string &name);

// Read a validated double from user input
double get_validated_double(const std::string &prompt);

#endif // SHARED_FUNCTIONS_H
