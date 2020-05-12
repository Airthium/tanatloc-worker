#ifndef _GETELEMENTS_
#define _GETELEMENTS_

#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TDocStd_Document.hxx>

#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <Quantity_Color.hxx>

/**
 * Get elements
 * @param shape Shape
 * @param type Type
 * @returns Elements
 */
std::vector<TopoDS_Shape> getElements (TopoDS_Shape shape, TopAbs_ShapeEnum type) {
  std::vector<TopoDS_Shape> elements = std::vector<TopoDS_Shape>();
  TopExp_Explorer explorer = TopExp_Explorer(shape, type);
  while (explorer.More()) {
    TopoDS_Shape element = explorer.Current();
    elements.push_back(element);
    explorer.Next();
  }
  return elements;
}

/**
 * Get elements
 * @param shape Shape
 * @param document Document
 * @param color Color
 * @param type Type
 * @returns Elements
 */
std::vector<TopoDS_Shape> getElements (TopoDS_Shape shape, Handle(TDocStd_Document) document, std::vector<std::pair<bool, Quantity_Color>> *color, TopAbs_ShapeEnum type) {
  std::vector<TopoDS_Shape> elements = std::vector<TopoDS_Shape>();
  TopExp_Explorer explorer = TopExp_Explorer(shape, type);
  while (explorer.More()) {
    TopoDS_Shape element = explorer.Current();

    // Get color, if exists
    Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(document->Main());
    Quantity_Color internalColor;
    Standard_Boolean res = colorTool->GetColor(element, XCAFDoc_ColorSurf, internalColor);
    color->push_back(std::pair<bool, Quantity_Color>(res, internalColor));

    elements.push_back(element);
    explorer.Next();
  }
  return elements;
}

/**
 * Get solids
 * @param shape Shape
 * @returns Solids
 */
std::vector<TopoDS_Shape> getSolids(TopoDS_Shape shape) {
  return getElements(shape, TopAbs_SOLID);
}

/**
 * Get solids
 * @param shape Shape
 * @param document Document
 * @param color Color
 * @returns Solids
 */
std::vector<TopoDS_Shape> getSolids(TopoDS_Shape shape, Handle(TDocStd_Document) document, std::vector<std::pair<bool, Quantity_Color>> *color) {
  return getElements(shape, document, color, TopAbs_SOLID);
}

/**
 * Get faces
 * @param shape Shape
 * @returns Faces
 */
std::vector<TopoDS_Shape> getFaces(TopoDS_Shape shape) {
  return getElements(shape, TopAbs_FACE);
}

/**
 * Get faces
 * @param shape Shape
 * @param document Document
 * @param color Color
 * @returns Faces
 */
std::vector<TopoDS_Shape> getFaces(TopoDS_Shape shape, Handle(TDocStd_Document) document, std::vector<std::pair<bool, Quantity_Color>> *color) {
  return getElements(shape, document, color, TopAbs_FACE);
}

/**
 * Get edges
 * @param shape Shape
 * @returns Faces
 */
std::vector<TopoDS_Shape> getEdges(TopoDS_Shape shape) {
  return getElements(shape, TopAbs_EDGE);
}

/**
 * Get edges
 * @param shape Shape
 * @param document Document
 * @param color Color
 * @returns Faces
 */
std::vector<TopoDS_Shape> getEdges(TopoDS_Shape shape, Handle(TDocStd_Document) document, std::vector<std::pair<bool, Quantity_Color>> *color) {
  return getElements(shape, document, color, TopAbs_EDGE);
}

#endif //_GETELEMENTS_
