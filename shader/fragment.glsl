
uniform sampler2D Texture0;
varying vec2 TexCoord;

void main(void)
{
//    vec4 rgba = vec4(1.0, 1.0, 0.0, 1.0); 
    vec4 rgba = texture2D(Texture0, TexCoord);
    //rgba.r *= 2;
    //rgba.g *= 2;
    gl_FragColor.rgba = rgba;
}
