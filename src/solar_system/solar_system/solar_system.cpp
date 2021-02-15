#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const int NUM = 9;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 500.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("vs_shader.vs", "fs_shader.fs");

    // load models
    // -----------

  
    Model** solarSystem = new Model*[NUM];
    solarSystem[0] = new Model(FileSystem::getPath("resources/objects/planets/sun/sun.obj"));
    solarSystem[1] = new Model(FileSystem::getPath("resources/objects/planets/mercury/mercury.obj"));
    solarSystem[2] = new Model(FileSystem::getPath("resources/objects/planets/venus/venus.obj"));
    solarSystem[3] = new Model(FileSystem::getPath("resources/objects/planets/earth/earth.obj"));
    solarSystem[4] = new Model(FileSystem::getPath("resources/objects/planets/mars/mars.obj"));
    solarSystem[5] = new Model(FileSystem::getPath("resources/objects/planets/jupiter/jupiter.obj"));
    solarSystem[6] = new Model(FileSystem::getPath("resources/objects/planets/saturn/saturn.obj"));
    solarSystem[7] = new Model(FileSystem::getPath("resources/objects/planets/uranus/uranus.obj"));
    solarSystem[8] = new Model(FileSystem::getPath("resources/objects/planets/neptune/neptune.obj"));
   
    float distanceFromSun[NUM] = { 0.0f, 80.0f, 100.0f, 150.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f };
    float rotationSpeed[NUM] = { 2.0f, 29.43f, -21.76f, 18.5f, 14.91f, 8.51f, 6.0f, 4.225f, 3.374f };
    float revolutionSpeed[NUM] = { 0.0f, 47.0f, 35.0f, 29.0f, 24.0f, 13.0f, 9.69f, 6.81f, 5.43f };
    float scaleFactor[NUM] = {5.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 1.5f, 1.5f};
    glm::vec3 revolutionAxis =  glm::vec3(0.0f, 1.0f, 0.0f);
    

    glm::vec3* rotationAxis = new glm::vec3[NUM];
    for (int i = 0; i < NUM; i++) {
        rotationAxis[i] = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    rotationAxis[7] = glm::vec3(1.0f, 0.0f, 0.0f);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

       

        for (unsigned int i = 0; i < NUM; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, currentFrame * revolutionSpeed[i] * glm::radians(1.0f), revolutionAxis);
            model = glm::translate(model, glm::vec3(distanceFromSun[i], 0.0f, 0.0f));
            model = glm::rotate(model, currentFrame * rotationSpeed[i] * glm::radians(10.0f), rotationAxis[i]);
            model = glm::scale(model, glm::vec3(scaleFactor[i], scaleFactor[i], scaleFactor[i]));
            shader.setMat4("model", model);
            solarSystem[i]->Draw(shader);
        
        }

 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 20.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 20.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 20.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 20.0f);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
