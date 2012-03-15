

// Texture
uniform sampler2D uTexColor;

// Values needed by frag
varying vec2 vTexCoord;

// Deferred Values
varying vec3 vPosition;
varying vec3 vNormal;

void main()
{
    gl_FragData[0] = texture2D(uTexColor, vTexCoord);
    gl_FragData[1] = vec4(vPosition, 1.0);
    gl_FragData[2] = vec4(vNormal/2.0 + 0.5, 1.0);
}
