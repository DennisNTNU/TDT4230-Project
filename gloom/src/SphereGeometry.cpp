#include "SphereGeometry.h"

#include <glad/glad.h>
#include <math.h>

void initVAOSphere(unsigned int * vaoID, unsigned int * indexCount, unsigned char n, unsigned char m, float r)
{

	int vertexCount = (n + 1)*(m + 1) - 2;
	int triangleCount = 2 * m + 2 * (n - 2) * m;
	*indexCount = 3 * triangleCount;

	unsigned short* indices = new unsigned short[*indexCount];
	float* vertexPoses = new float[3 * vertexCount];
	float* vertexNormals = new float[3 * vertexCount];
	float* vertexUVs = new float[2 * vertexCount];

	float pi = 3.141592653589793238462;
	int k = 0; // vertex component index
	int o = 0; // UV component index
	int l = 0; // index index

	float x, y, z;

	// case i = 0
	// m vertices
	// m triangles
	for (unsigned char j = 0; j < m; j++) {
		// index: (0, 6, 7) (1, 7, 8) .. (m-2, 2*m - 2, 2*m - 1) (m-1, 2*m - 1, 2*m)
		// (j, m+j, m+j+1)
		indices[l++] = j;
		indices[l++] = m + j;
		indices[l++] = j + m + 1;


		vertexUVs[o++] = float(j) / m;
		vertexUVs[o++] = 0.0;


		vertexPoses[k] = 0.0;
		vertexNormals[k++] = 0.0;

		vertexPoses[k] = 0.0;
		vertexNormals[k++] = 0.0;

		vertexPoses[k] = r;
		vertexNormals[k++] = 1.0;
	}

	// Inbetween cases
	// m+1 vertices
	// 2*m triangles
	for (unsigned char i = 1; i < n; i++) {
		for (unsigned char j = 0; j < m; j++) {

			vertexUVs[o++] = float(j) / m;
			vertexUVs[o++] = float(i) / n;

			x = sin(i*pi / n) * cos(j*2.0*pi / m);
			y = sin(i*pi / n) * sin(j*2.0*pi / m);
			z = cos(i*pi / n);

			vertexPoses[k] = r * x;
			vertexNormals[k++] = x;

			vertexPoses[k] = r * y;
			vertexNormals[k++] = y;

			vertexPoses[k] = r * z;
			vertexNormals[k++] = z;

			if (i != n - 1) {
				short a = m + (i - 1)*(m + 1) + j;
				short b = m + i * (m + 1) + j;

				indices[l++] = a;
				indices[l++] = b;
				indices[l++] = a + 1;

				indices[l++] = a + 1;
				indices[l++] = b;
				indices[l++] = b + 1;
			}
		}
		vertexUVs[o++] = 1.0;
		vertexUVs[o++] = float(i) / n;

		x = sin(i*pi / n);
		z = cos(i*pi / n);

		vertexPoses[k] = r * x;
		vertexNormals[k++] = x;

		vertexPoses[k] = 0.0;
		vertexNormals[k++] = 0.0;

		vertexPoses[k] = r * z;
		vertexNormals[k++] = z;
	}

	// case i = n-1
	// m vertices
	// m triangles

	for (unsigned char j = 0; j < m; j++) {
		// index: (0, 1, 2) (0, 2, 3) .. (0, m-1, m) (0, m, 1)
		// (m + (n - 2)*(m + 1) + j, m + (n - 1)*(m + 1) + j, m + (n - 2)*(m + 1) + j + 1)
		unsigned short a = m + (n - 2)*(m + 1) + j;
		indices[l++] = a;
		indices[l++] = a + m + 1; // m + (n - 1)*(m + 1) + j;
		indices[l++] = a + 1;


		vertexUVs[o++] = float(j) / m;
		vertexUVs[o++] = 1.0;

		vertexPoses[k] = 0.0;
		vertexNormals[k++] = 0.0;

		vertexPoses[k] = 0.0;
		vertexNormals[k++] = 0.0;

		vertexPoses[k] = -r;
		vertexNormals[k++] = -1.0;
	}

	glGenVertexArrays(1, vaoID);
	glBindVertexArray(*vaoID);

	unsigned int vboIDs[4];
	glGenBuffers(4, vboIDs); // indices, positions, normals, UVs

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, *indexCount*sizeof(unsigned short), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount*sizeof(float), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount*sizeof(float), vertexNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 2 * vertexCount*sizeof(float), vertexUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	delete[] indices;
	delete[] vertexPoses;
	delete[] vertexNormals;
	delete[] vertexUVs;

}


