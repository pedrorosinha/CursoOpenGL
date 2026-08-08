#include "shaderClass.h"
#include <iostream>

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

		// Fecha o arquivo.
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

	// Envia o código-fonte para o Vertex Shader.
	glShaderSource(
		vertexShader,
		1,
		&vertexSource,
		nullptr
	);

	// Compila o código do Vertex Shader.
	glCompileShader(vertexShader);

	// Verifica se a compilação foi realizada com sucesso.
	compileErrors(vertexShader, "VERTEX");


	// ========================================================
	// FRAGMENT SHADER
	//
	// Cria o Fragment Shader, fornece seu código
	// e depois compila.
	// ========================================================
	GLuint fragmentShader =
		glCreateShader(GL_FRAGMENT_SHADER);

	// Envia o código-fonte para o Fragment Shader.
	glShaderSource(
		fragmentShader,
		1,
		&fragmentSource,
		nullptr
	);

	// Compila o código do Fragment Shader.
	glCompileShader(fragmentShader);

	// Verifica se a compilação foi realizada com sucesso.
	compileErrors(fragmentShader, "FRAGMENT");


	// ========================================================
	// SHADER PROGRAM
	//
	// Cria um programa que irá reunir os shaders.
	// ========================================================
	ID = glCreateProgram();


	// Adiciona o Vertex Shader ao programa.
	glAttachShader(ID, vertexShader);

	// Adiciona o Fragment Shader ao programa.
	glAttachShader(ID, fragmentShader);


	// Liga os shaders formando um programa que pode
	// ser usado pela GPU.
	glLinkProgram(ID);

	// Verifica se o Shader Program foi ligado corretamente.
	compileErrors(ID, "PROGRAM");


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
	// Torna este Shader Program o programa ativo.
	glUseProgram(ID);
}


// ========================================================
// DELETE
//
// Libera o Shader Program da memória da GPU.
// ========================================================
void Shader::Delete()
{
	// Remove o Shader Program criado.
	glDeleteProgram(ID);
}


// ========================================================
// COMPILE ERRORS
//
// Verifica se um Shader foi compilado corretamente
// ou se um Shader Program foi ligado corretamente.
//
// "shader" -> ID do shader ou programa.
// "type"   -> identifica o tipo que está sendo verificado.
//
// VERTEX   -> verifica compilação do Vertex Shader.
// FRAGMENT -> verifica compilação do Fragment Shader.
// PROGRAM  -> verifica ligação do Shader Program.
// ========================================================
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Armazena o resultado da compilação ou ligação.
	GLint hasCompiled;

	// Armazena a mensagem de erro retornada pelo OpenGL.
	char infoLog[1024];


	// Se não for "PROGRAM", estamos verificando
	// a compilação de um Vertex ou Fragment Shader.
	if (type != "PROGRAM")
	{
		// Obtém o status da compilação do shader.
		glGetShaderiv(
			shader,
			GL_COMPILE_STATUS,
			&hasCompiled
		);


		// GL_FALSE significa que ocorreu um erro.
		if (hasCompiled == GL_FALSE)
		{
			// Obtém a mensagem de erro da compilação.
			glGetShaderInfoLog(
				shader,
				1024,
				NULL,
				infoLog
			);


			// Exibe o erro no console.
			std::cout
				<< "SHADER_COMPILATION_ERROR for:"
				<< type
				<< "\n"
				<< infoLog
				<< std::endl;
		}
	}
	else
	{
		// Verifica se o Shader Program foi ligado corretamente.
		glGetProgramiv(
			shader,
			GL_LINK_STATUS,
			&hasCompiled
		);


		// GL_FALSE significa que ocorreu um erro.
		if (hasCompiled == GL_FALSE)
		{
			// Obtém a mensagem de erro da ligação.
			glGetProgramInfoLog(
				shader,
				1024,
				NULL,
				infoLog
			);


			// Exibe o erro no console.
			std::cout
				<< "SHADER_LINKING_ERROR for:"
				<< type
				<< "\n"
				<< infoLog
				<< std::endl;
		}
	}
}