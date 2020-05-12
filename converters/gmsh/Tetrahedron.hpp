#ifndef _TETRAHEDRON_
#define _TETRAHEDRON_

typedef unsigned int uint;

/**
 * Tetrahedron class
 */
class Tetrahedron {
private:
  // Index 1
  uint index1;
  // Index 2
  uint index2;
  // Index 3
  uint index3;
  // Index 4
  uint index4;
  // Label
  uint label;

public:
  // Constructor
  Tetrahedron();
  // Constructor
  Tetrahedron(const uint, const uint, const uint, const uint, const uint);
  // Destructor
  ~Tetrahedron();

  // Set indices
  void setIndices(const uint, const uint, const uint, const uint);
  // Set label
  void setLabel(const uint);

  // Get index
  uint getIndex(const uint) const;
  // Get indices
  uint *getIndices() const;
  // Get label
  uint getLabel() const;
};

#endif
