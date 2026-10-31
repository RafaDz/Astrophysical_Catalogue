// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Stars class header. */

#include "celestial_objects.h"

#ifndef STAR_H
#define STAR_H
#include<iostream>
#include<cmath>
#include<vector>
#include<string>

class Star : public CelestialObject {  // Class representing a star, derived from CelestialObject
  private: 
    static int counter; // Static member to count the number of stars created

  protected: // Protected members
    std::string type;  // Type of the star (e.g., main sequence, red giant)
    std::string spectral_class; // Spectral class of the star (e.g., O, B, A, F, G, K, M)
    double mass; // Mass of the star in solar masses
    double radius; // Radius of the star in solar radii
    double temperature; // Surface temperature of the star in Kelvin
    double age; // Age of the star in billions of years
    double apparent_magnitude; // Apparent magnitude of the star
    double distance; // Distance to the star in megaparsecs (Mpc), same unit as galaxies
  
  public:
    // Constructor
    Star(std::string name, std::string type, std::string spectral_class, double mass, double radius, double temperature, double age, double apparent_magnitude, double distance);

    // Destructor
    virtual ~Star();

    // Function to get the number of stars created
    static int get_counter(); // Getter for counter

    // Getters
    std::string get_type() const { return type; } // Getter for type
    std::string get_spectral_class() const { return spectral_class; } // Getter for spectral class
    double get_mass() const { return mass; } // Getter for mass
    double get_radius() const { return radius; } // Getter for radius
    double get_temperature() const { return temperature; } // Getter for temperature
    double get_age() const { return age; } // Getter for age
    double get_apparent_magnitude() const { return apparent_magnitude; } // Getter for apparent magnitude
    double get_distance() const { return distance; } // Getter for distance

    // Setters
    void set_type(const std::string& type); // Setter for type
    void set_spectral_class(const std::string& spectral_class); // Setter for spectral class
    void set_mass(double mass); // Setter for mass
    void set_radius(double radius); // Setter for radius
    void set_temperature(double temperature); // Setter for temperature
    void set_age(double age); // Setter for age 
    void set_apparent_magnitude(double apparent_magnitude); // Setter for apparent magnitude
    void set_distance(double distance); // Setter for distance 

    // Function to calculate luminosity of the star (Stefan-Boltzmann Law)
    virtual double luminosity() const;

    // Function to calculate gravitational strength at the surface of the star
    virtual double gravitational_strength() const;

    // Function to calculate the absolute magnitude of the star
    virtual double absolute_magnitude() const;

    // Function to print the details of the star
    virtual void print() const override;
};

#endif // STAR_H