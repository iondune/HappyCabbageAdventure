varying vec3 vColor;

varying vec4 vCutoff;
varying vec4 vStart;

varying vec4 gPosition;

void main()
{
    if (gPosition.x < vStart.x)
        discard;//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else if (gPosition.x > vCutoff.x)
        discard;//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
