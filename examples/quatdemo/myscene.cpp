#include "manager.hpp"
#include "ecs.hpp"
#include "geometry/meshobj.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "transformation/interpolatedrotation.hpp"

void Manager::initialize()
{
    QString lPath(SRCDIR); //aus common.cmake !
    QString lSORSPATH(SORSDIR);

    auto lArrow = MeshOBJ(lPath + QString("models/arrow.obj"));
    auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lArrow);
    auto lIPRotation = addController<InterpolatedRotation>(lEnt);

    lIPRotation->rotateStart(90., QVector3D(.0, 1.0, 0.0)); // Gar nicht gut!
    lIPRotation->rotateEnd(-90, QVector3D(.0, 1.0, 0.));

    lIPRotation->setRepresentAsQuat(true);
    lIPRotation->setRepresentAsQuat(false);
    lIPRotation->setBackandForth(true);
    lIPRotation->setSpeed(0.005f);
}
