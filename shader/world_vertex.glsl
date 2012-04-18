
uniform mat4 cl_ModelViewProjectionMatrix;

attribute vec2 TexCoord0;
attribute vec3 HueInfo0;
attribute float Material0;

varying vec3 Normal;
varying vec3 HueInfo;
varying float Material;

void main(void) {
    gl_Position = cl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0].xy = TexCoord0;

    if (Material0 == 1.0) {
        // tile is water, store vertex position in normal
        Normal.xy = gl_Vertex.xy;
    } else {
        Normal = gl_Normal;
    }
    HueInfo = HueInfo0;
    Material = Material0;
}

