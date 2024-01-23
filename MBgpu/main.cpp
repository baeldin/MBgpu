#include <iostream>
#include <iomanip>
#include <string>

#include "stdafx.h"
#include "load_shaders.h"
#include "key_callbacks.h"

int main()
{
    int xSize = 1000;
    int ySize = xSize;
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }



    // Configure OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(xSize, ySize, "Mandelbrot Set", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLuint shaderProgram;
    glfwSetWindowUserPointer(window, &shaderProgram); // Store the shader program pointer in the window
    glfwSetKeyCallback(window, key_callback_reload);
    glfwMakeContextCurrent(window);



    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    loadShaders(shaderProgram);

    // Add this before the main loop
    // Vertex data for a fullscreen quad
    float vertices[] = {
        -2.5f, -1.5f,
         1.5f, -1.5f,
         1.5f,  1.5f,
        -2.5f,  1.5f
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    GLint resolutionLocation = glGetUniformLocation(shaderProgram, "resolution");
    glUniform2f(resolutionLocation, float(xSize), float(xSize));


    // Generate VAO and VBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    double lastTime = glfwGetTime();
    int frameCount = 0;


    // Update the main loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        static double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        GLint juliaSeedLocation = glGetUniformLocation(shaderProgram, "juliaSeed");
        
        // Draw your Mandelbrot Set using the shaderProgram
        glUseProgram(shaderProgram);
        glUniform2f(juliaSeedLocation, xpos, ypos);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Calculate FPS
        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - lastTime >= 1.0) // If at least one second has passed
        {
            // Display FPS in the window title
            std::stringstream title;
            title << "Mandelbrot Set - FPS: " << std::fixed << std::setprecision(2) << frameCount;
            glfwSetWindowTitle(window, title.str().c_str());

            // Reset frame count and update lastTime
            frameCount = 0;
            lastTime = currentTime;
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}