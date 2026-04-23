// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Exoplanet class implementation. Includes:
  * Set of physical constants for calculations.
  * Static counter to keep track of the number of exoplanets created.
  * Constructor and destructor, getters/setters for all properties.
  * Functions to calculate orbital period, surface gravity, density, escape velocity,
    habitable zone range and check if the exoplanet is in the habitable zone.
  * Print method to display exoplanet information. */

#include "exoplanet.h"

#include<iostream> // for std::cout, std::endl
#include<iomanip> // for std::setw
#include<stdexcept> // for std::invalid_argument, std::runtime_error
#include<cmath> // for std::sqrt, std::pow
#include<utility> // for std::pair

// Define static counter
int Exoplanet::counter = 0; // Initialize static counter to 0

// Physical constants
const double gravitational_constant = 6.67430e-11; // m^3 kg^-1 s^-2
const double earth_mass = 5.972e24; // kg
const double earth_radius = 6.371e6; // meters
const double number_pi = 3.141592653589793;

// Constructor
Exoplanet::Exoplanet(std::string name, std::string type, std::shared_ptr<Star> host_star_ptr, double planet_mass, double planet_radius,
  double semi_major_axis, double eccentricity) : CelestialObject(name, "Exoplanet"), type(type),
  planet_mass(planet_mass), planet_radius(planet_radius), semi_major_axis(semi_major_axis), eccentricity(eccentricity), host_star_ptr(std::move(host_star_ptr))
{
  ++counter;

  // Validate inputs
  if(planet_mass <= 0.0) 
  {
    throw std::invalid_argument("Planet mass must be positive.");
  }
  if(planet_radius <= 0.0) 
  {
    throw std::invalid_argument("Planet radius must be positive.");
  }
  if(semi_major_axis <= 0.0) 
  {
    throw std::invalid_argument("Semi-major axis must be positive.");
  }
  if(eccentricity < 0.0 || eccentricity >= 1.0) 
  {
    throw std::invalid_argument("Eccentricity must be in [0, 1).");
  }
}

// Destructor
Exoplanet::~Exoplanet() 
{
  --counter; // Decrement the counter when an exoplanet is destroyed
}

// Static function to get the number of exoplanets created
int Exoplanet::get_counter() 
{
  return counter; // Return the current count of exoplanets
}

// Setters
void Exoplanet::set_type(const std::string& type)
{
  if(type.empty()) throw std::invalid_argument("Type cannot be empty.");
  this->type = type;
}

void Exoplanet::set_planet_mass(double planet_mass)
{
  if(planet_mass <= 0) throw std::invalid_argument("Planet mass must be positive.");
  this->planet_mass = planet_mass;
}

void Exoplanet::set_planet_radius(double planet_radius)
{
  if(planet_radius <= 0) throw std::invalid_argument("Planet radius must be positive.");
  this->planet_radius = planet_radius;
}

void Exoplanet::set_semi_major_axis(double semi_major_axis)
{
  if(semi_major_axis <= 0) throw std::invalid_argument("Semi-major axis must be positive.");
  this->semi_major_axis = semi_major_axis;
}

void Exoplanet::set_eccentricity(double eccentricity)
{
  if(eccentricity < 0.0 || eccentricity >= 1.0) throw std::invalid_argument("Eccentricity must be in [0, 1).");
  this->eccentricity = eccentricity;
}

// Functions to calculate properties of the exoplanet
// Kepler's Third Law (a in AU, P in Earth years)
double Exoplanet::orbital_period() const 
{
  return std::sqrt(std::pow(semi_major_axis, 3));
}

// Surface gravity
double Exoplanet::surface_gravity() const 
{
  double mass_kg = planet_mass * earth_mass;
  double radius_m = planet_radius * earth_radius;
  return gravitational_constant * mass_kg / (radius_m * radius_m);
}

// Density
double Exoplanet::density() const 
{
  double volume = (4.0 / 3.0) * number_pi * std::pow(planet_radius * earth_radius, 3);
  return (planet_mass * earth_mass) / volume;
}

// Escape velocity
double Exoplanet::escape_velocity() const 
{
  double mass_kg = planet_mass * earth_mass;
  double radius_m = planet_radius * earth_radius;
  return std::sqrt(2 * gravitational_constant * mass_kg / radius_m);
}

// Habitable zone (based on host star's luminosity)
std::pair<double, double> Exoplanet::habitable_zone() const 
{
  if (!host_star_ptr)
  {
    throw std::runtime_error("Host star not assigned");
  }

  double L = host_star_ptr->luminosity(); // in solar units
  double inner = 0.95 * std::sqrt(L);
  double outer = 1.67 * std::sqrt(L);
  return {inner, outer};
}

// Check if the exoplanet is in the habitable zone
bool Exoplanet::is_in_habitable_zone() const 
{
  auto range = habitable_zone();
  double inner = range.first;
  double outer = range.second;
  return semi_major_axis >= inner && semi_major_axis <= outer;
}

// Print method
void Exoplanet::print() const 
{
  std::cout<<"\n=== Exoplanet Information ===\n"
    <<std::left
    <<std::setw(30)<<"Object Type:"<<get_object_type()<<"\n"
    <<std::setw(30)<<"Name:"<<get_name()<<"\n"
    <<std::setw(30)<<"Type:"<<type<<"\n"
    <<std::setw(30)<<"Host Star:"<<get_host_star_name()<<"\n"
    <<std::setw(30)<<"Mass [M_Earth]:"<<planet_mass<<"\n"
    <<std::setw(30)<<"Radius [R_Earth]:"<<planet_radius<<"\n"
    <<std::setw(30)<<"Semi-Major Axis [AU]:"<<semi_major_axis<<"\n"
    <<std::setw(30)<<"Eccentricity:"<<eccentricity<<"\n"
    <<std::setw(30)<<"Orbital Period [yr]:"<<orbital_period()<<"\n"
    <<std::setw(30)<<"Surface Gravity [m/s^2]:"<<surface_gravity()<<"\n"
    <<std::setw(30)<<"Density [kg/m^3]:"<<density()<<"\n"
    <<std::setw(30)<<"Escape Velocity [m/s]:"<<escape_velocity()<<"\n";

  // Check if the host star is assigned before calculating habitable zone
  // If not, print "N/A" for habitable zone range and in habitable zone status
  if(host_star_ptr) 
  {
    auto range = habitable_zone();
    double inner = range.first;
    double outer = range.second;
    std::cout<<std::setw(30)<<"Habitable Zone Range [AU]:"<<inner<<" - "<<outer<<"\n"
      <<std::setw(30)<<"In Habitable Zone:"<<(is_in_habitable_zone() ? "Yes" : "No")<<"\n";
  } 
  else 
  {
    std::cout<<std::setw(30)<<"Habitable Zone Range [AU]:"<<"N/A"<<"\n"
      <<std::setw(30)<<"In Habitable Zone:"<<"N/A"<<"\n";
  }
  std::cout<<"===============================\n";
}
