varying vec3 vColor;
varying vec2 vTexCoord;

uniform sampler2D uTexColor;

void main()
{
	gl_FragColor = texture2D(uTexColor, vTexCoord);
}
