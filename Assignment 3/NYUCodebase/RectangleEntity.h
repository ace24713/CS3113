#ifndef RECTANGLE_ENTITY_H
#define RECTANGLE_ENTITY_H

class ShaderProgram;

class RectangleEntity
{
public:
	RectangleEntity()
		: x(0), y(0), rotation(0), width(0), height(0), spin(0), speed(0), direction_x(0), direction_y(0)
	{}
	void Draw(ShaderProgram* program);
	void Advance(float elapsed);

	float x;
	float y;
	float rotation;

	float width;
	float height;

	float spin;
	float speed;
	float direction_x;
	float direction_y;
};

#endif // !RECTANGLE_ENTITY_H
