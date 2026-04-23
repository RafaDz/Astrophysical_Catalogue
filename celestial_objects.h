// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Celestial objects abstract class header. */

#ifndef CELESTIAL_OBJECTS_H
#define CELESTIAL_OBJECTS_H
#include<iostream>
#include<string>

class CelestialObject { // Base abstract class for celestial objects
  protected: // Protected members
    std::string name; // Name of the celestial object
    std::string object_type;  // Type of the celestial object (e.g., star, planet, galaxy)
    static int counter; // Static member to count the number of celestial objects created

  public:
    // Constructor
    CelestialObject(std::string name, std::string object_type);

    // Destructor
    // The destructor is virtual to ensure proper cleanup of derived classes
    virtual ~CelestialObject();

    virtual void print() const = 0; // Pure virtual function for printing details
    virtual std::string get_name() const; // Getter for name
    virtual std::string get_object_type() const; // Getter for object type

    // Static function to get the number of celestial objects created
    static int get_counter(); // Getter for counter

    // Setters
    virtual void set_name(const std::string& new_name);
    virtual void set_object_type(const std::string& new_type);
};

#endif // CELESTIAL_OBJECTS_H

