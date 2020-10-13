#include "Union.hpp"

#include <algorithm>

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
             std::vector<TopoDS_Shape> &tools, double tolerance)
    : m_tolerance(tolerance) {

  this->m_objects = TopTools_ListOfShape();
  std::for_each(
      objects.begin(), objects.end(),
      [this](const TopoDS_Shape &object) { this->m_objects.Append(object); });

  this->m_tools = TopTools_ListOfShape();
  std::for_each(tools.begin(), tools.end(), [this](const TopoDS_Shape &tool) {
    this->m_tools.Append(tool);
  });
}

/**
 * Set objects
 * @param objects Objects
 */
void Union::setObjects(std::vector<TopoDS_Shape> &objects) {
  this->m_objects = TopTools_ListOfShape();
  std::for_each(
      objects.begin(), objects.end(),
      [this](const TopoDS_Shape &object) { this->m_objects.Append(object); });
}

/**
 * Set tools
 * @param tools Tools
 */
void Union::setTools(std::vector<TopoDS_Shape> &tools) {
  this->m_tools = TopTools_ListOfShape();
  std::for_each(tools.begin(), tools.end(), [this](const TopoDS_Shape &tool) {
    this->m_tools.Append(tool);
  });
}

/**
 * Set tolerance
 * @param {double} tolerance Tolerance
 */
void Union::setTolerance(double tolerance) { this->m_tolerance = tolerance; }

/**
 * Compute
 */
TopoDS_Shape Union::compute() const {
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
