#version 120

uniform sampler2D Texture0;
uniform bool EmittedMoveWithEmitter;
uniform float CurrentTime;
uniform vec4 TextureInfo; // 0/1 starting tex coords, 2/3 size

varying vec4 Color;
varying float FrameIndex;

void main(void) {
    vec2 texCoords = TextureInfo.xy + gl_PointCoord.xy * TextureInfo.zw;
    texCoords.x += FrameIndex * TextureInfo.z;
    gl_FragColor = texture2D(Texture0, texCoords) * Color;
}
