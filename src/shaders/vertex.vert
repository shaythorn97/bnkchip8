R"(
#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

out vec4 vCol;

uniform mat4 u_vp;

void main()
{
    gl_Position = u_vp * vec4(aPos, 1.0f);
    vCol = aCol;
}
)";
