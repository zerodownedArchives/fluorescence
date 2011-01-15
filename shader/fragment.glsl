
uniform sampler2D HueTexture;
uniform sampler2D ObjectTexture;

uniform vec3 AmbientLightIntensity;
uniform vec3 GlobalLightIntensity;
uniform vec3 GlobalLightDirection;

varying vec3 Normal;

void main(void) {
    // sample actual pixel color
    vec4 rgba = texture2D(ObjectTexture, gl_TexCoord[0].xy);
    gl_FragColor.a = rgba.a;

    float globalAngle = clamp(dot(GlobalLightDirection, Normal), 0.0, 1.0);
    gl_FragColor.rgb = (AmbientLightIntensity + GlobalLightIntensity * globalAngle) * rgba.rgb;


    //vec2 hueTexCoord;
    //float hueId = 60.0;
    //hueTexCoord.x = (rgba.r + rgba.g + rgba.b) / 3.0001;
    //hueTexCoord.y = hueId / 3000.0;

    //rgba.rgb = texture2D(HueTexture, hueTexCoord).rgb;

    //gl_FragColor.rgba = rgba;
}
