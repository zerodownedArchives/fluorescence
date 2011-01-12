
uniform vec2 PositionOffset;
uniform mat4 cl_ModelViewProjectionMatrix;

attribute vec4 Position;
attribute vec2 TexCoord0;

void main(void) {
    vec4 movedPosition = vec4(Position.x - PositionOffset.x, Position.y - PositionOffset.y, Position.z, Position.w);

    gl_Position = cl_ModelViewProjectionMatrix * movedPosition;

    gl_TexCoord[0].xy = TexCoord0;
}

