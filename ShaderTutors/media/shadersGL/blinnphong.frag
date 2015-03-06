
#version 150

in vec3 wnorm;
in vec3 vdir;
in vec3 ldir;

out vec4 outColor;

void main()
{
	vec3 n = normalize(wnorm);
	vec3 l = normalize(ldir);
	vec3 v = normalize(vdir);
	vec3 h = normalize(v + l);

	float d = clamp(dot(l, n), 0.0, 1.0);
	float s = clamp(dot(h, n), 0.0, 1.0);

	s = pow(s, 80.0);

	outColor.rgb = vec3(d, d, d) + vec3(s, s, s);
	outColor.a = 1.0;
}
