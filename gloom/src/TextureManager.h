#pragma once

#include <string>
#include <vector>
#include <map>

struct GLTexture {
	unsigned int id;
	int width;
	int height;
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	GLTexture getTexture(std::string path);
private:
	bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);

	int numTextures;
	std::map<std::string, GLTexture> textures;
};

