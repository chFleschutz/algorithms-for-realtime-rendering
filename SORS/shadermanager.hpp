#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QHash>
#include <QDir>
#include <QString>

#include "shader.hpp"
#include "system.hpp"

//! Die ShaderManager Klasse erzeugt und verwaltet Shader.\n
//! Sie sorgt dafür, dass nur eine Instanz eines Shaders existiert.\n
//! Beispiel:
//! \code
//! Shader* shader = ShaderManager::getShader("basic.vert", "basic.frag"); // Erzeugt und gibt einen Shader vom
//!                                                                        // Typ "Shader" zurück
//! shader = ShaderManager::getShader("basic.vert", "basic.frag"); // Gibt Shader vom Typ "Shader" zurück. Da dieser
//!                          // bereits existiert wird dieselbe Adresse wie bei obigem Funktionsaufruf zurückgegeben
//! Shader* shader2 = ShaderManager::getShader("phong.vert", "phong.frag"); // Erzeugt und gibt einen Shader vom
//!                                                                         // Typ "Shader" zurück
//! MyShader* shader = ShaderManager::getShader<MyShader>("basic.vert", "basic.frag") // Erzeugt und gibt einen Shader
//!                                                                                   // vom Typ "MyShader" zurück
//! \endcode
class ShaderManager
{
public:
    //! Gibt Shader von gegebenem Templatetyp zurück.
    //! Falls noch kein Shader für den gebenenen Templatetyp sowie für die gegebenen Parameter existiert, wird ein
    //! Shader erzeugt. Die Reihenfolge der Shaderdateien ist Vertex, Fragment, Geometry, TCS und TES. Letztere können fehlen.
    template <typename T = Shader>
    static std::shared_ptr<Shader> getShader(QList<QString> pShaderPaths);
    static std::shared_ptr<Shader> getDefaultShader();

private:
    ShaderManager() = delete;

    //! Ein Shader pro kombination von Shader-Dateien sowie Typen.
    static QHash<QString, std::shared_ptr<Shader>> sShaders;
    //! Der aktuelle Ordner (Working-Directory) von dem aus die Pfade der Shader bestimmt werden. So sind die Pfade
    //! eindeutig und nicht zu lang.
    static QDir sDir;
};

//! Gibt Shader mit den jeweiligen Dateinamen zurück
//! Achtung: Shader, deren einzelne Module (z.B. vert und frag) alle gleiche Namen
//! haben, werden als gleiche Shader interpretiert!
//! Dadurch werden gleich Shader nicht "doppelt" angelegt
template <typename T>
inline std::shared_ptr<Shader> ShaderManager::getShader(QList<QString> pShaderPaths)
{
    QString lKey;

    for (auto& lShaderPath: pShaderPaths)
        lKey.append(sDir.relativeFilePath(lShaderPath));

    if (!sShaders.contains(lKey))
    {
        auto lShader = std::make_shared<T>();
        lShader->initialize(pShaderPaths);
        lShader->setKey(lKey);
        sShaders.insert(lKey, lShader);
        return static_cast<std::shared_ptr<T>>(lShader);
    }
    else
        return static_cast<std::shared_ptr<T>>(sShaders[lKey]);
}

//! Sorgt dafür, dass alle angelegten Shader vor dem Renderpass im richtigen/definierten Zustand sind.
class ShaderSystems: public System
{
public:
    static void preRender();
};

#endif // SHADERMANAGER_H
