#include "Camera.hpp"

#include "GLHelper.hpp"


void Camera::sendToShader(GLuint shaderHandle) const
{
    if (shaderHandle == 0 || shaderHandle == (GLuint)(-1)) // shader not yet loaded
        return;

    /* First retrieve locations */
    GLuint viewProjULoc = -1, projULoc = -1, viewULoc = -1;
//    try {
    viewProjULoc = getShaderUniformLoc(shaderHandle, "viewProjMatrix", false);
    projULoc = getShaderUniformLoc(shaderHandle, "projMatrix", false);
    viewULoc = getShaderUniformLoc(shaderHandle, "viewMatrix", false);
//    } catch (std::exception const& e) {
//        std::cerr << "Cube3D.drawCube: " << e.what() << std::endl;
//        return;
//    }

    if(viewProjULoc != (GLuint)(-1)) {
        GLCHECK(glUniformMatrix4fv(viewProjULoc, 1, GL_FALSE, glm::value_ptr(getViewProjMatrix())));
    }

    if(projULoc != (GLuint)(-1)) {
        GLCHECK(glUniformMatrix4fv(projULoc, 1, GL_FALSE, glm::value_ptr(getProjectionMatrix())));
    }

    if(viewULoc != (GLuint)(-1)) {
        GLCHECK(glUniformMatrix4fv(viewULoc, 1, GL_FALSE, glm::value_ptr(getViewMatrix())));
    }
}

glm::mat4 Camera::getViewProjMatrix() const
{
    return getProjectionMatrix() * getViewMatrix();
}
