#include "SceneParallax.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

#include "GLHelper.hpp"


SceneParallax::SceneParallax(unsigned int screenWidth, unsigned int screenHeight):
            _camera(glm::vec3(0.5,0.5,0), 1),
            _displayMode(PARALLAX),
            _amplitude(0.03f),
            _nbLayers(10),
            _interpolation(true),
            _selfShadow(true),
            _crop(false),
            _specularMapping(true),
            _vertBufferID(-1)
{
    _camera.setAspectRatio(screenWidth, screenHeight);
    _camera.setNearDist(0.01f);
    _camera.setFarDist(20.f);
    _spotlight.setFocusPoint(glm::vec3(0.5, 0.5, 0));
    _spotlight.setLongitude(3.f);

    /* Shader loading */
    if (!_simpleShader.loadFromFile("shaders/simple.vert", "shaders/simple.frag")) {
        std::cerr << "Error: unable to load shader simple" << std::endl;
    }
    if (!_shader.loadFromFile("shaders/parallax.vert", "shaders/parallax.frag")) {
        std::cerr << "Error: unable to load parallax" << std::endl;
    }

    /* Textures loading */
    if (!_colorTexture.loadFromFile("textures/color.jpg")) {
        std::cerr << "Error: unable to load textures//color.jpg" << std::endl;
    }
    _colorTexture.setSmooth(true);
    _colorTexture.setRepeated(true);
    if (!_normalsTexture.loadFromFile("textures/normals.jpg")) {
        std::cerr << "Error: unable to load textures/normals.jpg" << std::endl;
    }
    _normalsTexture.setSmooth(true);
    _normalsTexture.setRepeated(true);
    if (!_dispTexture.loadFromFile("textures/displacement.jpg")) {
        std::cerr << "Error: unable to load textures/displacement.jpg" << std::endl;
    }
    _dispTexture.setSmooth(true);
    _dispTexture.setRepeated(true);
    if (!_specTexture.loadFromFile("textures/specular.jpg")) {
        std::cerr << "Error: unable to load textures/specular.jpg" << std::endl;
    }
    _specTexture.setSmooth(true);
    _specTexture.setRepeated(true);

    /* Ground creation */
    std::vector<glm::vec2> vert;
    vert.push_back(glm::vec2(0,0));
    vert.push_back(glm::vec2(+1,+1));
    vert.push_back(glm::vec2(0,+1));
    vert.push_back(glm::vec2(0,0));
    vert.push_back(glm::vec2(+1,+1));
    vert.push_back(glm::vec2(+1,0));

    for (glm::vec2& v : vert)
        v = 10.f * (v - glm::vec2(.5, .5));
    GLCHECK(glGenBuffers(1, &_vertBufferID));

    //Activate buffer and send data to the graphics card
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertBufferID));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, vert.size()*sizeof(glm::vec2), vert.data(), GL_STATIC_DRAW));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

SceneParallax::~SceneParallax()
{
    if (_vertBufferID != (GLuint)(-1)) {
        GLCHECK(glDeleteBuffers(1, &_vertBufferID));
    }
}

std::string SceneParallax::getDisplayText (float fps) const
{
    std::stringstream s;

    s << "fps: " << static_cast<int>(fps) << std::endl << std::endl;

    std::string mode = "FLAT";
    if (_displayMode == NORMAL)
        mode = "NORMAL";
    else if (_displayMode == PARALLAX)
        mode = "PARALLAX";
    s << "display mode (D): " << mode << std::endl << std::endl;

    s << "amplitude (A/Z): " << std::setprecision(3) << _amplitude << std::endl;
    s << "layers (L/M): " << _nbLayers << std::endl;
    s << "interpolation (I): " << _interpolation << std::endl;
    s << "self-shadowing (S): " << _selfShadow << std::endl;
    s << "cropping (C): " << _crop << std::endl;
    s << "specular mapping (V): " << _specularMapping;

    return s.str();
}

