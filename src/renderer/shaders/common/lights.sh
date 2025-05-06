#ifndef LIGHTS_SH_HEADER_GUARD
#define LIGHTS_SH_HEADER_GUARD

uniform vec4 u_ambientLightIrradiance;    // xyz = color/intensity, w = unused
uniform vec4 u_directionalLightDirection; // xyz = direction (in view space), w = unused
uniform vec4 u_directionalLightIntensity; // xyz = color/intensity, w = unused

struct AmbientLight
{
    vec3 irradiance;
};

struct DirectionalLight
{
    vec3 direction; 
    vec3 intensity; 
};

AmbientLight getAmbientLight()
{
    AmbientLight light;
    light.irradiance = u_ambientLightIrradiance.xyz;
    return light;
}

DirectionalLight getDirectionalLight()
{
    DirectionalLight light;
    light.direction = normalize(u_directionalLightDirection.xyz);
    light.intensity = u_directionalLightIntensity.xyz;
    return light;
}

#endif // LIGHTS_SH_HEADER_GUARD
