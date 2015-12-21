#include "TexturedRectangle.h"

void TexturedRectangle::Draw(ShaderProgram * program) const
{
	const float rectV[] =	{ right, upper
							, left, upper
							, left, lower

							, left, lower
							, right, lower
							, right, upper };

	float leftTex = (hMirror ? 1.f : 0.f);
	float rightTex = 1.f - leftTex;

	const float texV[] =	{ rightTex, 1.f
							, leftTex, 1.f
							, leftTex, 0.f

							, leftTex, 0.f
							, rightTex, 0.f
							, rightTex, 1.f };

	Matrix modelMatrix;
	modelMatrix.Translate(point.x, point.y, 0);
	modelMatrix.Rotate(angle);

	program->setModelMatrix(modelMatrix);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, rectV);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texV);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
