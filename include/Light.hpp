#ifndef LIGHT_HPP_INCLUDED
#define LIGHT_HPP_INCLUDED

#include <GL/glew.h>
#include "glm.hpp"
#include <SFML/OpenGL.hpp>


class Light
{
    public:
        Light (glm::vec3 const& color=glm::vec3(1,1,1),
               float intensity=1.f);
        virtual ~Light() {}

        /* Assumes the shader is already binded */
        virtual void sendToShader (GLuint shaderHandle) const = 0;

        void setIntensity (float intensity);
        float getIntensity () const;

        void setColor (glm::vec3 const& color);
        glm::vec3 const& getColor () const;


    private:
        float _intensity;
        glm::vec3 _color;
};

#endif // LIGHT_HPP_INCLUDED
