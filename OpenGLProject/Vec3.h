#pragma once


namespace fwork
{
	class Vec3
	{
	public:
		float x = 0, y = 0, z = 0;
		Vec3() {}
		Vec3(float x, float y, float z):
			x(x), y(y), z(z)
		{

		}
	};
}