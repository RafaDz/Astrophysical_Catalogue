// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Galaxies class header. */

#include "celestial_objects.h"

#ifndef GALAXY_H
#define GALAXY_H
#include<iostream>
#include<cmath>
#include<vector>
#include<string>

class Galaxy : public CelestialObject {  // Class representing a galaxy, derived from CelestialObject
  private: 
    static int counter; // Static member to count the number of galaxies created

  protected: // Protected members
    std::string constellation;  // Constellation of the galaxy
    double distance;      // Distance to the galaxy in megaparsecs
    double apparent_magnitude; // Apparent magnitude of the galaxy
    double redshift;  // Redshift z of the galaxy
  
  public:
    // Constructor
    Galaxy(std::string name, std::string constellation, double distance, double apparent_magnitude, double redshift);

    // Destructor
    virtual ~Galaxy();

    // Function to get the number of galaxies created
    static int get_counter(); // Getter for counter

    // Getters
    std::string get_constellation() const { return constellation; } // Getter for constellation
    double get_distance() const { return distance; } // Getter for distance
    double get_apparent_magnitude() const { return apparent_magnitude; } // Getter for apparent magnitude
    double get_redshift() const { return redshift; } // Getter for redshift

    // Setters
    void set_constellation(const std::string& constellation);
    void set_distance(double distance);
    void set_apparent_magnitude(double apparent_magnitude);
    void set_redshift(double redshift);    

    // Function to calculate the absolute magnitude of the galaxy
    virtual double absolute_magnitude() const;

    // Function to calculate the velocity of the galaxy
    virtual double velocity() const;

    // Function to print the details of the galaxy
    virtual void print() const override;
};

#endif // GALAXY_H