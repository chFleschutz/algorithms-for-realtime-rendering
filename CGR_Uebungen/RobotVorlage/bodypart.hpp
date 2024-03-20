#ifndef BODYPART_HPP
#define BODYPART_HPP

#include <QVector3D>
#include "thirdparty/entt/single_include/entt/entt.hpp"

class BodyPart
{
public:
    BodyPart(float pWidth, float pHeight, float pDepth);
    BodyPart(float pWidth, float pHeight, float pDepth, float pRed, float pGreen, float pBlue);
    void setPosition(float pX, float pY, float pZ);
    QVector3D getPosition();
    void setColor(float pR, float pG, float pB);
    QVector3D getSize() const;
    entt::entity getTrafoEnt() {return mTrafoEnt;}
    entt::entity getRenderableEnt() {return mRenderableEnt;}
protected:
    QVector3D mSize;
    entt::entity mTrafoEnt;
    entt::entity mRenderableEnt;
};

#endif // BODYPART_HPP
