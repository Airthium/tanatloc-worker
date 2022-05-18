#include "GLTFWriter.hpp"

#include <IFSelect_ReturnStatus.hxx>
#include <Message_ProgressRange.hxx>
#include <RWGltf_CafWriter.hxx>

/**
 * Constructor
 */
GLTFWriter::GLTFWriter() = default;

/**
 * Constructor
 * @param fileName File name
 */
GLTFWriter::GLTFWriter(const std::string &fileName,
                       Handle(TDocStd_Document) & document,
                       TDF_LabelSequence &labels)
    : m_fileName(fileName), m_document(document), m_labels(labels) {}

/**
 * Write
 * @returns Status
 */
bool GLTFWriter::write() const {
  IFSelect_ReturnStatus status;

  RWGltf_CafWriter writer = RWGltf_CafWriter(this->m_fileName.c_str(), true);

  TColStd_IndexedDataMapOfStringString fileInfo;
  Message_ProgressRange progress = Message_ProgressRange();
  writer.Perform(this->m_document, this->m_labels, NULL, fileInfo, progress);

  return true;
}
