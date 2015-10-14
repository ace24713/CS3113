#include "SheetSpriteEntity.h"

#include "ShaderProgram.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

SheetSpriteEntity::SheetSpriteEntity(unsigned int textureID, float u, float v, float width, float height, const RectangleEntity& rectangle)
	: rectangle(rectangle)
	, textureID(textureID)
	, u(u)
	, v(v)
	, width(width)
	, height(height)
{}


SheetSpriteEntity::SheetSpriteEntity(unsigned int textureID, float sheetPixelWidth, float sheetPixelHeight, float spritePixelU, float spritePixelV, float spritePixelWidth, float spritePixelHeight, const RectangleEntity& rectangle)
	: SheetSpriteEntity(textureID, spritePixelU / sheetPixelWidth, spritePixelV / sheetPixelHeight, spritePixelWidth / sheetPixelWidth, spritePixelHeight / sheetPixelHeight, rectangle)
{}

void SheetSpriteEntity::Draw(ShaderProgram* program)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLfloat texCoords[] =	{ u + width, v + height
							, u, v + height
							, u, v

							, u, v
							, u + width, v
							, u + width, v + height };
	glEnableVertexAttribArray(program->texCoordAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	rectangle.Draw(program);
	glDisableVertexAttribArray(program->texCoordAttribute);
}