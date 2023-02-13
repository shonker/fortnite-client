#pragma once

namespace engine::offsets::core
{
	constexpr auto g_world = 0xc3ef7c8; // fn.dumps.host
	constexpr auto g_objects = 0xc072c78; // fn.dumps.host
}

namespace engine::offsets::world
{
	constexpr auto u_level = 0x30;
	constexpr auto owning_game_instance = 0x190;
}

namespace engine::offsets::u_level
{
	constexpr auto actors = 0x98;
}

namespace engine::offsets::u_game_instance
{
	constexpr auto local_player_actor = 0x38;
}

namespace engine::offsets::actor
{
	constexpr auto instigator = 0x170;

	constexpr auto root_component = 0x188;
}

namespace engine::offsets::u_player
{
	constexpr auto player_controller = 0x30;
}

namespace engine::offsets::controller
{
	constexpr auto character = 0x2D0;	
	constexpr auto is_local_player_controller = 0x554;

	constexpr auto acknowledged_pawn = 0x320;

	constexpr auto control_rotation = 0x288;
	constexpr auto u_player = 0x298;
}

namespace engine::offsets::temporary_camera_manager
{
	constexpr auto relative_rotation = 0x148;

	constexpr auto temp_rot_chain = 0xC8;

	constexpr auto size = 0x8;
}

namespace engine::offsets::pawn
{
	constexpr auto a_controller = 0x2B8;

	constexpr auto mesh = 0x300;
}

namespace engine::offsets::uscene_component
{
	constexpr auto relative_location = 0x11C;
}

namespace engine::offsets::skeletal_mesh_component
{
	constexpr auto animation_instance = 0x6A0;

	constexpr auto cached_bone_space = 0x8c8;

	constexpr auto component_to_world = 0x240;
}