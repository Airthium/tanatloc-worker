#ifndef _DXFCONVERTER_HPP_
#define _DXFCONVERTER_HPP_

#define DEBUG 1

#include <vector>

#include "dxflib/dl_creationadapter.h"

#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>

class DXFConverter : public DL_CreationAdapter {
private:
  std::string input;
  std::string output;

  std::vector<DL_VertexData> vertices;
  std::vector<TopoDS_Wire> wires;
  std::vector<TopoDS_Shape> faces;

  TopoDS_Shape shape;

  virtual void processCodeValuePair(unsigned int, const std::string&);
  virtual void addLine(const DL_LineData&);

  virtual void addArc(const DL_ArcData&);
  virtual void addCircle(const DL_CircleData&);

  virtual void addPolyline(const DL_PolylineData&);
  virtual void addVertex(const DL_VertexData&);

  virtual void endEntity();

  void buildWire();
  void buildFace();
  void buildShape();

public:
  DXFConverter();
  ~DXFConverter();

  void setInput(const std::string&);
  void setOutput(const std::string&);
  bool convert();
  bool write();
};

#endif //_DXFCONVERTER_HPP_
