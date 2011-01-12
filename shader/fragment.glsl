
uniform sampler2D HueTexture;
uniform sampler2D ObjectTexture;

void main(void) {
    // sample actual pixel color
    vec4 rgba = texture2D(ObjectTexture, gl_TexCoord[0].xy);

    vec2 hueTexCoord;
    float hueId = 60;
    hueTexCoord.x = (rgba.r + rgba.g + rgba.b) / 3.0001;
    hueTexCoord.y = hueId / 3000;

    rgba.rgb = texture2D(HueTexture, hueTexCoord);

    gl_FragColor.rgba = rgba;
}
