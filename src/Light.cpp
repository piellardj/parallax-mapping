#include "Light.hpp"

#include "GLHelper.hpp"


Light::Light(glm::vec3 const& color, float intensity):
            _intensity (intensity),
            _color (color)
{
}

void Light::sendToShader(GLuint shaderHandle) const
{
if (shaderHandle == 0 || shaderHandle == (GLuint)(-1)) // shader not yet loaded
        return;

    /* First retrieve locations */
    GLuint lightColorULoc = -1, lightIntensityULoc = -1;
//    try {
    lightColorULoc = getShaderUniformLoc(shaderHandle, "lightColor", false);
    lightIntensityULoc = getShaderUniformLoc(shaderHandle, "lightIntensity", false);
//    } catch (std::exception const& e) {
//        std::cerr << "Cube3D.drawCube: " << e.what() << std::endl;
//        return;
//    }



    if(lightColorULoc != (GLuint)(-1)) {
        GLCHECK(glUniform3f(lightColorULoc, getColor().r, getColor().g, getColor().b));
    }

    if(lightIntensityULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1f(lightIntensityULoc, getIntensity()));
    }
}

void Light::setIntensity (float intensity)
{
    _intensity = intensity;
}

float Light::getIntensity () const
{
    return _intensity;
}

void Light::setColor (glm::vec3 const& color)
{
    _color = color;
}

glm::vec3 const& Light::getColor () const
{
    return _color;
}
