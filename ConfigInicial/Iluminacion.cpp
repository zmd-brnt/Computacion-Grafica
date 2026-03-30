// Práctica 8                     Pérez Paitán Brent Armando
// 29/03/2026                                      320099649

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Cámara
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool keys[1024];
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Tiempo
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Día/Noche
bool isDay = true;

// Sol/Luna
glm::vec3 sunPos;
glm::vec3 moonPos;
float radius = 10.0f;
float speed = 0.5f;

// Prototipos
void KeyCallback(GLFWwindow*, int, int, int, int);
void MouseCallback(GLFWwindow*, double, double);
void DoMovement();

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "P8 Perez Paitan Brent Armando", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // MODELOS
    Model dog((char*)"Models/RedDog.obj");
    Model snowman((char*)"Models/P6/objSnow.obj");
    Model tree((char*)"Models/P6/Pino.obj");
    Model toy((char*)"Models/P6/Toy.obj");
    Model snow((char*)"Models/P6/snow_1_2.obj");
    Model house((char*)"Models/P6/House.obj");

    Model sunModel((char*)"Models/sun.obj");
    Model moonModel((char*)"Models/moon.obj");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // Fondo
        if (isDay)
            glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        else
            glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Movimiento sol/luna
        float angle = currentFrame * speed;

        sunPos = glm::vec3(radius * cos(angle), radius * sin(angle), 0.0f);
        moonPos = -sunPos;

        glm::vec3 activeLight = isDay ? sunPos : moonPos;

        lightingShader.Use();

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"),
            activeLight.x, activeLight.y, activeLight.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z);

        // Luz día/noche
        if (isDay)
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.4f, 0.35f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.9f, 0.6f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 0.9f, 0.7f);
        }
        else
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.05f, 0.05f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.2f, 0.3f, 0.6f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.3f, 0.4f, 0.7f);
        }

        glm::mat4 view = camera.GetViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        glm::mat4 model;

        // ESCENA
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 0));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        snowman.Draw(lightingShader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(-3, 0, 0));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(lightingShader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        house.Draw(lightingShader);

        // ===== SOL Y LUNA =====
        lampShader.Use();

        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // SOL (AMARILLO)
        glUniform3f(glGetUniformLocation(lampShader.Program, "objectColor"),
            1.0f, 1.0f, 0.0f);

        glm::mat4 modelSun = glm::translate(glm::mat4(1.0f), sunPos);
        modelSun = glm::scale(modelSun, glm::vec3(0.5f));

        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(modelSun));

        sunModel.Draw(lampShader);

        // LUNA (AZUL)
        glUniform3f(glGetUniformLocation(lampShader.Program, "objectColor"),
            0.2f, 0.4f, 1.0f);

        glm::mat4 modelMoon = glm::translate(glm::mat4(1.0f), moonPos);
        modelMoon = glm::scale(modelMoon, glm::vec3(0.4f));

        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(modelMoon));

        moonModel.Draw(lampShader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Movimiento
void DoMovement()
{
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        isDay = !isDay;
}

// Mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}