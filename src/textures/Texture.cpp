#include "Texture.h"

// ========================================================
// CONSTRUTOR DA TEXTURA
//
// Recebe:
// image     -> caminho da imagem.
// texType   -> tipo da textura, neste caso GL_TEXTURE_2D.
// slot      -> unidade de textura que será utilizada.
// format    -> formato dos dados da imagem.
// pixelType -> tipo dos dados de cada componente do pixel.
//
// O construtor:
// 1. Carrega a imagem
// 2. Cria a textura na GPU
// 3. Configura seus parâmetros
// 4. Envia os pixels para a GPU
// 5. Gera os mipmaps
// ========================================================
Texture::Texture(
	const char* image,
	GLenum texType,
	GLuint slot,
	GLenum format,
	GLenum pixelType)
{
	// Guarda o tipo da textura para ser utilizado posteriormente.
	type = texType;


	// Variáveis utilizadas para armazenar informações da imagem.
	int widthImg;
	int heightImg;
	int numColCh;


	// Inverte verticalmente a imagem durante o carregamento.
	//
	// Isso é necessário porque a origem das coordenadas
	// da imagem pode ser diferente da origem utilizada
	// pelas coordenadas de textura do OpenGL.
	stbi_set_flip_vertically_on_load(true);


	// Carrega a imagem do arquivo.
	//
	// bytes recebe os pixels da imagem.
	// widthImg recebe a largura.
	// heightImg recebe a altura.
	// numColCh recebe o número de canais de cor.
	unsigned char* bytes =
		stbi_load(
			image,
			&widthImg,
			&heightImg,
			&numColCh,
			0
		);


	// ========================================================
	// CRIAÇÃO DA TEXTURA
	// ========================================================

	// Gera um ID para a textura.
	glGenTextures(1, &ID);


	// Ativa a unidade de textura especificada.
	glActiveTexture(GL_TEXTURE0 + slot);
	// Armazena a unidade de textura para ser utilizada posteriormente.
	unit = slot;


	// Vincula a textura criada ao tipo GL_TEXTURE_2D.
	glBindTexture(texType, ID);


	// ========================================================
	// FILTROS DA TEXTURA
	//
	// GL_TEXTURE_MIN_FILTER -> como a textura será filtrada
	// quando for reduzida.
	//
	// GL_TEXTURE_MAG_FILTER -> como será filtrada
	// quando for ampliada.
	//
	// GL_NEAREST utiliza o pixel mais próximo.
	// ========================================================
	glTexParameteri(
		texType,
		GL_TEXTURE_MIN_FILTER,
		GL_NEAREST
	);

	glTexParameteri(
		texType,
		GL_TEXTURE_MAG_FILTER,
		GL_NEAREST
	);


	// ========================================================
	// REPETIÇÃO DA TEXTURA
	//
	// Define o que acontece quando as coordenadas de textura
	// ultrapassam o intervalo normal de 0.0 até 1.0.
	//
	// GL_REPEAT faz a textura se repetir.
	// ========================================================
	glTexParameteri(
		texType,
		GL_TEXTURE_WRAP_S,
		GL_REPEAT
	);

	glTexParameteri(
		texType,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT
	);


	// ========================================================
	// COR DA BORDA
	//
	// Define uma cor que poderia ser utilizada quando
	// a textura utiliza GL_CLAMP_TO_BORDER.
	//
	// Atualmente está comentado porque estamos utilizando
	// GL_REPEAT.
	// ========================================================

	// float flatColor[] = {
	//     1.0f, 1.0f, 1.0f, 1.0f
	// };

	// glTexParameterfv(
	//     texType,
	//     GL_TEXTURE_BORDER_COLOR,
	//     flatColor
	// );


	// ========================================================
	// ENVIO DA IMAGEM PARA A GPU
	//
	// Copia os pixels carregados da imagem para a textura
	// armazenada na GPU.
	//
	// widthImg  -> largura da imagem.
	// heightImg -> altura da imagem.
	// format    -> formato dos pixels.
	// pixelType -> tipo de cada componente.
	// bytes     -> dados da imagem.
	// ========================================================
	glTexImage2D(
		texType,
		0,
		GL_RGBA,
		widthImg,
		heightImg,
		0,
		format,
		pixelType,
		bytes
	);


	// Gera automaticamente os níveis de mipmap da textura.
	//
	// Mipmaps são versões menores da textura utilizadas
	// quando o objeto aparece mais distante da câmera.
	glGenerateMipmap(texType);


	// Libera os dados da imagem que estavam na memória da CPU.
	//
	// A imagem já foi enviada para a GPU.
	stbi_image_free(bytes);


	// Desvincula a textura.
	glBindTexture(texType, 0);
}


// ========================================================
// TEXTURE UNIT
//
// Associa a textura a uma variável uniform do Shader.
//
// uniform -> nome da variável no shader.
// unit    -> unidade de textura que será utilizada.
// ========================================================
void Texture::texUnit(
	Shader& shader,
	const char* uniform,
	GLuint unit)
{
	// Procura a localização da variável uniform no Shader.
	GLuint texUni =
		glGetUniformLocation(shader.ID, uniform);


	// Ativa o Shader Program.
	shader.Activate();


	// Informa ao shader qual unidade de textura
	// está associada à variável uniform.
	glUniform1i(texUni, unit);
}


// ========================================================
// BIND
//
// Torna esta textura a textura atualmente ativa
// para o tipo armazenado em "type".
// ========================================================
void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}


// ========================================================
// UNBIND
//
// Desvincula a textura atualmente associada.
// ========================================================
void Texture::Unbind()
{
	glBindTexture(type, 0);
}


// ========================================================
// DELETE
//
// Libera a textura da memória da GPU.
// ========================================================
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}