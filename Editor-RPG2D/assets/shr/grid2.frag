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
    vec2 localPos = floor(worldPos - rectPosition);

    vec2 m = vec2(gridWidth * 0.5);

    vec4 c = vec4(0.0);

    // Mały grid - zostaje jak był
    vec2 grid = mod(localPos + m, gridSize);

    if (grid.x < gridWidth || grid.y < gridWidth)
        c = gridSecondColor;

    // Główny grid
    float mainGridWidth = gridWidth * mainGridMultiplier;
    float halfMainGridWidth = mainGridWidth * 0.5;

    // Wewnętrzne grube linie - wyśrodkowane na chunkSize
    vec2 mainGrid = mod(localPos, chunkSize);
    vec2 mainGridDistance = min(mainGrid, chunkSize - mainGrid);

    bool innerMainGridX =
        mainGridDistance.x < halfMainGridWidth &&
        localPos.x > halfMainGridWidth &&
        localPos.x < rectSize.x - halfMainGridWidth;

    bool innerMainGridY =
        mainGridDistance.y < halfMainGridWidth &&
        localPos.y > halfMainGridWidth &&
        localPos.y < rectSize.y - halfMainGridWidth;

    // Zewnętrzne grube linie - zostają jak były
    bool outerMainGrid =
        localPos.x < mainGridWidth ||
        localPos.y < mainGridWidth ||
        localPos.x > rectSize.x - mainGridWidth ||
        localPos.y > rectSize.y - mainGridWidth;

    if (innerMainGridX || innerMainGridY || outerMainGrid)
    {
        c = gridMainColor;
    }

    gl_FragColor = c;
}