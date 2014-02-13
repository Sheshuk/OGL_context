// #version 120
#version 420

uniform mat4 Mproj;
uniform mat4 Mview;
uniform mat4 Mmod;
in vec3 vertex_position;

void main() {
  gl_Position = Mproj*Mview*vec4(vertex_position, 1.0);
  // gl_Position = Mview*vec4(vertex_position, 1.0);
  // gl_Position = vec4(vertex_position, 1.0);
}
