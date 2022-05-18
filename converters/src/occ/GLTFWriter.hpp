#ifndef _GLTFWRITER_
#define _GLTFWRITER_

#include <TDF_LabelSequence.hxx>
#include <TDocStd_Document.hxx>

class GLTFWriter {
private:
  std::string m_fileName = "";
  Handle(TDocStd_Document) m_document;
  TDF_LabelSequence m_labels;

public:
  // Constructor
  GLTFWriter();
  // Constructor
  GLTFWriter(const std::string &, Handle(TDocStd_Document) &,
             TDF_LabelSequence &);

  // Write
  bool write() const;
};

#endif
