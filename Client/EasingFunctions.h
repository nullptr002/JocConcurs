/*
*	Sursa fisier: https://github.com/nicolausYes/easing-functions
*	
*	Definitiile functiilor se gasesc in "EasingFunctions.cpp".
*	
*	Pe scurt, aceste functii sunt folosite pentru schimbarea unei valori non-linear
*	intr-o anumita perioada de timp.
*
*	Explicatie si vizualizare easing functions: https://easings.net/en
*	(in caz ca nu stiati deja)
*/

#pragma once 

enum easing_functions
{
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
	EaseInBack,
	EaseOutBack,
	EaseInOutBack,
	EaseInElastic,
	EaseOutElastic,
	EaseInOutElastic,
	EaseInBounce,
	EaseOutBounce,
	EaseInOutBounce
};

typedef double(*easingFunction)(double);

easingFunction getEasingFunction( easing_functions function );;

