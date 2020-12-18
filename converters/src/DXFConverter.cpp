#include <memory>

#include "DXFConverter.hpp"

#include "dxflib/dl_dxf.h"

#include <gp_Circ.hxx>
#include <gp_Pnt.hxx>

#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include <GC_MakeArcOfCircle.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include <BRepTools.hxx>

#include <BRep_Builder.hxx>

#include "logger/Logger.hpp"

// PUBLIC

DXFConverter::DXFConverter() { this->m_vertices.clear(); }

void DXFConverter::setInput(const std::string &input) { this->m_input = input; }

void DXFConverter::setOutput(const std::string &output) {
  this->m_output = output;
}

bool DXFConverter::convert() {
  auto dxf = std::make_unique<DL_Dxf>();
  if (!dxf->in(this->m_input, this)) {
    Logger::ERROR("DXFConverter::Unable to open" + this->m_input);
    return false;
  }
  return true;
}

bool DXFConverter::write() const {
  return BRepTools::Write(this->m_shape, this->m_output.c_str());
}

// PRIVATE

// dxflib

void DXFConverter::processCodeValuePair(unsigned int code,
                                        const std::string &value) {
  if (value == "ENDSEC") {
    Logger::DEBUG("DXFConverter::ENDSEC");
    this->buildFace();
  }

  else if (value == "EOF") {
    Logger::DEBUG("DXFConverter::EOF");
    this->buildShape();
  }
}

void DXFConverter::addLine(const DL_LineData &data) {
  Logger::DEBUG("DXFConverter::LINE");

  DL_VertexData v1 = {data.x1, data.y1, 0, 0};
  DL_VertexData v2 = {data.x2, data.y2, 0, 0};

  this->m_vertices.push_back(v1);
  this->m_vertices.push_back(v2);

  this->buildWire();
}

void DXFConverter::addArc(const DL_ArcData &) {
  Logger::DEBUG("DXFConverter::ARC");
  Logger::ERROR("TODO");
}

void DXFConverter::addCircle(const DL_CircleData &data) {
  Logger::DEBUG("DXFConverter::CIRCLE");

  gp_Circ circle;
  gp_Pnt center(data.cx, data.cy, 0);
  circle.SetLocation(center);
  circle.SetRadius(data.radius);

  auto edgeBuilder = BRepBuilderAPI_MakeEdge(circle);
  TopoDS_Edge edge = edgeBuilder.Edge();

  auto wireBuilder = BRepBuilderAPI_MakeWire(edge);
  TopoDS_Wire wire = wireBuilder.Wire();

  this->m_wires.push_back(wire);
}

void DXFConverter::addPolyline(const DL_PolylineData &data) {
  Logger::DEBUG("DXFConverter::POLYLINE");
}

void DXFConverter::addVertex(const DL_VertexData &data) {
  Logger::DEBUG("DXFConverter::VERTEX");

  this->m_vertices.push_back(data);
}

void DXFConverter::endEntity() {
  Logger::DEBUG("DXFConverter::END ENTITY");

  this->buildWire();
}

// occ

void DXFConverter::buildWire() {
  Logger::DEBUG("DXFConverter::buildWire");

  auto size = (uint)this->m_vertices.size();
  if (!size)
    return;

  BRepBuilderAPI_MakeWire wireBuilder = BRepBuilderAPI_MakeWire();
  for (uint i = 0; i < size; ++i) {
    DL_VertexData v1 = this->m_vertices[i];
    DL_VertexData v2 = this->m_vertices[(i + 1) % size];

    TopoDS_Edge edge;
    if (v1.bulge != 0.) {
      gp_Pnt point1(v1.x, v1.y, 0);
      gp_Pnt point2(v2.x, v2.y, 0);

      // Thanks to
      // https://github.com/FreeCAD/FreeCAD/blob/master/src/Mod/Draft/importDXF.py
      gp_Vec chord(point1, point2);
      double sagitta = (v1.bulge * chord.Magnitude()) / 2.;
      gp_Vec perp = chord.Crossed(gp_Vec(0, 0, 1));
      gp_Vec startPoint(point1.X() + 0.5 * chord.X(),
                        point1.Y() + 0.5 * chord.Y(), 0);
      perp.Normalize();
      gp_Vec endPoint = perp * sagitta;
      gp_Pnt point3(startPoint.X() + endPoint.X(),
                    startPoint.Y() + endPoint.Y(), 0);

      GC_MakeArcOfCircle circleArc(point1, point3, point2);
      opencascade::handle<Geom_TrimmedCurve> edgeGeometry = circleArc.Value();

      auto edgeBuilder = BRepBuilderAPI_MakeEdge(edgeGeometry);
      edge = edgeBuilder.Edge();
    } else {
      gp_Pnt point1(v1.x, v1.y, 0);
      gp_Pnt point2(v2.x, v2.y, 0);
      BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
      BRepBuilderAPI_MakeVertex vertexBuilder2(point2);
      TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
      TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

      BRepBuilderAPI_MakeEdge edgeBuilder =
          BRepBuilderAPI_MakeEdge(vertex1, vertex2);
      edge = edgeBuilder.Edge();
    }

    wireBuilder.Add(edge);
  }
  TopoDS_Wire wire = wireBuilder.Wire();

  // Save wire
  this->m_wires.push_back(wire);

  // Clear vertices
  this->m_vertices.clear();
}

void DXFConverter::buildFace() {
  Logger::DEBUG("DXFConverter::buildFace");

  auto size = (uint)this->m_wires.size();
  if (size == 0)
    return;

  // Build face
  BRepBuilderAPI_MakeFace faceBuilder =
      BRepBuilderAPI_MakeFace(this->m_wires[0], true);
  for (uint i = 1; i < size; ++i) {
    TopoDS_Wire wire = this->m_wires[i];
    faceBuilder.Add(wire);
  }

  // Save face
  TopoDS_Shape face = faceBuilder.Shape();
  this->m_faces.push_back(face);

  // Clear wires
  this->m_wires.clear();
}

void DXFConverter::buildShape() {
  Logger::DEBUG("DXFConverter::buildShape");

  // Check if wire build needed
  this->buildWire();
  // Check if face build needed
  this->buildFace();

  auto size = (uint)this->m_faces.size();
  if (size == 0)
    return;

  // Build group of faces
  TopoDS_Compound group;
  BRep_Builder builder;
  builder.MakeCompound(group);
  for (uint i = 0; i < size; ++i) {
    builder.Add(group, this->m_faces[i]);
  }

  // Save
  this->m_shape = group;

  // Clear faces
  this->m_faces.clear();
}
