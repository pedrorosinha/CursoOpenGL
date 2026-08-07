#include <iostream>
#include <glad/glad.h>      // Carrega as funções do OpenGL
#include <GLFW/glfw3.h>     // Cria a janela e gerencia eventos

// ============================================================
// VERTEX SHADER
// Executado para cada vértice.
// Recebe a posição do vértice e define sua posição na tela.
// ============================================================
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// ============================================================
// FRAGMENT SHADER
// Executado para cada fragmento/pixel do objeto.
// Define a cor final que será desenhada.
// ============================================================
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {

	// Inicializa a GLFW
	glfwInit();

	// ========================================================
	// CONFIGURAÇÃO DO CONTEXTO OPENGL
	// Solicita uma janela usando OpenGL 3.3 Core Profile.
	// ========================================================
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// ========================================================
	// CRIAÇÃO DA JANELA
	// Cria uma janela de 800x800 pixels.
	// ========================================================
	GLFWwindow* window =
		glfwCreateWindow(800, 800, "Janela OpenGL", nullptr, nullptr);

	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return -1;
	}


	// Define essa janela como o contexto OpenGL atual.
	glfwMakeContextCurrent(window);


	// ========================================================
	// GLAD
	// Carrega as funções do OpenGL que serão utilizadas
	// através do driver da placa de vídeo.
	// ========================================================
	gladLoadGL();


	// Define a área da janela onde o OpenGL irá desenhar.
	glViewport(0, 0, 800, 800);


	// ========================================================
	// VERTEX SHADER
	// Cria, fornece o código e compila o Vertex Shader.
	// ========================================================
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(
		vertexShader,
		1,
		&vertexShaderSource,
		nullptr
	);

	glCompileShader(vertexShader);


	// ========================================================
	// FRAGMENT SHADER
	// Cria, fornece o código e compila o Fragment Shader.
	// ========================================================
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(
		fragmentShader,
		1,
		&fragmentShaderSource,
		nullptr
	);

	glCompileShader(fragmentShader);


	// ========================================================
	// SHADER PROGRAM
	// Cria um programa que irá reunir os shaders.
	// ========================================================
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Liga os shaders formando um programa que pode ser usado pela GPU.
	glLinkProgram(shaderProgram);


	// Depois que foram ligados ao programa,
	// os shaders individuais não são mais necessários.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// ========================================================
	// VÉRTICES
	//
	// Cada vértice possui 3 valores:
	// X, Y e Z.
	//
	// Os 3 primeiros vértices formam o triângulo externo.
	// Os outros 3 representam os vértices internos.
	// ========================================================
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		 0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};


	// ========================================================
	// ÍNDICES
	//
	// Define a ordem em que os vértices serão utilizados
	// para formar os triângulos.
	//
	// Cada grupo de 3 índices representa um triângulo.
	// ========================================================
	GLuint indices[] =
	{
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Upper triangle
		5, 4, 1  // Lower right triangle
	};


	// ========================================================
	// VAO, VBO E EBO
	//
	// VAO = guarda a configuração de como os vértices
	//       devem ser interpretados.
	//
	// VBO = armazena os dados dos vértices.
	//
	// EBO = armazena os índices usados para acessar os vértices.
	// ========================================================
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	// Ativa o VAO.
	// As configurações feitas enquanto ele está ativo
	// ficam associadas a ele.
	glBindVertexArray(VAO);


	// Ativa o VBO como buffer de vértices.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	// ========================================================
	// ENVIA OS VÉRTICES PARA A GPU
	//
	// Copia os dados de "vertices" da memória da CPU
	// para o VBO.
	// ========================================================
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW
	);


	// Ativa o EBO como buffer de índices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Copia os índices da CPU para o EBO.
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices),
		indices,
		GL_STATIC_DRAW
	);


	// ========================================================
	// CONFIGURAÇÃO DOS ATRIBUTOS
	//
	// Explica ao OpenGL como interpretar os dados armazenados
	// no VBO.
	//
	// location 0 -> posição do vértice
	// 3          -> X, Y e Z
	// GL_FLOAT   -> cada valor é float
	// stride     -> cada vértice possui 3 floats
	// ========================================================
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0
	);


	// Ativa o atributo de posição do vértice.
	glEnableVertexAttribArray(0);


	// Desassocia o VBO e o VAO.
	//
	// O EBO NÃO é desassociado aqui porque sua associação
	// faz parte do estado do VAO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// ========================================================
	// CONFIGURAÇÃO INICIAL DA TELA
	// Define a cor que será usada para limpar a tela.
	// ========================================================
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	// Mostra o conteúdo desenhado na janela.
	glfwSwapBuffers(window);


	// ========================================================
	// LOOP PRINCIPAL
	//
	// Enquanto a janela estiver aberta:
	//
	// 1. Limpa a tela
	// 2. Ativa o Shader Program
	// 3. Ativa o VAO
	// 4. Desenha usando os índices do EBO
	// 5. Mostra o resultado
	// 6. Processa eventos
	// ========================================================
	while (!glfwWindowShouldClose(window)) {

		// Limpa a tela
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Diz à GPU qual programa de shaders deve ser usado.
		glUseProgram(shaderProgram);


		// Ativa o VAO com as configurações dos vértices.
		// O VAO também possui a associação com o EBO.
		glBindVertexArray(VAO);


		// ====================================================
		// DESENHO COM ÍNDICES
		//
		// GL_TRIANGLES -> grupos de 3 índices formam triângulos.
		// 9            -> utiliza 9 índices.
		// GL_UNSIGNED_INT -> cada índice é um unsigned int.
		// 0            -> começa no primeiro índice do EBO.
		//
		// 9 índices = 3 triângulos.
		// ====================================================
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);


		// Troca o buffer traseiro pelo dianteiro.
		// Isso faz o desenho aparecer na janela.
		glfwSwapBuffers(window);


		// Processa teclado, mouse e eventos da janela.
		glfwPollEvents();
	}


	// ========================================================
	// LIMPEZA
	//
	// Libera os recursos que foram criados na GPU.
	// ========================================================
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);


	// Fecha a janela e encerra a GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}