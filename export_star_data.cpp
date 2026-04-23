// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Star export functions implementation file.
 * Function to write a csv header.
 * Function to export properties to txt.
 * Function to export properties to csv.
 * Function to export single star details.
 */

#include "star.h"
#include "shared_functions.h"
#include "export_template.h"

#include<fstream> // for std::ofstream
#include<iomanip> // for std::setw
#include<iostream> // for std::ostream
#include<string> // for std::string
#include<vector> // for std::vector
#include<algorithm> // for std::transform
#include<memory> // for std::shared_ptr

// Write CSV header for stars
void write_star_csv_header(std::ostream& out)
{
  out<<"object_type,name,type,spectral_class,mass,radius,temperature,age,apparent_magnitude,distance,luminosity,gravity\n";
}

// Write a single star in TXT format
void write_star_txt(std::ostream& out, const Star& star)
{
  out<<std::left
     <<std::setw(30)<<"Object Type:"<<star.get_object_type()<<"\n"
     <<std::setw(30)<<"Name:"<<star.get_name()<<"\n"
     <<std::setw(30)<<"Type:"<<star.get_type()<<"\n"
     <<std::setw(30)<<"Spectral Class:"<<star.get_spectral_class()<<"\n"
     <<std::setw(30)<<"Mass [M_Sun]:"<<star.get_mass()<<"\n"
     <<std::setw(30)<<"Radius [R_Sun]:"<<star.get_radius()<<"\n"
     <<std::setw(30)<<"Temperature [K]:"<<star.get_temperature()<<"\n"
     <<std::setw(30)<<"Age [Gyr]:"<<star.get_age()<<"\n"
     <<std::setw(30)<<"Apparent Magnitude:"<<star.get_apparent_magnitude()<<"\n"
     <<std::setw(30)<<"Distance [Mpc]:"<<star.get_distance()<<"\n"
     <<std::setw(30)<<"Absolute Magnitude:"<<star.absolute_magnitude()<<"\n"
     <<std::setw(30)<<"Luminosity [L_Sun]:"<<star.luminosity()<<"\n"
     <<std::setw(30)<<"Surface Gravity [m/s^2]:"<<star.gravitational_strength()<<"\n";
}

// Write a single star in CSV format
void write_star_csv(std::ostream& out, const Star& star)
{
  out<<star.get_object_type()<<","<<star.get_name()<<","<<star.get_type()<<","<<star.get_spectral_class()<<",";
  out<<star.get_mass()<<","<<star.get_radius()<<","<<star.get_temperature()<<","<<star.get_age()<<",";
  out<<star.get_apparent_magnitude()<<","<<star.get_distance()<<","<<star.absolute_magnitude()<<","<<star.luminosity()<<","<<star.gravitational_strength()<<"\n";
}

// Export a single star to file
void export_star_to_file(const Star& star)
{
  export_object_to_file<Star>(
    star,
    "star",
    write_star_txt,
    write_star_csv,
    write_star_csv_header
  );
}
