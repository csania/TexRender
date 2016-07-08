#version 430

in layout(location = 0) vec2 position;

out float fragPosY;

void main()
{
	fragPosY = position.y;
	gl_Position = vec4(position, 0.0, 1.0);

}

/*
precision mediump float;

attribute vec2 aPosition;
attribute vec2 aUv;

uniform vec2 offsetPosition;

varying vec2 vUv;

void main(void) {
	vUv = aUv;
	gl_Position  = vec4(aPosition + offsetPosition, 0.0, 1.0);
}



precision mediump float;

uniform sampler2D texture;
uniform sampler2D perlin1;
uniform sampler2D perlin2;

uniform vec2 centerUv;
uniform vec2 strength;
uniform float alpha;

uniform vec2 offset1;
uniform vec2 offset2;
uniform mat2 rotate;

varying vec2 vUv;

void main(void) {
	vec2 uv1 = vUv * 4.0;
	vec2 uv2 = vUv * 4.0;
	vec2 center = vec2(0.5, 0.5);

	uv1 -= center;
	uv2 -= center;
	uv1 = rotate * uv1;
	uv2 = rotate * uv2;
	uv1 += center;
	uv2 += center;
	uv1 += offset1;
	uv2 += offset2;

	vec2 str = rotate * strength;

	vec4 rgba1 = texture2D(perlin1, uv1) - 0.5;
	vec4 rgba2 = texture2D(perlin2, uv2) - 0.5;

	uv1 = vec2(rgba1.x, rgba1.y) * str;
	uv2 = vec2(rgba2.x, rgba2.y) * str;

	vec2 uv = vUv + uv1 + uv2;

	vec2 minUv = vec2(0, 0);
	vec2 maxUv = vec2(0.25, 0.25);

	if (uv.x > maxUv.x) {
		uv.x = maxUv.x;
	} else if (uv.x < minUv.x) {
		uv.x = minUv.x;
	}
	if (uv.y > maxUv.y) {
		uv.y = maxUv.y;
	} else if (uv.y < minUv.y) {
		uv.y = minUv.y;
	}

	uv += centerUv - 0.125;

	vec4 color = texture2D(texture, uv);
	color.w = color.w*alpha;
	gl_FragColor = color;
}
	*/