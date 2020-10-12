#include "Union.hpp"

#include <BRepAlgoAPI_Fuse.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
Union::Union() : objects(), tools(), tolerance(0.) {}

/**
 * Constructor
 * @param objects Objects
 * @param tools Tools
 */
Union::Union(std::vector<TopoDS_Shape> &objects,
             std::vector<TopoDS_Shape> &tools, double tolerance) {
  this->objects = TopTools_ListOfShape();
  this->tools = TopTools_ListOfShape();

  for (int i = 0; i < objects.size(); ++i)
    this->objects.Append(objects[i]);

  for (int i = 0; i < tools.size(); ++i)
    this->tools.Append(tools[i]);

  this->tolerance = tolerance;
}

/**
 * Destructor
 */
Union::~Union() {
  this->objects.Clear();
  this->tools.Clear();
}

/**
 * Set objects
 * @param objects Objects
 */
void Union::setObjects(std::vector<TopoDS_Shape> &objects) {
  this->objects = TopTools_ListOfShape();
  for (int i = 0; i < objects.size(); ++i)
    this->objects.Append(objects[i]);
}

/**
 * Set tools
 * @param tools Tools
 */
void Union::setTools(std::vector<TopoDS_Shape> &tools) {
  this->tools = TopTools_ListOfShape();
  for (int i = 0; i < tools.size(); ++i)
    this->tools.Append(tools[i]);
}

/**
 * Set tolerance
 * @param {double} tolerance Tolerance
 */
void Union::setTolerance(double tolerance) { this->tolerance = tolerance; }

/**
 * Compute
 */
TopoDS_Shape Union::compute() {
  BRepAlgoAPI_Fuse fuse;
  TopoDS_Shape result;

  fuse.SetRunParallel(true);

  fuse.SetArguments(this->objects);
  fuse.SetTools(this->tools);

  if (this->tolerance > 0.)
    fuse.SetFuzzyValue(this->tolerance);

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
