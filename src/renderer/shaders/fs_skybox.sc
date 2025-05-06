$input v_texcoord0

/*
 * Copyright 2014-2016 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "common/bgfx_shader.sh"
#include "common/bgfx.sh"
#include "common/samplers.sh"

SAMPLERCUBE(s_skyboxCubemap, SAMPLER_SKYBOX_CUBEMAP);
SAMPLER2D(s_gbufferDepth, SAMPLER_DEFERRED_DEPTH);

uniform mat4 u_cameraMtx;

void main()
{
	float deviceDepth = texture2D(s_gbufferDepth, v_texcoord0).x;
	float depth       = toClipSpaceDepth(deviceDepth);

    if (deviceDepth < 0.9999)
    {
        discard;
    }

	float fov = radians(45.0);
	float height = tan(fov*0.5);
	float aspect = height*(u_viewRect.z / u_viewRect.w);
	vec2 tex = (2.0*v_texcoord0-1.0) * vec2(aspect, height);
#if !BGFX_SHADER_LANGUAGE_GLSL
	tex.y = -tex.y;
#endif // !BGFX_SHADER_LANGUAGE_GLSL

	vec3 dir = instMul(u_cameraMtx, vec4(tex, 1.0, 0.0) ).xyz;
	dir = normalize(dir);

	gl_FragColor = vec4(textureCube(s_skyboxCubemap, dir).xyz, 1.0);
}
