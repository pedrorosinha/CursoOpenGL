#include "EBO.h"


// ========================================================
// CONSTRUTOR
//
// Cria o EBO, ativa-o e envia os índices para a GPU.
//
// O EBO armazena a ordem em que os vértices serão
// utilizados durante o desenho.
// ========================================================
EBO::EBO(GLuint* indices, GLsizeiptr size) {

	// Gera um identificador para o EBO.
	glGenBuffers(1, &ID);

	// Ativa o EBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

	// Envia os índices da CPU para a memória da GPU.
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		size,
		indices,
		GL_STATIC_DRAW
	);
}


// ========================================================
// BIND
//
// Ativa este EBO.
// ========================================================
void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}


// ========================================================
// UNBIND
//
// Desativa o EBO atualmente associado.
// ========================================================
void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


// ========================================================
// DELETE
//
// Libera o EBO da memória da GPU.
// ========================================================
void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}