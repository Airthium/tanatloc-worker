#include "Tetrahedron.hpp"

/**
 * Constructor
 */
Tetrahedron::Tetrahedron () :
  index1(0), index2(0), index3(0), index4(0), label(0) {}

/**
 * Constructor
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param index4 Index 4
 * @param label Label
 */
Tetrahedron::Tetrahedron(const uint index1, const uint index2, const uint index3, const uint index4, const uint label) :
  index1(index1), index2(index2), index3(index3), index4(index4), label(label) {}

/**
 * Destructor
 */
Tetrahedron::~Tetrahedron () {
  this->index1 = 0;
  this->index2 = 0;
  this->index3 = 0;
  this->index4 = 0;
  this->label = 0;
}

/**
 * Set indices
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param index4 Index 4
 */
void Tetrahedron::setIndices (const uint index1, const uint index2, const uint index3, const uint index4) {
  this->index1 = index1;
  this->index2 = index2;
  this->index3 = index3;
  this->index4 = index4;
}

/**
 * Set label
 * @param label Label
 */
void Tetrahedron::setLabel (const uint label) {
  this->label = label;
}

/**
 * Get index
 * @param i index
 * @returns Index i
 */
uint Tetrahedron::getIndex (const uint i) const {
  if (i == 0)
    return this->index1;
  else if (i == 1)
    return this->index2;
  else if (i == 2)
    return this->index3;
  else if (i == 3)
    return this->index4;
  else
    return 0;
}

/**
 * Get indices
 * @returns Indices
 */
uint *Tetrahedron::getIndices () const {
  uint *indices = new uint[4];
  indices[0] = index1;
  indices[1] = index2;
  indices[2] = index3;
  indices[3] = index4;
  return indices;
}

/**
 * Get label
 * @returns Label
 */
uint Tetrahedron::getLabel () const {
  return this->label;
}
