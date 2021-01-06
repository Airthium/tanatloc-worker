#ifndef _VERTEX_
#define _VERTEX_

#include <vector>

using uint = unsigned int;

/**
 * Vertex class
 */
class Vertex {
private:
  // Coordinate x
  double m_x = 0;
  // Coordinate y
  double m_y = 0;
  // Coordinate z
  double m_z = 0;

public:
  // Constructor
  Vertex();
  Vertex(const double, const double, const double);

  // Set coordinates
  void setCoordinates(const double, const double, const double);

  // Get coordinate
  double getCoordinate(const uint) const;
  // Get coordinates
  std::vector<double> getCoordinates() const;
};

#endif
