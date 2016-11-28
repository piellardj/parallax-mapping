#include "Renderable.hpp"

void Renderable::draw (GLuint shaderHandle, Camera const& camera) const
{
    if (shaderHandle == 0 || shaderHandle == (GLuint)(-1)) // shader not yet loaded
        return;

    do_draw(shaderHandle, camera);
}
