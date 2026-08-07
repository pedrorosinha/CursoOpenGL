#include <iostream>
#include <glad/glad.h>      // Carrega as funções modernas do OpenGL
#include <GLFW/glfw3.h>     // Cria janelas e recebe entrada do teclado/mouse

// ---------------- SHADERS ----------------
// Vertex Shader: executado para cada vértice.
// Define a posição de cada ponto do objeto.
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}";

// Fragment Shader: executado para cada pixel.
// Define a cor final do objeto.
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}";

int main()
{
    // Inicializa a biblioteca GLFW
    glfwInit();

    // Solicita um contexto OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Vértices do triângulo (x, y, z)
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
    };

    // Cria a janela e o contexto OpenGL
    GLFWwindow* window =
        glfwCreateWindow(800, 800, "Janela OpenGL", nullptr, nullptr);

    if (!window)
    {
        std::cout << "Erro ao criar janela\n";
        glfwTerminate();
        return -1;
    }

    // Torna esta janela o contexto OpenGL ativo
    glfwMakeContextCurrent(window);

    // GLAD obtém os ponteiros das funções do driver da placa de vídeo
    gladLoadGL();

    // Define qual região da janela será usada para desenhar
    glViewport(0, 0, 800, 800);

    // ----------- COMPILAÇÃO DOS SHADERS -----------

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Programa que reúne todos os shaders utilizados
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // Após criar o programa, os shaders individuais não são mais necessários
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ----------- BUFFERS -----------

    GLuint VAO, VBO;

    // Cria os identificadores
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // OpenGL é uma máquina de estados.
    // Ao fazer Bind, tudo que vier depois atua nesse objeto.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Envia os vértices da RAM para a memória da GPU
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    // Explica ao OpenGL como interpretar cada vértice
    // Local 0 -> posição
    // 3 floats -> x y z
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0);

    // Ativa o atributo de posição
    glEnableVertexAttribArray(0);

    // Desassocia os objetos (boa prática)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ----------- LOOP PRINCIPAL -----------

    while (!glfwWindowShouldClose(window))
    {
        // Limpa a tela com a cor definida
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Seleciona o programa de shaders
        glUseProgram(shaderProgram);

        // Seleciona o VAO contendo os vértices
        glBindVertexArray(VAO);

        // Desenha 3 vértices formando um triângulo
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Exibe a imagem na janela
        glfwSwapBuffers(window);

        // Processa teclado, mouse e eventos da janela
        glfwPollEvents();
    }

    // ----------- LIMPEZA -----------

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}