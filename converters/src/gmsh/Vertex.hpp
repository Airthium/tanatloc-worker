#ifndef _VERTEX_
#define _VERTEX_

typedef unsigned int uint;

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
  double *getCoordinates() const;
};

#endif
