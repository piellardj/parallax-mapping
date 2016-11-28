#ifndef TRACKBALLCAMERA_HPP_INCLUDED
#define TRACKBALLCAMERA_HPP_INCLUDED

#include "glm.hpp"

#include "CameraPerspective.hpp"
#include "TrackballObject.hpp"


/* Trackball camera class, computing viewMatrix annd projectionMatrix.
 *
 * Position is defined by a latitude, a longitutde and a distance from focusPoint.
 */
class TrackballCamera: public CameraPerspective, public TrackballObject
{
    public:
        TrackballCamera (glm::vec3 const& focusPoint=glm::vec3(0,0,0),
                         float distance=2.f,
                         float latitude=3.1415f/8.f,
                         float longitude=-3.1415f/4.f);

        virtual glm::mat4 const& getViewMatrix() const;

        /* Position de la caméra */
        virtual glm::vec3 getCameraPosition () const;

    protected:
        virtual void positionChanged();

    private:
        void computeViewMatrix();


    private:
        const glm::vec3 _cameraUp;
        glm::mat4 _viewMatrix;
};

#endif // TRACKBALLCAMERA_HPP_INCLUDED
