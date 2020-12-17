#include "Tetrahedron.hpp"

/**
 * Constructor
 */
Tetrahedron::Tetrahedron() = default;

/**
 * Constructor
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param index4 Index 4
 * @param label Label
 */
Tetrahedron::Tetrahedron(const uint index1, const uint index2,
                         const uint index3, const uint index4, const uint label)
    : m_index1(index1), m_index2(index2), m_index3(index3), m_index4(index4),
      m_label(label) {}

/**
 * Set indices
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param index4 Index 4
 */
void Tetrahedron::setIndices(const uint index1, const uint index2,
                             const uint index3, const uint index4) {
  this->m_index1 = index1;
  this->m_index2 = index2;
  this->m_index3 = index3;
  this->m_index4 = index4;
}

/**
 * Set label
 * @param label Label
 */
void Tetrahedron::setLabel(const uint label) { this->m_label = label; }

/**
 * Get index
 * @param i index
 * @returns Index i
 */
uint Tetrahedron::getIndex(const uint i) const {
  if (i == 0)
    return this->m_index1;
  else if (i == 1)
    return this->m_index2;
  else if (i == 2)
    return this->m_index3;
  else if (i == 3)
    return this->m_index4;
  else
    return 0;
}

/**
 * Get indices
 * @returns Indices
 */
uint *Tetrahedron::getIndices() const {
  auto *indices = new uint[4];
  indices[0] = this->m_index1;
  indices[1] = this->m_index2;
  indices[2] = this->m_index3;
  indices[3] = this->m_index4;
  return indices;
}

/**
 * Get label
 * @returns Label
 */
uint Tetrahedron::getLabel() const { return this->m_label; }
