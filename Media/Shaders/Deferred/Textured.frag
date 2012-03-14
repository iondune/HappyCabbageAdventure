

// Texture
uniform sampler2D uTexColor;

// Values needed by frag
varying vec2 vTexCoord;

// Deferred Values
varying vec4 vPosition;
varying vec3 vNormal;

// Deferred Output
layout(location = 0) out vec3 outColor;
layout(location = 1) out float outDepth;
layout(location = 2) out vec3 outNormal;

void main()
{
    outColor = texture2D(uTexColor, vTexCoord).xyz;
    outDepth = (vPosition.z)/10.0;
    outNormal = vNormal/2.0 + 0.5;
}
