$input a_position

#include "common/bgfx_shader.sh"
#include "common/bgfx.sh"

void main()
{
    gl_Position = vec4(a_position.xy, 0.0, 1.0);
}
