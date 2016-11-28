#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <memory>


/* Base class for all scenes.
 * Meant to be linked in the Viewer class */
class Scene
{
    public:
        virtual ~Scene() {}

        virtual std::string getName () const = 0;

        virtual std::string getParametersInfo () const = 0;

        /* For parameters adjustments */
        virtual void handleEvents (sf::Event const& event) = 0;

        /* Mouse moved. Movement is supposed to be given in normalized windows coordinates */
        virtual void mouseMoved(sf::Vector2f const& relativeMovement) {}

        virtual void prepareDraw(sf::RenderWindow& window) {};

        /* Draws in the current OpenGL context. */
        virtual void drawScene(sf::RenderWindow& window) = 0;
};

typedef std::shared_ptr<Scene> ScenePtr;

#endif // SCENE_HPP_INCLUDED
