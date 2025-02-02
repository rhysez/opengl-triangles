#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* initWindow();

// Representation of GLSL language.
// We use GLSL to write shader source code.
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

// This array of vertices makes up our triangle shape.
// These values are all based between -1.0 and 1.0. 

// Two triangles parallel.
float vertices[] = {
     -0.5f,  0.5f, 0.0f,  // top
    -1.0f, -0.5f, 0.0f,  // bottom left
     0.0f, -0.5f, 0.0f,  // bottom right 

     0.5f, 0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
     1.0f, -0.5f, 0.0f,
};

// These indices represent the order in which we want to draw our vertices.
// This is called 'indexed drawing'.
// We create an EBO (element buffer object) to store our indices. 
// Then, when we draw the object, we interpret our VBO in the order our EBO specifies.
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2,
    3, 4, 5
};

int main()
{
    // --- Window creation ---
    GLFWwindow* window = initWindow();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // --- Build and compile the shader program ---
    unsigned int VBO, VAO, EBO, vertexShader, fragmentShader, shaderProgram;

    // Vertex Array Object set up
    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO); 

    // Vertex Buffer Object set up
    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Buffer Object set up
    glGenBuffers(1, &EBO); // Generates Elements buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Binds our buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glCreateShader takes our shader type as an argument.
    // We use GL_VERTEX_SHADER because we are creating a vertex shader.
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // --- Fragment Shader ---
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // --- Shader Program ---
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Delete previous shader instancess.
    // They're no longer needed, as all shaders are now accessed via shaderProgram.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window)) // Render loop.
    {
        // Input.
        processInput(window);
        
        // Rendering.
        glClearColor(0.0f, 0.0f, 0.7f, 1.0f); // Specifies what colour glClear should render (state setter).
        glClear(GL_COLOR_BUFFER_BIT); // Renders the colour set in glClearColor (state user). 

        // For each interation,
        // Re-bind shader program and vertex array.
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draws vertices from VBO in order that EBO specifies (indexed drawing).
        glBindVertexArray(0);

        // glfwSwapBuffers handles drawing pixels to the screen from a memory buffer containing colour values.
        // In graphics, drawing in a 'single buffer' requires drawing each pixel sequentially until a full image is create.
        // Single buffers can result in artifacting/flickering for this reason.
        // glfwSwapBuffers uses 'double buffering' which draws pixels behind the existing image,
        // and once the buffer has been filled, the front and back buffers are swapped. 
        // This prevents artifacting and flickering.
        glfwSwapBuffers(window);
        glfwPollEvents(); // Checks if any events have occured, e.g. key presses.
    }

    // Free memory allocations and close the window. 
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int height, int width)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    // Checks pressed key against the GLFW_KEY_ESCAPE constant value.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* initWindow() {
    glfwInit(); // Initialises GLFW so that we can use the lib's functions.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create window" << '\n';
        glfwTerminate();
        exit(0);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(0);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, 800, 600);

    return window;
}