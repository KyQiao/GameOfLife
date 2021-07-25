#include <iostream>
#include <string>

#include "argh.h"
#include "life.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

double TARGET_FPS = 30.0;

// Function prototypes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Shaders
const GLchar *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
    "}\n\0";

void renderVertex(int H, int W, std::vector<GLfloat> *V) {
  GLdouble Hstep = 2.0 / (H + 1.0);
  GLdouble Wstep = 2.0 / (W + 1.0);
  GLint index = 0;
  GLfloat x = -1, y = 1;
  V->resize(3 * (H + 1) * (W + 1));

  // loop for row
  for (GLint i = 0; i < H + 1; i++) {
    // loop for column
    for (GLint j = 0; j < W + 1; j++) {
      index = 3 * (i * (W + 1) + j);
      V->data()[index] = x;
      V->data()[index + 1] = y;
      V->data()[index + 2] = 0.0f;
      x += static_cast<GLfloat>(Wstep);
    }
    y -= static_cast<GLfloat>(Hstep);
    x = -1;
  }
};
int renderIndices(std::vector<GLint> *indices, int *data, int H, int W) {
  GLint index, indiceIndex = 0;
  int res = 0;
  // loop for row
  indices->resize(3 * (H + 1) * (W + 1));
  for (GLint i = 0; i < H; i++) {
    // loop for column
    for (GLint j = 0; j < W; j++) {
      index = (i * W + j);
      if (data[index] == 1) {
        indices->data()[indiceIndex] = index + i;
        indices->data()[indiceIndex + 1] = index + i + 1;
        indices->data()[indiceIndex + 2] = index + i + W + 2;
        indices->data()[indiceIndex + 3] = index + i;
        indices->data()[indiceIndex + 4] = index + i + W + 1;
        indices->data()[indiceIndex + 5] = index + i + W + 2;
        indiceIndex += 6;
        res += 1;
      }
    }
  };
  return 6 * res;
};

GLFWwindow *setWindow(int H, int W) {
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow *window = glfwCreateWindow(W, H, "LIFE GAME", nullptr, nullptr);
  glfwSetWindowSizeLimits(window, 200, 200, 800, 800);
  glfwSetWindowAspectRatio(window, W, H);
  glfwMakeContextCurrent(window);
  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);

  return window;
}

void init() {
  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();
}

void setViewpoint(GLFWwindow *window) {
  // Define the viewport dimensions
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
}

GLuint setVertexShader() {
  // Vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Check for compile time errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  return vertexShader;
}

GLuint setFragmentShader() {
  // Fragment shader
  GLint success;
  GLchar infoLog[512];
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Check for compile time errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return fragmentShader;
}

GLuint linkShader(GLuint &vertexShader, GLuint &fragmentShader) {
  GLint success;
  GLchar infoLog[512];
  // Link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  return shaderProgram;
}

void bindBuffer(GLuint &VBO, GLuint &VAO, GLuint &EBO, std::vector<GLfloat> *vertices, std::vector<GLint> *indices) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices->size(), vertices->data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * indices->size(), indices->data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray(0);  // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

void help() {
  std::cout << "Game of Life with OpenGL \n"
            << "Use a random configuration or input a rle file\n"
            << "    GameOfLife --help: display this message\n"
            << "               --np: # of threads used, default all\n"
            << "               -f: read a file with format rle\n"
            << "               -b: no PBC. If read from file, -b by default, otherwise PBC by default.\n"
            << "               -d: work if not PBC, a edge is add when reading from file,default 10\n"
            << "               -H -W -N: height width and # of init live cell\n"
            << "               --fps: control the target fps, default 30\n"
            << "example:\n"
            << "    GameOfLife -f=file -np=4\n"
            << "    GameOfLife -f=file -np=4 -d=30\n"
            << "    GameOfLife -H=400 -W=400 -N=10000 -np=4 --fps=40\n"
            << "    GameOfLife -H=100 -W=100 -N=5000 -np=4 -b\n"
            << std::endl;
}

// The MAIN function, from here we start the application and run the game loop
int main(int argc, char const *argv[]) {
  // arg parse part
  auto cmdl = argh::parser(argc, argv);

  std::string file;
  bool bound = false;
  int np = -1;
  int H, W, N, edge = 10;
  life *game = nullptr;

  if (argc == 1 || cmdl["help"]) {
    help();
    return 0;
  }
  if (!(cmdl("fps") >> TARGET_FPS)) {
    TARGET_FPS = 30.0;
  };
  if (!(cmdl("np") >> np)) {
    np = -1;
  };
  if (cmdl["b"]) {
    bound = true;
    if (!(cmdl("d") >> edge)) {
      edge = 10;
    }
  };

  if (cmdl("f") >> file) {
    bound = true;
    game = new life(file, edge);
    H = game->getH();
    W = game->getW();
  } else if (cmdl("H") >> H && cmdl("W") >> W && cmdl("N") >> N) {
    if (N > W * H) {
      std::cerr << "size too small or live cell too much" << std::endl;
      return -1;
    }
    game = new life(H, W, N, 534134);
  } else {
    std::cout << "incomplete or wrong input\n";
    help();
    return -1;
  }

  int *data = game->getData();

  GLFWwindow *window = setWindow(H, W);
  init();
  setViewpoint(window);

  // Build and compile our shader program
  GLuint vertexShader, fragmentShader;

  vertexShader = setVertexShader();
  fragmentShader = setFragmentShader();
  GLint shaderProgram = linkShader(vertexShader, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // ###################################################################################

  // ###################################################################################

  std::vector<GLfloat> *vertices = new std::vector<GLfloat>;
  std::vector<GLint> *indices = new std::vector<GLint>;

  renderVertex(H, W, vertices);
  int size;
  size = renderIndices(indices, data, H, W);

  // bind VBO VAO EBO
  GLuint VBO, VAO, EBO;

  bindBuffer(VBO, VAO, EBO, vertices, indices);

  // fps count and control
  int frameCount = 0;
  double previousTime = glfwGetTime();
  double lasttime = glfwGetTime();
  // Game loop
  while (!glfwWindowShouldClose(window)) {
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    double currentTime = glfwGetTime();
    frameCount++;
    if (currentTime - previousTime >= 1.0) {
      int width, height;
      glfwGetWindowSize(window, &width, &height);
      // Display the frame count here any way you want.
      glfwSetWindowTitle(window, (std::string("GAME OF LIFE FPS") + std::to_string(frameCount) + std::string(" ") + std::to_string(width) + std::string(" X ") + std::to_string(height))
                                     .c_str());
      frameCount = 0;
      previousTime = currentTime;
    }

    game->update(np, bound);
    data = game->getData();
    size = renderIndices(indices, data, H, W);
    // std::cout << size << std::endl;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * indices->size(), indices->data(), GL_STATIC_DRAW);

    // Render
    // Clear the colorbuffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // size is the number of vertex used
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap the screen buffers
    glfwSwapBuffers(window);
    while (glfwGetTime() < lasttime + 1.0 / TARGET_FPS) {
      // TODO: Put the thread to sleep, yield, or simply do nothing
    }
    lasttime += 1.0 / TARGET_FPS;
  }
  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  delete vertices, indices, game;
  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();

  return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
