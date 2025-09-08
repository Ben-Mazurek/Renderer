//Shader subclass specifically for creating the main raymarching shader
#pragma once
#include "Shader.h"
#include<stdio.h>
#include<time.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

class RmShader :
    public Shader
{
private:
    float startTime;

    struct PhongMat {       //Vec4s are used instead of vec3s because of glsl padding reqirements https://stackoverflow.com/questions/38172696/should-i-ever-use-a-vec3-inside-of-a-uniform-buffer-or-shader-storage-buffer-o
        glm::vec4 AmbRef;
        glm::vec4 diffMat;
        glm::vec4 specMat;
        glm::vec4 AmbLight;
        float shiny;
    };
public:
    RmShader(int screenWidth, int screenHeight);
    void Update() override;
    void setViewRot(glm::mat3);
    void setViewPos(glm::vec3);
};

