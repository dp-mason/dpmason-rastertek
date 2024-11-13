# version 400

// input
in vec3 inputPosition;
in vec3 inputColor;

// output
out vec3 color;

// Uniform vars - information that stays the same across all gpu threads
uniform mat4 worldMatrix; // 4x4 identity matrix?
uniform mat4 viewMatrix; // Camera view?
uniform mat4 projectionMatrix; // perspective or orthographic 2d projection?

// shader
void main(void)
{
    gl_Position = vec4(inputPosition, 1.0) * worldMatrix * viewMatrix * projectionMatrix;

    color = inputColor;
}