#include "Triangulation.hpp"

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <Poly_Connect.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <StdPrs_ToolTriangulatedShape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <gp_Ax2.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
Triangulation::Triangulation() = default;

/**
 * Constructor
 * @param shape Shape
 */
Triangulation::Triangulation(MainDocument &mainDocument)
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

  // TopoDS_Compound compound = this->m_mainDocument.getCompound();
  TDF_LabelSequence labels = this->m_mainDocument.getLabels();

  for (uint i = 1; i <= labels.Size(); ++i) {
    // Shape
    TDF_Label label = labels.Value(i);
    TopoDS_Shape shape = this->m_mainDocument.getShape(label);
    BRepMesh_IncrementalMesh mesh(shape, this->m_maxBb * meshQuality);

    // Components
    TDF_LabelSequence subLabels;
    this->m_mainDocument.getComponents(label, subLabels);
    for (uint j = 1; j <= subLabels.Size(); ++j) {
      // Shape
      TDF_Label subLabel = subLabels.Value(j);
      TopoDS_Shape subShape = this->m_mainDocument.getShape(subLabel);
      BRepMesh_IncrementalMesh subMesh(subShape, this->m_maxBb * meshQuality);
    }
  }

  // BRepMesh_IncrementalMesh mesh(compound, this->m_maxBb * meshQuality);

  // if (dimension == 2) {
  //   TopExp_Explorer faceExplorer;
  //   for (faceExplorer.Init(compound, TopAbs_FACE); faceExplorer.More();
  //        faceExplorer.Next()) {
  //     TopoDS_Shape face = faceExplorer.Current();
  //     // this->triangulateFace(face);

  //     TopExp_Explorer edgeExplorer;
  //     for (edgeExplorer.Init(face, TopAbs_EDGE); edgeExplorer.More();
  //          edgeExplorer.Next()) {
  //       TopoDS_Shape edge = edgeExplorer.Current();
  //       this->triangulateEdge(edge);
  //     }
  //   }
  // } else {
  //   // 3D
  //   TopExp_Explorer solidExplorer;
  //   for (solidExplorer.Init(compound, TopAbs_SOLID); solidExplorer.More();
  //        solidExplorer.Next()) {
  //     TopoDS_Shape solid = solidExplorer.Current();

  //     TopExp_Explorer faceExplorer;
  //     for (faceExplorer.Init(solid, TopAbs_FACE); faceExplorer.More();
  //          faceExplorer.Next()) {
  //       TopoDS_Shape face = faceExplorer.Current();
  //       this->triangulateFace(face);
  //     }
  //   }
  // }
}

/**
 * Triangulate face
 * @param shape Shape
 */
void Triangulation::triangulateFace(const TopoDS_Shape &face) const {
  BRepMesh_IncrementalMesh mesh(face, this->m_maxBb * meshQuality);
}

/**
 * Triangulate edge
 * @param shape Shape
 */
void Triangulation::triangulateEdge(const TopoDS_Shape &edge) const {
  BRepMesh_IncrementalMesh mesh(edge, this->m_maxBb * meshQuality);
}
