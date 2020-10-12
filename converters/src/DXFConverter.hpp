#ifndef _DXFCONVERTER_HPP_
#define _DXFCONVERTER_HPP_

#include <vector>

#include "dxflib/dl_creationadapter.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>

class DXFConverter : public DL_CreationAdapter {
private:
  std::string m_input = "";
  std::string m_output = "";

  std::vector<DL_VertexData> m_vertices;
  std::vector<TopoDS_Wire> m_wires;
  std::vector<TopoDS_Shape> m_faces;

  TopoDS_Shape m_shape;

  virtual void processCodeValuePair(unsigned int, const std::string &) override;
  virtual void addLine(const DL_LineData &) override;

  virtual void addArc(const DL_ArcData &) override;
  virtual void addCircle(const DL_CircleData &) override;

  virtual void addPolyline(const DL_PolylineData &) override;
  virtual void addVertex(const DL_VertexData &) override;

  virtual void endEntity() override;

  void buildWire();
  void buildFace();
  void buildShape();

public:
  DXFConverter();
  ~DXFConverter();

  void setInput(const std::string &);
  void setOutput(const std::string &);
  bool convert();
  bool write() const;
};

#endif //_DXFCONVERTER_HPP_
