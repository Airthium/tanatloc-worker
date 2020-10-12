#include "Union.hpp"

#include <BRepAlgoAPI_Fuse.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
Union::Union() {}

/**
 * Constructor
 * @param objects Objects
 * @param tools Tools
 */
Union::Union(std::vector<TopoDS_Shape> &objects,
             std::vector<TopoDS_Shape> &tools, double tolerance) {
  this->m_objects = TopTools_ListOfShape();
  this->m_tools = TopTools_ListOfShape();

  for (int i = 0; i < objects.size(); ++i)
    this->m_objects.Append(objects[i]);

  for (int i = 0; i < tools.size(); ++i)
    this->m_tools.Append(tools[i]);

  this->m_tolerance = tolerance;
}

/**
 * Destructor
 */
Union::~Union() {
  this->m_objects.Clear();
  this->m_tools.Clear();
}

/**
 * Set objects
 * @param objects Objects
 */
void Union::setObjects(std::vector<TopoDS_Shape> &objects) {
  this->m_objects = TopTools_ListOfShape();
  for (int i = 0; i < objects.size(); ++i)
    this->m_objects.Append(objects[i]);
}

/**
 * Set tools
 * @param tools Tools
 */
void Union::setTools(std::vector<TopoDS_Shape> &tools) {
  this->m_tools = TopTools_ListOfShape();
  for (int i = 0; i < tools.size(); ++i)
    this->m_tools.Append(tools[i]);
}

/**
 * Set tolerance
 * @param {double} tolerance Tolerance
 */
void Union::setTolerance(double tolerance) { this->m_tolerance = tolerance; }

/**
 * Compute
 */
TopoDS_Shape Union::compute() {
  BRepAlgoAPI_Fuse fuse;
  TopoDS_Shape result;

  fuse.SetRunParallel(true);

  fuse.SetArguments(this->m_objects);
  fuse.SetTools(this->m_tools);

  if (this->m_tolerance > 0.)
    fuse.SetFuzzyValue(this->m_tolerance);

  fuse.Build();

  if (!fuse.IsDone()) {
    Logger::ERROR("Fuse failed");
    return TopoDS_Shape();
  }

  result = fuse.Shape();

  ShapeUpgrade_UnifySameDomain unify(result);
  unify.Build();
  result = unify.Shape();

  return result;
}
