#version 300 es
precision highp float;

in vec2 tex_pos;
in vec2 pos;
out vec4 color;

uniform sampler2D image;
uniform float     ambience;
uniform float     lighting;

struct Light {
  vec2 position;

  float constant;
  float linear;
  float quadratic;
};
uniform Light light;

void main()
{
  vec2 real_tex_pos = vec2(tex_pos.xy);

  real_tex_pos /= vec2(textureSize(image, 0));

  vec4 tex_color = vec4(texture(image, real_tex_pos));
  //if (tex_color.a < 1.0) discard;
  color = tex_color;

  // Light stuff
  float distance = length(light.position.xy - pos.xy);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  if (lighting > 0.0)
    color = vec4(color.rgb * (ambience * attenuation * lighting), color.a);
}
