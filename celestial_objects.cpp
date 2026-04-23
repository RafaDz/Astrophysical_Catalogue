// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Celestial Object abstract class implementation. Includes: 
 * Parametrized constructor and destructor, getters/setters. 
 * The class is used as a base class for other celestial objects. 
 * It includes a static counter to keep track of the number of objects created. 
 * The destructor is virtual to ensure proper cleanup of derived classes. 
 * The class also includes a pure virtual function for printing details. 
 * The setters include error handling for empty strings. 
 * The getters are used to retrieve the name and type of the object. 
 * The static function returns the current count of celestial objects created. 
 */

#include "celestial_objects.h"

#include <stdexcept> // for std::invalid_argument

int CelestialObject::counter = 0; // Initialize static counter to 0

// Constructor
CelestialObject::CelestialObject(std::string name, std::string object_type)
  : name(name), object_type(object_type) 
  { 
    ++counter; // Increment the counter for each new celestial object created
  } 

// Destructor
CelestialObject::~CelestialObject() 
{ 
  --counter; // Decrement the counter when a celestial object is destroyed
} 

// Getters
std::string CelestialObject::get_name() const 
{ 
  return name; 
} 
std::string CelestialObject::get_object_type() const 
{ 
  return object_type; 
}
int CelestialObject::get_counter() 
{ 
  return counter; 
} 

// Setters
void CelestialObject::set_name(const std::string& new_name) 
{
  if(new_name.empty())
  {
    throw std::invalid_argument("Name cannot be empty.");
  }
    name = new_name;
}

void CelestialObject::set_object_type(const std::string& new_type) 
{
  if(new_type.empty())
  {
    throw std::invalid_argument("Object type cannot be empty.");
  }
  object_type = new_type;
}
