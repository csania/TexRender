#version 430

out vec4 color;

in float fragPosY;

uniform int CurrentScheme;

float adjustColorFromCurves(float origColor)
{
	vec2 CurveColorCoordA = vec2(0.25f, 0.75f);
	vec2 CurveColorCoordB = vec2(0.75f, 0.25f);
	vec3 ColorCurveLineSlopes = vec3(1.0f, 1.0f, 1.0f);

	bool inX = bool(step(origColor, CurveColorCoordA.x));
	bool inY = bool(step(origColor, CurveColorCoordB.x));
	bool inZ = bool(step(origColor, 1.0f));

	return float((inX && inY )) * origColor * ColorCurveLineSlopes.x + float((!inX && inY)) * ( (origColor - CurveColorCoordA.x) * ColorCurveLineSlopes.y + (1 - CurveColorCoordA.y) ) + float((!inY && inZ)) * ((origColor - CurveColorCoordB.x) * ColorCurveLineSlopes.z + (1 - CurveColorCoordB.y));
}

float colormap_red_Rainbow(float x) {
         if (x < 100.0) {
                 return (-9.55123422981038E-02 * x + 5.86981763554179E+00) * x - 3.13964093701986E+00;
         }
         else {
                 return 5.25591836734694E+00 * x - 8.32322857142857E+02;
         }
}

float colormap_green_Rainbow(float x) {
         if (x < 150.0) {
                 return 5.24448979591837E+00 * x - 3.20842448979592E+02;
         }
         else {
                 return -5.25673469387755E+00 * x + 1.34195877551020E+03;
         }
}

float colormap_blue_Rainbow(float x) {
         if (x < 80.0) {
                 return 4.59774436090226E+00 * x - 2.26315789473684E+00;
         }
         else {
                 return -5.25112244897959E+00 * x + 8.30385102040816E+02;
         }
}

vec4 colormapRainbow(float x) {
         float t = x * 255.0;
         float r = clamp(colormap_red_Rainbow(t) / 255.0, 0.0, 1.0);
         float g = clamp(colormap_green_Rainbow(t) / 255.0, 0.0, 1.0);
         float b = clamp(colormap_blue_Rainbow(t) / 255.0, 0.0, 1.0);
         return vec4(r, g, b, 1.0);
}


float colormap_red_Bone(float x) {
    if (x < 0.75) {
        return 8.0 / 9.0 * x - (13.0 + 8.0 / 9.0) / 1000.0;
    } else {
        return (13.0 + 8.0 / 9.0) / 10.0 * x - (3.0 + 8.0 / 9.0) / 10.0;
    }
}

float colormap_green_Bone(float x) {
    if (x <= 0.375) {
        return 8.0 / 9.0 * x - (13.0 + 8.0 / 9.0) / 1000.0;
    } else if (x <= 0.75) {
        return (1.0 + 2.0 / 9.0) * x - (13.0 + 8.0 / 9.0) / 100.0;
    } else {
        return 8.0 / 9.0 * x + 1.0 / 9.0;
    }
}

float colormap_blue_Bone(float x) {
    if (x <= 0.375) {
        return (1.0 + 2.0 / 9.0) * x - (13.0 + 8.0 / 9.0) / 1000.0;
    } else {
        return 8.0 / 9.0 * x + 1.0 / 9.0;
    }
}

vec4 colormapBone(float x) {
    float r = clamp(colormap_red_Bone(x), 0.0, 1.0);
    float g = clamp(colormap_green_Bone(x), 0.0, 1.0);
    float b = clamp(colormap_blue_Bone(x), 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}

float colormap_red_Copper(float x) {
    return 80.0 / 63.0 * x + 5.0 / 252.0;
}

float colormap_green_Copper(float x) {
    return 0.7936 * x - 0.0124;
}

float colormap_blue_Copper(float x) {
    return 796.0 / 1575.0 * x + 199.0 / 25200.0;
}

vec4 colormapCopper(float x) {
    float r = clamp(colormap_red_Copper(x), 0.0, 1.0);
    float g = clamp(colormap_green_Copper(x), 0.0, 1.0);
    float b = clamp(colormap_blue_Copper(x), 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}

vec4 colormapHot(float x) {
    float r = clamp(8.0 / 3.0 * x, 0.0, 1.0);
    float g = clamp(8.0 / 3.0 * x - 1.0, 0.0, 1.0);
    float b = clamp(4.0 * x - 3.0, 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}

void main()
{
	float NormMinD = -0.35;
	float NormMaxD = -0.70;


	float redC1 = (fragPosY - NormMinD) / (NormMaxD - NormMinD);
	float redC = adjustColorFromCurves (redC1);


	int SelectedScheme = CurrentScheme;
	if(SelectedScheme == 0) {
		color = colormapRainbow(redC);
	}
	if(SelectedScheme == 1) {
		color = colormapBone(redC);
	}
	if(SelectedScheme == 2) {
		color = colormapCopper(redC);
	}
	if(SelectedScheme == 3) {
		color = colormapHot(redC);
	}

}