void initVAOSphereNoNormals(unsigned int * vaoID, unsigned int * indexCount, unsigned char n, unsigned char m, float r)
{

	int vertexCount = (n + 1)*(m + 1) - 2;
	int triangleCount = 2 * m + 2 * (n - 2) * m;
	*indexCount = 3 * triangleCount;

	unsigned short* indices = new unsigned short[*indexCount];
	float* vertexPoses = new float[3 * vertexCount];
	float* vertexUVs = new float[2 * vertexCount];

	float pi = 3.141592653589793238462;
	int k = 0; // vertex component index
	int o = 0; // UV component index
	int l = 0; // index index

			   // case i = 0
			   // m vertices
			   // m triangles
	for (unsigned char j = 0; j < m; j++) {
		// index: (0, 6, 7) (1, 7, 8) .. (m-2, 2*m - 2, 2*m - 1) (m-1, 2*m - 1, 2*m)
		// (j, m+j, m+j+1)
		indices[l++] = j;
		indices[l++] = m + j;
		indices[l++] = j + m + 1;


		vertexUVs[o++] = float(j) / m;
		vertexUVs[o++] = 0.0;


		vertexPoses[k++] = 0.0;
		vertexPoses[k++] = 0.0;
		vertexPoses[k++] = r;
	}

	// Inbetween cases
	// m+1 vertices
	// 2*m triangles
	for (unsigned char i = 1; i < n; i++) {
		for (unsigned char j = 0; j < m; j++) {

			vertexUVs[o++] = float(j) / m;
			vertexUVs[o++] = float(i) / n;

			vertexPoses[k++] = r * sin(i*pi / n) * cos(j*2.0*pi / m);
			vertexPoses[k++] = r * sin(i*pi / n) * sin(j*2.0*pi / m);
			vertexPoses[k++] = r * cos(i*pi / n);

			if (i != n - 1) {
				short a = m + (i - 1)*(m + 1) + j;
				short b = m + i * (m + 1) + j;

				indices[l++] = a;
				indices[l++] = b;
				indices[l++] = a + 1;

				indices[l++] = a + 1;
				indices[l++] = b;
				indices[l++] = b + 1;
			}
		}
		vertexUVs[o++] = 1.0;
		vertexUVs[o++] = float(i) / n;

		vertexPoses[k++] = r * sin(i*pi / n);
		vertexPoses[k++] = 0.0;
		vertexPoses[k++] = r * cos(i*pi / n);
	}

	// case i = n-1
	// m vertices
	// m triangles
	for (unsigned char j = 0; j < m; j++) {
		// index: (0, 1, 2) (0, 2, 3) .. (0, m-1, m) (0, m, 1)
		// (m + (n - 2)*(m + 1) + j, m + (n - 1)*(m + 1) + j, m + (n - 2)*(m + 1) + j + 1)
		unsigned short a = m + (n - 2)*(m + 1) + j;
		indices[l++] = a;
		indices[l++] = a + m + 1; // m + (n - 1)*(m + 1) + j;
		indices[l++] = a + 1;


		vertexUVs[o++] = float(j) / m;
		vertexUVs[o++] = 1.0;

		vertexPoses[k++] = 0.0;
		vertexPoses[k++] = 0.0;
		vertexPoses[k++] = -r;
	}

	glGenVertexArrays(1, vaoID);
	glBindVertexArray(*vaoID);

	unsigned int vboIDs[4];
	glGenBuffers(4, vboIDs); // indices, positions, normals, UVs

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, *indexCount*sizeof(unsigned short), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount*sizeof(float), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 2 * vertexCount*sizeof(float), vertexUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	delete[] indices;
	delete[] vertexPoses;
	delete[] vertexUVs;

}

