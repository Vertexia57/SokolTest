#pragma once

namespace lost
{

	struct Vector2D
	{
		float x;
		float y;

		Vector2D& lerp(Vector2D other, float alpha)
		{
			x = x + (other.x - x) * alpha;
			y = y + (other.y - y) * alpha;

			return *this;
		}

		Vector2D operator+(Vector2D vec)
		{
			return Vector2D{ x + vec.x, y + vec.y };
		}

		Vector2D operator-(Vector2D vec)
		{
			return Vector2D{ x - vec.x, y - vec.y };
		}
	};

	struct Transform2D
	{
		Vector2D position = { 0, 0 };
		float rotation    = { 0 };
		Vector2D scale    = { 0, 0 };

		Transform2D& lerp(Transform2D& other, float alpha)
		{
			position.lerp(other.position, alpha);
			rotation = rotation + (other.rotation - rotation) * alpha;
			scale.lerp(other.scale, alpha);

			return *this;
		}

		Transform2D& operator+(Transform2D vec)
		{
			Transform2D transform{};
			transform.position = position + vec.position;
			transform.rotation = rotation + vec.rotation;
			// Account for scale here
			return transform;
		}

		Transform2D& operator-(Transform2D vec)
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

			left = x;
			right = x + w;
			top = y;
			bottom = y + h;
		}

		bool inBounds(Vector2D location) const
		{
			if (location.x >= x && location.x < x + w && location.y >= y && location.y < y + h)
				return true;
			return false;
		}

		Bound2D operator+(Vector2D vec) 
		{
			return Bound2D{ x + vec.x, y + vec.y, w, h };
		}

		Bound2D operator-(Vector2D vec)
		{
			return Bound2D{ x - vec.x, y - vec.y, w, h };
		}

	};

	static float lerp(float a, float b, float delta)
	{
		return a + (b - a) * delta;
	}

}