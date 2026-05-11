// palette_lake.frag
uniform sampler2D texture;
uniform vec2 resolution;
uniform float time;
varying vec2 worldPos;
uniform vec2 startPos;



// OCEAN /////////////////////////////////////////////////////////////////////////////////////////////////////////
float oceanRandom(float x) {
 
    return fract(sin(x) * 10000.);
          
}

float oceanNoise(vec2 p) {

    return oceanRandom(p.x + p.y * 10000.);
            
}

vec2 sw(vec2 p) { return vec2(floor(p.x), floor(p.y)); }
vec2 se(vec2 p) { return vec2(ceil(p.x), floor(p.y)); }
vec2 nw(vec2 p) { return vec2(floor(p.x), ceil(p.y)); }
vec2 ne(vec2 p) { return vec2(ceil(p.x), ceil(p.y)); }

float oceanSmoothNoise(vec2 p) {

    vec2 interp = smoothstep(0., 1., fract(p));
    float s = mix(oceanNoise(sw(p)), oceanNoise(se(p)), interp.x);
    float n = mix(oceanNoise(nw(p)), oceanNoise(ne(p)), interp.x);
    return mix(s, n, interp.y);
        
}

float oceanFractalNoise(vec2 p) {

    float x = 0.;
    x += oceanSmoothNoise(p      );
    x += oceanSmoothNoise(p * 2. ) / 2.;
    x += oceanSmoothNoise(p * 4. ) / 4.;
    x += oceanSmoothNoise(p * 8. ) / 8.;
    x += oceanSmoothNoise(p * 16.) / 16.;

    x /= 1. + 1./2. + 1./4. + 1./8. + 1./16.;
    return x;
            
}

float oceanMovingNoise(vec2 p) {
 
    float x = oceanFractalNoise(p + time);
    float y = oceanFractalNoise(p - time);
    return oceanFractalNoise(p + vec2(x, y));   
    
}

float oceanNestedNoise(vec2 p) {
    
    float x = oceanMovingNoise(p);
    float y = oceanMovingNoise(p + 100.);
    return oceanMovingNoise(p + vec2(x, y));
    
}

// BOG //////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 permute(vec4 x) {
  return mod((34.0 * x + 1.0) * x, 289.0);
}

vec3 permute(vec3 x) {
  return mod((34.0 * x + 1.0) * x, 289.0);
}

vec2 cellular2x2x2(vec3 P) {
	#define K 0.142857142857
	#define Ko 0.428571428571
	#define K2 0.020408163265306
	#define Kz 0.166666666667
	#define Kzo 0.416666666667
	#define jitter 0.8
	vec3 Pi = mod(floor(P), 289.0);
 	vec3 Pf = fract(P);
	vec4 Pfx = Pf.x + vec4(0.0, -1.0, 0.0, -1.0);
	vec4 Pfy = Pf.y + vec4(0.0, 0.0, -1.0, -1.0);
	vec4 p = permute(Pi.x + vec4(0.0, 1.0, 0.0, 1.0));
	p = permute(p + Pi.y + vec4(0.0, 0.0, 1.0, 1.0));
	vec4 p1 = permute(p + Pi.z);
	vec4 p2 = permute(p + Pi.z + vec4(1.0));
	vec4 ox1 = fract(p1*K) - Ko;
	vec4 oy1 = mod(floor(p1*K), 7.0)*K - Ko;
	vec4 oz1 = floor(p1*K2)*Kz - Kzo;
	vec4 ox2 = fract(p2*K) - Ko;
	vec4 oy2 = mod(floor(p2*K), 7.0)*K - Ko;
	vec4 oz2 = floor(p2*K2)*Kz - Kzo;
	vec4 dx1 = Pfx + jitter*ox1;
	vec4 dy1 = Pfy + jitter*oy1;
	vec4 dz1 = Pf.z + jitter*oz1;
	vec4 dx2 = Pfx + jitter*ox2;
	vec4 dy2 = Pfy + jitter*oy2;
	vec4 dz2 = Pf.z - 1.0 + jitter*oz2;
	vec4 d1 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
	vec4 d2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;

#if 0
	d1 = min(d1, d2);
	d1.xy = min(d1.xy, d1.wz);
	d1.x = min(d1.x, d1.y);
	return sqrt(d1.xx);
#else
	vec4 d = min(d1,d2);
	d2 = max(d1,d2);
	d.xy = (d.x < d.y) ? d.xy : d.yx;
	d.xz = (d.x < d.z) ? d.xz : d.zx;
	d.xw = (d.x < d.w) ? d.xw : d.wx;
	d.yzw = min(d.yzw, d2.yzw);
	d.y = min(d.y, d.z);
	d.y = min(d.y, d.w);
	d.y = min(d.y, d2.x);
	return sqrt(d.xy);
#endif
}


