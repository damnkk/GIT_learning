// clang-format off
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// clang-format on
#include <cnpy.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <stdint.h>
#include <string>

using namespace std::filesystem;

#define iWidth 512
#define iHeight 512
#define iDepth 117

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
static std::string imagePath = "E:/repository/GIT_learning/volumeData";
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::string readFile(const std::string &path) {
  std::fstream fin(path);
  if (!fin.is_open()) {
    std::cout << "ERROR:Failed to open shader file :" + path << std::endl;
  }
  std::string res, temp;
  while (std::getline(fin, temp)) {
    res += temp + '\n';
  }
  fin.close();
  return res;
}

class Camera {
public:
  glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
  float pitch = 0.0f, yaw = 0.0f, row = 0.0f;
  float m_zNear = 0.01f, m_zFar = 100.f, m_fov = 70.0f, m_aspect = 1.0f;
  float left = -1.0f, right = 1.0f, buttom = -1.0f, top = 1.0f;

  glm::mat4 getViewMat() {
    m_direction.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    m_direction.y = sin(glm::radians(pitch));
    m_direction.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    return glm::lookAt(m_position, m_position + m_direction, m_up);
  }

  glm::mat4 getPrjectMat() {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
  }
};

Camera camera;

std::vector<uint16_t> getArrayFromNpy(const std::string &spath) {
  // std::vector<std::vector<std::vector<uint16_t>>> res;
  // for (int i = 0; i < 117; ++i) {
  //   std::string name = "slice";
  //   std::string num = std::to_string(i);
  //   std::string tail = ".npy";
  //   name += num;
  //   name += tail;
  //   auto test = cnpy::npy_load(spath + '/' + name);
  //   std::vector<std::vector<uint16_t>> slice(
  //       test.shape[0], std::vector<uint16_t>(test.shape[1]));
  //   uint16_t *data = test.data<uint16_t>();
  //   for (int i = 0; i < test.shape[0]; ++i) {
  //     for (int j = 0; j < test.shape[1]; ++j) {
  //       slice[i][j] = data[i * test.shape[0] + j];
  //     }
  //   }
  //   res.push_back(slice);
  // }
  // return res;
  std::vector<uint16_t> res(512 * 512 * 117);
  for (int z = 0; z < 117; ++z) {
    std::string name = "slice";
    std::string num = std::to_string(z);
    std::string tail = ".npy";
    name += num;
    name += tail;
    auto test = cnpy::npy_load(spath + '/' + name);
    uint16_t *data = test.data<uint16_t>();
    for (int y = 0; y < test.shape[0]; ++y) {
      for (int x = 0; x < test.shape[1]; ++x) {
        res[z * test.shape[0] * test.shape[1] + y * test.shape[1] + x] =
            data[y * test.shape[0] + x] * 255 / 4000;
      }
    }
  }
  return res;
};

// clang-format off
float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f};
// clang-format on

float lastX = 300, lastY = 300;
bool firstCall = true;

void mouseCallBack(GLFWwindow *window, double xPos, double yPos) {
  if (firstCall) {
    lastX = xPos;
    lastY = yPos;
    firstCall = false;
    return;
  }
  float sensitive = 0.035;
  float yDiff = yPos - lastY;
  float xDiff = xPos - lastX;
  camera.pitch -= sensitive * yDiff;
  camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);
  camera.yaw += sensitive * xDiff;
  lastX = xPos;
  lastY = yPos;
}

void keyCallBack(GLFWwindow *window) {
  float sensitivity = 0.007;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.m_position += sensitivity * camera.m_direction;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.m_position -= sensitivity * camera.m_direction;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.m_position -=
        sensitivity *
        glm::normalize(glm::cross(camera.m_direction, camera.m_up));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.m_position +=
        sensitivity *
        glm::normalize(glm::cross(camera.m_direction, camera.m_up));
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera.m_position += sensitivity * glm::vec3(0, 1, 0);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    camera.m_position -= sensitivity * glm::vec3(0, 1, 0);
  }
}
GLuint headTexture;

void create3DTexture(std::vector<uint16_t> &arr) {

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &headTexture);
  glBindTexture(GL_TEXTURE_3D, headTexture);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, 512, 512, 117, 0, GL_R16UI,
               GL_UNSIGNED_SHORT, arr.data());
  glEnable(GL_TEXTURE_3D);
  glBindTexture(GL_TEXTURE_3D, 0);
}

