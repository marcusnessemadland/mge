$input v_normal, v_tangent, v_texcoord0

#define READ_MATERIAL

#include "common/bgfx_shader.sh"
#include "common/util.sh"
#include "common/pbr.sh"

void main()
{
    PBRMaterial mat = pbrMaterial(v_texcoord0);

    // Calculate normal
    vec3 N = convertTangentNormal(v_normal, v_tangent, mat.normal);
    N = mul(u_view, vec4(N, 0.0)).xyz;
   
    // Calculate roughness
    mat.roughnessSquared = specularAntiAliasing(N, mat.roughnessSquared);

    // Pack G-Buffer
    gl_FragData[0] = vec4(mat.diffuseReflectance, mat.roughnessSquared);
    gl_FragData[1] = vec4(packNormal(N), 0.0, 0.0);
    gl_FragData[2] = vec4(mat.fresnelReflectance, mat.metallic);
    gl_FragData[3] = vec4(mat.emissive, mat.occlusion);
}