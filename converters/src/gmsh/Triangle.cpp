#include "Triangle.hpp"

/**
 * Constructor
 */
Triangle::Triangle() {}

/**
 * Constructor
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param label Label
 */
Triangle::Triangle(const uint index1, const uint index2, const uint index3,
                   const uint label)
    : m_index1(index1), m_index2(index2), m_index3(index3), m_label(label) {}

/**
 * Set indices
 * @param index1 Index 1
 * @param index1 Index 2
 * @param index1 Index 3
 */
void Triangle::setIndices(const uint index1, const uint index2,
                          const uint index3) {
  this->m_index1 = index1;
  this->m_index2 = index2;
  this->m_index3 = index3;
}

/**
 * Set label
 * @param label Label
 */
void Triangle::setLabel(const uint label) { this->m_label = label; }

/**
 * Get index
 * @param i Index
 * @returns Index i
 */
uint Triangle::getIndex(const uint i) const {
  if (i == 0)
    return this->m_index1;
  else if (i == 1)
    return this->m_index2;
  else if (i == 2)
    return this->m_index3;
  else
    return 0;
}

/**
 * Get indices
 * @returns Indices
 */
uint *Triangle::getIndices() const {
  auto *indices = new uint[3];
  indices[0] = this->m_index1;
  indices[1] = this->m_index2;
  indices[2] = this->m_index3;
  return indices;
}

/**
 * Get label
 * @returns Label
 */
uint Triangle::getLabel() const { return this->m_label; }
