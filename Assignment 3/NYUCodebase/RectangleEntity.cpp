#include "RectangleEntity.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include "ShaderProgram.h"

void RectangleEntity::Draw(ShaderProgram* program)
{
	static const float squareV[] =	{ .5f, .5f
									, -.5f, .5f
									, -.5f, -.5f

									, -.5f, -.5f
									, .5f, -.5f
									, .5f, .5f };

	Matrix modelMatrix;
	modelMatrix.Translate(x, y, 0);
	modelMatrix.Scale(width, height, 1.f);
	modelMatrix.Rotate(rotation);

	program->setModelMatrix(modelMatrix);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, squareV);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
}

void RectangleEntity::Advance(float elapsed)
{
	rotation += spin * elapsed;
	x += direction_x * speed * elapsed;
	y += direction_y * speed * elapsed;
}