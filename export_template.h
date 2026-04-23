// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Export to a file template file.
 * This template can be applied to all derived classes.
 * The function prompts the user for a filename and format, and handles file creation and writing.
 * It also includes error handling for invalid filenames and formats.
 * The function uses a lambda function to write the object to the file in the specified format.
 * The function is designed to be reusable for different types of objects.
 */

#ifndef EXPORT_TEMPLATE_H
#define EXPORT_TEMPLATE_H

#include "shared_functions.h"

#include<string> // for std::string
#include<fstream> // for std::ofstream
#include<functional> // for std::function
#include<iostream> // for std::cout, std::cin
#include<algorithm> // for std::transform

#include <filesystem>
namespace fs = std::filesystem; // for filesystem operations

// Templated function to export a single object to file
template <typename T>
void export_object_to_file(
  const T& object,
  const std::string& label,
  std::function<void(std::ostream&, const T&)> write_txt,
  std::function<void(std::ostream&, const T&)> write_csv,
  std::function<void(std::ostream&)> write_csv_header = nullptr)
{
  if(!yes_no_handling("\nWould you like to export this " + label + "'s data to a file? (yes/no): ")) 
  {
    std::cout<<"Export skipped.\n";
    return;
  }

  std::string filename, format, full_path; // file name and format

  // Prompt for file name
  // Loop until a valid filename is provided
  while(true) 
  {
    std::cout<<"Enter file name (without extension): ";
    std::getline(std::cin, filename);
  
    if (is_valid_filename(filename))
      break;
  
    std::cerr<<"Invalid filename. Avoid special characters like . , ! ? : ; etc.\n";
  }

  // Prompt for export format
  // Loop until a valid format is chosen
  while(true)
  {
    std::cout<<"Export format? (txt / csv): ";
    std::getline(std::cin, format);
    std::transform(format.begin(), format.end(), format.begin(), ::tolower);

    if(format == "txt" || format == "csv")
    {
      fs::create_directories("Exports");
      full_path = "Exports/" + filename + "." + format;

      std::ofstream out = open_output_file(full_path);
      if(!out) return;

      if(format == "txt")
      {
        write_txt(out, object);
      }
      else // format == "csv"
      {
        if(write_csv_header) write_csv_header(out);
        write_csv(out, object);
      }

      break; // Export successful
    }

    std::cerr<<"Invalid format. Please choose 'txt' or 'csv'.\n";
  }
  std::cout<<"Exported to "<<full_path<<".\n";
}

#endif // EXPORT_TEMPLATE_H