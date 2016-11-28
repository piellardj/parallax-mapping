#ifndef SPOTLIGHT_HPP_INCLUDED
#define SPOTLIGHT_HPP_INCLUDED

#include "MeshRenderable.hpp"
#include "TrackballObject.hpp"
#include "Light.hpp"


class SpotLight: public Renderable, public TrackballObject, public Light
{
    public:
        SpotLight (glm::vec3 const& focusPoint=glm::vec3(0,0,0),
                   float distance=2.f,
                   float latitude=3.1415f/8.f,
                   float longitude=-3.1415f/4.f);

        /* Assumes the shader is already binded */
        virtual void sendToShader (GLuint shaderHandle) const;

        glm::vec3 getSpotDir () const;

        /* Maximum value accepted for dot(fromLight, spotDir).
         * For omnidirectional light, set to -2 */
        void setSpotSize (float spotSize);
        float getSpotSize() const;

    protected:
        virtual void positionChanged();

    private:
        void updateMesh ();

        /* Does the actual drawing, once Renderable::draw() checked the shader is ready. */
        virtual void do_draw (GLuint shaderHandle, Camera const& camera) const;


    private:
        float _spotSize;

        MeshRenderablePtr _mesh;
};


typedef std::shared_ptr<SpotLight> SpotLightPtr;

#endif // SPOTLIGHT_HPP_INCLUDED
