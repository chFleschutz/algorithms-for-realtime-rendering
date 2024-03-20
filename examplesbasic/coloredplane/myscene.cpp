#include "manager.hpp"
#include "ecs.hpp"
#include "shadermanager.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "geometry/simpleplane.hpp"

class ColoredPlane: public SimplePlane
{
public:
    ColoredPlane(): SimplePlane(1.0f) {}
protected:
    virtual void fillColors(QVector<QVector4D>& colors) override;
};

void ColoredPlane::fillColors(QVector<QVector4D>& colors)
{
    colors.clear();
    for (unsigned int y = 0; y < mDivs; y++)
    {
        for (unsigned int x = 0; x <= mDivs; x++)
        {
            colors.append(QVector4D(x / (float)mDivs * mXSize, (y + 1) / (float)mDivs * mYSize, 1.0f, 1.0f));
            colors.append(QVector4D(x / (float)mDivs * mXSize, y / (float)mDivs * mYSize, 1.0f, 1.0f));
        }
    }
}

void Manager::initialize()
{
    QString lPath(SRCDIR); //aus common.cmake !
    QString lSORSPATH(SORSDIR);

    auto lColoredPlane = addRenderable<GeometryBase, ColoredPlane>(ColoredPlane());

    auto lKeyTrans = addController<KeyboardTransformationController>(lColoredPlane);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');

}
