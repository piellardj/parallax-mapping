#include "GLHelper.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

#include <SFML/OpenGL.hpp>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"


void gl_CheckError(const char* file, unsigned int line, const char* expression)
{
    GLenum errorCode = glGetError();
    while (errorCode != GL_NO_ERROR) {
        std::string fileString = file;
        std::string error = "Unknown error";
        std::string description  = "No description";

        switch (errorCode) {
            case GL_INVALID_ENUM:
            {
                error = "GL_INVALID_ENUM";
                description = "An unacceptable value has been specified for an enumerated argument.";
                break;
            }

            case GL_INVALID_VALUE:
            {
                error = "GL_INVALID_VALUE";
                description = "A numeric argument is out of range.";
                break;
            }

            case GL_INVALID_OPERATION:
            {
                error = "GL_INVALID_OPERATION";
                description = "The specified operation is not allowed in the current state.";
                break;
            }

            case GL_STACK_OVERFLOW:
            {
                error = "GL_STACK_OVERFLOW";
                description = "This command would cause a stack overflow.";
                break;
            }

            case GL_STACK_UNDERFLOW:
            {
                error = "GL_STACK_UNDERFLOW";
                description = "This command would cause a stack underflow.";
                break;
            }

            case GL_OUT_OF_MEMORY:
            {
                error = "GL_OUT_OF_MEMORY";
                description = "There is not enough memory left to execute the command.";
                break;
            }

//            case GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION:
//            {
//                error = "GL_INVALID_FRAMEBUFFER_OPERATION";
//                description = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
//                break;
//            }
        }

        // Log the error
        std::cerr << "OpenGL error in "
                  << fileString.substr(fileString.find_last_of("\\/") + 1) << "(" << line << ")."
                  << "\nExpression:\n   " << expression
                  << "\nError description:\n   " << error << "\n   " << description << "\n"
                  << std::endl;
                  
        errorCode = glGetError();
    }
}


GLuint getShaderHandle (sf::Shader const& shader, bool throwExcept)
{
    GLuint shaderID = -1;
    GLCHECK(shaderID = shader.getNativeHandle());
    if (shaderID == (GLuint)(-1) && throwExcept)
        throw std::runtime_error("Error: unable to find shader handle");

    return shaderID;
}

GLuint getShaderUniformLoc (GLuint shaderHandle, std::string const& name, bool throwExcept)
{
    GLuint uniformID = -1;
    GLCHECK(uniformID = glGetUniformLocation(shaderHandle, name.c_str()));

    if (uniformID == (GLuint)(-1) && throwExcept)
        throw std::runtime_error("Error: unable to find uniform " + name);
    //else
    //    std::cerr << "Warning: unable to find uniform " << name << std::endl;

    return uniformID;
}

GLuint getShaderAttributeLoc (GLuint shaderHandle, std::string const& name, bool throwExcept)
{
    GLuint attributeID = -1;
    GLCHECK(attributeID = glGetAttribLocation(shaderHandle, name.c_str()));

    if (attributeID == (GLuint)(-1) && throwExcept)
        throw std::runtime_error("Error: unable to find attribute " + name);
    //else
    //    std::cerr << "Warning: unable to find attribute " << name << std::endl;

    return attributeID;
}

void computeCube (std::vector<glm::vec3>& vertices,
                  std::vector<glm::vec3>& normals)
{

    vertices.resize(4*3*2);
    vertices[0] = glm::vec3(0, 0, 0);
    vertices[1] = glm::vec3(1, 0, 1);
    vertices[2] = glm::vec3(0, 0, 1);
    vertices[3] = glm::vec3(0, 0, 0);
    vertices[4] = glm::vec3(1, 0, 0);
    vertices[5] = glm::vec3(1, 0, 1);

    vertices[6+0] = glm::vec3(1, 0, 0);
    vertices[6+1] = glm::vec3(1, 1, 1);
    vertices[6+2] = glm::vec3(1, 0, 1);
    vertices[6+3] = glm::vec3(1, 0, 0);
    vertices[6+4] = glm::vec3(1, 1, 0);
    vertices[6+5] = glm::vec3(1, 1, 1);

    vertices[12+0] = glm::vec3(1, 1, 0);
    vertices[12+1] = glm::vec3(0, 1, 0);
    vertices[12+2] = glm::vec3(0, 1, 1);
    vertices[12+3] = glm::vec3(1, 1, 0);
    vertices[12+4] = glm::vec3(0, 1, 1);
    vertices[12+5] = glm::vec3(1, 1, 1);

    vertices[18+0] = glm::vec3(0, 1, 0);
    vertices[18+1] = glm::vec3(0, 0, 1);
    vertices[18+2] = glm::vec3(0, 1, 1);
    vertices[18+3] = glm::vec3(0, 1, 0);
    vertices[18+4] = glm::vec3(0, 0, 0);
    vertices[18+5] = glm::vec3(0, 0, 1);

    normals.resize(4*3*2);
    for (unsigned int i = 0 ; i < normals.size() ; ++i) {
        int face = i / 6u;
        if (face == 0)
            normals[i] = glm::vec3(0,-1,0);
        else if (face == 1)
            normals[i] = glm::vec3(1,0,0);
        else if (face == 2)
            normals[i] = glm::vec3(0,1,0);
        else
            normals[i] = glm::vec3(-1,0,0);
    }
}

bool read_obj(const std::string& filename,
        std::vector<glm::vec3>& positions,
        std::vector<unsigned int>& triangles,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords
        )
{
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    bool ret = tinyobj::LoadObj(shapes, materials, err, filename.c_str());

    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }

    if (!ret)
    {
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < shapes.size(); i++)
    {
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        triangles.resize(shapes[i].mesh.indices.size());
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++)
        {
            triangles[3*f+0] = shapes[i].mesh.indices[3*f+0];
            triangles[3*f+1] = shapes[i].mesh.indices[3*f+1];
            triangles[3*f+2] = shapes[i].mesh.indices[3*f+2];
        }
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        positions.resize(shapes[i].mesh.positions.size()/3);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++)
        {
            positions[v] = glm::vec3(shapes[i].mesh.positions[3*v+0], shapes[i].mesh.positions[3*v+1],shapes[i].mesh.positions[3*v+2]);
        }

        assert((shapes[i].mesh.normals.size() % 3) == 0);
        normals.resize(shapes[i].mesh.normals.size()/3);
        for (size_t n = 0; n < shapes[i].mesh.normals.size() / 3; n++)
        {
            normals[n] = glm::vec3(shapes[i].mesh.normals[3*n+0], shapes[i].mesh.normals[3*n+1],shapes[i].mesh.normals[3*n+2]);
        }

        assert((shapes[i].mesh.texcoords.size() % 2) == 0);
        texcoords.resize(shapes[i].mesh.texcoords.size()/2);
        for (size_t t = 0; t < shapes[i].mesh.texcoords.size() / 2; t++)
        {
            texcoords[t] = glm::vec2(shapes[i].mesh.texcoords[2*t+0], shapes[i].mesh.texcoords[2*t+1]);
        }
    }

    return ret;
}
