// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Export catalogue menu template. 
 * Includes export menu providing option to export to a file (TXT or CSV)
   either full list or individual object details.
 * These details contain all informations including calculated properties.
 * File is automatically created in dedicated 'Exports' folder (even if it doesn't exist it will be created).
 * Inputs are case insensitive, and menu choices validated.
 */

#ifndef EXPORT_CATALOGUE_MENU_H
#define EXPORT_CATALOGUE_MENU_H

#include "shared_functions.h"  // for open_output_file

#include<iostream> // for std::cout, std::cin
#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<string> // for std::string
#include<fstream> // for std::ofstream
#include<functional> // for std::function
#include<algorithm> // for std::transform
#include<filesystem> // for std::filesystem (C++17)

namespace fs = std::filesystem ; // Alias for filesystem namespace

template <typename T>
void export_catalogue_menu(
  const std::vector<std::shared_ptr<T>> &catalogue,
  const std::string &label,
  std::function<void(std::ostream &, const T &)> write_txt,
  std::function<void(std::ostream &, const T &)> write_csv,
  std::function<void(std::ostream &)> write_csv_header = nullptr,
  std::function<void(const T &)> export_single = nullptr,
  const std::string &info_text = "")
{
  // Check if the catalogue is empty
  if (catalogue.empty())
  {
    std::cout<<"\nNo "<<label<<" objects in the catalogue.\n";
    return;
  }

  // Display the catalogue
  std::cout<<"\n "<<label<<" Catalogue:\n";
  for(const auto &item : catalogue)
    std::cout<<" - "<<item->get_name()<<"\n";

    // Display the export menu
    std::cout<<"\n=== Export Menu ===\n";
    while(true) 
    {
      std::cout<<"\nWould you like to:\n"
        <<"  (1) Export the entire "<<label<<" catalogue\n"
        <<"  (2) Export an individual "<<label<<" by name\n"
        <<"  (3) Skip export\n"
        <<"  (4) Info\n"
        <<"Enter 1-4: ";
    
      std::string choice;
      std::getline(std::cin, choice);
    
      // === Choice 1: Export the entire catalogue ===
      if(choice == "1")
      {
        // Prompt for file name and format
        std::string filename, format, full_path;

        while (true) 
        {
          std::cout<<"Enter file name (without extension): ";
          std::getline(std::cin, filename);
        
          if (is_valid_filename(filename))
            break;
        
          std::cerr<<"Invalid filename. Avoid special characters like . , ! ? : ; etc.\n";
        }
        while (true)
        {
          std::cout<<"Export format? (txt / csv): ";
          std::getline(std::cin, format);
          std::transform(format.begin(), format.end(), format.begin(), ::tolower); // convert to lowercase
        
          if(format == "txt" || format == "csv")
          {
            fs::create_directories("Exports"); // Ensure the directory exists
            // Build full path
            full_path = "Exports/" + filename + "." + format;
            std::ofstream out = open_output_file(full_path);
            if (!out) return; // Check if the file can be opened
        
            if(format == "txt") 
            {
              for (const auto &item : catalogue)
                write_txt(out, *item), out << "\n";
            } 
            else 
            {
              if(write_csv_header) write_csv_header(out);
              for (const auto &item : catalogue)
                write_csv(out, *item);
            }
        
            break; // Successful export, exit format loop
          }
            std::cerr<<"Invalid format. Please choose 'txt' or 'csv'.\n"; // Invalid format message
        }
        // Close the file
        std::cout<<"Exported to "<<full_path<<"."<<"\n";
        break; // done
      }

      // === Choice 2: Export an individual object ===
      else if(choice == "2" && export_single)
      {
        while(true)
        {
          std::cout<<"Enter the exact name of the "<<label<<" to export: ";
          std::string target_name;
          std::getline(std::cin, target_name);
      
          bool found = false;
          for(const auto &item : catalogue)
          {
            std::string item_name = item->get_name();
            std::transform(item_name.begin(), item_name.end(), item_name.begin(), ::tolower);
            std::transform(target_name.begin(), target_name.end(), target_name.begin(), ::tolower);
      
            if(item_name == target_name)
            {
              export_single(*item);
              found = true;
              break;
            }
          }
      
          if(found)
            break; // Successful export, return to main menu
      
          std::cerr<<label<<" not found: "<<target_name<<"\n";
      
          if(!yes_no_handling("Would you like to try another name? (yes/no): "))
            break; // User chose not to retry
        }
      }

      // === Choice 3: Skip export ===
      else if(choice == "3")
      {
        std::cout<<"Export skipped.\n";
        break;
      }

      // === Choice 4: Info ===
      else if(choice == "4")
      {
        std::cout<<"\n--- "<<label<<" Info ---\n"<<info_text<<"\n";
      }
      else
      {
        std::cerr<<"Invalid input. Please enter 1, 2, 3 or 4.\n";
      }
    }
}

#endif // EXPORT_CATALOGUE_MENU_H