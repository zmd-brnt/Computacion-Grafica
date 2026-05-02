// Práctica 12                Pérez Paitán Brent Armando
// 03/05/2026                 320099649

#include <iostream>
#include <cmath>
#include <fstream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void guardarKeyframes();
void cargarKeyframes();
void borrarAnimacion(); // <- NUEVA FUNCION

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,2.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f,  0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegLeft = 0.0f;
float FLegRight = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;

//KeyFrames
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;
float rotBodyPitch = 0.0f;
float rotBodyRoll = 0.0f;

#define MAX_FRAMES 30
int i_max_steps = 800;
int i_curr_steps = 0;

typedef struct _frame {
    float rotDog;
    float rotDogInc;
    float dogPosX;
    float dogPosY;
    float dogPosZ;
    float incX;
    float incY;
    float incZ;
    float head;
    float headInc;
    float tail;
    float tailInc;
    float FLegLeft;
    float FLegLeftInc;
    float FLegRight;
    float FLegRightInc;
    float RLegs;
    float RLegsInc;
    float rotBodyPitch;
    float rotBodyPitchInc;
    float rotBodyRoll;
    float rotBodyRollInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

// Guardado de la animación en archivo
void guardarKeyframes() {
    std::ofstream archivo("animacion.txt");
    if (archivo.is_open()) {
        archivo << FrameIndex << "\n";
        for (int i = 0; i < FrameIndex; i++) {
            archivo << KeyFrame[i].dogPosX << " "
                << KeyFrame[i].dogPosY << " "
                << KeyFrame[i].dogPosZ << " "
                << KeyFrame[i].rotDog << " "
                << KeyFrame[i].head << " "
                << KeyFrame[i].tail << " "
                << KeyFrame[i].FLegLeft << " "
                << KeyFrame[i].FLegRight << " "
                << KeyFrame[i].RLegs << " "
                << KeyFrame[i].rotBodyPitch << " "
                << KeyFrame[i].rotBodyRoll << "\n";
        }
        archivo.close();
        printf("Keyframes guardados en archivo 'animacion.txt'.\n");
    }
    else {
        printf("Error al guardar el archivo.\n");
    }
}

// Carga de la animación desde archivo
void cargarKeyframes() {
    std::ifstream archivo("animacion.txt");
    if (archivo.is_open()) {
        archivo >> FrameIndex;
        if (FrameIndex > MAX_FRAMES) FrameIndex = MAX_FRAMES;

        for (int i = 0; i < FrameIndex; i++) {
            archivo >> KeyFrame[i].dogPosX
                >> KeyFrame[i].dogPosY
                >> KeyFrame[i].dogPosZ
                >> KeyFrame[i].rotDog
                >> KeyFrame[i].head
                >> KeyFrame[i].tail
                >> KeyFrame[i].FLegLeft
                >> KeyFrame[i].FLegRight
                >> KeyFrame[i].RLegs
                >> KeyFrame[i].rotBodyPitch
                >> KeyFrame[i].rotBodyRoll;
        }
        archivo.close();
        printf("Keyframes cargados con exito. Total frames: %d\n", FrameIndex);
    }
    else {
        printf("No se encontro archivo de animacion previo. Empieza a crear tu animacion guardando con 'K'.\n");
    }
}

// Para reiniciar todo presionando 'R'
void borrarAnimacion() {
    FrameIndex = 0;

    // Regresar al perrito a su posición inicial
    dogPosX = 0.0f; dogPosY = 0.0f; dogPosZ = 0.0f;
    rotDog = 0.0f; rotBodyPitch = 0.0f; rotBodyRoll = 0.0f;
    head = 0.0f; tail = 0.0f;
    FLegLeft = 0.0f; FLegRight = 0.0f; RLegs = 0.0f;

    // Guardar (al estar FrameIndex en 0, limpia el archivo de texto)
    guardarKeyframes();

    // Detener animación si se estaba reproduciendo
    play = false;

    printf("\n=== ANIMACION REINICIADA ===\nEl perrito y los frames han vuelto a cero.\n\n");
}

void saveFrame(void) {
    if (FrameIndex >= MAX_FRAMES) {
        printf("Capacidad maxima de frames alcanzada (%d). Presiona 'R' para reiniciar.\n", MAX_FRAMES);
        return;
    }

    printf("Guardando frame index %d...\n", FrameIndex);

    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;
    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].tail = tail;
    KeyFrame[FrameIndex].FLegLeft = FLegLeft;
    KeyFrame[FrameIndex].FLegRight = FLegRight;
    KeyFrame[FrameIndex].RLegs = RLegs;
    KeyFrame[FrameIndex].rotBodyPitch = rotBodyPitch;
    KeyFrame[FrameIndex].rotBodyRoll = rotBodyRoll;

    FrameIndex++;

    // Al guardar un frame en memoria, también lo guardamos en el archivo
    guardarKeyframes();
}

