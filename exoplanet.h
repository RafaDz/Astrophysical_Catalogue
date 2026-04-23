// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Exoplanets class header. */

#include "celestial_objects.h"
#include "star.h" // Include the Star class header for host star reference

#ifndef EXOPLANET_H
#define EXOPLANET_H
#include<iostream> // for std::cout, std::endl
#include<cmath> // for std::sqrt, std::pow
#include<vector> // for std::vector
#include<string> // for std::string
#include<memory> // for std::shared_ptr
#include<utility> // for std::pair

class Exoplanet : public CelestialObject {  // Class representing a star, derived from CelestialObject
  private: 
    static int counter; // Static member to count the number of exoplanets created

  protected: // Protected members
    std::string type;  // Type of the exoplanet (e.g., terrestrial, gas giant)
    std::shared_ptr<Star> host_star_ptr; // Name of the host star
    double planet_mass; // Mass of the exoplanet in Earth masses
    double planet_radius; // Radius of the exoplanet in Earth radii
    double semi_major_axis; // Semi-major axis of the exoplanet's orbit in AU
    double eccentricity; // Eccentricity of the exoplanet's orbit
    
  
  public:
    // Constructor
    Exoplanet(std::string name, std::string type, std::shared_ptr<Star> host_star_ptr, double planet_mass, double planet_radius,
      double semi_major_axis, double eccentricity);

    // Destructor
    virtual ~Exoplanet();

    // Function to get the number of exoplanets created
    static int get_counter(); // Getter for counter

    // Getters
    std::string get_type() const { return type; } // Getter for type
    std::shared_ptr<Star> get_host_star() const { return host_star_ptr; } // Getter for host star pointer
    std::string get_host_star_name() const // Getter for host star name
    {
      return host_star_ptr ? host_star_ptr->get_name() : "N/A"; // Return "N/A" if host star is not assigned
    } 
    double get_planet_mass() const { return planet_mass; } // Getter for planet mass
    double get_planet_radius() const { return planet_radius; } // Getter for planet radius
    double get_semi_major_axis() const { return semi_major_axis; } // Getter for semi-major axis
    double get_eccentricity() const { return eccentricity; } // Getter for eccentricity

    // Setters
    void set_type(const std::string& type); // Setter for type
    void set_host_star(std::shared_ptr<Star> star_ptr); // Setter for host star pointer
    void set_host_star_name(const std::string& star_name); // Setter for host star name
    void set_planet_mass(double planet_mass); // Setter for planet mass
    void set_planet_radius(double planet_radius); // Setter for planet radius
    void set_semi_major_axis(double semi_major_axis); // Setter for semi-major axis
    void set_eccentricity(double eccentricity); // Setter for eccentricity

    // Function to calculate the orbital period of the exoplanet (Kepler's Third Law)
    virtual double orbital_period() const;

    // Function to calculate the surface gravity of the exoplanet
    virtual double surface_gravity() const;

    // Function to calculate density of the exoplanet
    virtual double density() const;

    // Function to find the habitable zone of the exoplanet
    virtual std::pair<double, double> habitable_zone() const;

    // Function to calculate the escape velocity of the exoplanet
    virtual double escape_velocity() const;

    // Function to check whether the exoplanet is in the habitable zone
    virtual bool is_in_habitable_zone() const;

    // Function to print the details of the exoplanet
    // It overrides the pure virtual function from the base class CelestialObject
    virtual void print() const override;
};

#endif // EXOPLANET_H