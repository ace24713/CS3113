#ifndef TEXTURED_RECTANGLE_H
#define TEXTURED_RECTANGLE_H

#include "CollisionBox.h"
#include "ShaderProgram.h"

class TexturedRectangle
{
public:
	unsigned texture = 0;

	Point point;

	float lower = 0.f;
	float upper = 0.f;
	float left = 0.f;
	float right = 0.f;

	float angle = 0.f;
	bool hMirror = false;

	void Draw(ShaderProgram* program) const;
};

#endif // !TEXTURED_RECTANGLE_H
