#version 120

uniform sampler2D Texture0;
uniform bool EmittedMoveWithEmitter;
uniform float CurrentTime;

varying vec4 Color;

void main(void) {
    gl_FragColor = texture2D(Texture0, gl_PointCoord.xy) * Color;
}