// LAVA //////////////////////////////////////////////

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

float lavaNoise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ), 
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ), 
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

vec3 lavaFunc(vec3 color, vec2 uv, float detail, float power,
              float colorMul, float glowRate, bool animate, float noiseAmount)
{
    vec3 rockColor = vec3(0.09 + abs(sin(time * .75)) * .03, 0.02, .02);
    float minDistance = 1.;
    uv *= detail;
    uv *= 10.0;

    vec2 cell = floor(uv);
    vec2 frac = fract(uv);
    
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
        	vec2 cellDir = vec2(float(i), float(j));
            vec2 randPoint = random2(cell + cellDir);
            randPoint += noise(uv) * noiseAmount;
            randPoint = animate ? 0.5 + 0.5 * sin(time * .35 + 6.2831 * randPoint) : randPoint;
            minDistance = min(minDistance, length(cellDir + randPoint - frac));
        }
    }
    	
    float powAdd = sin(uv.x * 2. + time * glowRate) + sin(uv.y * 2. + time * glowRate);
    vec3 outColor = vec3(color * pow(minDistance, power + powAdd * .95) * colorMul);
    outColor.rgb = mix(rockColor, outColor.rgb, minDistance);
    return outColor;
}

// MAIN /////////////////////////////////////////////////////////////////////////////////////////////////////////
void main()
{
    vec4 texcolor = texture2D(texture, gl_TexCoord[0].xy);

    if(texcolor.x == 50./255. && texcolor.y == 61./255. && texcolor.z == 186./255. && texcolor.w == 1.0) {

        vec2 coords = (worldPos.xy-startPos)/resolution;
        float pixelationFactor = 0.00390625;
        vec2 pixelCoord = floor(coords / pixelationFactor) * pixelationFactor;
        float n = oceanNestedNoise(pixelCoord * 25.60);
        vec3 color = mix(vec3(.4, .6, 1.), vec3(.05, .1, 1.), n);
        vec3 darkcolor = color * 0.7;
        gl_FragColor = vec4(darkcolor, 1.) * gl_Color;

    } else if (texcolor.x == 13./255. && texcolor.y == 69./255. && texcolor.z == 30./255. && texcolor.w == 1.0) {

        float pixelSize = 2.0;
        vec2 pixelatedWorldPos = floor(worldPos / pixelSize) * pixelSize;
        vec2 st = (2.0 * (pixelatedWorldPos - startPos) - resolution.xy) / resolution.y;
        st.y = st.y * 2.0;
        st *= 10.0;
        vec2 F = cellular2x2x2(vec3(st, time / 8.0));
        float n = smoothstep(0.4, 0.5, F.x);
        vec4 final_color;
        if(n < 0.6)
            final_color = vec4(0.15, 0.35, 0.15, 1.0);
        else
            final_color = vec4(0.15*n, 0.25*n, 0.15*n, 1.0);

        gl_FragColor = final_color;

    } else if (texcolor.x == 237./255. && texcolor.y == 28./255. && texcolor.z == 36./255. && texcolor.w == 1.0) {

        vec2 uv = (worldPos.xy-startPos)/resolution;
    	uv.x *= resolution.x / resolution.y;
        float pixelSize = 4.0;
      	vec2 grid = pixelSize / resolution;
        uv = floor(uv/grid) * grid;
    	vec4 fragColor = vec4(0.);
        float addPower = -1.5;
    	fragColor.rgb += lavaFunc(vec3(1.5, .45, 0.), uv*2, 3.,  2.5+addPower, 1.15, 1.5, false, 1.5);
    	fragColor.rgb += lavaFunc(vec3(1.5, 0., 0.), uv*2, 6., 3.+addPower, .4, 1., false, 0.);
    	fragColor.rgb += lavaFunc(vec3(1.2, .4, 0.), uv*2, 8., 4.+addPower, .2, 1.9, true, 0.5);
    	gl_FragColor = vec4(fragColor.rgb, 1.0);

    } else {
        gl_FragColor = texcolor * gl_Color;
    }
}