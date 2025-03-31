$input a_position, a_normal, a_tangent, a_texcoord0
$output v_normal, v_tangent, v_texcoord0

#include "common/common.sh"

uniform mat3 u_normalMatrix;

void main()
{
    // Compute world position
    vec4 worldPos = mul(u_model[0], vec4(a_position, 1.0) );
    gl_Position = mul(u_viewProj, worldPos);

    // Pass normal, tangent, texcoords
    v_normal = mul(u_normalMatrix, a_normal);
    v_tangent = mul(u_model[0], vec4(a_tangent, 0.0)).xyz;
    v_texcoord0 = a_texcoord0;
}