#include "Vertex.hpp"

/**
 * Constructor
 */
Vertex::Vertex() = default;

/**
 * Constructor
 * @param {double} X
 * @param {double} Y
 * @param {double} Z
 */
Vertex::Vertex(const double x, const double y, const double z)
    : m_x(x), m_y(y), m_z(z) {}

/**
 * Set coordinates
 * @param x Coordinate x
 * @param y Coordinate y
 * @param z Coordinate z
 */
void Vertex::setCoordinates(const double x, const double y, const double z) {
  this->m_x = x;
  this->m_y = y;
  this->m_z = z;
}

/**
 * Get coordinate
 * @param i Index
 * @returns Coordinate i
 */
double Vertex::getCoordinate(const uint i) const {
  if (i == 0)
    return this->m_x;
  else if (i == 1)
    return this->m_y;
  else if (i == 2)
    return this->m_z;
  else
    return 0.;
}

/**
 * Get coordinates
 * @returns Coordinates
 */
std::vector<double> Vertex::getCoordinates() const {
  auto coordinates = std::vector<double>();
  coordinates.push_back(this->m_x);
  coordinates.push_back(this->m_y);
  coordinates.push_back(this->m_z);
  return coordinates;
}
