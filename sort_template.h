// PHYS 30762 Programming in C++
// Assignment 6
// Student ID: 10868355
/* Sort template header file. 
 * This file contains the template function for sorting a catalogue of objects.
 * The sorting is done based on a specified property using a lambda function as an accessor.
 * The sorting order can be either ascending or descending.
 * The template function is designed to work with any type of object that has the specified property.
 */

#ifndef SORT_TEMPLATE_H
#define SORT_TEMPLATE_H

#include<vector> // for std::vector
#include<iostream> // for std::cout, std::endl
#include<memory> // for std::shared_ptr
#include<algorithm> // for std::sort
#include<functional> // for std::function

// Enum to define sorting order
enum class SortOrder { Ascending, Descending };

// Function to sort a catalogue of objects based on a specified property
// using a lambda function as an accessor to retrieve the property value.
template <typename T>
void sort_catalogue(
  std::vector<std::shared_ptr<T>>& catalogue,
  std::function<double(const std::shared_ptr<T>&)> accessor,
  SortOrder order = SortOrder::Ascending)
{
  // Check if the catalogue is empty
  std::sort(catalogue.begin(), catalogue.end(),
    [accessor, order](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b)
    {
      return (order == SortOrder::Ascending) ? // ascending order
             (accessor(a) < accessor(b)) :
             (accessor(a) > accessor(b));
    });
}

#endif // SORT_TEMPLATE_H