GLuint create2DTexture(std::string path) {
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  int textureWidth, textureHeight, numComp;
  unsigned char *image =
      stbi_load(path.c_str(), &textureWidth, &textureHeight, nullptr, 3);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  // delete[] image;
  return tex;
}

static GLubyte image[iDepth][iHeight][iWidth][3];
static GLuint texName;
void makeImage(void) // 造纹理
{
  int s, t, r;

  for (r = 0; r < iDepth; r++) {
    std::string name = "slice";
    std::string num = std::to_string(r);
    std::string tail = ".npy";
    name += num;
    name += tail;
    auto test = cnpy::npy_load(imagePath + '/' + name);
    uint16_t *data = test.data<uint16_t>();
    for (t = 0; t < iHeight; t++)
      for (s = 0; s < iWidth; s++) {
        image[r][t][s][0] = (GLubyte)(data[t * iHeight + s] * 255 / 4000);
        // if (data[t * iHeight + s] > 0) {
        //   auto temp = data[t * iHeight + s];
        // }

        image[r][t][s][1] = (GLubyte)(t * 255 / iHeight);
        image[r][t][s][2] = (GLubyte)(r * 255 / iDepth);
      }
  }
}

void createTex3D() {
  makeImage(); // 产生纹理
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &texName);
  glBindTexture(GL_TEXTURE_3D, texName);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // q是一个缩放因子，相当于顶点坐标中的w。实际应用在纹理读取中的坐标应该是s/q,t/q,r/q。默认情况下，q是1.0。通常情况下貌似没什么用，但是在一些产生纹理坐标的高级算法比如阴影贴图中
  // s、t、r分别相当于普通坐标系中的x、y、z三个方向。分别对应glTexImage3D中的参数width、height、depth。
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, iWidth, iHeight, iDepth, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
}

int main() {
  // glfw: initialize and configure
  // ------------------------------
  camera.m_position = glm::vec3(0.0, 0.0, 3.0f);
  auto array = getArrayFromNpy("E:/repository/GIT_learning/volumeData/");
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertSource =
      readFile("E:/repository/GIT_learning/shader/vertex.vert");
  const char *vertCodePtr = vertSource.c_str();
  glShaderSource(vertexShader, 1, &vertCodePtr, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragSource =
      readFile("E:/repository/GIT_learning/shader/fragment.frag");
  const char *fragCodePtr = fragSource.c_str();
  glShaderSource(fragmentShader, 1, &fragCodePtr, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // link shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s),
  // and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallBack);
  glEnable(GL_DEPTH_TEST);
  // create3DTexture(array);
  createTex3D();

  GLuint tex2D = create2DTexture("E:/repository/GIT_learning/texture2D.png");

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    keyCallBack(window);

    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto viewMat = camera.getViewMat();
    auto projectMat = camera.getPrjectMat();
    auto finalMat = projectMat * viewMat;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "VPMatrix"), 1,
                       GL_FALSE, glm::value_ptr(finalMat));
    glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1,
                 glm::value_ptr(camera.m_position));

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, texName);
    glUniform1i(glGetUniformLocation(shaderProgram, "headTexture"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2D);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2D"), 1);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {

  glViewport(0, 0, width, height);
}

// // clang-format off
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// // clang-format on

// #include <iostream>

// void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void processInput(GLFWwindow *window);

// // settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// const char *vertexShaderSource =
//     "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//     "}\0";
// const char *fragmentShaderSource =
//     "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//     "}\n\0";

// int main() {
//   // glfw: initialize and configure
//   // ------------------------------
//   glfwInit();
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
//   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//   // glfw window creation
//   // --------------------
//   GLFWwindow *window =
//       glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//   if (window == NULL) {
//     std::cout << "Failed to create GLFW window" << std::endl;
//     glfwTerminate();
//     return -1;
//   }
//   glfwMakeContextCurrent(window);
//   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//   // glad: load all OpenGL function pointers
//   // ---------------------------------------
//   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//     std::cout << "Failed to initialize GLAD" << std::endl;
//     return -1;
//   }

