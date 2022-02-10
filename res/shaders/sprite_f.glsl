#version 300 es
precision highp float;

in vec2 tex_pos;
in vec2 pos;
out vec4 color;

uniform sampler2D image;
uniform float     ambience;
uniform float     lighting;

uniform float     num_points;

struct PointLight {
  vec2 position;

  float constant;
  float linear;
  float quadratic;
};
#define MAX_POINT_LIGHTS 16
uniform PointLight point_lights[MAX_POINT_LIGHTS];

float calc_point_light(PointLight light)
{
  float distance = length(light.position.xy - pos.xy);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  return attenuation;
}

void main()
{
  vec2 real_tex_pos = vec2(tex_pos.xy);

  real_tex_pos /= vec2(textureSize(image, 0));

  vec4 tex_color = vec4(texture(image, real_tex_pos));
  //if (tex_color.a < 1.0) discard;
  color = tex_color;

  // Light stuff
  float attenuation = 0.0;
  for (int i = 0; i < int(num_points); ++i) {
    attenuation += calc_point_light(point_lights[i]);
  }

  if (lighting > 0.0)
    color = vec4(color.rgb * min(1.0, attenuation), color.a);
}
