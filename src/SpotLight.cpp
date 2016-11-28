#include "SpotLight.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "GLHelper.hpp"


SpotLight::SpotLight (glm::vec3 const& focusPoint,
                      float distance,
                      float latitude,
                      float longitude):
            Renderable::Renderable(),
            TrackballObject::TrackballObject(focusPoint, distance, latitude, longitude),
            Light::Light(),
            _spotSize(0.8f)
{
    try {
        _mesh = std::make_shared<MeshRenderable>("models/arrow.obj", glm::vec4(1,1,0,1));
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    updateMesh ();
}

void SpotLight::sendToShader (GLuint shaderHandle) const
{
    Light::sendToShader(shaderHandle);

    if (shaderHandle == 0 || shaderHandle == (GLuint)(-1)) // shader not yet loaded
        return;

    /* First retrieve locations */
    GLuint lightPosULoc = -1, spotDirULoc = -1, spotSizeULoc = -1;
//    try {
    spotDirULoc = getShaderUniformLoc(shaderHandle, "normalizedSpotDirection", false);
    spotSizeULoc = getShaderUniformLoc(shaderHandle, "spotSize", false);
    lightPosULoc = getShaderUniformLoc(shaderHandle, "lightWorldPos", false);
//    } catch (std::exception const& e) {
//        std::cerr << "Cube3D.drawCube: " << e.what() << std::endl;
//        return;
//    }

    if (spotSizeULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1f(spotSizeULoc, getSpotSize()));
    }
    if (spotDirULoc != (GLuint)(-1)) {
        glm::vec3 spotDir = getSpotDir();
        GLCHECK(glUniform3f(spotDirULoc, spotDir.x, spotDir.y, spotDir.z));
    }
    if(lightPosULoc != (GLuint)(-1)) {
        glm::vec3 worldPos = getWorldPosition();
        GLCHECK(glUniform3f(lightPosULoc, worldPos.x, worldPos.y, worldPos.z));
    }
}

glm::vec3 SpotLight::getSpotDir () const
{
    return -getRelativePosition() / glm::length(getRelativePosition());
}

void SpotLight::setSpotSize (float spotSize)
{
    _spotSize = spotSize;
}

float SpotLight::getSpotSize() const
{
    return _spotSize;
}

void SpotLight::positionChanged()
{
    updateMesh();
}

void SpotLight::updateMesh ()
{
    if (!_mesh)
        return;

    glm::mat4 modelMatrix; //identity

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
    modelMatrix = glm::inverse(glm::lookAt(getWorldPosition(), getFocusPoint(), glm::vec3(0, 0, 1))) * modelMatrix;

    _mesh->setModelMatrix (modelMatrix);
}

void SpotLight::do_draw (GLuint shaderHandle, Camera const& camera) const
{
    if (_mesh)
        _mesh->draw (shaderHandle, camera);
}
