#ifndef _MAIN_DOCUMENT_
#define _MAIN_DOCUMENT_

#include <vector>

#include <Quantity_Color.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Compound.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

const Quantity_Color OCCDefaultColor(1., 1., 0., Quantity_TOC_RGB);
const Quantity_Color TanatlocDefaultColor(0.75, 0.75, 0.75, Quantity_TOC_RGB);

struct Color {
  float r = 0.75;
  float g = 0.75;
  float b = 0.75;
};

struct Element {
  std::string name;
  std::string uuid;
  int label;
  Color color;
};

class MainDocument {
private:
  int m_dimension;
  std::string m_type;
  std::vector<Element> m_solids;
  std::vector<Element> m_faces;
  std::vector<Element> m_edges;

  Handle(XCAFApp_Application) m_app;
  Handle(XCAFDoc_ShapeTool) m_shapeTool;
  Handle(XCAFDoc_ColorTool) m_colorTool;

public:
  Handle(TDocStd_Document) document;

  // Constructor
  MainDocument();

  void setDimension(int);
  void setType(std::string &);

  // 3D
  TDF_Label add3DSolid(const TopoDS_Shape &);
  TDF_Label add3DSolid(const TopoDS_Shape &, const Quantity_Color &);
  TDF_Label add3DFace(const TDF_Label &, const TopoDS_Shape &);
  TDF_Label add3DFace(const TDF_Label &, const TopoDS_Shape &,
                      const Quantity_Color &);

  // 2D
  TDF_Label add2DFace(const TopoDS_Shape &);
  TDF_Label add2DEdge(const TDF_Label &, const TopoDS_Shape &);

  // Add shape
  TDF_Label addShape(const TopoDS_Shape &, const std::string &) const;
  TDF_Label addShape(const TopoDS_Shape &, const Quantity_Color &,
                     const std::string &) const;

  // Add component
  TDF_Label addComponent(const TDF_Label &, const TopoDS_Shape &,
                         const std::string &) const;
  TDF_Label addComponent(const TDF_Label &, const TopoDS_Shape &,
                         const Quantity_Color &, const std::string &) const;

  // Get labels
  TDF_LabelSequence getLabels() const;

  // Get compound
  TopoDS_Compound getCompound() const;

  // Get shape
  TopoDS_Shape getShape(const TDF_Label &) const;

  // Get sub shapes
  void getComponents(const TDF_Label &, TDF_LabelSequence &) const;

  // Get shape color
  Quantity_Color getShapeColor(const TopoDS_Shape &shape) const;

  // Write description
  bool writeDescription(std::string &) const;
};

#endif
