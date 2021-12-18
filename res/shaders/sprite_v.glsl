#version 300 es
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_pos_in;

out vec2 tex_pos;

uniform mat4 projection;

void main()
{
  gl_Position = projection * vec4(pos.x, pos.y, pos.z, 1.0);
  tex_pos = tex_pos_in;
}
