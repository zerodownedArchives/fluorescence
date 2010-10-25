
attribute vec4 Position;
attribute vec2 TexCoord0;

uniform mat4 cl_ModelViewProjectionMatrix;

varying vec2 TexCoord;

void main(void)
{
	Position.x = Position.x - 300;
	Position.y -= 200;
	gl_Position = cl_ModelViewProjectionMatrix * Position;
	
	TexCoord = TexCoord0;
}

