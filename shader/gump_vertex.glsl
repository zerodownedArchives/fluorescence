
uniform mat4 cl_ModelViewProjectionMatrix;

attribute vec2 TexCoord0;
attribute vec2 HueInfo0;

varying vec2 HueInfo;

void main(void) {
    gl_Position = cl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0].xy = TexCoord0;

    HueInfo = HueInfo0;
}

