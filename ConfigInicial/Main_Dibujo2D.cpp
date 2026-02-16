//	Practica 2							Pérez Paitán Brent Armando
//	15 Febrero 2026										 320099649

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 1000, HEIGHT = 800;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "P2 Perez Paitan Brent Armando", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		-0.7f,   0.9f, 0.0f,    0.6f,0.8f,1.0f,  //  1
		-0.5f,   0.7f, 0.0f,    0.6f,0.8f,1.0f,  //  2
		-0.3f,   0.5f, 0.0f,    0.6f,0.8f,1.0f,  //  3
		 0.0f,   0.6f, 0.0f,    0.6f,1.0f,0.8f,  //  4*
		 0.3f,   0.5f, 0.0f,    0.8f,0.7f,1.0f,  //  5
		 0.5f,   0.7f, 0.0f,    0.8f,0.7f,1.0f,  //  6
		 0.7f,   0.9f, 0.0f,    0.8f,0.7f,1.0f,  //  7
		 0.7f,   0.6f, 0.0f,    0.8f,0.7f,1.0f,  //  8
		 0.7f,   0.4f, 0.0f,    0.8f,0.7f,1.0f,  //  9
		 0.6f,   0.2f, 0.0f,    0.8f,0.7f,1.0f,  // 10
		 
		 0.8f,   0.0f, 0.0f,    0.6f,1.0f,0.8f,  // 11
		 0.7f,  -0.4f, 0.0f,    1.0f,0.7f,0.6f,  // 12
		 0.3f,  -0.6f, 0.0f,    1.0f,0.7f,0.6f,  // 13
		 0.2f,  -0.8f, 0.0f,    1.0f,0.7f,0.6f,  // 14
		 0.0f,  -0.9f, 0.0f,    0.6f,1.0f,0.8f,  // 15*
		-0.2f,  -0.8f, 0.0f,    1.0f,1.0f,0.7f,  // 16
		-0.3f,  -0.6f, 0.0f,    1.0f,1.0f,0.7f,  // 17
		-0.7f,  -0.4f, 0.0f,    1.0f,1.0f,0.7f,  // 18
		-0.8f,   0.0f, 0.0f,    0.6f,1.0f,0.8f,  // 19*
		-0.6f,   0.2f, 0.0f,    0.6f,0.8f,1.0f,  // 20
		
		-0.7f,   0.4f, 0.0f,    0.6f,0.8f,1.0f,  // 21
		-0.7f,   0.6f, 0.0f,    0.6f,0.8f,1.0f,  // 22
		-0.5f,   0.4f, 0.0f,    0.6f,0.8f,1.0f,  // 23
		 0.0f,   0.4f, 0.0f,    0.6f,1.0f,0.8f,  // 24*
		 0.5f,   0.4f, 0.0f,    0.8f,0.7f,1.0f,  // 25
		-0.3f,   0.2f, 0.0f,    0.6f,1.0f,0.8f,  // 26*
		 0.3f,   0.2f, 0.0f,    0.6f,1.0f,0.8f,  // 27*
		 0.5f,   0.0f, 0.0f,    0.6f,1.0f,0.8f,  // 28
		 0.4f,   0.0f, 0.0f,    0.6f,1.0f,0.8f,  // 29
		-0.4f,   0.0f, 0.0f,    0.6f,1.0f,0.8f,  // 30*

		-0.5f,   0.0f, 0.0f,    0.6f,1.0f,0.8f,  // 31*
		-0.2f,  -0.1f, 0.0f,    1.0f,1.0f,0.7f,  // 32
		 0.2f,  -0.1f, 0.0f,    1.0f,0.7f,0.6f,  // 33
		 0.2f,  -0.2f, 0.0f,    1.0f,0.7f,0.6f,  // 34
		 0.0f,  -0.2f, 0.0f,    0.6f,1.0f,0.8f,  // 35*
		-0.2f,  -0.2f, 0.0f,    1.0f,1.0f,0.7f,  // 36
		-0.3f,  -0.3f, 0.0f,    1.0f,1.0f,0.7f,  // 37
		 0.3f,  -0.3f, 0.0f,    1.0f,0.7f,0.6f,  // 38
		 0.0f,  -0.5f, 0.0f,    0.6f,1.0f,0.8f,  // 39*
		-0.1f,  -0.6f, 0.0f,    1.0f,1.0f,0.7f,  // 40

		 0.1f,  -0.6f, 0.0f,    1.0f,0.7f,0.6f,  // 41
		 0.1f,  -0.7f, 0.0f,    1.0f,0.7f,0.6f,  // 42
		-0.1f,  -0.7f, 0.0f,    1.0f,1.0f,0.7f,  // 43
		 0.0f,  -0.8f, 0.0f,    0.6f,1.0f,0.8f,  // 44
	};
	unsigned int indices[] = {  // note that we start from 0!
		21,1, 21,22, 1,22, 22,2, 19,22, 2,23, 22,25, 25,23, 25,29, 25,30, 30,29, 19,30, 18,30,
		30,17, 30,36, 17,36, 36,29, 36,35, 35,29, 35,31, 31,29, 31,25, 31,23, 31,34, 35,34, 36,34, 36,38, 36,39, 39,38, 36,16, 15,42, 42,39, 42,43,
		23,4, 23,26, 4,24, 5,24, 5,7, 7,24, 24,9, 26,24, 28,26, 28,27, 27,9, 27,10, 27,26,
		27,11, 37,27, 37,11, 37,12, 41,13, 43,41, 41,40, 40,38, 40,37, 38,37, 37,34, 37,33, 37,28, 33,28, 33,32, 34,33, 34,32, 32,28, 32,26, 32,23,
		3,14
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


        /*glPointSize(5);
        glDrawArrays(GL_POINTS,0,43);*/
        
        //glDrawArrays(GL_LINES,0,21);
        /*glDrawArrays(GL_LINE_LOOP,0,43);*/
        
        //glDrawArrays(GL_TRIANGLES,0,3);
        //glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);

		//	Contorno de la figura
		glDrawArrays(GL_LINE_LOOP, 0, 21);

		//	Cuadrante Superior izquierdo
		glDrawElements(GL_LINES, 26, GL_UNSIGNED_INT, (void*)0);

		// Cuadrante Inferior izquierdo
		glDrawElements(GL_LINES, 40, GL_UNSIGNED_INT, (void*)(26 * sizeof(unsigned int)));

		//	Cuadrante Superior derecho
		glDrawElements(GL_LINES, 26, GL_UNSIGNED_INT, (void*)(66 * sizeof(unsigned int)));

		//	Cuadrante Inferior izquierdo
		glDrawElements(GL_LINES, 40, GL_UNSIGNED_INT, (void*)(92 * sizeof(unsigned int)));
        
        // Linea vertical 
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(132 * sizeof(unsigned int)));

        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}