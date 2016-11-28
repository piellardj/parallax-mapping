#ifndef RENDERABLE_HPP_INCLUDED
#define RENDERABLE_HPP_INCLUDED

#include <GL/glew.h>
#include "glm.hpp"
#include <SFML/OpenGL.hpp>

#include "Camera.hpp"


class Renderable
{
    public:
        /* A valid OpenGL context must be valid to call the constructor */
        Renderable() {}
        virtual ~Renderable() {}

        /* Draws in the current active OpenGL context.
         * Assumes the shader is already binded.
         */
        virtual void draw(GLuint shaderHandle, Camera const& camera) const;

    protected:
        /* Does the actual drawing, once Renderable::draw() checked the shader is ready. */
        virtual void do_draw (GLuint shaderHandle, Camera const& camera) const = 0;
};

#endif // RENDERABLE_HPP_INCLUDED
