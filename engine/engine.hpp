#pragma once

#include <mutex>

#include "types/types.hpp"
#include "offsets/offsets.hpp"

namespace engine::values
{
	extern std::mutex global_locker;
	extern std::mutex visuals_vector_locker;

	extern std::mutex aim_vector_locker;

	extern types::camera_manager_t camera_manager;
	extern types::game_world_t game_world;

	extern engine::types::local_player_t local_player;

	extern std::vector< engine::types::actor_t > aim_actors;
	extern std::vector< engine::types::actor_t > visual_actors;

	extern std::uintptr_t process_base;
}

namespace engine::tools
{
	std::pair< std::int32_t, std::int32_t > get_screen_size( );
}

namespace engine::functions
{
	void initiate( );

	void update_aim_actors_vector( );

	void update_visual_actors_vector( );
}