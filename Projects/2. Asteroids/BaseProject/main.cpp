#include "main.h"

#include "Game.h"
#include "Ship.h"


// Handles Window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main() {
    // GLFW WINDOW HINTS
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW WINDOW CREATION
    GLFWwindow* window = glfwCreateWindow(Settings::WIDTH, Settings::HEIGHT, "The Real", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD INIT
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // The game
    Game game = Game();

    game.reload();

    GameObject background = GameObject(
        {
            // Positions          // Texture
            -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // BL
             1.0f, -1.0f,  0.0f,  1.0f, 0.0f, // BR
             1.0f,  1.0f,  0.0f,  1.0f, 1.0f, // TR
            -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, // TL
        },
        {
             0,  1,  2,  // 1st triangle
             0,  2,  3,  // 2nd triangle
        },
        Shader("shaders/background.vs", "shaders/background.fs"),
        Image("assets/background.png", GL_RGBA),
        glm::vec3(0.0f, 0.2f, game.camera.Position.z - 1.0f)
    );

    // gaming mode
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // RENDER LOOP
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        background.update(game.camera, game.getDeltaTime());
        background.draw();

        game.update(window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}