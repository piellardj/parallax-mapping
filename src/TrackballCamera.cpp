#include "TrackballCamera.hpp"

#include "Utilities.hpp"

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>


static const float F_PI = 3.141592654f;

TrackballCamera::TrackballCamera (glm::vec3 const& focusPoint, float distance, float latitude, float longitude):
            CameraPerspective::CameraPerspective(),
            TrackballObject::TrackballObject(focusPoint, distance, latitude, longitude),
            _cameraUp(glm::vec3(0,0,1))
{
    computeViewMatrix();
}

void TrackballCamera::positionChanged()
{
    computeViewMatrix();
}

glm::vec3 TrackballCamera::getCameraPosition() const
{
    return TrackballObject::getWorldPosition();
}

glm::mat4 const& TrackballCamera::getViewMatrix() const
{
    return _viewMatrix;
}

void TrackballCamera::computeViewMatrix()
{
    _viewMatrix = glm::lookAt(getCameraPosition(), getFocusPoint(), _cameraUp);
}
