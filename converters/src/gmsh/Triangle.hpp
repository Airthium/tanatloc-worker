#ifndef _TRIANGLE_
#define _TRIANGLE_

typedef unsigned int uint;

/**
 * Triangle class
 */
class Triangle {
private:
  // Index 1
  uint m_index1 = 0;
  // Index 2
  uint m_index2 = 0;
  // Index 3
  uint m_index3 = 0;
  // Label
  uint m_label = 0;

public:
  // Constructor
  Triangle();
  // Constructor
  Triangle(const uint, const uint, const uint, const uint);

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
