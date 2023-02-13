#pragma once

#include <windows.h>
#include <string_view>
#include <unordered_map>

#include <variant>
#include <fstream>

#include "../../../engine/structures/structures.hpp"

enum features : int32_t
{
	aimbot_enable,
	aimbot_smoothness,
	aimbot_bone,
	aimbot_max_distance,
	aimbot_show_fov,
	aimbot_snap_lines,
	aimbot_fov,
	aimbot_fov_segments,
	aimbot_hotkey,

	display_max_distance,

	display_bounding_box,
	display_filled_box,
	display_players_name,
	display_corner_box,
	display_head_hitbox,
	display_distance,

	aimbot_fov_color,

	bounding_box_color,
	filled_box_color,
	corner_box_color,
	player_names_color,
	hitbox_color,
	distance_color,
	health_color
};

namespace menu::config
{
	extern std::variant< bool, int, float, engine::structures::vector_t, engine::structures::vector4_t > config_map[];
}