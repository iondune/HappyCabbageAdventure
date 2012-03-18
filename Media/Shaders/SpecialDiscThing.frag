varying vec3 vColor;

varying float vCutoff;
varying float vStart;

varying vec4 gPosition;

void main()
{
    if (gPosition.x < vStart)
        discard;//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else if (gPosition.x > vCutoff)
        discard;//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
