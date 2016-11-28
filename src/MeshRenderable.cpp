#include "MeshRenderable.hpp"

#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include <fstream>

#include "GLHelper.hpp"
#include "Utilities.hpp"


MeshRenderable::MeshRenderable(std::string const& filename,
                               glm::vec4 const& color) :
            Renderable::Renderable(),
            _textured(false),
            _pBuffer(-1), _cBuffer(-1), _nBuffer(-1), _tBuffer(-1), _iBuffer(-1)
{
    read_obj(filename, _positions, _indices, _normals, _texCoords);

    _colors.resize(_positions.size(), color);
    if (color.r < 0.f) {
        for (std::size_t i=0; i < _colors.size(); ++i)
            _colors[i] = randomColor();
    }

    //Create buffers
    GLCHECK(glGenBuffers(1, &_pBuffer)); //vertices
    GLCHECK(glGenBuffers(1, &_cBuffer)); //colors
    GLCHECK(glGenBuffers(1, &_nBuffer)); //normals
    GLCHECK(glGenBuffers(1, &_tBuffer)); //tex
    GLCHECK(glGenBuffers(1, &_iBuffer)); //indices

    //Activate buffer and send data to the graphics card
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _pBuffer));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, _positions.size()*sizeof(glm::vec3), _positions.data(), GL_STATIC_DRAW));
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _cBuffer));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, _colors.size()*sizeof(glm::vec4), _colors.data(), GL_STATIC_DRAW));
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _nBuffer));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, _normals.size()*sizeof(glm::vec3), _normals.data(), GL_STATIC_DRAW));
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _tBuffer));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, _texCoords.size()*sizeof(glm::vec2), _texCoords.data(), GL_STATIC_DRAW));
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iBuffer));
    GLCHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

MeshRenderable::~MeshRenderable()
{
    if (_pBuffer != (GLuint)(-1)) {
        GLCHECK(glDeleteBuffers(1, &_pBuffer));
    }
    if (_cBuffer != (GLuint)(-1)) {
        GLCHECK(glDeleteBuffers(1, &_cBuffer));
    }
    if (_nBuffer != (GLuint)(-1)) {
        GLCHECK(glDeleteBuffers(1, &_nBuffer));
    }
    if (_tBuffer != (GLuint)(-1)) {
        GLCHECK(glDeleteBuffers(1, &_tBuffer));
    }
    if (_iBuffer != (GLuint)(-1)) {
        GLCHECK(glDeleteBuffers(1, &_iBuffer));
    }
}

void MeshRenderable::loadUVTexture(std::string const& filename)
{
    _textured  = _textured || _UVTexture.loadFromFile(filename);
}

void MeshRenderable::setTextured(bool textured)
{
    _textured = textured;
}

bool MeshRenderable::isTextured() const
{
    return _textured;
}

glm::mat4 const& MeshRenderable::getModelMatrix() const
{
    return _modelMatrix;
}

void MeshRenderable::setModelMatrix (glm::mat4 const& matrix)
{
    _modelMatrix = matrix;
}

void MeshRenderable::do_draw(GLuint shaderHandle, Camera const& camera) const
{
    glm::mat4 normalMatrix = glm::inverseTranspose(camera.getViewMatrix() * _modelMatrix);

    /* First retrieve locations */
    GLuint posALoc = -1, colorALoc = -1, normalALoc = -1, texALoc = -1;
    GLuint modelMatrixULoc = -1, normalMatrixULoc = -1, texturedULoc = -1, UVULoc = -1;

//    try {
        modelMatrixULoc = getShaderUniformLoc(shaderHandle, "modelMatrix", false);
        normalMatrixULoc = getShaderUniformLoc(shaderHandle, "normalMatrix", false);
        texturedULoc = getShaderUniformLoc(shaderHandle, "textured", false);
        UVULoc = getShaderUniformLoc(shaderHandle, "UVTexture", false);

        posALoc = getShaderAttributeLoc(shaderHandle, "vPosition", false);
        colorALoc = getShaderAttributeLoc(shaderHandle, "vColor", false);
        normalALoc = getShaderAttributeLoc(shaderHandle, "vNormal", false);
        texALoc = getShaderAttributeLoc(shaderHandle, "vTexCoords", false);
//    } catch (std::exception const& e) {
//        std::cerr << "Cube3D.drawCube: " << e.what() << std::endl;
//        return;
//    }

    if(modelMatrixULoc != (GLuint)(-1)) {
        GLCHECK(glUniformMatrix4fv(modelMatrixULoc, 1, GL_FALSE, glm::value_ptr(_modelMatrix)));
    }
    if(normalMatrixULoc != (GLuint)(-1)) {
        GLCHECK(glUniformMatrix4fv(normalMatrixULoc, 1, GL_FALSE, glm::value_ptr(normalMatrix)));
    }
    if (texturedULoc != (GLuint)(-1)) {
        GLCHECK(glUniform1ui(texturedULoc, _textured));
    }
    if (UVULoc != (GLuint)(-1)) {
        GLCHECK(glBindTexture(GL_TEXTURE_2D, _UVTexture.getNativeHandle()));
    }

    if(posALoc != (GLuint)(-1)) {
        GLCHECK(glEnableVertexAttribArray(posALoc));
        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _pBuffer));
        GLCHECK(glVertexAttribPointer(posALoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorALoc != (GLuint)(-1) && !_colors.empty()) {
        GLCHECK(glEnableVertexAttribArray(colorALoc));
        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _cBuffer));
        GLCHECK(glVertexAttribPointer(colorALoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(texALoc != (GLuint)(-1) && !_texCoords.empty()) {
        GLCHECK(glEnableVertexAttribArray(texALoc));
        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _tBuffer));
        GLCHECK(glVertexAttribPointer(texALoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(normalALoc != (GLuint)(-1) && !_normals.empty()) {
        GLCHECK(glEnableVertexAttribArray(normalALoc));
        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, _nBuffer));
        GLCHECK(glVertexAttribPointer(normalALoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    GLCHECK(glEnable(GL_DEPTH_TEST));
    GLCHECK(glEnable(GL_CULL_FACE));
    //Draw triangles elements
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iBuffer));
    GLCHECK(glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void*)0));
    //GLCHECK(glDisable(GL_CULL_FACE));

    if (UVULoc != (GLuint)(-1)) {
        GLCHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }
    if(posALoc != (GLuint)(-1)) {
        GLCHECK(glDisableVertexAttribArray(posALoc));
    }

    if(colorALoc != (GLuint)(-1)) {
        GLCHECK(glDisableVertexAttribArray(colorALoc));
    }

    if(texALoc != (GLuint)(-1)) {
        GLCHECK(glDisableVertexAttribArray(texALoc));
    }

    if(normalALoc != (GLuint)(-1)) {
        GLCHECK(glDisableVertexAttribArray(normalALoc));
    }

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
