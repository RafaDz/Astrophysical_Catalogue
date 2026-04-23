// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Exoplanet export functions implementation file.
 * Function to write a csv header.
 * Function to export properties to txt.
 * Function to export properties to csv.
 * Function to export single exoplanet details. */

#include "exoplanet.h"
#include "shared_functions.h"
#include "export_template.h"

#include<fstream> // for std::ofstream
#include<iomanip> // for std::setw
#include<iostream> // for std::ostream
#include<string> // for std::string
#include<vector> // for std::vector
#include<algorithm> // for std::transform 
#include<memory> // for std::shared_ptr

// CSV header for exoplanets (with computed values)
void write_exoplanet_csv_header(std::ostream& out)
{
  // CSV header for exoplanets (with computed values)
  out<<"object_type,name,type,host_star,mass,radius,semi_major_axis,eccentricity,";
  out<<"orbital_period,surface_gravity,density,escape_velocity,";
  out<<"habitable_range,is_habitable\n";
}

// Write single exoplanet in TXT format
void write_exoplanet_txt(std::ostream& out, const Exoplanet& exoplanet)
{
  // Output the exoplanet data in a formatted manner
  out<<std::left
     <<std::setw(30)<<"Object Type:"<<exoplanet.get_object_type()<<"\n"
     <<std::setw(30)<<"Name:"<<exoplanet.get_name()<<"\n"
     <<std::setw(30)<<"Type:"<<exoplanet.get_type()<<"\n"
     <<std::setw(30)<<"Host Star:"<<(exoplanet.get_host_star() ? exoplanet.get_host_star()->get_name() : "N/A")<<"\n"
     <<std::setw(30)<<"Mass [M_Earth]:"<<exoplanet.get_planet_mass()<<"\n"
     <<std::setw(30)<<"Radius [R_Earth]:"<<exoplanet.get_planet_radius()<<"\n"
     <<std::setw(30)<<"Semi-Major Axis [AU]:"<<exoplanet.get_semi_major_axis()<<"\n"
     <<std::setw(30)<<"Eccentricity:"<<exoplanet.get_eccentricity()<<"\n"
     <<std::setw(30)<<"Orbital Period [yr]:"<<exoplanet.orbital_period()<<"\n"
     <<std::setw(30)<<"Surface Gravity [m/s^2]:"<<exoplanet.surface_gravity()<<"\n"
     <<std::setw(30)<<"Density [kg/m^3]:"<<exoplanet.density()<<"\n"
     <<std::setw(30)<<"Escape Velocity [m/s]:"<<exoplanet.escape_velocity()<<"\n";

  // Habitable zone and habitability
  try 
  {
    auto range = exoplanet.habitable_zone();
    double inner = range.first;
    double outer = range.second;

    out<<std::setw(30)<<"Habitable Zone [AU]:"<<inner<<" - "<<outer<<"\n"
       <<std::setw(30)<<"In Habitable Zone:"<<(exoplanet.is_in_habitable_zone() ? "Yes" : "No")<<"\n";
  } 
  catch (...) // Catch any exceptions from habitable zone calculation
  {
    out<<std::setw(30)<<"Habitable Zone [AU]:"<<"N/A\n" // If an exception occurs, set values to N/A
       <<std::setw(30)<<"In Habitable Zone:"<<"N/A\n";
  }
}

// Write single exoplanet in CSV format
void write_exoplanet_csv(std::ostream& out, const Exoplanet& exoplanet)
{
  // CSV header for exoplanets (with computed values)
  out<<exoplanet.get_object_type()<<","<<exoplanet.get_name()<<","<<exoplanet.get_type()<<",";
  out<<(exoplanet.get_host_star() ? exoplanet.get_host_star()->get_name() : "N/A")<<",";
  out<<exoplanet.get_planet_mass()<<","<<exoplanet.get_planet_radius()<<","<<exoplanet.get_semi_major_axis()<<","<<exoplanet.get_eccentricity()<<",";
  out<<exoplanet.orbital_period()<<","<<exoplanet.surface_gravity()<<","<<exoplanet.density()<<","<<exoplanet.escape_velocity()<<",";

  // Habitable zone and habitability
  try 
  {
    auto range = exoplanet.habitable_zone();
    double inner = range.first;
    double outer = range.second;
    out<<inner<<"-"<<outer<<","<<(exoplanet.is_in_habitable_zone() ? "Yes" : "No");
  } 
  catch (...) // Catch any exceptions from habitable zone calculation
  {
    out<<"N/A,N/A"; // If an exception occurs, set values to N/A
  }
  out<<"\n";
}

// Export a single exoplanet to file
void export_exoplanet_to_file(const Exoplanet& exoplanet)
{
  export_object_to_file<Exoplanet>(
    exoplanet,
    "exoplanet",
    write_exoplanet_txt,
    write_exoplanet_csv,
    write_exoplanet_csv_header
  );
}