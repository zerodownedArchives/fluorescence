
uniform mat4 cl_ModelViewProjectionMatrix;

attribute vec2 TexCoord0;
attribute vec3 HueInfo0;

varying vec3 Normal;
varying vec3 HueInfo;

void main(void) {
    gl_Position = cl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0].xy = TexCoord0;

    Normal = gl_Normal;
    HueInfo = HueInfo0;
}

