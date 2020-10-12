#include "Triangle.hpp"

/**
 * Constructor
 */
Triangle::Triangle() : index1(0), index2(0), index3(0), label(0) {}

/**
 * Constructor
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param label Label
 */
Triangle::Triangle(const uint index1, const uint index2, const uint index3,
                   const uint label)
    : index1(index1), index2(index2), index3(index3), label(label) {}

/**
 * Destructor
 */
Triangle::~Triangle() {
  this->index1 = 0;
  this->index2 = 0;
  this->index3 = 0;
  this->label = 0;
}

/**
 * Set indices
 * @param index1 Index 1
 * @param index1 Index 2
 * @param index1 Index 3
 */
void Triangle::setIndices(const uint index1, const uint index2,
                          const uint index3) {
  this->index1 = index1;
  this->index2 = index2;
  this->index3 = index3;
}

/**
 * Set label
 * @param label Label
 */
void Triangle::setLabel(const uint label) { this->label = label; }

/**
 * Get index
 * @param i Index
 * @returns Index i
 */
uint Triangle::getIndex(const uint i) const {
  if (i == 0)
    return this->index1;
  else if (i == 1)
    return this->index2;
  else if (i == 2)
    return this->index3;
  else
    return 0;
}

/**
 * Get indices
 * @returns Indices
 */
uint *Triangle::getIndices() const {
  uint *indices = new uint[3];
  indices[0] = index1;
  indices[1] = index2;
  indices[2] = index3;
  return indices;
}

/**
 * Get label
 * @returns Label
 */
uint Triangle::getLabel() const { return this->label; }
