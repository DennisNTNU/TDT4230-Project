#version 430 core
#pragma optionNV(unroll all)

in vec3 fragmentPosition;

uniform layout(location = 3) vec3 camPos;
uniform layout(location = 4) vec3 sunPos;
uniform layout(location = 5) vec3 earthPos;
uniform layout(location = 6) float earthRadius;
uniform layout(location = 7) float atmosphereRadius;

out vec4 color;

bool intersections(float p, float q);

void main()
{
	float earthRadius_2 = earthRadius*earthRadius;
	float atmosphereRadius_2 = atmosphereRadius*atmosphereRadius;

	vec3 viewDir = normalize(fragmentPosition - camPos);

	float earthCamDistance = distance(camPos, earthPos);
	float earthFragDistance = distance(fragmentPosition, earthPos);

	float p = 2.0*dot(camPos - earthPos, viewDir);
	float q1 = pow(earthCamDistance, 2) - earthRadius_2;
	float q2 = pow(earthCamDistance, 2) - atmosphereRadius_2;

	float fragmentOverEarth = 0.0;

	if (intersections(p, q2) && (earthFragDistance > earthRadius)) 
	{	// AT LEAST ONE POSITIVE INTERSECTION WITH THE ATMOSPHERE
	
		float tmin = max(0.0, -p/2 - sqrt(p*p/4.0 - q2));
		float tmax = 1.0e+15;


		if (intersections(p, q1))
		{	// AT LEAST ONE POSITIVE INTERSECTION WITH THE EARTH
			tmax = -p/2 - sqrt(p*p/4.0 - q1);
			fragmentOverEarth = 1.0;
		}
		else
		{	// NO INTERSECTIONS WITH THE EARTH, VIEW DIR POINTS THROUGH ATMOSPHERE INTO SPACE
			tmax = -p/2 + sqrt(p*p/4.0 - q2);
		}

		float delta = tmax - tmin;
		vec3 scatteringCoefficient = vec3(5.8e-6, 13.5e-6, 33.1e-6);

		vec3 p_i;
		const int viewRaySamples = 70;

		vec4 col;
		vec4 prevCol;
		vec3 sunDir;
		float samplePointAltitude;
		float prevSamplePointAltitude;

		float lineInAtmosphereDist;

		for (int i = viewRaySamples; i > 0; i--)
		{
			p_i = camPos + (tmin + i * delta / float(viewRaySamples)) * viewDir;
			sunDir = normalize(sunPos - p_i);

			if (i == viewRaySamples) 
			{
				lineInAtmosphereDist = distance(p_i, camPos + tmin*viewDir);
			}

			float p  = 2.0 * dot(p_i - earthPos, sunDir);
			float earthToRaySamplePointDist = distance(p_i, earthPos);
			float q1 = pow(earthToRaySamplePointDist, 2) - earthRadius_2*0.95;

			samplePointAltitude = earthToRaySamplePointDist - earthRadius;

			float D = p * p / 4.0 - q1;

			if ( (D < 0.0) || ( (D >= 0.0) && ( (-p/2 + sqrt(D)) < 0.0) ) )
			{	// ray from viewRaySamplePoint to sun has no positive intersection with earth

				// r is parameter to traverse line from line of sight sample in sun direction
				float q2 = pow(earthToRaySamplePointDist, 2) - atmosphereRadius_2;
				float rmin = 0;
				float rmax = - p / 2 + sqrt(p * p / 4.0 - q2);
				float delta_r = rmax - rmin;

				vec3 s_m = p_i + rmax * sunDir;

				float lineToSunInAtmosphereDist = distance(p_i, s_m);

				vec3 chrom = vec3(0.175*exp(-lineToSunInAtmosphereDist/1000.0), 0.408*exp(-lineToSunInAtmosphereDist/500.0), 0.9*exp(-lineToSunInAtmosphereDist/300.0));
				float maximum = max(chrom.x, max(chrom.y, chrom.z));
	
				if (chrom.x > chrom.y) {				
					maximum /= exp(-lineToSunInAtmosphereDist/3000.0);
				}

				col = vec4(chrom/maximum, exp(-samplePointAltitude / 80.0));
			}
			else
			{ 	// ray from viewRaySamplePoint to sun has a positive intersection with earth
				// which means sample point lies in earth shadow / night side of atmosphere

				col = vec4(0.0, 0.0, 0.0, 0.0);
			}

			if (i != viewRaySamples) {
				prevCol = mix(prevCol, col, exp(-prevSamplePointAltitude / 80.0));
			}else{
				prevCol = col;
			}
			prevSamplePointAltitude = samplePointAltitude;
		}

		
		float lookUpFactor_alpha = acos(max(0.0, dot(viewDir, normalize(camPos))))/3.14159265358979;
		float lookUpFactor_color = acos(pow(max(0.0, dot(viewDir, normalize(camPos))), 0.6))/3.14159265358979;

		prevCol.a *= 10*(1.0 - exp(-lineInAtmosphereDist/2000));
		prevCol.a *= 2.0*(1.0 - lookUpFactor_alpha);
		prevCol.xyz *= 1.0 + 0.5*lookUpFactor_color;


		color = prevCol;


	}
	else
	{
		// VIEW DIR POINTS INTO SPACE
		color = vec4(0.0, 0.0, 0.0, 0.0);
	}

	
}

bool intersections(float p, float q)
{
	float D = (p*p / 4.0 - q);
	if (D < 0.0)
	{
		// No intersection
		return false;
	}
	if (-p/2 + sqrt(D) < 0.0)
	{
		// No positive intersection
		return false;
	}
	
	// At least one positive intersection
	return true;
}




