#ifndef _TRIANGLE_
#define _TRIANGLE_

typedef unsigned int uint;

/**
 * Triangle class
 */
class Triangle {
private:
  // Index 1
  uint index1;
  // Index 2
  uint index2;
  // Index 3
  uint index3;
  // Label
  uint label;

public:
  // Constructor
  Triangle();
  // Constructor
  Triangle(const uint, const uint, const uint, const uint);
  // Destructor
  ~Triangle();

  // Set indices
  void setIndices(const uint, const uint, const uint);
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
