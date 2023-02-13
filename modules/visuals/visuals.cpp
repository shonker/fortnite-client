#include "visuals.hpp"

#include <mutex>
#include <cmath>

#include "../../engine/engine.hpp"

#include "../../settings/settings.hpp"

#include "../../renderer/renderer.hpp"
#include "../../renderer/menu/config/config.hpp"

void modules::visuals::callback( )
{
	std::unique_lock<std::mutex> initiate_thread_locker( engine::values::global_locker );

	const auto game_world_copy = engine::values::game_world;

	const auto local_player_copy = engine::values::local_player;

	initiate_thread_locker.unlock( );

	if ( !game_world_copy.instance || !local_player_copy.instance )
		return;

	std::unique_lock<std::mutex> update_visual_actors_locker( engine::values::visuals_vector_locker );

	const auto visual_actors_copy = engine::values::visual_actors;

	update_visual_actors_locker.unlock( );

	for ( const auto actor : visual_actors_copy )
	{
		const auto draw_list = ImGui::GetBackgroundDrawList( );

		auto root_world_position = actor.get_mesh( ).get_bone( engine::structures::bones::root );

		if ( !root_world_position.has_value( ) )
			continue;

		engine::structures::vector_t root_screen_position;

		if ( engine::types::screen::world_to_screen( root_world_position.value( ), root_screen_position ) )		
			draw_list->AddOutlinedText( renderer::imgui::elements::fonts::pro_font_windows, 10.f, { root_screen_position.x, root_screen_position.y }, ImColor{ 255, 0, 0 }, "Player" );
	}
}