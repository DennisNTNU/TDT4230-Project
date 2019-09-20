#include "TextureManager.h"


#include <../vendor/glfw/include/GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

#include "gloom/gloom.hpp"
#include "picoPNG.h"


void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int a;
	std::cin >> a;
}


TextureManager::TextureManager()
{
	numTextures = 0;
}


TextureManager::~TextureManager()
{
}

GLTexture TextureManager::getTexture(std::string path)
{
	std::map<std::string, GLTexture>::iterator mit = textures.begin();

	for (; mit != textures.end(); mit++) {
		if (mit->first == path) {
			return mit->second;
		}
	}

	GLTexture tex;

	std::vector<unsigned char> in;
	std::vector<unsigned char> out;

	unsigned long width, height;

	if (readFileToBuffer(path, in)) {
		fatalError("Failed to load PNG file to buffer!");
	}

	int errorCode = decodePNG(out, width, height, &(in[0]), in.size(), true);
	if (errorCode != 0) {
		fatalError("decodePNG failed with error " + std::to_string(errorCode));
	}


	tex.width = width;
	tex.height = height;

	glGenTextures(1, &(tex.id));
	glBindTexture(GL_TEXTURE_2D, tex.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, 0x84FE, 64.0f); // GL_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FE

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	//glPixelStorei(GL_PACK_ROW_LENGTH, 200);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	textures.insert(std::pair<std::string, GLTexture> (path, tex));

	return tex;
}

bool TextureManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return true;
	}

	//seek to the end
	file.seekg(0, std::ios::end);
	//get the file size
	//long int fileSize = file.tellg();
	//std::streamoff fileSize = file.tellg();
	long long int fileSize = file.tellg();
	//seek back to the beginning
	file.seekg(0, std::ios::beg);
	//reduce file size by any header bytes that might be present
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);
	file.close();

	return false;
}
