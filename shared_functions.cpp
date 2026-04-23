// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Shared functions implementation file.
 * This file contains functions for handling user input, file operations, and validation.
 * It includes functions for yes/no handling, file opening, filename validation,
   and reading validated double values.
 * The functions are designed to be reusable and modular, allowing for easy integration into other parts of the program.
 * The functions also include error handling for invalid inputs and file operations.  
 */

#include "shared_functions.h"

#include<iostream> // for std::cout, std::endl
#include<fstream> // for std::ofstream
#include<string> // for std::string
#include<algorithm> // for std::transform
#include<sstream> // for std::istringstream

// Function to handle yes/no input from the user
bool yes_no_handling(const std::string& prompt) 
{
  std::string response;

  // Prompt the user until a valid response is received
  // Convert the response to lowercase for case-insensitive comparison
  while(true) 
  {
    std::cout << prompt;
    std::getline(std::cin, response);

    std::transform(response.begin(), response.end(), response.begin(), ::tolower);

    if(response == "yes" || response == "y") return true;
    if(response == "no"  || response == "n") return false;

    // If the response is invalid, prompt again
    std::cout<<"Invalid input. Please type 'yes' or 'no'.\n";
  }
}
// Open output file with error handling
std::ofstream open_output_file(const std::string &filename)
{
  std::ofstream out(filename);
  if(!out)
    std::cerr<<"Error: Could not open file: "<<filename<<"\n";
  return out;
}

// Check if the filename is valid (no invalid characters)
bool is_valid_filename(const std::string& name) 
{
  // Disallowed characters in most file systems
  const std::string invalid_chars = R"(\/:*?"<>|.,!?:;)"; // Add more characters as needed
  return !name.empty() && name.find_first_of(invalid_chars) == std::string::npos; // Check if name is empty or contains invalid characters
}

// Read a validated double from user input
double get_validated_double(const std::string& prompt) 
{
  double value; // Prompt the user for a double value
  // Loop until a valid double is entered
  while (true) 
  {
    std::cout<<prompt;
    std::string line;
    std::getline(std::cin, line); // Read a line of input
    std::istringstream iss(line); // Create a string stream from the input line
    if (iss>>value&&iss.eof()) // Check if the input is a valid double and no extra characters are present
    {
      return value;
    }
    std::cerr<<"Invalid input. Please enter a number (e.g. 1.0 or 2e-3).\n";
  }
}



