#include "common/bgfx_shader.sh"
#include "common/samplers.sh"
#include "common/pbr.sh"
#include "common/lights.sh"
#include "common/util.sh"

// G-Buffer
SAMPLER2D(s_texDiffuseA,   SAMPLER_DEFERRED_DIFFUSE_A);
SAMPLER2D(s_texNormal,     SAMPLER_DEFERRED_NORMAL);
SAMPLER2D(s_texF0Metallic, SAMPLER_DEFERRED_F0_METALLIC);
SAMPLER2D(s_texDepth,      SAMPLER_DEFERRED_DEPTH);

uniform vec4 u_lightIndexVec;
#define u_lightIndex uint(u_lightIndexVec.x)

void main()
{
    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;

    vec4 diffuseA = texture2D(s_texDiffuseA, texcoord);
    vec3 N = unpackNormal(texture2D(s_texNormal, texcoord).xy);
    vec4 F0Metallic = texture2D(s_texF0Metallic, texcoord);

    // unpack material parameters used by the PBR BRDF function
    PBRMaterial mat;
    mat.diffuseReflectance = diffuseA.xyz;
    mat.roughnessSquared = diffuseA.w;
    mat.fresnelReflectance = F0Metallic.xyz;
    mat.metallic = F0Metallic.w;

    // get fragment position
    vec4 screen = gl_FragCoord;
    screen.z = texture2D(s_texDepth, texcoord).x;
    vec3 fragPos = screen2Eye(screen).xyz;

    // lighting
    vec3 radianceOut = vec3_splat(0.0);

    DirectionalLight light = getDirectionalLight();

    vec3 V = normalize(-fragPos); // view vector
    float NoV = abs(dot(N, V)) + 1e-5;
    vec3 msFactor = vec3_splat(1.0);

    vec3 L = -light.direction; // light coming *from* the direction
    vec3 radianceIn = light.intensity;
    float NoL = saturate(dot(N, L));
    radianceOut += BRDF(V, L, N, NoV, NoL, mat) * msFactor * radianceIn * NoL;

    gl_FragColor = vec4(radianceOut, 1.0);
}