void initVAOSphere_MinimalVertexCount(unsigned int * vaoID, unsigned int * indexCount, unsigned char n, unsigned char m, float r)
{
	// This function is untested and may have some bugs

	int numVerts = 2 + (n - 1)*m;
	int floatCount = 3 * numVerts;

	int numTris = 2 * m + 2 * (n - 2)*m;
	*indexCount = 3 * numTris;

	unsigned short* indices = new unsigned short[*indexCount];
	float* vertexPoses = new float[floatCount];
	float* vertexNormals = new float[floatCount];

	float pi = 3.141592653589793238462;
	int k = 0; // vertex primitive index
	int l = 0; // index primitive index

	float x, y, z;

	// case i = 0
	// one vertex

	vertexPoses[k] = 0.0;
	vertexNormals[k++] = 0.0;

	vertexPoses[k] = 0.0;
	vertexNormals[k++] = 0.0;

	vertexPoses[k] = r;
	vertexNormals[k++] = 0.0;

	// m triangles
	for (int j = 0; j < m; j++) {
		// index: (0, 1, 2) (0, 2, 3) .. (0, m-1, m) (0, m, 1)
		// (0, (j+1), (j+1)%m + 1)
		indices[l++] = 0;
		indices[l++] = j + 1;
		indices[l++] = (j + 1) % m + 1;
	}

	for (unsigned char i = 1; i < n; i++) {
		for (unsigned char j = 0; j < m; j++) {


			x = sin(i*pi / n) * cos(j*2.0*pi / m);
			y = sin(i*pi / n) * sin(j*2.0*pi / m);
			z = cos(i*pi / n);


			vertexPoses[k] = r * x;
			vertexNormals[k] = x;

			vertexPoses[k] = r * y;
			vertexNormals[k] = y;

			vertexPoses[k] = r * z;
			vertexNormals[k] = z;



			// index: 
			// a = 1+(i-1)*m + j
			// b = (1+j)%m + (i-1)*m + 1

			// (a, a + m, b) (b, a + m, b + m)
			if (i != n - 1) {
				int a = 1 + (i - 1)*m + j;
				int b = (1 + j) % m + (i - 1)*m + 1;

				indices[l++] = a;
				indices[l++] = a + m;
				indices[l++] = b;

				indices[l++] = b;
				indices[l++] = a + m;
				indices[l++] = b + m;
			}
		}
	}

	// case i = n-1
	vertexPoses[k] = 0.0;
	vertexNormals[k++] = 0.0;

	vertexPoses[k] = 0.0;
	vertexNormals[k++] = 0.0;

	vertexPoses[k] = -r;
	vertexNormals[k++] = -1.0;

	for (unsigned char j = 0; j < m; j++) {
		// index: (0, 1, 2) (0, 2, 3) .. (0, m-1, m) (0, m, 1)
		// ((n-2)*m + j + 1, (n-1)*m + 1, (n-2)*m + (j+1)%m + 1)
		indices[l++] = (n - 2)*m + j + 1;
		indices[l++] = (n - 1)*m + 1;
		indices[l++] = (n - 2)*m + (j + 1) % m + 1;
	}

	glGenVertexArrays(1, vaoID);
	glBindVertexArray(*vaoID);

	unsigned int vboIDs[3];
	glGenBuffers(3, vboIDs); // indices, positions, normals

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, *indexCount*sizeof(unsigned short), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, floatCount*sizeof(float), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, floatCount*sizeof(float), vertexNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	delete[] indices;
	delete[] vertexPoses;
	delete[] vertexNormals;
}