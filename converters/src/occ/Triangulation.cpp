#include "Triangulation.hpp"

#include "../logger/Logger.hpp"
#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>

/**
 * Constructor
 */
Triangulation::Triangulation() = default;

/**
 * Constructor
 * @param shape Shape
 */
Triangulation::Triangulation(const MainDocument &mainDocument)
    : m_mainDocument(mainDocument) {
  this->computeBb();
}

/**
 * Compute bounding box and max dimensions
 */
void Triangulation::computeBb() {
  TopoDS_Compound compound = this->m_mainDocument.getCompound();

  Bnd_Box boundingBox;
  double xMin;
  double yMin;
  double zMin;
  double xMax;
  double yMax;
  double zMax;

  BRepBndLib::Add(compound, boundingBox);
  boundingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

  double xDim = std::abs(xMax - xMin);
  double yDim = std::abs(yMax - yMin);
  double zDim = std::abs(zMax - zMin);

  this->m_minBb = std::min(xDim, std::min(yDim, zDim));
  this->m_maxBb = std::max(xDim, std::max(yDim, zDim));
}

/**
 * Triangulate
 */
void Triangulation::triangulate() {
  TDF_LabelSequence labels = this->m_mainDocument.getLabels();

  // Shapes
  for (uint i = 1; i <= labels.Size(); ++i) {
    TDF_Label label = labels.Value(i);
    TopoDS_Shape shape = this->m_mainDocument.getShape(label);
    BRepMesh_IncrementalMesh mesh(shape, this->m_maxBb * meshQuality);

    // Sub Shapes
    TDF_LabelSequence subLabels;
    this->m_mainDocument.getComponents(label, subLabels);
    for (uint j = 1; j <= subLabels.Size(); ++j) {
      TDF_Label subLabel = subLabels.Value(j);
      TopoDS_Shape subShape = this->m_mainDocument.getShape(subLabel);
      BRepMesh_IncrementalMesh subMesh(subShape, this->m_maxBb * meshQuality);
    }
  }
}
