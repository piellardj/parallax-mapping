#ifndef TRACKBALLOBJECT_HPP_INCLUDED
#define TRACKBALLOBJECT_HPP_INCLUDED

#include "glm.hpp"



/* Trackball object base class.
 *
 * Position is defined by a latitude, a longitutde and a distance from focusPoint.
 */
class TrackballObject
{
    public:
        TrackballObject (glm::vec3 const& focusPoint,
                         float distance,
                         float latitude,
                         float longitude);
        virtual ~TrackballObject() {}

        void setFocusPoint (glm::vec3 focusPoint);
        glm::vec3 const& getFocusPoint () const;

        /* Positive, clamped to 0 otherwise */
        void setDistance (float distance);
        float getDistance () const;

        /* In radians */
        void setLatitude (float latitude);
        float getLatitude () const;

        /* In radians, between [-PI/2, PI/2], clamped otherwise*/
        void setLongitude (float longitude);
        float getLongitude () const;

        glm::vec3 getRelativePosition () const;
        glm::vec3 getWorldPosition () const;

    protected:
        virtual void positionChanged () {}


    private:
        glm::vec3 _focusPoint;
        float _distance;
        float _latitude;
        float _longitude;
};

#endif // TRACKBALLOBJECT_HPP_INCLUDED
