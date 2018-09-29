#pragma once

namespace DXUtil
{
	struct RefreshRate
	{
		unsigned int Numerator;
		unsigned int Denominator;
	};

	struct Color
	{
		float Red;
		float Green;
		float Blue;
		float Alpha;
	};

	struct Canvas
	{
		long Width;
		long Height;
	};
}