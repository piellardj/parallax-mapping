#ifndef SCENEPARALLAX_HPP_INCLUDED
#define SCENEPARALLAX_HPP_INCLUDED

#include "SpotLight.hpp"
#include "TrackballCamera.hpp"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>


class SceneParallax
{
    public:
        SceneParallax(unsigned int screenWidth, unsigned int screenHeight);
        ~SceneParallax();

        std::string getDisplayText (float fps) const;

        /* For parameters adjustments */
        void handleEvents (sf::Event const& event);

        /* Mouse moved. Movement is supposed to be given in normalized windows coordinates */
        void mouseMoved(sf::Vector2f const& relativeMovement);

        /* Draws in the current OpenGL context. */
        void drawScene();


    private:
        enum Mode{FLAT, NORMAL, PARALLAX};

        TrackballCamera _camera;
        SpotLight _spotlight;

        Mode _displayMode;
        float _amplitude;
        unsigned int _nbLayers;
        bool _interpolation;
        bool _selfShadow;
        bool _crop;
        bool _specularMapping;

        sf::Texture _colorTexture;
        sf::Texture _normalsTexture;
        sf::Texture _dispTexture;
        sf::Texture _specTexture;

        GLuint _vertBufferID;

        sf::Shader _simpleShader;
        sf::Shader _shader;
};

#endif // SCENEPARALLAX_HPP_INCLUDED
