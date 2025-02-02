#version 330 core

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D text;
uniform vec4 color;

void main()
{
    float d = texture(text, fragTexCoord).r;
    float aaf = fwidth(d);
    float alpha = smoothstep(0.5 - aaf, 0.5 + aaf, d);
    fragColor = color * vec4(1.0, 1.0, 1.0, alpha);
}
