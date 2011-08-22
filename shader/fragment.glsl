
uniform sampler2D HueTexture;
uniform sampler2D ObjectTexture;

uniform vec3 AmbientLightIntensity;
uniform vec3 GlobalLightIntensity;
uniform vec3 GlobalLightDirection;

varying vec3 Normal;
varying vec2 HueInfo;

void main(void) {
    // sample actual pixel color
    vec4 rgba = texture2D(ObjectTexture, gl_TexCoord[0].xy);

    if (rgba.a == 0) {
        discard;
    }

    gl_FragColor.a = rgba.a;

    if (HueInfo[1] != 0.0) {
        // object has hue
        if (HueInfo[0] == 0.0 || (HueInfo[0] == 1.0 && rgba.r == rgba.g && rgba.g == rgba.b)) {
            // object is either non-partial-hue or current pixel is grey
            vec2 hueTexCoord;
            float hueId = HueInfo[1];
            hueTexCoord.x = (rgba.r + rgba.g + rgba.b) / 3.0001;
            hueTexCoord.y = hueId / 3000.0;

            rgba.rgb = texture2D(HueTexture, hueTexCoord).rgb;
        }
    }


    float globalAngle = clamp(dot(GlobalLightDirection, Normal), 0.0, 1.0);
    gl_FragColor.rgb = (AmbientLightIntensity + GlobalLightIntensity * globalAngle) * rgba.rgb;
}
