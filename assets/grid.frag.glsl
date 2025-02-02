#version 330 core

in vec2 fragPosition;
out vec4 fragColor;

uniform vec4 color;
uniform float cellSize;
uniform float gridWidth;

float grid(vec2 fragCoord, float space, float gridWidth)
{
    vec2 p = fragCoord - vec2(.5);
    vec2 size = vec2(gridWidth);

    vec2 a1 = mod(p - size, space);
    vec2 a2 = mod(p + size, space);
    vec2 a = a2 - a1;

    float g = min(a.x, a.y);
    return clamp(g, 0., 1.0);
}

void main()
{
    float grid = grid(fragPosition, cellSize, gridWidth);
    if (grid == 1.)
        discard;

    fragColor = color;
}
