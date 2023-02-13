#include "engine.hpp"

#include "types/types.hpp"

#include "../settings/settings.hpp"
#include "../renderer/menu/config/config.hpp"

std::mutex engine::values::global_locker;
std::mutex engine::values::visuals_vector_locker;

std::mutex engine::values::aim_vector_locker;

std::uintptr_t engine::values::process_base = 0;

//engine::types::camera_manager_t engine::values::camera_manager { 0 };
engine::types::game_world_t engine::values::game_world { 0 };

engine::types::local_player_t engine::values::local_player;

std::vector< engine::types::actor_t > engine::values::aim_actors;
std::vector< engine::types::actor_t > engine::values::visual_actors;

std::once_flag setup_bases;

static HWND game;

std::pair< std::int32_t, std::int32_t > engine::tools::get_screen_size( )
{
	if ( !game )
		game = lf( FindWindowA )( xs( "UnrealWindow" ), nullptr );

	RECT screen_dimensions{ };

	if ( lf( GetClientRect )( game, &screen_dimensions ) )
	{
		return { screen_dimensions.right - screen_dimensions.left, screen_dimensions.bottom - screen_dimensions.top };
	}

	return { 0, 0 };
}

void engine::functions::initiate( )
{
	while ( true )
	{
		std::call_once( setup_bases, [=]()
			{
				engine::values::process_base = driver.get_base( );
			});

		const auto temporary_game_world_value = driver.read< types::game_world_t >( engine::values::process_base + engine::offsets::core::g_world );

		const auto temporary_u_level_value = temporary_game_world_value.get_u_level( );

		const auto temporary_game_instance_value = temporary_game_world_value.get_owning_game_instance( );

		const auto temporary_local_player_value = temporary_game_instance_value.get_local_player( );

		//const auto temporary_camera_manager_value = temporary_local_player_value.get_player_controller( ).get_camera_manager( );

		std::unique_lock<std::mutex> initiate_thread_locker( engine::values::global_locker );

		//engine::values::camera_manager = temporary_camera_manager_value;

		engine::values::game_world = temporary_game_world_value;

		engine::values::local_player = temporary_local_player_value;

		//std::printf( "camera_manager %p\n", engine::values::camera_manager.instance );
		std::printf( "game_world %p\n", engine::values::game_world.instance );
		std::printf( "local_player %p\n", engine::values::local_player.instance );

		initiate_thread_locker.unlock( );

		if ( !engine::values::game_world.instance )
			continue;

		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}
}

void engine::functions::update_aim_actors_vector( )
{
	while ( true )
	{
		std::unique_lock<std::mutex> update_aim_entities_global_locker( engine::values::global_locker );

		if ( !engine::values::game_world.instance )
		{
			update_aim_entities_global_locker.unlock( );

			std::unique_lock<std::mutex> update_aim_entities_locker( engine::values::aim_vector_locker );

			engine::values::aim_actors.clear( );

			update_aim_entities_locker.unlock( );

			continue;
		}

		types::game_world_t local_game_world_clone { engine::values::game_world.instance };

		update_aim_entities_global_locker.unlock( );

		const auto temporary_aim_entities = local_game_world_clone.get_u_level( ).get_actors( );

		std::unique_lock<std::mutex> update_aim_entities_locker( engine::values::aim_vector_locker );

		engine::values::aim_actors = temporary_aim_entities;

		update_aim_entities_locker.unlock( );

		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
}

void engine::functions::update_visual_actors_vector( )
{
	while ( true )
	{
		std::unique_lock<std::mutex> update_visual_entities_global_locker( engine::values::global_locker );

		if ( !engine::values::game_world.instance )
		{
			update_visual_entities_global_locker.unlock( );

			std::unique_lock<std::mutex> update_visual_entities_locker( engine::values::visuals_vector_locker );

			engine::values::visual_actors.clear( );

			update_visual_entities_locker.unlock( );

			continue;
		}

		types::game_world_t local_game_world_clone { engine::values::game_world.instance };

		update_visual_entities_global_locker.unlock( );

		const auto temporary_visual_entities = local_game_world_clone.get_u_level( ).get_actors( );

		std::unique_lock<std::mutex> update_visual_entities_locker( engine::values::visuals_vector_locker );

		engine::values::visual_actors = temporary_visual_entities;

		update_visual_entities_locker.unlock( );

		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
}