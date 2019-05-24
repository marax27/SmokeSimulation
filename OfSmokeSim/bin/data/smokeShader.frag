#version 150

in vec2 texCoordVarying;

out vec4 outputColor;

uniform float alpha;

void main() {
    float color = 0.8;
    outputColor = vec4(color, color, color, alpha);
}