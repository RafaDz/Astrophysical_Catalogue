// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Star class implementation. 
 * This class represents a star and includes properties such as mass, radius, temperature, 
   age, apparent magnitude, and distance.
 * It also includes methods to calculate luminosity, surface gravity, and absolute magnitude.
 * Includes function to print star details in the console.
 * The class is derived from CelestialObject and includes error handling for invalid inputs.
 * The class maintains a static counter to keep track of the number of stars created.
 * The constructor initializes the star's properties and validates the inputs.
 * The destructor decrements the counter when a star is destroyed.
 * The class includes getters and setters for all properties, with validation in the setters.
 */

#include "star.h"

#include<iostream> // for std::cout
#include<stdexcept> // for std::invalid_argument
#include<cmath> // for std::pow, std::log10
#include<string> // for std::string
#include<iomanip> // for std::setw, std::left, std::right

// Define static counter
int Star::counter = 0; // Initialize static counter to 0

// Constants
const double number_pi = 3.141592653589793;
const double stefan_boltzmann = 5.670374419e-8;     // W·m⁻²·K⁻⁴
const double solar_radius = 6.957e8;                // meters
const double solar_mass = 1.9885e30;                // kg
const double gravitational_constant = 6.67430e-11;  // m³·kg⁻¹·s⁻²
const double solar_luminosity = 3.828e26;           // watts

// Constructor
Star::Star(std::string name, std::string type, std::string spectral_class, double mass, double radius, double temperature, 
          double age, double apparent_magnitude, double distance)
  : CelestialObject(name, "Star"), type(type), spectral_class(spectral_class), mass(mass), radius(radius), 
          temperature(temperature), age(age), apparent_magnitude(apparent_magnitude), distance(distance)
{

  if(name.empty() || type.empty() || spectral_class.empty())
  {  
    throw std::invalid_argument("Name, type, and spectral class must not be empty.");
  }
  if(mass <= 0.0)
  {  
    throw std::invalid_argument("Mass must be positive.");
  }
  if(radius <= 0.0)
  {    
    throw std::invalid_argument("Radius must be positive.");
  }
  if(temperature <= 0.0)
  {  
    throw std::invalid_argument("Temperature must be positive.");
  }
  if(age < 0.0)
  {  
    throw std::invalid_argument("Age cannot be negative.");
  }
  if(apparent_magnitude < -30.0 || apparent_magnitude > 30.0)
  {  
    throw std::invalid_argument("Apparent magnitude is out of expected range (-30 < m < 30).");
  }
  if(distance < 0.0)
  {
    throw std::invalid_argument("Distance cannot be negative.");
  }

  ++counter; // Count the object only after validation, so a rejected object is not counted
}

// Destructor
Star::~Star() 
{
  --counter; // Decrement the counter when a star is destroyed
}

// Static function to get the number of stars created
int Star::get_counter() 
{
  return counter; // Return the current count of stars
}

// Setters with validation
void Star::set_type(const std::string& type_new) 
{
  if(type_new.empty()) 
    throw std::invalid_argument("Type cannot be empty.");
    type = type_new;
}

void Star::set_spectral_class(const std::string& spectral_class_new) 
{
  if(spectral_class_new.empty()) 
  {
    throw std::invalid_argument("Spectral class cannot be empty.");
  }
    spectral_class = spectral_class_new;
}

void Star::set_mass(double mass_new) 
{
  if(mass_new <= 0.0) 
  {
    throw std::invalid_argument("Mass must be positive.");
  } 
    mass = mass_new;
}

void Star::set_radius(double radius_new) 
{
  if(radius_new <= 0.0)
  {
    throw std::invalid_argument("Radius must be positive.");
  }
    radius = radius_new;
}

void Star::set_temperature(double temperature_new) 
{
  if(temperature_new <= 0.0)
  {
    throw std::invalid_argument("Temperature must be positive.");
  }
    temperature = temperature_new;
}

void Star::set_age(double age_new) 
{
  if(age_new < 0.0)
  { 
    throw std::invalid_argument("Age cannot be negative.");
  }
    age = age_new;
}

void Star::set_apparent_magnitude(double apparent_magnitude_new) 
{
  if(apparent_magnitude_new < -30.0 || apparent_magnitude_new > 30.0) 
  {
    throw std::invalid_argument("Apparent magnitude is out of expected range (-30 < m < 30).");
  }
    apparent_magnitude = apparent_magnitude_new;
}

void Star::set_distance(double distance_new) 
{
  if(distance_new < 0.0) 
  {
    throw std::invalid_argument("Distance cannot be negative.");
  }
    distance = distance_new;
}

// Luminosity
double Star::luminosity() const 
{
  double R_m = radius * solar_radius; // Convert radius to meters
  double L_w = 4 * number_pi * std::pow(R_m, 2) * stefan_boltzmann * std::pow(temperature, 4); // Luminosity in watts
  return L_w / solar_luminosity; // Luminosity in solar units
}

// Surface gravity
double Star::gravitational_strength() const 
{
  double R_m = radius * solar_radius; // Convert radius to meters
  double M_kg = mass * solar_mass; // Convert mass to kg
  return gravitational_constant * M_kg / std::pow(R_m, 2); // Gravitational strength in m/s²
}

// Absolute magnitude
double Star::absolute_magnitude() const 
{
  double distance_pc = distance * 1.0e6; // Mpc to pc
  return apparent_magnitude - 5.0 * std::log10(distance_pc / 10.0);
}

// Print function
void Star::print() const
{
  std::cout<<"\n=== Star Information ===\n"
    <<std::left
    <<std::setw(30)<<"Object Type:"<<get_object_type()<<"\n"
    <<std::setw(30)<<"Name:"<<get_name()<<"\n"
    <<std::setw(30)<<"Type:"<<type<<"\n"
    <<std::setw(30)<<"Spectral Class:"<<spectral_class<<"\n"
    <<std::setw(30)<<"Mass [M_Sun]:"<<mass<<"\n"
    <<std::setw(30)<<"Radius [R_Sun]:"<<radius<<"\n"
    <<std::setw(30)<<"Temperature [K]:"<<temperature<<"\n"
    <<std::setw(30)<<"Age [Gyr]:"<<age<<"\n"
    <<std::setw(30)<<"Apparent Magnitude:"<<apparent_magnitude<<"\n"
    <<std::setw(30)<<"Distance [Mpc]:"<<distance<<"\n"
    <<std::setw(30)<<"Absolute Magnitude:"<<absolute_magnitude()<<"\n"
    <<std::setw(30)<<"Luminosity [L_Sun]:"<<luminosity()<<"\n"
    <<std::setw(30)<<"Surface Gravity [m/s^2]:"<<gravitational_strength()<<"\n"
    <<"===============================\n";
}
