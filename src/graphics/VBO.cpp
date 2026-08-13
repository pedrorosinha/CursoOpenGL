#include "VBO.h"


// ========================================================
// CONSTRUTOR
//
// Cria o VBO, ativa-o e envia os vértices para a GPU.
//
// O VBO armazena os dados dos vértices.
// ========================================================
VBO::VBO(GLfloat* vertices, GLsizeiptr size) {

	// Gera um identificador para o VBO.
	glGenBuffers(1, &ID);

	// Ativa o VBO como buffer de vértices.
	glBindBuffer(GL_ARRAY_BUFFER, ID);

	// Envia os vértices da CPU para a memória da GPU.
	glBufferData(
		GL_ARRAY_BUFFER,
		size,
		vertices,
		GL_STATIC_DRAW
	);
}


// ========================================================
// BIND
//
// Ativa este VBO.
// ========================================================
void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}


// ========================================================
// UNBIND
//
// Desativa o VBO atualmente associado.
// ========================================================
void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// ========================================================
// DELETE
//
// Libera o VBO da memória da GPU.
// ========================================================
void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}