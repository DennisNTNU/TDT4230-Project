#include "Stars.h"

#include <iostream>
#include "Asterism.h"

Stars::Stars(Gloom::Shader * shader, std::string starChartPath)
{
	_shader = shader;
	_modelMatrix = glm::mat4(1.0f);
	if (readStarChart(starChartPath))
	{
		std::cout << "Failed to read starchart file!" << std::endl;
	}
	initVAO_Stars();
	initVAO_Asterisms();
}

Stars::~Stars()
{
}

void Stars::draw(glm::mat4 * perspView)
{
	_shader->activate();

	glPointSize(2.0f);
	glBindVertexArray(_vaoID_stars);

	glUniformMatrix4fv(2, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	glDrawArrays(GL_POINTS, 0, _vertexCount_stars);
	//glDrawArrays(GL_LINE_STRIP, 0, _vertexCount_stars);
	//glDrawArrays(GL_LINES, 0, _vertexCount_stars);

	//glDrawArrays(GL_TRIANGLES, 0, _vertexCount_stars);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, _vertexCount_stars);

	glPointSize(1.0f);


	glBindVertexArray(_vaoID_asterisms);

	glUniformMatrix4fv(2, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	glDrawArrays(GL_LINES, 0, _vertexCount_asterisms);

	_shader->deactivate();
}

void Stars::initVAO_Stars()
{
	glGenVertexArrays(1, &_vaoID_stars);
	glBindVertexArray(_vaoID_stars);

	unsigned int _vboID[2];
	glGenBuffers(2, _vboID);


	// one vertex per star
	_vertexCount_stars = int(stars.size());
	int numDoubles = 3 * _vertexCount_stars; // positions
	int numFloats = 4 * _vertexCount_stars; // colors

	double* vertexPoses = new double[numDoubles];
	float* vertexColors = new float[numFloats];

	int k = 0;
	int j = 0;
	double dist = 0.0;

	for (unsigned int i = 0; i < _vertexCount_stars; i++) {

		dist = (600.0 + stars[i].dist) * 6378.0/8.0;

		vertexPoses[k++] = dist * cos(stars[i].ra*3.141592653589793238462 / 12.0) * cos(stars[i].dec*3.141592653589793238462 / 180.0);
		vertexPoses[k++] = dist * sin(stars[i].ra*3.141592653589793238462 / 12.0) * cos(stars[i].dec*3.141592653589793238462 / 180.0);
		vertexPoses[k++] = dist * sin(stars[i].dec*3.141592653589793238462 / 180.0);


		//float factor = 0.9f - 0.9f * (*stars)[i].mag / 8.0f;
		float factor = float(1.0 / (1.0 + pow(2.718281828, stars[i].mag - 3.5)));

		vertexColors[j++] = factor;
		vertexColors[j++] = factor;
		vertexColors[j++] = factor;
		vertexColors[j++] = 1.0;
	}



	glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, numDoubles*sizeof(double), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, numFloats*sizeof(float), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(1);


	delete[] vertexPoses;
	delete[] vertexColors;
}

void Stars::initVAO_Asterisms()
{
	
	
	
	glGenVertexArrays(1, &_vaoID_asterisms);
	glBindVertexArray(_vaoID_asterisms);

	unsigned int _vboID[2];
	glGenBuffers(2, _vboID);


	Asterism asterisms;
	asterisms.init();


	_vertexCount_asterisms = 2 * asterisms.edges.size();
	//_vertexCount_asterisms = 2 * 2;

	double* vertexPoses = new double[3 * _vertexCount_asterisms];
	float* vertexColors = new float[4 * _vertexCount_asterisms];
	
	for (unsigned int i = 0; i < _vertexCount_asterisms / 2; i++)
	{
		int indexStart = hip_to_index[asterisms.edges[i].star1];
		int indexEnd = hip_to_index[asterisms.edges[i].star2];

		//double distStart = (150.0 + stars[indexStart].dist) * 6378.0;
		//double distEnd = (150.0 + stars[indexEnd].dist) * 6378.0;
		double distStart = (600.0 + stars[indexStart].dist) * 6378.0/8.0;
		double distEnd = (600.0 + stars[indexEnd].dist) * 6378.0/8.0;

		vertexPoses[6 * i + 0] = distStart * cos(stars[indexStart].ra*3.141592653589793238462 / 12.0) * cos(stars[indexStart].dec*3.141592653589793238462 / 180.0);
		vertexPoses[6 * i + 1] = distStart * sin(stars[indexStart].ra*3.141592653589793238462 / 12.0) * cos(stars[indexStart].dec*3.141592653589793238462 / 180.0);
		vertexPoses[6 * i + 2] = distStart * sin(stars[indexStart].dec*3.141592653589793238462 / 180.0);
		vertexPoses[6 * i + 3] = distEnd * cos(stars[indexEnd].ra*3.141592653589793238462 / 12.0) * cos(stars[indexEnd].dec*3.141592653589793238462 / 180.0);
		vertexPoses[6 * i + 4] = distEnd * sin(stars[indexEnd].ra*3.141592653589793238462 / 12.0) * cos(stars[indexEnd].dec*3.141592653589793238462 / 180.0);
		vertexPoses[6 * i + 5] = distEnd * sin(stars[indexEnd].dec*3.141592653589793238462 / 180.0);

		vertexColors[8 * i + 0] = 0.7;
		vertexColors[8 * i + 1] = 0.7;
		vertexColors[8 * i + 2] = 0.9;
		vertexColors[8 * i + 3] = 0.3;
		vertexColors[8 * i + 4] = 0.7;
		vertexColors[8 * i + 5] = 0.7;
		vertexColors[8 * i + 6] = 0.9;
		vertexColors[8 * i + 7] = 0.3;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * _vertexCount_asterisms * sizeof(double), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * _vertexCount_asterisms * sizeof(float), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(1);


	delete[] vertexPoses;
	delete[] vertexColors;
}

bool Stars::readStarChart(std::string path)
{
	std::ifstream file(path, std::ios::in);
	if (file.fail()) {
		perror(path.c_str());
		return true;
	}

	std::vector<std::string> rawData;

	file.seekg(0, std::ios::beg);


	char line[256];

	while (!file.eof()) {
		file.getline(line, 160);
		rawData.emplace_back(std::string(line));
	}

	file.close();

	int k = int(rawData.size());
	Star star;
	std::string temp;
	int valueID;

	int starIndex = 0;
	double maxDist = 0.0;
	double filterDist = 1200.0;
	double filterMag = 7.0;

	// Iterates through each line i.e. through stars
	for (int i = 1; i < k; i++) {

		temp = "";
		valueID = 0;
		// iterates through chars in line
		for (unsigned int j = 0; j < rawData[i].size(); j++) {
			if (rawData[i][j] == ',') {

				switch (++valueID) {
				case 1:
					star.id = atoi(temp.c_str());
					break;
				case 2:
					star.hip = atoi(temp.c_str());
					break;
				case 8: // right ascention
					star.ra = atof(temp.c_str());
					break;
				case 9: // declination
					star.dec = atof(temp.c_str());
					break;
				case 10: // distance
					star.dist = atof(temp.c_str());
					break;
				case 11: // magnitude
					star.mag = atof(temp.c_str());
					break;
				case 12: // absolute Magnitude
					star.absMag = atof(temp.c_str());
					j = 160;
					break;
				default:

					break;
				}
				temp = "";
			}
			else {
				temp = temp + rawData[i][j];
			}
		}
		if (star.mag < filterMag && star.dist < filterDist) {
			stars.emplace_back(star);
			hip_to_index[star.hip] = starIndex++;
			if (maxDist < star.dist) { maxDist = star.dist; }
		}
		if (i % 1500 == 0) {
			std::cout << i << " ";
		}

	}

	std::cout << " There are " << stars.size() << " stars with magnitude less than " << filterMag << " and nearer than " << filterDist << ". Maximal Distance of those is " << maxDist << std::endl;
	//std::cout << "ID: " << stars[0].id << " RA: " << stars[0].ra << " Dec: " << stars[0].dec << " Dist: " << stars[0].dist << " AbsMag: " << stars[0].absMag << std::endl;

	return false;
}