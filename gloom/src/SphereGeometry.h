#pragma once

#include <glad/glad.h>
#include <math.h>

void initVAOSphere(unsigned int * vaoID, unsigned int * indexCount, unsigned char n, unsigned char m, float r);

void initVAOSphereNoNormals(unsigned int * vaoID, unsigned int * indexCount, unsigned char n, unsigned char m, float r);

void initVAOSphere_MinimalVertexCount(unsigned int * vaoID, unsigned int * indexCount, unsigned char n, unsigned char m, float r);