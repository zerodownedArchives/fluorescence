
attribute vec4 Position;
attribute vec2 TexCoord0;
attribute vec2 PositionOffset;

uniform mat4 cl_ModelViewProjectionMatrix;

varying vec2 TexCoord;

void main(void) {
	Position.x += PositionOffset.x;
	Position.y += PositionOffset.y;
	
	gl_Position = cl_ModelViewProjectionMatrix * Position;
	
	TexCoord = TexCoord0;
}

