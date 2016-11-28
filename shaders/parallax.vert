#version 130


uniform mat4 viewProjMatrix; //world space -> window space

in vec2 vPosition;

out vec3 vertWorldPos;


void main( void )
{
    vertWorldPos = vec3(vPosition,0);
    
    gl_Position = viewProjMatrix * vec4(vPosition, 0, 1);
}
