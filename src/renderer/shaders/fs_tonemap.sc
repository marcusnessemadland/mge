#include "common/bgfx_shader.sh"
#include "common/bgfx.sh"

SAMPLER2D(s_texColor, 0);

void main()
{

    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;
    vec4 result = texture2D(s_texColor, texcoord);
    gl_FragColor = result;
}
