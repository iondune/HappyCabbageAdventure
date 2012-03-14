

// Texture
uniform sampler2D uTexColor;

// Values needed by frag
varying vec2 vTexCoord;

// Deferred Values
varying vec3 vPosition;
varying vec3 vNormal;

// Deferred Output
layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outPosition;
layout(location = 2) out vec3 outNormal;

void main()
{
    outColor = texture2D(uTexColor, vTexCoord).xyz;
    outPosition = vPosition;
    outNormal = vNormal/2.0 + 0.5;
}
