#include <iostream>
#include <glad/glad.h>       // Carrega as funções do OpenGL
#include <GLFW/glfw3.h>      // Cria a janela e gerencia eventos
#include <stb/stb_image.h>  // Carrega imagens para serem usadas como texturas
#include <glm/glm.hpp>       // Biblioteca de matemática para gráficos 3D
#include <glm/gtc/matrix_transform.hpp> // Funções de transformação de matrizes
#include <glm/gtc/type_ptr.hpp> // Converte matrizes para ponteiros de float

#include "Texture.h"         // Cria e gerencia Texturas
#include "shaderClass.h"     // Cria e gerencia o Shader Program
#include "VAO.h"             // Cria e gerencia o Vertex Array Object
#include "VBO.h"             // Cria e gerencia o Vertex Buffer Object
#include "EBO.h"             // Cria e gerencia o Element Buffer Object
#include "Camera.h"          // Cria e gerencia a câmera

const unsigned int width = 800;
const unsigned int height = 800;


int main() {

	// Inicializa a GLFW.
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
		glfwCreateWindow(width, height, "Janela OpenGL", nullptr, nullptr);

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
	glViewport(0, 0, width, height);


	// ========================================================
	// VÉRTICES
	//
	// Cada vértice possui 8 valores:
	//
	// X, Y, Z -> posição do vértice
	// R, G, B -> cor do vértice
	// U, V    -> coordenada da textura
	//
	// As coordenadas U e V indicam qual parte da imagem
	// será aplicada em cada vértice.
	// ========================================================
	GLfloat vertices[] =
	{
		-0.5f, 0.0f, 0.5f,     0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		 0.5f, 0.0f, 0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 5.0f,
		 0.0f, 0.8f, 0.0f,     0.92f, 0.86f, 0.76f,   2.5f, 5.0f
	};


	// ========================================================
	// ÍNDICES
	//
	// Define a ordem em que os vértices serão utilizados
	// para formar os triângulos.
	//
	// Cada grupo de 3 índices representa um triângulo.
	//
	// Os dois triângulos juntos formam um quadrado.
	// ========================================================
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
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
	// Cada vértice possui 8 floats:
	//
	// 0 -> posição (X, Y, Z)
	// 1 -> cor      (R, G, B)
	// 2 -> textura  (U, V)
	//
	// O stride é 8 * sizeof(float), pois precisamos
	// pular todos os 8 valores para chegar ao próximo vértice.
	// ========================================================

	// Configura o atributo de posição.
	VAO1.LinkAttrib(
		VBO1,
		0,
		3,
		GL_FLOAT,
		8 * sizeof(float),
		(void*)0
	);


	// Configura o atributo de cor.
	VAO1.LinkAttrib(
		VBO1,
		1,
		3,
		GL_FLOAT,
		8 * sizeof(float),
		(void*)(3 * sizeof(float))
	);


	// Configura o atributo das coordenadas da textura.
	VAO1.LinkAttrib(
		VBO1,
		2,
		2,
		GL_FLOAT,
		8 * sizeof(float),
		(void*)(6 * sizeof(float))
	);


	// Desativa os objetos após terminar a configuração.
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// ========================================================
	// TEXTURA
	//
	// Cria uma textura utilizando a imagem "brick.png".
	//
	// GL_TEXTURE_2D -> textura 2D.
	// GL_TEXTURE0  -> unidade de textura utilizada.
	// GL_RGBA      -> formato dos dados da imagem.
	// GL_UNSIGNED_BYTE -> cada componente possui 1 byte.
	// ========================================================
	Texture popcat(
		"brick.png",
		GL_TEXTURE_2D,
		GL_TEXTURE0,
		GL_RGBA,
		GL_UNSIGNED_BYTE
	);


	// Associa a textura à variável "tex0" do Shader.
	//
	// O shader poderá utilizar essa variável para
	// acessar a textura durante o desenho.
	popcat.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade para renderização 3D

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)); // Inicializa a câmera

	// ========================================================
	// LOOP PRINCIPAL
	//
	// Enquanto a janela estiver aberta:
	//
	// 1. Limpa a tela
	// 2. Ativa o Shader Program
	// 3. Atualiza o Uniform "scale"
	// 4. Ativa a textura
	// 5. Ativa o VAO
	// 6. Desenha usando os índices do EBO
	// 7. Mostra o resultado
	// 8. Processa eventos
	// ========================================================
	while (!glfwWindowShouldClose(window)) {

		// Limpa a tela.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Ativa o Shader Program.
		shaderProgram.Activate();

		// Atualiza a matriz da câmera com base nos inputs do usuário.
		camera.Inputs(window);

		// Atualiza a matriz de projeção e view no shader.
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Ativa a textura.
		popcat.Bind();

		// Ativa o VAO com a configuração dos vértices.
		VAO1.Bind();

		// ====================================================
		// DESENHO COM ÍNDICES
		//
		// GL_TRIANGLES -> grupos de 3 índices formam triângulos.
		// sizeof(indices) / sizeof(int) -> número de índices.
		// GL_UNSIGNED_INT -> cada índice é um unsigned int.
		// 0            -> começa no primeiro índice do EBO.
		//
		// 6 índices = 2 triângulos.
		// ====================================================
		glDrawElements(
			GL_TRIANGLES,
			sizeof(indices) / sizeof(int),
			GL_UNSIGNED_INT,
			0
		);

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

	// Libera a textura da memória da GPU.
	popcat.Delete();


	// Fecha a janela e encerra a GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}