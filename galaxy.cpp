// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Galaxies class implementation. */

#include "galaxy.h"

#include<iostream> // for std::cout, std::endl
#include<string> // for std::string
#include<cmath> // for std::log10
#include<stdexcept> // For std::invalid_argument
#include<iomanip> // for std::setw, std::left, std::right

// Define static counter
int Galaxy::counter = 0; // Initialize static counter to 0

Galaxy::Galaxy(std::string name, std::string constellation, double distance, double apparent_magnitude, double redshift)
  : CelestialObject(name, "Galaxy"), constellation(constellation), distance(distance), apparent_magnitude(apparent_magnitude), redshift(redshift) 
  {

    // Validate inputs
    if(name.empty()) // Check if name is empty
    {  
      throw std::invalid_argument("Name cannot be empty.");
    }

    if(constellation.empty()) // Check if constellation is empty
    {  
      throw std::invalid_argument("Constellation cannot be empty.");
    }

    if(distance <= 0.0 || distance > 10000.0) // Check if distance is within observable range
    {  
      throw std::invalid_argument("Distance must be within (0-10000] Mpc (Edge of Observable Universe).");
    }

    if(apparent_magnitude < -30.0 || apparent_magnitude > 30.0) // Check if apparent magnitude is out of range
    {  
      throw std::invalid_argument("Apparent magnitude is out of expected range (-30 < m < 30).");
    }

    if(redshift < 0.0) // Check if redshift is negative
    {  
      throw std::invalid_argument("Redshift cannot be negative.");
    }

    ++counter; // Count the object only after validation, so a rejected object is not counted
  }

// Destructor
Galaxy::~Galaxy() 
{
  --counter; // Decrement the counter when a galaxy is destroyed
}

// Static function to get the number of galaxies created
int Galaxy::get_counter() 
{
  return counter; // Return the current count of galaxies
}

// Setters
void Galaxy::set_constellation(const std::string& constellation) // Setter for constellation
{
  if(constellation.empty()) 
  {
    throw std::invalid_argument("Constellation cannot be empty.");
  }
  this->constellation = constellation;
}


void Galaxy::set_distance(double distance) // Setter for distance
{
  if(distance <= 0.0 || distance > 10000.0)
  {
    throw std::invalid_argument("Distance must be within (0-10000] Mpc.");
  }
  this->distance = distance;
}

void Galaxy::set_apparent_magnitude(double apparent_magnitude) // Setter for apparent magnitude
{
  if(apparent_magnitude < -30.0 || apparent_magnitude > 30.0)
  {
    throw std::invalid_argument("Apparent magnitude is out of expected range (-30 < m < 30).");
  }
  this->apparent_magnitude = apparent_magnitude;
}

void Galaxy::set_redshift(double redshift) // Setter for redshift
{
  if(redshift < 0.0)
  {
    throw std::invalid_argument("Redshift cannot be negative.");
  }
  this->redshift = redshift;
}

// Function to calculate the absolute magnitude of the galaxy
double Galaxy::absolute_magnitude() const 
{
  double distance_pc = distance * 1.0e6; // Mpc to pc
  return apparent_magnitude - 5.0 * std::log10(distance_pc / 10.0);
}

// Function to calculate the velocity of the galaxy
double Galaxy::velocity() const 
{
  const double speed_of_light_km_s = 299792.458;
  return redshift * speed_of_light_km_s;
}

// Function to print the details of the galaxy
void Galaxy::print() const
{
  std::cout<<"\n=== Galaxy Information ===\n"
    <<std::left<<std::setw(30)<<"Object Type:"<<get_object_type()<<"\n"
    <<std::setw(30)<<"Name:"<<get_name()<<"\n"
    <<std::setw(30)<<"Constellation:"<<constellation<<"\n"
    <<std::setw(30)<<"Distance [Mpc]:"<<distance<<"\n"
    <<std::setw(30)<<"Apparent Magnitude:"<<apparent_magnitude<<"\n"
    <<std::setw(30)<<"Redshift:"<<redshift<<"\n"
    <<std::setw(30)<<"Velocity [km/s]:"<<velocity()<<"\n"
    <<std::setw(30)<<"Absolute Magnitude:"<<absolute_magnitude()<<"\n"
    <<"===============================\n";
}