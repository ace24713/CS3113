#ifndef COLLISION_RECTANGLE_H
#define COLLISION_RECTANGLE_H

struct CollisionRectangle
{
	float x;
	float y;
	float width;
	float height;

	bool inside(float p_x, float p_y)
	{
		p_x -= x;
		p_y -= y;
		return (-width / 2.f <= p_x && p_x <= width / 2.f && -height / 2.f <= p_y && p_y <= height / 2.f);
	}

	friend bool overlap(const CollisionRectangle& lhs, const CollisionRectangle& rhs)
	{
		float bottom1 = lhs.y - lhs.height / 2.f;
		float bottom2 = rhs.y - rhs.height / 2.f;
		float top1 = lhs.y + lhs.height / 2.f;
		float top2 = rhs.y + rhs.height / 2.f;
		float left1 = lhs.x - lhs.width / 2.f;
		float left2 = rhs.x - rhs.width / 2.f;
		float right1 = lhs.x + lhs.width / 2.f;
		float right2 = rhs.x + rhs.width / 2.f;
		return !(bottom1 > top2 || top1 < bottom2 || left1 > right2 || right1 < left2);
	}
};

#endif // !COLLISION_RECTANGLE_H