void resetElements(void) {
    if (FrameIndex == 0) return; // Prevención por si no hay frames

    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;
    FLegLeft = KeyFrame[0].FLegLeft;
    FLegRight = KeyFrame[0].FLegRight;
    RLegs = KeyFrame[0].RLegs;
    rotDog = KeyFrame[0].rotDog;
    rotBodyPitch = KeyFrame[0].rotBodyPitch;
    rotBodyRoll = KeyFrame[0].rotBodyRoll;
}

void interpolation(void) {
    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
    KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
    KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
    KeyFrame[playIndex].FLegLeftInc = (KeyFrame[playIndex + 1].FLegLeft - KeyFrame[playIndex].FLegLeft) / i_max_steps;
    KeyFrame[playIndex].FLegRightInc = (KeyFrame[playIndex + 1].FLegRight - KeyFrame[playIndex].FLegRight) / i_max_steps;
    KeyFrame[playIndex].RLegsInc = (KeyFrame[playIndex + 1].RLegs - KeyFrame[playIndex].RLegs) / i_max_steps;
    KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
    KeyFrame[playIndex].rotBodyPitchInc = (KeyFrame[playIndex + 1].rotBodyPitch - KeyFrame[playIndex].rotBodyPitch) / i_max_steps;
    KeyFrame[playIndex].rotBodyRollInc = (KeyFrame[playIndex + 1].rotBodyRoll - KeyFrame[playIndex].rotBodyRoll) / i_max_steps;
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "P12 Perez Paitan Brent Armando", nullptr, nullptr);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    //models
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Inicializamos todo el arreglo de frames a cero
    for (int i = 0; i < MAX_FRAMES; i++) {
        KeyFrame[i].dogPosX = 0; KeyFrame[i].dogPosY = 0; KeyFrame[i].dogPosZ = 0;
        KeyFrame[i].incX = 0; KeyFrame[i].incY = 0; KeyFrame[i].incZ = 0;
        KeyFrame[i].rotDog = 0; KeyFrame[i].rotDogInc = 0;
        KeyFrame[i].head = 0; KeyFrame[i].headInc = 0;
        KeyFrame[i].tail = 0; KeyFrame[i].tailInc = 0;
        KeyFrame[i].FLegLeft = 0; KeyFrame[i].FLegLeftInc = 0;
        KeyFrame[i].FLegRight = 0; KeyFrame[i].FLegRightInc = 0;
        KeyFrame[i].RLegs = 0; KeyFrame[i].RLegsInc = 0;
        KeyFrame[i].rotBodyPitch = 0; KeyFrame[i].rotBodyPitchInc = 0;
        KeyFrame[i].rotBodyRoll = 0; KeyFrame[i].rotBodyRollInc = 0;
    }

    // Cargamos la animación manual guardada anteriormente (si existe)
    cargarKeyframes();

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        glm::mat4 modelTemp = glm::mat4(1.0f);

        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);

        //Piso
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // ============================================================
        // JERARQUIA DEL PERRO ACTUALIZADA
        // ============================================================

        // Base: Posición global del perro en el mundo y giros principales
        glm::mat4 modelDogBase = glm::translate(glm::mat4(1.0f), glm::vec3(dogPosX, dogPosY, dogPosZ));
        modelDogBase = glm::rotate(modelDogBase, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        modelDogBase = glm::rotate(modelDogBase, glm::radians(rotBodyRoll), glm::vec3(0.0f, 0.0f, 1.0f));

        // Body: Pivote en la cadera para sentarse (Pitch)
        modelTemp = modelDogBase;
        modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.0f, -0.288f));
        modelTemp = glm::rotate(modelTemp, glm::radians(rotBodyPitch), glm::vec3(1.0f, 0.0f, 0.0f));
        modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.0f, 0.288f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTemp));
        DogBody.Draw(lightingShader);

        glm::mat4 modelBodyPitched = modelTemp;

        // Head
        model = modelBodyPitched;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Tail 
        model = modelBodyPitched;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Front Left Leg
        model = modelBodyPitched;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegLeft), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Front Right Leg
        model = modelBodyPitched;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegRight), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Back Left Leg (Se queda en la base para no flotar al sentarse)
        model = modelDogBase;
        model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Back Right Leg
        model = modelDogBase;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Ball transparency
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // Lamp object
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    // Dog Controls Manuales Actualizados
    if (keys[GLFW_KEY_4]) head -= 0.05f;
    if (keys[GLFW_KEY_5]) head += 0.05f;

    if (keys[GLFW_KEY_6]) FLegLeft -= 0.05f;
    if (keys[GLFW_KEY_7]) FLegLeft += 0.05f;
    if (keys[GLFW_KEY_Z]) FLegRight -= 0.05f; // Nueva tecla pata delantera derecha
    if (keys[GLFW_KEY_X]) FLegRight += 0.05f; // Nueva tecla pata delantera derecha

    if (keys[GLFW_KEY_8]) RLegs -= 0.05f;
    if (keys[GLFW_KEY_9]) RLegs += 0.05f;

    if (keys[GLFW_KEY_0]) tail -= 0.05f;
    if (keys[GLFW_KEY_P]) tail += 0.05f;

    if (keys[GLFW_KEY_2]) rotBodyPitch += 0.1f; // Pitch (Inclinación sentarse)
    if (keys[GLFW_KEY_3]) rotBodyPitch -= 0.1f;

    if (keys[GLFW_KEY_C]) rotBodyRoll += 0.1f;  // Roll (Muertito)
    if (keys[GLFW_KEY_V]) rotBodyRoll -= 0.1f;

    if (keys[GLFW_KEY_COMMA]) rotDog += 0.1f;   // Yaw (Girar lados)
    if (keys[GLFW_KEY_PERIOD]) rotDog -= 0.1f;

    if (keys[GLFW_KEY_H]) dogPosZ += 0.0025f;
    if (keys[GLFW_KEY_Y]) dogPosZ -= 0.0025f;
    if (keys[GLFW_KEY_G]) dogPosX -= 0.0025f;
    if (keys[GLFW_KEY_J]) dogPosX += 0.0025f;
    if (keys[GLFW_KEY_M]) dogPosY -= 0.00025f;
    if (keys[GLFW_KEY_N]) dogPosY += 0.00025f;

    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // Light Controls
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.0005f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.0005f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.0005f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.0005f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.0005f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.0005f;
}

