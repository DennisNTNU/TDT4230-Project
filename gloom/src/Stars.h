#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "gloom/shader.hpp"

struct Star {
	int id;
	int hip;
	double ra;
	double dec;
	double dist;
	double mag;
	double absMag;
	std::vector<int> connections;

	Star()
	{
		id = -1;
		hip = -1;
		ra = 0.0;
		dec = 0.0;
		dist = 100.0;
		mag = 15.0;
		absMag = 15.0;
	}
};

class Stars
{
public:
	Stars(Gloom::Shader* shader, std::string starChartPath);
	~Stars();

	void draw(glm::mat4 * perspView);

	glm::mat4 _modelMatrix;

private:

	void initVAO_Stars();
	void initVAO_Asterisms();

	bool readStarChart(std::string path);

	std::vector<Star> stars;
	std::map<int, int> hip_to_index;

	Gloom::Shader* _shader;

	unsigned int _vaoID_stars;
	unsigned int _vaoID_asterisms;
	/*
	unsigned int _numVertices_stars;
	unsigned int _numVertices_asterisms;*/

	unsigned int _vertexCount_stars;
	unsigned int _vertexCount_asterisms;
};

