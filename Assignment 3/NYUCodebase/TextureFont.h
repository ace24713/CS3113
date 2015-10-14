#ifndef TEXTURE_FONT_H
#define TEXTURE_FONT_H

#include <string>

class ShaderProgram;

class TextureFont
{
public:
	TextureFont() {}
	TextureFont(unsigned int textureID, float spacing);

	void Draw(const std::string& text, float size, float x, float y, ShaderProgram* program);

	unsigned int textureID;
	float spacing;
};

#endif // !TEXTURE_FONT_H
