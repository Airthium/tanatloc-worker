#ifndef _UNION_HPP_
#define _UNION_HPP_

#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Shape.hxx>

#include <vector>

class Union {
private:
  double tolerance;
  TopTools_ListOfShape objects;
  TopTools_ListOfShape tools;

public:
  // Constructor
  Union();
  // Constructor
  Union(std::vector<TopoDS_Shape>& objects, std::vector<TopoDS_Shape>& tools, double tolerance);
  // Destructor
  ~Union();

  // Set objects
  void setObjects (std::vector<TopoDS_Shape>& objects);

  // Set tools
  void setTools (std::vector<TopoDS_Shape>& tools);

  // Set tolerance
  void setTolerance(double tolerance);

  // Compute
  TopoDS_Shape compute();
};

#endif