// CORREGIDO: Las teclas clave ahora solo registran 1 click por pulsación para no llenar los frames por error
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Reproducir Animación (L)
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (play == false && (FrameIndex > 1))
        {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
        }
        else
        {
            play = false;
        }
    }

    // Guardar Frame (K)
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (FrameIndex < MAX_FRAMES)
        {
            saveFrame();
        }
    }

    // Reiniciar Animación (R)
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        borrarAnimacion();
    }

    // Escape para salir
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Registrar teclas mantenidas para DoMovement
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // Luces (Espacio)
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        active = !active;
        if (active) Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
        else Light1 = glm::vec3(0);
    }
}

void Animation() {
    if (play)
    {
        if (i_curr_steps >= i_max_steps)
        {
            playIndex++;
            if (playIndex > FrameIndex - 2)
            {
                printf("termina anim\n");
                playIndex = 0;
                play = false;
            }
            else
            {
                i_curr_steps = 0;
                interpolation();
            }
        }
        else
        {
            dogPosX += KeyFrame[playIndex].incX;
            dogPosY += KeyFrame[playIndex].incY;
            dogPosZ += KeyFrame[playIndex].incZ;
            head += KeyFrame[playIndex].headInc;
            tail += KeyFrame[playIndex].tailInc;
            FLegLeft += KeyFrame[playIndex].FLegLeftInc;
            FLegRight += KeyFrame[playIndex].FLegRightInc;
            RLegs += KeyFrame[playIndex].RLegsInc;
            rotDog += KeyFrame[playIndex].rotDogInc;
            rotBodyPitch += KeyFrame[playIndex].rotBodyPitchInc;
            rotBodyRoll += KeyFrame[playIndex].rotBodyRollInc;

            i_curr_steps++;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}