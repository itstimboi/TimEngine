#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aSize;

uniform mat4 uProjection;
uniform mat4 uView;
uniform vec3 uBillboardPos;
uniform float uBillboardSize;

out vec2 TexCoords;

void main() {
    // Calculate billboard orientation
    vec3 cameraRight = normalize(vec3(uView[0][0], uView[1][0], uView[2][0]));
    vec3 cameraUp = normalize(vec3(uView[0][1], uView[1][1], uView[2][1]));
    
    // Offset position based on quad size and camera vectors
    vec3 offset = cameraRight * aPos.x * uBillboardSize + cameraUp * aPos.y * uBillboardSize;
    vec3 finalPos = uBillboardPos + offset;
    
    TexCoords = aTexCoords;
    gl_Position = uProjection * uView * vec4(finalPos, 1.0);
}   