
uniform mat4 cl_ModelViewProjectionMatrix;

uniform sampler2D Texture0;
uniform vec3 EmitterMovement;
uniform float CurrentTime;

attribute vec3 LocationStart;
attribute vec3 VelocityStart;
attribute vec3 VelocityEnd;

attribute vec2 Lifetime; // 0 - creation, 1 - expiration

attribute vec4 ColorStart;
attribute vec4 ColorEnd;

varying vec4 Color;


void main(void) {
    float age = CurrentTime - Lifetime[0];
    float expireAge = (Lifetime[1] - Lifetime[0]);
    float normalizedAge = age / expireAge;
    
    if (normalizedAge > 1.0) {
        Color = vec4(0);
        return;
    }
    
    // interpolate position
    vec3 positionDelta = VelocityStart * normalizedAge + 
            (VelocityEnd - VelocityStart) * normalizedAge * normalizedAge / 2.0;
    
    gl_Position.xyz = LocationStart + EmitterMovement + positionDelta * expireAge;
    gl_Position.w = 1;
    
    gl_Position = cl_ModelViewProjectionMatrix * gl_Position;
    
    gl_PointSize = 5.0;
    
    // interpolate color
    Color = mix(ColorStart, ColorEnd, normalizedAge);
}