//   // build and compile our shader program
//   // ------------------------------------
//   // vertex shader
//   unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//   glCompileShader(vertexShader);
//   // check for shader compile errors
//   int success;
//   char infoLog[512];
//   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//   if (!success) {
//     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//     std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//               << infoLog << std::endl;
//   }
//   // fragment shader
//   unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//   glCompileShader(fragmentShader);
//   // check for shader compile errors
//   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//   if (!success) {
//     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//     std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
//               << infoLog << std::endl;
//   }
//   // link shaders
//   unsigned int shaderProgram = glCreateProgram();
//   glAttachShader(shaderProgram, vertexShader);
//   glAttachShader(shaderProgram, fragmentShader);
//   glLinkProgram(shaderProgram);
//   // check for linking errors
//   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//   if (!success) {
//     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
//               << infoLog << std::endl;
//   }
//   glDeleteShader(vertexShader);
//   glDeleteShader(fragmentShader);

//   // set up vertex data (and buffer(s)) and configure vertex attributes
//   // ------------------------------------------------------------------
//   float vertices[] = {
//       0.5f,  0.5f,  0.0f, // top right
//       0.5f,  -0.5f, 0.0f, // bottom right
//       -0.5f, -0.5f, 0.0f, // bottom left
//       -0.5f, 0.5f,  0.0f  // top left
//   };
//   unsigned int indices[] = {
//       // note that we start from 0!
//       0, 1, 3, // first Triangle
//       1, 2, 3  // second Triangle
//   };
//   unsigned int VBO, VAO, EBO;
//   glGenVertexArrays(1, &VAO);
//   glGenBuffers(1, &VBO);
//   glGenBuffers(1, &EBO);
//   // bind the Vertex Array Object first, then bind and set vertex buffer(s),
//   and
//   // then configure vertex attributes(s).
//   glBindVertexArray(VAO);

//   glBindBuffer(GL_ARRAY_BUFFER, VBO);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
//                GL_STATIC_DRAW);

//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void
//   *)0); glEnableVertexAttribArray(0);

//   // note that this is allowed, the call to glVertexAttribPointer registered
//   VBO
//   // as the vertex attribute's bound vertex buffer object so afterwards we
//   can
//   // safely unbind
//   glBindBuffer(GL_ARRAY_BUFFER, 0);

//   // remember: do NOT unbind the EBO while a VAO is active as the bound
//   element
//   // buffer object IS stored in the VAO; keep the EBO bound.
//   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//   // You can unbind the VAO afterwards so other VAO calls won't accidentally
//   // modify this VAO, but this rarely happens. Modifying other VAOs requires
//   a
//   // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
//   // VBOs) when it's not directly necessary.
//   glBindVertexArray(0);

//   // uncomment this call to draw in wireframe polygons.
//   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//   // render loop
//   // -----------
//   while (!glfwWindowShouldClose(window)) {
//     // input
//     // -----
//     processInput(window);

//     // render
//     // ------
//     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT);

//     // draw our first triangle
//     glUseProgram(shaderProgram);
//     glBindVertexArray(
//         VAO); // seeing as we only have a single VAO there's no need to bind
//         it
//               // every time, but we'll do so to keep things a bit more
//               organized
//     // glDrawArrays(GL_TRIANGLES, 0, 6);
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//     // glBindVertexArray(0); // no need to unbind it every time

//     // glfw: swap buffers and poll IO events (keys pressed/released, mouse
//     moved
//     // etc.)
//     //
//     -------------------------------------------------------------------------------
//     glfwSwapBuffers(window);
//     glfwPollEvents();
//   }

//   // optional: de-allocate all resources once they've outlived their purpose:
//   // ------------------------------------------------------------------------
//   glDeleteVertexArrays(1, &VAO);
//   glDeleteBuffers(1, &VBO);
//   glDeleteBuffers(1, &EBO);
//   glDeleteProgram(shaderProgram);

//   // glfw: terminate, clearing all previously allocated GLFW resources.
//   // ------------------------------------------------------------------
//   glfwTerminate();
//   return 0;
// }

// // process all input: query GLFW whether relevant keys are pressed/released
// this
// // frame and react accordingly
// //
// ---------------------------------------------------------------------------------------------------------
// void processInput(GLFWwindow *window) {
//   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//     glfwSetWindowShouldClose(window, true);
// }

// // glfw: whenever the window size changed (by OS or user resize) this
// callback
// // function executes
// //
// ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
//   // make sure the viewport matches the new window dimensions; note that
//   width
//   // and height will be significantly larger than specified on retina
//   displays. glViewport(0, 0, width, height);
// }