#include <iostream>
#include <glad/glad.h>      // Carrega as funções do OpenGL
#include <GLFW/glfw3.h>     // Cria a janela e gerencia eventos

#include "shaderClass.h"    // Cria e gerencia o Shader Program
#include "VAO.h"            // Cria e gerencia o Vertex Array Object
#include "VBO.h"            // Cria e gerencia o Vertex Buffer Object
#include "EBO.h"            // Cria e gerencia o Element Buffer Object


int main() {

	// Inicializa a GLFW
	glfwInit();


	// ========================================================
	// CONFIGURAÇÃO DO CONTEXTO OPENGL
	//
	// Solicita uma janela usando OpenGL 3.3 Core Profile.
	// ========================================================
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// ========================================================
	// CRIAÇÃO DA JANELA
	//
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
	//
	// Carrega as funções do OpenGL que serão utilizadas
	// através do driver da placa de vídeo.
	// ========================================================
	gladLoadGL();


	// Define a área da janela onde o OpenGL irá desenhar.
	glViewport(0, 0, 800, 800);


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
		-0.5f, -0.5f * float(sqrt(3)) / 3,      0.0f, 0.8f, 0.3f, 0.02f,        // Lower left corner
		 0.5f, -0.5f * float(sqrt(3)) / 3,      0.0f, 0.8f, 0.3f, 0.02f,        // Lower right corner
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3,  0.0f, 1.0f, 0.6f, 0.32f,        // Upper corner
		-0.25f, 0.5f * float(sqrt(3)) / 6,      0.0f, 0.9f, 0.45f, 0.17f,       // Inner left
		 0.25f, 0.5f * float(sqrt(3)) / 6,      0.0f, 0.9f, 0.45f, 0.17f,       // Inner right
		 0.0f, -0.5f * float(sqrt(3)) / 3,      0.0f, 0.8f, 0.3f, 0.02f         // Inner down
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
	// SHADER
	//
	// Cria o Shader Program a partir dos arquivos:
	//
	// default.vert -> Vertex Shader
	// default.frag -> Fragment Shader
	//
	// A classe Shader cuida de carregar, compilar e
	// conectar os dois shaders.
	// ========================================================
	Shader shaderProgram("default.vert", "default.frag");


	// ========================================================
	// VAO
	//
	// Cria o Vertex Array Object.
	//
	// O VAO guarda a configuração dos dados dos vértices
	// que serão utilizados durante o desenho.
	// ========================================================
	VAO VAO1;

	// Ativa o VAO.
	VAO1.Bind();


	// ========================================================
	// VBO
	//
	// Cria o Vertex Buffer Object e envia os vértices
	// para a memória da GPU.
	// ========================================================
	VBO VBO1(vertices, sizeof(vertices));


	// ========================================================
	// EBO
	//
	// Cria o Element Buffer Object e envia os índices
	// para a memória da GPU.
	// ========================================================
	EBO EBO1(indices, sizeof(indices));


	// ========================================================
	// CONFIGURAÇÃO DO VAO
	//
	// Informa ao VAO como os dados armazenados no VBO
	// devem ser interpretados.
	//
	// O índice 0 corresponde ao atributo "aPos" do Vertex Shader.
	// O índice 1 corresponde ao atributo "aColor" do Vertex Shader.
	// ========================================================
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Desativa os objetos após terminar a configuração.
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


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


		// Ativa o Shader Program.
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);


		// Ativa o VAO com a configuração dos vértices.
		VAO1.Bind();


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
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();


	// Fecha a janela e encerra a GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}