void SceneParallax::handleEvents (sf::Event const& event)
{
    switch (event.type) {
        case sf::Event::Resized:
            _camera.setAspectRatio(event.size.width, event.size.height);
        break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::D) {
                _displayMode = static_cast<Mode>((_displayMode + 1) % 3);
            }
            else if (event.key.code == sf::Keyboard::A) {
                _amplitude += 0.01f;
            } else if (event.key.code == sf::Keyboard::Z) {
                _amplitude = std::max(0.f, _amplitude-0.01f);
            }
            else if (event.key.code == sf::Keyboard::L) {
                _nbLayers++;
            } else if (event.key.code == sf::Keyboard::M) {
                _nbLayers = (_nbLayers == 1) ? 1 : _nbLayers-1;
            }
            else if (event.key.code == sf::Keyboard::I) {
                _interpolation = !_interpolation;
            }
            else if (event.key.code == sf::Keyboard::S) {
                _selfShadow = !_selfShadow;
            }
            else if (event.key.code == sf::Keyboard::C) {
                _crop = !_crop;
            }
            else if (event.key.code == sf::Keyboard::V) {
                _specularMapping = !_specularMapping;
            }
        break;
        case sf::Event::MouseWheelScrolled:
            {
                float distance = _camera.getDistance();
                distance *= 1.f + 0.02f * event.mouseWheelScroll.delta;
                _camera.setDistance(distance);
            }
        break;
        default:
            break;
    }
}

void SceneParallax::mouseMoved(sf::Vector2f const& relativeMovement)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        TrackballObject* trackball = nullptr;
        float invertedAxis;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            trackball = &_spotlight;
            invertedAxis = 1.f;
        } else {
            trackball = &_camera;
            invertedAxis = -1.f;
        }

        float latitude = trackball->getLatitude();
        float longitude = trackball->getLongitude();

        longitude += invertedAxis * 2.f * M_PI * relativeMovement.x;
        latitude += invertedAxis * 0.5f * M_PI * relativeMovement.y;

        trackball->setLatitude(latitude);
        trackball->setLongitude(longitude);
    }
}

void SceneParallax::drawScene ()
{
    GLuint shaderHandle = _shader.getNativeHandle();
    if (shaderHandle == 0 || shaderHandle == (GLuint)(-1))
        return;

    GLCHECK(glClearColor(0.8, 0.8, 0.8, 1.0));

    /* Actual drawing */
    sf::Shader::bind(&_shader);
    _shader.setParameter("colorTexture", _colorTexture);
    _shader.setParameter("normalsTexture", _normalsTexture);
    _shader.setParameter("dispTexture", _dispTexture);
    _shader.setParameter("specularTexture", _specTexture);
    _shader.setParameter("amplitude", _amplitude);
    _camera.sendToShader(shaderHandle);
    _spotlight.sendToShader(shaderHandle);

    GLuint posALoc = -1, modeULoc = -1, layersULoc = -1, interpolationULoc = -1, selfShadowULoc = -1, cropULoc = -1, specULoc = -1;
    posALoc = getShaderAttributeLoc(shaderHandle, "vPosition");
    modeULoc = getShaderUniformLoc(shaderHandle, "mode");
    layersULoc = getShaderUniformLoc(shaderHandle, "nbLayers");
    interpolationULoc = getShaderUniformLoc(shaderHandle, "interpolation");
    selfShadowULoc = getShaderUniformLoc(shaderHandle, "selfShadow");
    cropULoc = getShaderUniformLoc(shaderHandle, "crop");
    specULoc = getShaderUniformLoc(shaderHandle, "specularMapping");

    if (modeULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1ui(modeULoc, _displayMode));
    }
    if(posALoc != (GLuint)(-1)) {
        GLCHECK(glEnableVertexAttribArray(posALoc));
        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertBufferID));
        GLCHECK(glVertexAttribPointer(posALoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (layersULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1ui(layersULoc, _nbLayers));
    }
    if (interpolationULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1ui(interpolationULoc, _interpolation));
    }
    if (selfShadowULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1ui(selfShadowULoc, _selfShadow));
    }
    if (cropULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1ui(cropULoc, _crop));
    }
    if (specULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1ui(specULoc, _specularMapping));
    }

    GLCHECK(glDisable(GL_CULL_FACE));
    GLCHECK(glDrawArrays(GL_TRIANGLES, 0, 6));

    if(posALoc != (GLuint)(-1)) {
        GLCHECK(glDisableVertexAttribArray(posALoc));
    }
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));


    sf::Shader::bind(&_simpleShader);
    _camera.sendToShader(_simpleShader.getNativeHandle());
    _spotlight.draw(_simpleShader.getNativeHandle(), _camera);


    sf::Shader::bind(0);
}
