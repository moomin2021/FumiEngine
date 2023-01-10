#include "Easing.h"
#include <DirectXMath.h>

/// ‰ü•ÏŒãƒR[ƒh

#pragma region Back
float Easing::Back::easeIn(float start, float end, float t)
{
	float c1 = 1.70158;
	float c3 = c1 + 1.0f;
	float time = (c3 * t * t * t - c1 * t * t);
	return start * (1.0f - time) + end * time;
}

float Easing::Back::easeOut(float start, float end, float t)
{
	float c1 = 1.70158f;
	float c3 = c1 + 1.0f;
	float time = 1 + c3 * pow(t - 1, 3) + c1 * pow(t - 1, 2);
	return start * (1.0f - time) + end * time;
}

float Easing::Back::easeInOut(float start, float end, float t)
{
	float c1 = 1.70158f;
	float c2 = c1 * 1.525f;
	float time = 0.0f;
	if (t < 0.5f) time = (pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2;
	else time = (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
	return start * (1.0f - time) + end * time;
}
#pragma endregion

#pragma region Bounce
float Easing::Bounce::easeIn(float t, float b, float c, float d)
{
	return c - Easing::Bounce::easeOut(d - t, 0, c, d) + b;
}

float Easing::Bounce::easeOut(float t, float b, float c, float d)
{
	if ((t /= d) < (1 / 2.75f)) {
		return c * (7.5625f * t * t) + b;
	}
	else if (t < (2 / 2.75f)) {
		float postFix = t -= (1.5f / 2.75f);
		return c * (7.5625f * (postFix)*t + .75f) + b;
	}
	else if (t < (2.5 / 2.75)) {
		float postFix = t -= (2.25f / 2.75f);
		return c * (7.5625f * (postFix)*t + .9375f) + b;
	}
	else {
		float postFix = t -= (2.625f / 2.75f);
		return c * (7.5625f * (postFix)*t + .984375f) + b;
	}
}

float Easing::Bounce::easeInOut(float t, float b, float c, float d)
{
	if (t < d / 2) return Easing::Bounce::easeIn(t * 2, 0, c, d) * .5f + b;
	else return Easing::Bounce::easeOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}
#pragma endregion

#pragma region Circ
float Easing::Circ::easeIn(float start, float end, float t)
{
	float time = 1.0f - sqrt(1 - pow(t, 2));
	return start * (1.0f - time) + end * time;
}

float Easing::Circ::easeOut(float start, float end, float t)
{
	float time = sqrt(1 - pow(t - 1, 2));
	return start * (1.0f - time) + end * time;
}

float Easing::Circ::easeInOut(float start, float end, float t)
{
	float time;
	if (t < 0.5f) time = 16.0f * t * t * t * t * t;
	else time = 1.0f - pow(-2.0f * t + 2.0f, 5) / 2.0f;
	return start * (1.0f - time) + end * time;
}
#pragma endregion

#pragma region Quint
float Easing::Quint::easeIn(float start, float end, float t)
{
	float time = t * t * t * t * t;
	return start * (1.0f - time) + end * time;
}

float Easing::Quint::easeOut(float start, float end, float t)
{
	float time = 1.0f - pow(1 - t, 5);
	return start * (1.0f - time) + end * time;
}

float Easing::Quint::easeInOut(float start, float end, float t)
{
	float time;
	if (t < 0.5f) time = 4.0f * t * t * t * t * t;
	else time = 1.0f - pow(-2 * t + 2, 5) / 2;
	return start * (1.0f - time) + end * time;
}
#pragma endregion

#pragma region Cubic
float Easing::Cubic::easeIn(float start, float end, float t)
{
	float time = t * t * t;
	return start * (1.0f - time) + end * time;
}

float Easing::Cubic::easeOut(float start, float end, float t)
{
	float time = 1.0f - pow(1 - t, 3);
	return start * (1.0f - time) + end * time;
}

float Easing::Cubic::easeInOut(float start, float end, float t)
{
	float time;
	if (t < 0.5f) time = 4.0f * t * t * t;
	else time = 1.0f - pow(-2 * t + 2, 3) / 2;
	return start * (1.0f - time) + end * time;
}
#pragma endregion