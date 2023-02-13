#include "aimbot.hpp"

#include <map>
#include <mutex>
#include <cmath>

#include "../../settings/settings.hpp"
#include "../../renderer/renderer.hpp"
#include "../../renderer/menu/config/config.hpp"

auto distance_from_cursor(  const engine::structures::vector_t& target )
{
	const auto [ w, h ] = engine::tools::get_screen_size( );

	return std::sqrt(  std::pow(  target.x - w / 2, 2 ) + std::pow(  target.y - h / 2, 2 ) );
}

void modules::aimbot::visual::draw_fov( )
{
	if (  std::get< bool >(  menu::config::config_map[features::aimbot_show_fov] ) && std::get< int >(  menu::config::config_map[features::aimbot_fov] ) > 0 )
    {
        const auto w = GetSystemMetrics(  SM_CXSCREEN ), h = GetSystemMetrics(  SM_CYSCREEN );

        const auto draw_list = ImGui::GetBackgroundDrawList( );

		const auto col = std::get< engine::structures::vector4_t >(  menu::config::config_map[features::aimbot_fov_color] );

        draw_list->AddCircle(  { w / 2.f, h / 2.f }, std::get< int >(  menu::config::config_map[features::aimbot_fov] ) * 10, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, std::get< int >(  menu::config::config_map[features::aimbot_fov_segments] ), 2.f );
    }
}

std::pair< engine::structures::vector_t, engine::types::actor_t > modules::aimbot::components::get_filtered_targets( )
{
	/*std::unique_lock<std::mutex> initiate_thread_locker( engine::values::global_locker );

	const auto game_world_copy = engine::values::game_world;

	const auto camera_copy = engine::values::camera_manager;

	const auto local_player_copy = engine::values::local_player;

	initiate_thread_locker.unlock( );

	std::unique_lock<std::mutex> update_aim_actors_locker( engine::values::aim_vector_locker );

	const auto aim_actors_copy = engine::values::aim_actors; auto best_distance = 10000.f;

	engine::types::actor_t filtered_target { };
	engine::structures::vector_t filtered_target_position { };

	if ( !camera_copy.instance || !game_world_copy.instance )
		return std::pair( filtered_target_position, filtered_target );

	update_aim_actors_locker.unlock( );

	for ( const auto actor : aim_actors_copy )
	{
		
	}

	return std::pair( filtered_target_position, filtered_target );

	update_aim_actors_locker.lock( );*/
}

void modules::aimbot::callback( )
{

}