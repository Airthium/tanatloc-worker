#include "StepReader.hpp"

#include "../logger/Logger.hpp"
#include <IFSelect_ReturnStatus.hxx>
#include <Quantity_Color.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>

/**
 * Constructor
 */
StepReader::StepReader() = default;

/**
 * Constructor
 * @param fileName File name
 */
StepReader::StepReader(const std::string &fileName) : m_fileName(fileName) {}

/**
 * Read
 * @returns Status
 */
bool StepReader::read() {
  /// READ
  // Document
  MainDocument originalMainDocument;

  // Reader
  IFSelect_ReturnStatus status;
  STEPCAFControl_Reader caf_reader = STEPCAFControl_Reader();
  status = caf_reader.ReadFile(this->m_fileName.c_str());
  if (status != IFSelect_RetDone) {
    Logger::ERROR("Unable to read " + this->m_fileName);
    return false;
  }

  // Transfer
  if (!caf_reader.Transfer(originalMainDocument.document)) {
    Logger::ERROR("Unable to transfert root");
    return false;
  }

  // Compound
  TopoDS_Compound originalCompound = originalMainDocument.getCompound();

  /// CLEAN DOCUMENT
  // Solids & faces loop
  std::string type = "geometry";
  this->m_mainDocument.setType(type);
  this->m_mainDocument.setDimension(3);

  TopExp_Explorer solidExplorer;
  for (solidExplorer.Init(originalCompound, TopAbs_SOLID); solidExplorer.More();
       solidExplorer.Next()) {
    // Solid
    TopoDS_Shape solid = solidExplorer.Current();
    Quantity_Color solidColor = originalMainDocument.getShapeColor(solid);

    // New solid
    TDF_Label solidLabel = this->m_mainDocument.add3DSolid(solid, solidColor);

    TopExp_Explorer faceExplorer;
    for (faceExplorer.Init(solid, TopAbs_FACE); faceExplorer.More();
         faceExplorer.Next()) {
      // Face
      TopoDS_Shape face = faceExplorer.Current();
      Quantity_Color faceColor = originalMainDocument.getShapeColor(face);

      // New face
      this->m_mainDocument.add3DFace(solidLabel, face, faceColor);
    }
  }

  return true;
}

MainDocument StepReader::getMainDocument() const {
  return this->m_mainDocument;
}
