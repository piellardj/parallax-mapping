#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "GLHelper.hpp"
#include "SceneParallax.hpp"


static void initGLEW()
{
    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if( GLEW_OK != err )
        std::cerr << "Error while initializing GLEW: " << glewGetErrorString(err) << std::endl;

    std::cout << "Using GLEW version: " << glewGetString(GLEW_VERSION) << std::endl << std::endl;
}

sf::Vector2f getRelativeMouseCoords(sf::Window const& window);

bool isMouseInWindow(sf::Window const& window);

int main()
{
    /* Window creation, OpenGL initialization */
    sf::ContextSettings openGL3DContext(1, 0, 0, //depth, no stencil, no antialiasing
                                        3, 0, //openGL 3.0 requested
                                        sf::ContextSettings::Default);
    sf::RenderWindow window;
    window.create(sf::VideoMode(800,600), "Parallax mapping",
                  sf::Style::Default,
                  openGL3DContext);
    window.setVerticalSyncEnabled(true);

    /* Checking if the requested OpenGL version is available */
    std::cout << "Using OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Using OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;

    initGLEW();

    if (!sf::Shader::isAvailable()) {
        std::cerr << "Shaders on not available." << std::endl;
        return EXIT_FAILURE;
    }

    /* Font loading */
    sf::Font font;
    if (!font.loadFromFile("fonts/font.ttf")) {
        std::cerr << "Warning: unable to load fonts/font.ttf." << std::endl;
    }
    sf::Text text("", font, 18);
    text.setColor(sf::Color::White);

    /* Scene loading */
    SceneParallax scene(window.getSize().x, window.getSize().y);

    const sf::Clock clock;
    sf::Clock fpsClock;
    sf::Vector2f mousePos = getRelativeMouseCoords(window);
    unsigned int loop = 0;
    while (window.isOpen()) {
        /* Events handling */
        sf::Event event;
        while (window.pollEvent(event)) {
            scene.handleEvents(event);

            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                break;
                case sf::Event::Resized:
                    window.setView(sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height)));
                break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                break;
                default:
                    break;
            }
        }

        sf::Vector2f newMousePos = getRelativeMouseCoords(window);
        if (isMouseInWindow(window) && window.hasFocus()) {
            scene.mouseMoved(newMousePos - mousePos);
        }
        mousePos = newMousePos;

        /* Drawing */
        fpsClock.restart();
        GLCHECK(glViewport(0, 0, window.getSize().x, window.getSize().y));
        GLCHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCHECK(glClearColor(0.2, 0.2, 0.2, 1.0));

        scene.drawScene();

        /* For displaying text, SMFL used old OpenGL */
        float fps = 1.f / fpsClock.getElapsedTime().asSeconds();
        text.setString(scene.getDisplayText(fps));
        window.pushGLStates();
        window.draw(text);
        window.popGLStates();

        window.display();

        loop++;
    }

    std::cout << "Average FPS: " << static_cast<float>(loop) / clock.getElapsedTime().asSeconds() << std::endl;

    return EXIT_SUCCESS;
}

sf::Vector2f getRelativeMouseCoords(sf::Window const& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    sf::Vector2f pos(mousePos.x, window.getSize().y - mousePos.y);
    pos.x /= static_cast<float>(window.getSize().x);
    pos.y /= static_cast<float>(window.getSize().y);
    return pos;
}

bool isMouseInWindow(sf::Window const& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return (mousePos.x >= 0) && (mousePos.y >= 0) &&
           (mousePos.x < window.getSize().x) && (mousePos.y < window.getSize().y);
}
