#include "app_imgui_glfw.h"

AppImGuiGlfw::AppImGuiGlfw(ImGuiBase *imgui_base)
  : ImGuiGlfwBase(imgui_base) {
}

AppImGuiGlfw::~AppImGuiGlfw() {
}

void AppImGuiGlfw::Update(const AppDataPtr &data) {
  assert(data);
  set_clear_color(data->clear_color);
}

void AppImGuiGlfw::OnInit() {
  ImGuiGlfwBase::OnInit();

  // Create Vertex Array Object
  GLuint &vao = vao_;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // X, Y, Z, R, G, B
  GLfloat vertexs[] = {
    -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  };

  // Create Vertex Buffer Object
  GLuint &vbo = vbo_;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

  // Shader sources
  const char *vertex_shader_source = R"glsl(
    #version 150 core
    in vec3 position;
    in vec3 color;
    out vec3 Color;
    void main() {
      Color = color;
      gl_Position = vec4(position, 1.0);
    })glsl";
  const char *fragment_shader_source = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main() {
      outColor = vec4(Color, 1.0);
    })glsl";

  // Create and compile the vertex shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source , NULL);
  glCompileShader(vertex_shader);
  vertex_shader_ = vertex_shader;

  // Create and compile the fragment shader
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source , NULL);
  glCompileShader(fragment_shader);
  fragment_shader_ = fragment_shader;

  // Link the vertex and fragment shader into a shader program
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glBindFragDataLocation(shader_program, 0, "outColor");
  glLinkProgram(shader_program);
  shader_program_ = shader_program;

  // Use our shader
  glUseProgram(shader_program_);

  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shader_program_, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
      6 * sizeof(GLfloat), 0);

  GLint colAttrib = glGetAttribLocation(shader_program_, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
      6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void AppImGuiGlfw::OnDraw() {
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void AppImGuiGlfw::OnDestroy() {
  glDeleteProgram(shader_program_);
  glDeleteShader(vertex_shader_);
  glDeleteShader(fragment_shader_);

  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
}
