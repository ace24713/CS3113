#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include "ShaderProgram.h"

struct Point
{
	float x = 0.f;
	float y = 0.f;
};

struct AABB
{
	Point center;
	Point offset;

	float width = 0.f;
	float height = 0.f;

	float halfWidth() const { return width / 2.f; }
	float halfHeight() const { return height / 2.f; }
	float bottom() const { return center.y + offset.y - halfHeight(); }
	float top() const { return center.y + offset.y + halfHeight(); }
	float left() const { return center.x + offset.x - halfWidth(); }
	float right() const { return center.x + offset.x + halfWidth(); }

	void debugDraw(ShaderProgram* program) const
	{
		static const float squareV[] = { .5f, .5f
			, -.5f, .5f
			, -.5f, -.5f

			, -.5f, -.5f
			, .5f, -.5f
			, .5f, .5f };

		Matrix modelMatrix;
		modelMatrix.Translate(center.x + offset.x, center.y + offset.y, 0);
		modelMatrix.Scale(width, height, 1.f);

		program->setModelMatrix(modelMatrix);
		glEnableVertexAttribArray(program->positionAttribute);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, squareV);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program->positionAttribute);
	}
};

inline bool colliding(const AABB& lhs, const AABB& rhs)
{
	return !(lhs.bottom() > rhs.top()
			|| lhs.top() < rhs.bottom()
			|| lhs.left() > rhs.right()
			|| lhs.right() < rhs.left());
}

inline bool colliding(const AABB& lhs, const Point& rhs)
{
	return !(lhs.bottom() > rhs.y
			 || lhs.top() < rhs.y
			 || lhs.left() > rhs.x
			 || lhs.right() < rhs.x);
}

inline bool colliding(const Point& rhs, const AABB& lhs) { return colliding(lhs, rhs); }


#endif // !COLLISION_BOX_H
