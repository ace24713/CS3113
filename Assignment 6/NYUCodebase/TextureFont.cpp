#include "TextureFont.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>

#include "ShaderProgram.h"

TextureFont::TextureFont(unsigned int textureID, float spacing)
	: textureID(textureID)
	, spacing(spacing)
{}

void TextureFont::Draw(const std::string& text, float size, float x, float y, ShaderProgram* program)
{
	float texture_size = 1.f / 16.f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	for (size_t i = 0; i < text.size(); ++i)
	{
		float texture_x = float(text[i] % 16) * texture_size;
		float texture_y = float(text[i] / 16) * texture_size;
		float x_off = x + (spacing + 1.f)*size*i;
		float y_off = y;
		vertexData.insert(vertexData.end(),
			{ x_off + size, y_off + size
			, x_off, y_off + size
			, x_off, y_off

			, x_off, y_off
			, x_off + size, y_off
			, x_off + size, y_off + size });
		texCoordData.insert(texCoordData.end(),
			{ texture_x + texture_size, texture_y
			, texture_x, texture_y
			, texture_x, texture_y + texture_size

			, texture_x, texture_y + texture_size
			, texture_x + texture_size, texture_y + texture_size
			, texture_x + texture_size, texture_y });
	}
	program->setModelMatrix(Matrix());

	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}