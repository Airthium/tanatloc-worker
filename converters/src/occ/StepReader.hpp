#ifndef _STEPREADER_
#define _STEPREADER_

#include <vector>

#include "MainDocument.hpp"

class StepReader {
private:
  std::string m_fileName = "";
  MainDocument m_mainDocument;

public:
  // Constructor
  StepReader();
  // Constructor
  explicit StepReader(const std::string &);

  // Read
  bool read();

  // Get main document
  MainDocument getMainDocument() const;
};

#endif
