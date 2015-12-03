#ifndef RECTANGLE_ENTITY_H
#define RECTANGLE_ENTITY_H

class ShaderProgram;

class RectangleEntity
{
public:
	RectangleEntity()
		: x(0), y(0), width(0), height(0), velocity_x(0), velocity_y(0), acceleration_x(0), acceleration_y(0)
	{}
	void Draw(ShaderProgram* program);
	void Advance(float elapsed);

	float x;
	float y;

	float width;
	float height;

	float velocity_x;
	float velocity_y;

	float acceleration_x;
	float acceleration_y;
};

#endif // !RECTANGLE_ENTITY_H
