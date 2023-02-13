#pragma once

#include "../../engine/engine.hpp"

namespace modules::aimbot
{
	namespace visual
	{
		void draw_fov( );
	}

	namespace components
	{
		std::pair< engine::structures::vector_t, engine::types::actor_t > get_filtered_targets( );
	}

	void callback( );
}