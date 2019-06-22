#version 440 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float _deltaTime;

//Texture resolution.
const vec2 _resolution    = vec2(1280 / 720);
//Frecuency of the scanline.
float _frequency    = 100 * _deltaTime;
//Auxiliar color varible.
const vec4  _color        = vec4(1.0, 1.0, 1.0, 1.0);
//Color offset amount.
float _glitchAmount = 2 * (1 - _deltaTime);

float random( vec2 p )
{
    vec2 K1 = vec2(
        23.14069263277926, // e^pi (Gelfond's constant)
         2.665144142690225 // 2^sqrt(2) (Gelfondâ€“Schneider constant)
    );
    return fract(cos(dot(p,K1)) * 12345.6789);
}

void main()
{
	//Catch texture coordinates to operate.
	vec2 tex = TexCoords;
	//Generate a random number to determine when apply some movement on the texture coordinates.
	float number = random(vec2(_deltaTime, 1));
	//Apply texture coordinates movement.
	if(number > 0.92)
	{
		//Texture coordinates movement.
		tex.x += sin(tex.y * 8 * number * 3.14159 + _deltaTime);
		tex.y += cos(tex.x * 8 * number * 3.14159 + _deltaTime);
	}
	//Create a new texture to apply colour offset.
	vec4 value = texture2D(screenTexture, tex);
	// Color offset glitch.
	value.r = texture2D(screenTexture, tex + vec2(_glitchAmount / _resolution.x, _deltaTime)).r;
	value.g = texture2D(screenTexture, tex + vec2(-_glitchAmount / _resolution.y, _deltaTime)).g;
	value.b = texture2D(screenTexture, tex + vec2(-_glitchAmount / _resolution.x, _deltaTime)).b;
	//Scanline
	value.rgb *= 0.8 + 0.2 * sin(_frequency * gl_FragCoord.y + _deltaTime);
	FragColor = _color * value;
} 