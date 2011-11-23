uniform mat4 cl_ModelViewProjectionMatrix;
uniform bool EmittedMoveWithEmitter;
uniform float CurrentTime;

attribute vec3 PositionStart;
attribute vec3 VelocityStart;
attribute vec3 VelocityEnd;

attribute float CreationTime;
attribute float LifetimeSeconds;

attribute vec4 ColorStart;
attribute vec4 ColorEnd;


void main(void) {
    float age = CurrentTime - CreationTime;
    float normalizedAge = age / LifetimeSeconds;
    
    vec3 positionDelta = VelocityStart * normalizedAge + 
            (VelocityEnd - VelocityStart) * normalizedAge * normalizedAge / 2.0;
    
    gl_Position.xyz = PositionStart + positionDelta * LifetimeSeconds;
    
    gl_Position = cl_ModelViewProjectionMatrix * gl_Position;
}

