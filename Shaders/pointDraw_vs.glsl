#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_gpu_shader5 : enable

attribute uint VertexAttrib;
attribute vec3 VertexPosition;
attribute uint VertexColour;
uniform mat4 MVP;

uniform float PointSize;
uniform int Magenta;
uniform int Demo;
uniform vec3 NodeOffset;

uniform int ColorByElevation;
uniform float NormMinD;
uniform float NormMaxD;
uniform vec3 PlaneNorm;
uniform float VolFNormals[18];
uniform float VolFDistance[6];

uniform vec2 CurveColorCoordA;
uniform vec2 CurveColorCoordB;
uniform vec3 ColorCurveLineSlopes;


bool pointInsideVolume(vec3 point)
{
	int p = 0;
	for(p = 0; p < 6; ++p) {
		if( VolFNormals[3*p] * point.x + VolFNormals[3*p + 1] * point.y + VolFNormals[3*p + 2] * point.z - VolFDistance[p] <= 0 ){
			return false;
		}
	}
	return true;
}

float adjustColorFromCurves(float origColor)
{

	bool inX = bool(step(origColor, CurveColorCoordA.x));
	bool inY = bool(step(origColor, CurveColorCoordB.x));
	bool inZ = bool(step(origColor, 1.0f));

	// Performs the same task as multiple if statemets below. Optimised for continuous pipeline flow

	return float((inX && inY )) * origColor * ColorCurveLineSlopes.x + float((!inX && inY)) * ( (origColor - CurveColorCoordA.x) * ColorCurveLineSlopes.y + (1 - CurveColorCoordA.y) ) + float((!inY && inZ)) * ((origColor - CurveColorCoordB.x) * ColorCurveLineSlopes.z + (1 - CurveColorCoordB.y));


	//float adjustedColor = 0;
	//if(origColor < CurveColorCoordA.x) {
	//	return adjustedColor = origColor * ColorCurveLineSlopes.x;
	//}
	//if(origColor > CurveColorCoordA.x && origColor < CurveColorCoordB.x) {
	//	return adjustedColor = (origColor - CurveColorCoordA.x) * ColorCurveLineSlopes.y + (1 - CurveColorCoordA.y);
	//}
	//if(origColor > CurveColorCoordB.x) {
	//	return adjustedColor = (origColor - CurveColorCoordB.x) * ColorCurveLineSlopes.z + (1 - CurveColorCoordB.y);
	//}
	//return 0.0;
}

float modifyRed(float linearRed)
{
	float modifiedRed = 0;

	if(linearRed < 0.5) {
		modifiedRed = linearRed * 0.625f;
	}
	else {
		modifiedRed = linearRed * 1.6f;
	}

	return modifiedRed;
}

float colormap_red(float x) {
         if (x < 100.0) {
                 return (-9.55123422981038E-02 * x + 5.86981763554179E+00) * x - 3.13964093701986E+00;
         }
         else {
                 return 5.25591836734694E+00 * x - 8.32322857142857E+02;
         }
}

float colormap_green(float x) {
         if (x < 150.0) {
                 return 5.24448979591837E+00 * x - 3.20842448979592E+02;
         }
         else {
                 return -5.25673469387755E+00 * x + 1.34195877551020E+03;
         }
}

float colormap_blue(float x) {
         if (x < 80.0) {
                 return 4.59774436090226E+00 * x - 2.26315789473684E+00;
         }
         else {
                 return -5.25112244897959E+00 * x + 8.30385102040816E+02;
         }
}

vec4 colormap(float x) {
         float t = x * 255.0;
         float r = clamp(colormap_red(t) / 255.0, 0.0, 1.0);
         float g = clamp(colormap_green(t) / 255.0, 0.0, 1.0);
         float b = clamp(colormap_blue(t) / 255.0, 0.0, 1.0);
         return vec4(r, g, b, 1.0);
}


void main()
{
	float alfa = 1.0;
	// passing over ProjMat * ModMat since gl_modellViewProjectionMatrix  is depreciated
		vec3 vp = VertexPosition + NodeOffset;
	gl_Position = MVP * vec4(vp, 1.0);

	gl_ClipVertex = gl_ModelViewMatrix * vec4(vp, 1.0);
	gl_PointSize = PointSize;
		// check the clash bit on the attrib
		uint VertexAttribIsolated = VertexAttrib;
	if ((VertexAttribIsolated & 0x00000008) == 8) {
		gl_PointSize = 5.0;
		VertexAttribIsolated &= 0xFFFFFFF7;
	}
	// unpack a rgba unsigned int  to 4 floats
		vec4 Color = unpackUnorm4x8(VertexColour);
	if ((Magenta == 1) && (VertexAttribIsolated != 0)) {
		Color.r = clamp(0.666666 * (0.5 + Color.r), 0.0, 1.0);
		Color.b = clamp(0.666666 * (0.5 + Color.b), 0.0, 1.0);
		gl_FrontColor = vec4(Color.r, 0.0, Color.b, alfa);
	}
	else if (VertexAttribIsolated == 4) {
		Color.r = clamp(0.666666 * (0.5 + Color.r), 0.0, 1.0);
		gl_FrontColor = vec4(Color.r, 0.0, 0.0, alfa);
	}
	else if (VertexAttribIsolated == 0) {
		gl_FrontColor = vec4(Color.rgb, 0.0);
	}
	else if ((Demo != 1) && ((VertexAttribIsolated & 0x20) == 0x20)) {
		gl_FrontColor = vec4(Color.rgb, 1.0);
	}
	else {
		gl_FrontColor = vec4(Color.rgba);
	}

	if(ColorByElevation != 0) {
		if(pointInsideVolume(vp)) {
			float value = dot(PlaneNorm, VertexPosition);
			float redC = (value - NormMinD) / (NormMaxD - NormMinD);

			redC = adjustColorFromCurves(redC);

			float modifiedRed = modifyRed(redC);
			gl_FrontColor = colormap(redC);

			float delta = abs(VertexPosition.z*2 - int (VertexPosition.z *2));
			if(delta < 0.1) {
				gl_FrontColor = vec4(0.0, 0.0, 0.0, 1.0);
			}
		}
	}
}
