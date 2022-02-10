#version 300 es
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec2 tex_pos_in;

out vec2 tex_pos;
out vec2 pos;

uniform mat4 projection;

void main()
{
  gl_Position = projection * vec4(pos_in.x, pos_in.y, pos_in.z, 1.0);
  tex_pos = tex_pos_in;
  pos = vec2(pos_in.x, pos_in.y);
}
