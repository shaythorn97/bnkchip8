R"(
#version 330

out vec4 fragColour;
in vec4 vCol;

void main()
{
    fragColour = vCol;
}
)";
