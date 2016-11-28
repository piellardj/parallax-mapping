#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <GL/glew.h>
#include "glm.hpp"


/* Camera */
class Camera
{
    public:
        Camera () {}
        virtual ~Camera() {}

        /* Assumes the shader is already binded */
        virtual void sendToShader (GLuint shaderHandle) const;

        /*  Matrice projection * viewMatrix */
        glm::mat4 getViewProjMatrix () const;

        virtual glm::mat4 const& getViewMatrix() const = 0;
        virtual glm::mat4 const& getProjectionMatrix() const = 0;
};

#endif // CAMERA_HPP_INCLUDED
