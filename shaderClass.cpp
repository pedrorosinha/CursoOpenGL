#include "shaderClass.h"


// ========================================================
// LEITURA DO ARQUIVO
//
// Abre um arquivo de shader e retorna todo o conteúdo
// como uma string.
//
// Isso permite manter o Vertex Shader e o Fragment Shader
// em arquivos separados do código C++.
// ========================================================
std::string get_file_contents(const char* filename) {

	// Abre o arquivo em modo binário.
	std::ifstream in(filename, std::ios::binary);

	if (in) {

		std::string contents;

		// Vai para o final do arquivo para descobrir seu tamanho.
		in.seekg(0, std::ios::end);

		// Reserva espaço na string para todo o conteúdo.
		contents.resize(in.tellg());

		// Volta para o início do arquivo.
		in.seekg(0, std::ios::beg);

		// Lê todo o conteúdo para a string.
		in.read(&contents[0], contents.size());

		in.close();

		return contents;
	}

	// Caso não consiga abrir o arquivo, lança um erro.
	throw(errno);
}


// ========================================================
// CONSTRUTOR DO SHADER
//
// Recebe os caminhos dos arquivos:
// Vertex Shader e Fragment Shader.
//
// Depois:
// 1. Lê os arquivos
// 2. Compila os shaders
// 3. Cria o Shader Program
// 4. Liga os shaders ao programa
// ========================================================
Shader::Shader(
	const char* vertexFile,
	const char* fragmentFile)
{
	// Lê o código dos dois arquivos.
	std::string vertexCode =
		get_file_contents(vertexFile);

	std::string fragmentCode =
		get_file_contents(fragmentFile);


	// Converte as strings para const char*,
	// formato esperado pelo glShaderSource().
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	// ========================================================
	// VERTEX SHADER
	//
	// Cria o Vertex Shader, fornece seu código
	// e depois compila.
	// ========================================================
	GLuint vertexShader =
		glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(
		vertexShader,
		1,
		&vertexSource,
		nullptr
	);

	glCompileShader(vertexShader);


	// ========================================================
	// FRAGMENT SHADER
	//
	// Cria o Fragment Shader, fornece seu código
	// e depois compila.
	// ========================================================
	GLuint fragmentShader =
		glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(
		fragmentShader,
		1,
		&fragmentSource,
		nullptr
	);

	glCompileShader(fragmentShader);


	// ========================================================
	// SHADER PROGRAM
	//
	// Cria um programa que irá reunir os shaders.
	// ========================================================
	ID = glCreateProgram();


	// Adiciona os shaders ao programa.
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);


	// Liga os shaders formando um programa que pode
	// ser usado pela GPU.
	glLinkProgram(ID);


	// Depois que foram ligados ao programa,
	// os shaders individuais não são mais necessários.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


// ========================================================
// ACTIVATE
//
// Seleciona este Shader Program para ser utilizado
// durante o desenho.
// ========================================================
void Shader::Activate()
{
	glUseProgram(ID);
}


// ========================================================
// DELETE
//
// Libera o Shader Program da memória da GPU.
// ========================================================
void Shader::Delete()
{
	glDeleteProgram(ID);
}