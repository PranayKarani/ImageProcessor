#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 texCoord;

out vec2 tc;

uniform int makeInverse;

void main(){

    if(makeInverse > 0){
        gl_Position = vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0f);
    } else {
        gl_Position = vec4(vertexPos.x, -vertexPos.y, vertexPos.z, 1.0f);
    }
    tc = texCoord;

}

