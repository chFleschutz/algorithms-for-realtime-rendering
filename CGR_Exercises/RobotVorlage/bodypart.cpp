#include "bodypart.hpp"
#include "manager.hpp"
#include "rendering/renderable.hpp"
#include "transformation/transformation.hpp"
#include "geometry/geometrybase.hpp"
#include "geometry/simplecube.hpp"
#include "rendering/color.hpp"
#include "transformation/relationship.hpp"

BodyPart::BodyPart(float pWidth, float pHeight, float pDepth):
    mSize(pWidth, pHeight, pDepth),
    mTrafoEnt(entt::to_entity(ECS, Manager::addTransformationEntity())),
    mRenderableEnt(Manager::addRenderable<GeometryBase, SimpleCube>(SimpleCube(pWidth, pHeight, pDepth)))
{
    Relationship::addChild(mTrafoEnt, mRenderableEnt);
}

BodyPart::BodyPart(float pWidth, float pHeight, float pDepth, float pRed, float pGreen, float pBlue):
    BodyPart(pWidth, pHeight, pDepth)
{
    setColor(pRed, pGreen, pBlue);
}

void BodyPart::setPosition(float pX, float pY, float pZ)
{
    auto &lTrafo = ECS.get<Transformation>(mTrafoEnt);
    lTrafo.translate(pX, pY, pZ);
}

QVector3D BodyPart::getPosition()
{
    auto lTrafo = ECS.get<Transformation>(mTrafoEnt);
    return lTrafo.getMatrix().column(4).toVector3D();
}

void BodyPart::setColor(float pR, float pG, float pB)
{
    auto lColor = std::make_shared<Color>(pR, pG, pB, 1.f);
    ECS.get<Renderable>(mRenderableEnt).addProperty(lColor);
}

QVector3D BodyPart::getSize() const
{
    return mSize;
}
