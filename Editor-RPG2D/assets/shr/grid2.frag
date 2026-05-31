// grid2.frag
varying vec2 worldPos;

uniform vec2 rectPosition;
uniform vec2 rectSize;

uniform vec2 gridSize;
uniform vec2 chunkSize;
uniform float gridWidth;
uniform float mainGridMultiplier;
uniform vec4 gridMainColor;
uniform vec4 gridSecondColor;

void main()
{
    vec2 localPos = worldPos - rectPosition;
    vec2 m = vec2(gridWidth * 0.5);

    vec4 c = vec4(0.0);

    // Mały grid
    vec2 grid = mod(localPos + m, gridSize);

    if (grid.x < gridWidth || grid.y < gridWidth)
        c = gridSecondColor;

    // Główny grid
    vec2 mainGrid = mod(localPos + m, chunkSize);

    float mainGridWidth = gridWidth * mainGridMultiplier;

    if (mainGrid.x < mainGridWidth ||
        mainGrid.y < mainGridWidth ||
        localPos.x < mainGridWidth ||
        localPos.y < mainGridWidth ||
        localPos.x > rectSize.x - mainGridWidth ||
        localPos.y > rectSize.y - mainGridWidth)
    {
        c = gridMainColor;
    }

    gl_FragColor = c;
}