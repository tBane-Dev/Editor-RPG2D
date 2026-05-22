uniform vec2 rectPos;
uniform vec2 rectSize;

void main()
{
    // piksel względem prostokąta
    vec2 local = gl_FragCoord.xy - vec2(rectPos.x, 1.0-rectPos.y);

    // szachownica
    float checkerSize = 8.0;
    float cx = mod(floor(local.x / checkerSize), 2.0);
    float cy = mod(floor(local.y / checkerSize), 2.0);
    float checker = mod(cx + cy, 2.0);

    vec3 checkerColor = mix(vec3(0.35), vec3(0.25), checker);

    gl_FragColor = vec4(checkerColor, 1.0);
}