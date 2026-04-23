// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Galaxy export functions implementation file. 
 * Function to write a csv header.
 * Function to export properties to txt.
 * Function to export properties to csv.
 * Function to export single galaxy details.
 */

#include "galaxy.h"
#include "shared_functions.h"
#include "export_template.h"

#include<fstream> // for std::ofstream
#include<iomanip> // for std::setw
#include<string> // for std::string
#include<iostream> // for std::cout, std::endl
#include<algorithm> // for std::transform
#include<vector> // for std::vector
#include<memory> // for std::shared_ptr
#include<functional> // for std::function

// Write CSV header
void write_csv_header(std::ostream& out)
{
  out<<"object_type,name,constellation,distance_mpc,apparent_magnitude,redshift,velocity_km_s,absolute_magnitude\n";
}

// Write single galaxy in TXT format
void write_galaxy_txt(std::ostream& out, const Galaxy& galaxy)
{
  // Output the galaxy data in a formatted manner
  out<<std::left
     <<std::setw(30)<<"Object Type:"<<galaxy.get_object_type()<<"\n"
     <<std::setw(30)<<"Name:"<<galaxy.get_name()<<"\n"
     <<std::setw(30)<<"Constellation:"<<galaxy.get_constellation()<<"\n"
     <<std::setw(30)<<"Distance [Mpc]:"<<galaxy.get_distance()<<"\n"
     <<std::setw(30)<<"Apparent Magnitude:"<<galaxy.get_apparent_magnitude()<<"\n"
     <<std::setw(30)<<"Redshift:"<<galaxy.get_redshift()<<"\n"
     <<std::setw(30)<<"Velocity [km/s]:"<<galaxy.velocity()<<"\n"
     <<std::setw(30)<<"Absolute Magnitude:"<<galaxy.absolute_magnitude()<<"\n";
}

// Write single galaxy in CSV format
void write_galaxy_csv(std::ostream& out, const Galaxy& galaxy)
{
  // Output the galaxy data in CSV format (coma-separated values)
  out<<galaxy.get_object_type()<<","<<galaxy.get_name()<<","<<galaxy.get_constellation()<<","<<galaxy.get_distance()<<","
    <<galaxy.get_apparent_magnitude()<<","<<galaxy.get_redshift()<<","<<galaxy.velocity()<<","<<galaxy.absolute_magnitude()<<"\n";
}

// Export a single galaxy to file
void export_galaxy_to_file(const Galaxy& galaxy)
{
  export_object_to_file<Galaxy>(
    galaxy,
    "galaxy",
    write_galaxy_txt,
    write_galaxy_csv,
    write_csv_header
  );
}
