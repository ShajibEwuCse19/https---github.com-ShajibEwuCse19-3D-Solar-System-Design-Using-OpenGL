#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <shader_m.h>
#include <camera.h>
#include <model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
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
    Shader shader("src/10.2.instancing.vs", "src/10.2.instancing.fs");

    // load models
    // -----------
   //Model planet8("resources/objects/planet/planet.obj");

   //Model planet8("resources/objects/moon/Mercurio.obj");
   //Model planet7("resources/objects/Saturn/13906_Saturn_v1_l3.obj");
    Model planet("resources/objects/sun/Neptune.obj");
    Model planet1("resources/objects/mercury/Neptune.obj");
    Model planet2("resources/objects/venus/Neptune.obj");
    Model planet3("resources/objects/earth/Earth.obj");
    Model planet4("resources/objects/mars/planet.obj");
    Model planet5("resources/objects/jupiter/jupiter.obj");
    Model planet6("resources/objects/Saturn/saturn1.obj");
    Model planet7("resources/objects/Uranus/saturn1.obj");
    Model planet8("resources/objects/neptune/Neptune.obj");
    
    Model satelite("resources/objects/satellite/source/SatelliteSubstancePainter.obj");
    Model star("resources/objects/star/mc-stars.obj");
    Model planet9("resources/objects/moon/Neptune.obj");
    Model ship("resources/objects/spaceship/source/Vigil/Vigil.obj");



 
    // generate a large list of semi-random model transformation matrices
    // ------------------------------------------------------------------
    unsigned int amount = 1000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 50.0;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 40) / 2000.0 + 0.0001);

        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
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
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();;
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // draw planet
        // draw planet
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        shader.setMat4("model", model);
        planet.Draw(shader);




        // mercury
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*1), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(25.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setMat4("model", model);
        planet1.Draw(shader);
        //venus
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*0.73), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(30.0f, 0.0f, 13.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setMat4("model", model);
        planet2.Draw(shader);

        //earth
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*0.62), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(35.0f, 0.0f, 27.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setMat4("model", model);
        planet3.Draw(shader);

        //moon
        // model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*2), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setMat4("model", model);
        planet9.Draw(shader);



        //mars
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*0.50), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(40.0f, 0.0f, 40.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        shader.setMat4("model", model);
        planet4.Draw(shader);

        //jupiter
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*0.27), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(50.0f, 0.0f, 70.0f));
        model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
        shader.setMat4("model", model);
        planet5.Draw(shader);


        //saturn
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*0.20), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(64.0f, 0.0f, 120.0f));
        model = glm::scale(model, glm::vec3(4.8f, 4.8f, 4.8f));
        shader.setMat4("model", model);
        planet6.Draw(shader);

        //uranus
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*0.14), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(75.0f, 0.0f, 175.0f));
        model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
        shader.setMat4("model", model);
        planet7.Draw(shader);

        //neptune
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(glfwGetTime()*0.11), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(84.0f, 0.0f, 215.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setMat4("model", model);
        planet9.Draw(shader);

        // draw meteorites
        for (unsigned int i = 0; i < amount; i++)
        {
            shader.setMat4("model", modelMatrices[i]);
            star.Draw(shader);
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
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}