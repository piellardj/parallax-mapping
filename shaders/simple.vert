#version 130


uniform mat4 viewProjMatrix; //proj * view product, precomputed on CPU
uniform mat4 modelMatrix; //model -> world

attribute vec3 vPosition;
attribute vec4 vColor;

out vec4 vertColor;


void main()
{
    vertColor = vColor;
    
    gl_Position = viewProjMatrix * modelMatrix * vec4(vPosition, 1);
}
