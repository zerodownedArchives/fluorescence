
uniform mat4 cl_ModelViewProjectionMatrix;

attribute vec2 TexCoord0;
attribute vec3 HueInfo0;
attribute float RenderEffect0;

varying vec3 Normal;
varying vec3 HueInfo;
varying float RenderEffect;

void main(void) {
    gl_Position = cl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0].xy = TexCoord0;

    if (RenderEffect0 == 1 && false) {
        // tile is water, store vertex position in normal
        Normal.xy = gl_Position.xy;
    } else {
        Normal = gl_Normal;
    }
    HueInfo = HueInfo0;
    RenderEffect = RenderEffect0;
}

