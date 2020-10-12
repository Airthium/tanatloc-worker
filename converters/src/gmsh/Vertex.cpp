#include "Vertex.hpp"

/**
 * Constructor
 */
Vertex::Vertex() : x(0.), y(0.), z(0.) {}

/**
 * Destructor
 */
Vertex::~Vertex() {
  this->x = 0.;
  this->y = 0.;
  this->z = 0.;
}

/**
 * Set coordinates
 * @param x Coordinate x
 * @param y Coordinate y
 * @param z Coordinate z
 */
void Vertex::setCoordinates(const double x, const double y, const double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

/**
 * Get coordinate
 * @param i Index
 * @returns Coordinate i
 */
double Vertex::getCoordinate(const uint i) const {
  if (i == 0)
    return this->x;
  else if (i == 1)
    return this->y;
  else if (i == 2)
    return this->z;
  else
    return 0.;
}

/**
 * Get coordinates
 * @returns Coordinates
 */
double *Vertex::getCoordinates() const {
  double *coordinates = new double[3];
  coordinates[0] = this->x;
  coordinates[1] = this->y;
  coordinates[2] = this->z;
  return coordinates;
}
