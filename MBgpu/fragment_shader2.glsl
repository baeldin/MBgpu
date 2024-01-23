#version 430 core

uniform vec2 resolution = vec2(1000., 1000.);

out vec4 fragColor;

void main() {
    vec2 uv = fragCoord / resolution.xy
    fragColor = vec4(0.8, uv.x*0.1, uv.x*0.3, 1.0);
    
}