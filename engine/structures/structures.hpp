#pragma once

#include <algorithm>

namespace engine::structures
{
	enum bones : std::uint32_t
	{
		root = 0,
		head = 68,
	};

	struct rotator_t
	{
		float pitch;
		float yaw;
		float roll;

		rotator_t clamp( )
		{
			pitch = std::clamp( remainderf( pitch, 360.0f ), -89.0f, 89.0f );
			yaw = std::clamp( remainderf( yaw, 360.0f ), -180.0f, 180.0f );

			return { pitch, yaw };
		}

		rotator_t normalize( )
		{
			if (pitch > 180)
				pitch -= 360.0f;
			if (pitch < 180)
				pitch += 360.0f;
			if (yaw > 180)
				yaw -= 360.0f;
			if (yaw < 180)
				yaw += 360.0f;

			return { pitch, yaw };
		}

		rotator_t operator-( const rotator_t& rot ) const
		{
			return { pitch - rot.pitch, yaw - rot.yaw, roll - rot.roll };
		}

		rotator_t operator+( const rotator_t& rot ) const
		{
			return { pitch + rot.pitch, yaw + rot.yaw, roll + rot.roll };
		}

		rotator_t operator/( const float& f ) const
		{
			return { pitch / f, yaw / f, roll / f };
		}
	};

	struct quat_t
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct vector_t
	{
		float x;
		float y;
		float z;

		const float dot( const vector_t& vec ) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		const float distance( vector_t vector )
		{
			return sqrtf( powf( vector.x - x, 2.0 ) + powf( vector.y - y, 2.0 ) + powf( vector.z - z, 2.0 ) );
		}

		vector_t operator-( const vector_t& vec ) const
		{
			return { x - vec.x, y - vec.y, z - vec.z };
		}
	};

	struct vector4_t
	{
		float x;
		float y;
		float z;
		float w;

		float color[4] = { x, y, z, w };

		const float dot( const vector_t& vec ) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		vector_t operator-( const vector_t& vec ) const
		{
			return { x - vec.x, y - vec.y, z - vec.z };
		}
	};

	struct matrix3x4_t
	{
		union
		{
			float matrix3x4[ 3 ][ 4 ];
		};
	};

	struct matrix4x4_t
	{
		union
		{
			float matrix4x4[ 4 ][ 4 ];
		};
	};

	struct transform_t
	{
		quat_t rotation;
		vector_t translation;
		uint32_t pad;
		vector_t scale_3d;
	};
}