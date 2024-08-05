#pragma once
#include <math.h>

namespace lost
{

	struct IntVector2D
	{
		int x;
		int y;

		IntVector2D operator+(IntVector2D vec) const
		{
			return IntVector2D{ x + vec.x, y + vec.y };
		}

		IntVector2D operator-(IntVector2D vec) const
		{
			return IntVector2D{ x - vec.x, y - vec.y };
		}
	};

	struct Vector2D
	{
		float x;
		float y;

		Vector2D& lerp(Vector2D other, float alpha)
		{
			if (fabsf(x - other.x) <= 0.00001)
				x = other.x;
			else
				x = x + (other.x - x) * alpha;
			if (fabsf(y - other.y) <= 0.00001)
				y = other.y;
			else
				y = y + (other.y - y) * alpha;

			return *this;
		}

		Vector2D operator+(Vector2D vec) const
		{
			return Vector2D{ x + vec.x, y + vec.y };
		}

		Vector2D operator-(Vector2D vec) const
		{
			return Vector2D{ x - vec.x, y - vec.y };
		}

		Vector2D operator*(Vector2D vec) const
		{
			return Vector2D{ x * vec.x, y * vec.y };
		}

		Vector2D operator/(Vector2D vec) const
		{
			return Vector2D{ x / vec.x, y / vec.y };
		}

		Vector2D operator*(float val) const
		{
			return Vector2D{ x * val, y * val };
		}

		Vector2D operator/(float val) const
		{
			return Vector2D{ x / val, y / val };
		}

		float dist(Vector2D other) const
		{
			return sqrtf( powf(x - other.x, 2) + powf(y - other.y, 2) );
		}

		float magnitude()
		{
			return sqrtf(powf(x, 2) + powf(y, 2));
		}

		Vector2D& normalize()
		{
			float mag = magnitude();
			if (mag != 0.0f)
			{
				x /= mag;
				y /= mag;
			}
			else
			{
				x = 0.0f;
				y = 0.0f;
			}

			return *this;
		}

		Vector2D normalized()
		{
			float mag = magnitude();
			if (mag != 0.0f)
				return { x / mag, y / mag };
			return { 0.0f, 0.0f };
		}

		IntVector2D toInt()
		{
			return IntVector2D{ (int)x, (int)y };
		}
	};

	struct Transform2D
	{
		Vector2D position = { 0, 0 };
		float rotation    = { 0 };
		Vector2D scale    = { 1, 1 };

		Transform2D& lerp(Transform2D& other, float alpha)
		{
			position.lerp(other.position, alpha);

			if (fabsf(rotation - other.rotation) <= 0.00001)
				rotation = other.rotation;
			else
				rotation = rotation + (other.rotation - rotation) * alpha;

			scale.lerp(other.scale, alpha);

			return *this;
		}

		Transform2D operator+(Transform2D vec) const
		{
			Transform2D transform{};
			transform.position = position + vec.position;
			transform.rotation = rotation + vec.rotation;
			// Account for scale here
			return transform;
		}

		Transform2D operator-(Transform2D vec) const
		{
			Transform2D transform{};
			transform.position = position - vec.position;
			transform.rotation = rotation - vec.rotation;
			// Account for scale here
			return transform;
		}

	};

	struct Bound2D
	{
		float x, y, w, h;
		float left, right, top, bottom;

		Bound2D() 
			: x(0), y(0), w(0), h(0), left(0), right(0), top(0), bottom(0)
		{

		}

		Bound2D(float _x, float _y, float _w, float _h)
		{
			x = _x;
			y = _y;
			w = _w;
			h = _h;
			calcSides();
		}

		void calcSides()
		{
			left = x;
			right = x + w;
			top = y;
			bottom = y + h;
		}

		bool inBounds(Vector2D location) const
		{
			return (location.x >= x && location.x < x + w && location.y >= y && location.y < y + h);
		}

		bool inBounds(Bound2D bounds) const
		{
			return (bounds.x + bounds.w - 1 >= x && bounds.x <= x + w - 1 && bounds.y + bounds.h - 1 >= y && bounds.y <= y + h - 1);
		}

		Bound2D operator+(Vector2D vec) const
		{
			return Bound2D{ x + vec.x, y + vec.y, w, h };
		}

		Bound2D operator-(Vector2D vec) const
		{
			return Bound2D{ x - vec.x, y - vec.y, w, h };
		}

		Bound2D operator*(Vector2D vec) const
		{
			return Bound2D{ x, y, w * vec.x, h * vec.y };
		}

		Bound2D operator*(float val) const
		{
			return Bound2D{ x, y, w * val, h * val };
		}

		void operator+=(Vector2D vec)
		{
			x += vec.x;
			y += vec.y;
			calcSides();
		}

		void operator-=(Vector2D vec)
		{
			x -= vec.x;
			y -= vec.y;
			calcSides();
		}

	};

	static float lerp(float a, float b, float delta)
	{
		return a + (b - a) * delta;
	}

	static float maxLerp(float a, float b, float delta, float maxChange)
	{
		return a + fmaxf(fminf((b - a) * delta, maxChange), -maxChange);
	}

}