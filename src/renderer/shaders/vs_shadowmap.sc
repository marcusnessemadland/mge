$input a_position

#include "common/bgfx_shader.sh"
#include "common/bgfx.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
}
