#include <stddef.h>
#include "pixel.h"
#include "tmath.h"

Px_rgba hsva2rgba(Px_hsva hsva)
{
	float h = hsva.h;
	float s = hsva.s;
	float v = hsva.v;

	int l = h / 60; // l = floor(h/60)
	float m = h /60.0 - l;

	float d = v * (1-s);
	float e = v * (1 - m*s);
	float f = v * (1 - (1-m)*s);
	
	float r = 0, g = 0, b = 0;
	switch (l) {
	case 0:
		r = v, g = f, b = d;
		break;
	case 1:
		r = e, g = v, b = d;
		break;
	case 2:
		r = d, g = v, b = f;
		break;
	case 3:
		r = d, g = e, b = v;
		break;
	case 4:
		r = f, g = d, b = v;
		break;
	case 5:
		r = v, g = d, b = e;
		break;
	}

	Px_def rgba = {r * 255, g * 255, b * 255, hsva.a * 255};
	return rgba;
}

Px_hsva rgba2hsva(Px_rgba rgba)
{
	float r = rgba.r / 255.0f;
	float g = rgba.g / 255.0f;
	float b = rgba.b / 255.0f;

	float mini = min3(r, g, b);
	float maxi = max3(r, g, b);
	float diff = maxi - mini;

	float v = maxi;
	float s = (v != 0) ? diff / v : 0;
	float h = 0;

	if (diff == 0)
		h = 0;
	else if (v == r)
		h =   0 + 60 * (g-b) / diff;
	else if (v == g)
		h = 120 + 60 * (b-r) / diff;
	else // v == b
		h = 240 + 60 * (r-g) / diff;
	
	if (h < 0.0)
		h += 360;
	
	Px_hsva hsva = {h, s, v, rgba.a / 255.0};
	return hsva;
}
