// grid.frag

varying vec2 worldPos;

uniform vec2 rectPosition;
uniform float gridSize;
uniform float chunkSize;
uniform float gridWidth;
uniform vec4 gridMainColor;
uniform vec4 gridSecondColor;

void main()
{
    vec2 localPos = worldPos - rectPosition;
    vec2 m = vec2(gridWidth/2.0, gridWidth/2.0);

    vec2 grid = mod(localPos+m, gridSize);
    vec4 c = vec4(0.0, 0.0, 0.0, 0.0);

    if (grid.x < gridWidth || grid.y < gridWidth)
        c = gridSecondColor;

    vec2 mainGrid = mod(localPos+2.0*m, gridSize * chunkSize);

    if (mainGrid.x < gridWidth*2.0 || mainGrid.y < gridWidth*2.0)
        c = gridMainColor;

    gl_FragColor = c;
}