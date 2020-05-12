#ifndef _VERTEX_
#define _VERTEX_

typedef unsigned int uint;

/**
 * Vertex class
 */
class Vertex {
private:
  // Coordinate x
  double x;
  // Coordinate y
  double y;
  // Coordinate z
  double z;

public:
  // Constructor
  Vertex();
  // Destructor
  ~Vertex();

  // Set coordinates
  void setCoordinates(const double, const double, const double);

  // Get coordinate
  double getCoordinate(const uint) const;
  // Get coordinates
  double *getCoordinates() const;
};

#endif
