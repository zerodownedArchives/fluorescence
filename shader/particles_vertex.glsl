
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

attribute vec2 Sizes; // 0 - start, 1 - end
attribute vec2 FrameIndices; // 0 - start, 1 - end

varying vec4 Color;
varying float FrameIndex;


void main(void) {
    float age = CurrentTime - Lifetime[0];
    float expireAge = (Lifetime[1] - Lifetime[0]);
    float normalizedAge = age / expireAge;
    
    // interpolate position
    vec3 positionDelta = VelocityStart * normalizedAge + 
            (VelocityEnd - VelocityStart) * normalizedAge * normalizedAge / 2.0;
    
    gl_Position.xyz = LocationStart + EmitterMovement + positionDelta * expireAge;
    gl_Position.w = 1;
    
    gl_Position = cl_ModelViewProjectionMatrix * gl_Position;
    
    gl_PointSize = mix(Sizes.x, Sizes.y, normalizedAge);
    
    FrameIndex = floor(mix(FrameIndices.x, FrameIndices.y, normalizedAge));
    
    // interpolate color
    Color = mix(ColorStart, ColorEnd, normalizedAge);
}

