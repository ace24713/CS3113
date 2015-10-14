#ifndef SHEET_SPRITE_ENTITY_H
#define SHEET_SPRITE_ENTITY_H

#include "RectangleEntity.h"

class SheetSpriteEntity
{
public:
	SheetSpriteEntity() {}
	SheetSpriteEntity(unsigned int textureID, float u = 0.f, float v = 0.f, float width = 1.f, float height = 1.f, const RectangleEntity& rectangle = RectangleEntity());
	SheetSpriteEntity(unsigned int textureID, float sheetPixelWidth, float sheetPixelHeight, float spritePixelU, float spritePixelV, float spritePixelWidth, float spritePixelHeight, const RectangleEntity& rectangle = RectangleEntity());

	void Draw(ShaderProgram* program);
	void Advance(float elapsed) { rectangle.Advance(elapsed); }

	RectangleEntity rectangle;

	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
};

#endif // !SHEET_SPRITE_ENTITY_H
