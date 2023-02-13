#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "../offsets/offsets.hpp"
#include "../structures/structures.hpp"

#include "../../driver/driver.hpp"

namespace engine::types
{
	struct game_world_t;
	struct game_objects_t;
	struct camera_manager_t;
	struct local_player_t;

	struct u_level_t;
	struct u_game_instance_t;

	struct actor_t;

	struct pawn_t;
	struct local_pawn_t;

	struct character_t;
	struct character_movement_t;

	struct mesh_t;
	struct animation_instance_t;

	struct u_player_t;
	struct player_controller_t;
	
	struct world_settings_t;	
}

namespace engine::types
{
	enum actor_type_t : std::uintptr_t
	{
		player_athena_c = 0x91BA960,
		player_pawn = 0x93EEAE8
	};

	struct object_t
	{
		template< class type_t = std::uintptr_t >
		[[nodiscard]] type_t as( ) const
		{
			return type_t{ instance };
		}

		template< actor_type_t type >
		[[nodiscard]] bool is_a( std::uintptr_t process_base )
		{
			const auto vftable = driver.read( instance );

			return vftable == process_base + type;
		}

		std::uintptr_t instance;
	};

	struct mesh_t : public object_t
	{
		std::optional< engine::structures::vector_t > get_bone( engine::structures::bones bone );

		std::optional< engine::structures::vector_t > get_relative_location( );

		[[nodiscard]] engine::structures::transform_t get_component_to_world( ) const;
	};

	struct stemp_rotation_t : public object_t
	{ 
		[[nodiscard]] float y_value( );
	};

	struct root_component_t : public object_t
	{
		[[nodiscard]] float get_temporary_rotation( ) const;
	};

	struct local_pawn_t : public object_t 
	{
		[[nodiscard]] root_component_t get_root_component( ) const;
	};

	struct u_player_t : public object_t { };
	struct player_controller_t : public object_t
	{
		camera_manager_t get_camera_manager( );

		[[nodiscard]] local_pawn_t get_pawn( ) const;
	};

	struct pawn_t : public object_t
	{
		[[nodiscard]] player_controller_t get_player_controller( ) const;
	};

	struct actor_t : public object_t
	{
		[[nodiscard]] pawn_t get_pawn( ) const;

		[[nodiscard]] mesh_t get_mesh( ) const;

		//[[nodiscard]] mesh_t get_root_component( ) const;
	}; 

	struct world_settings_t : public object_t { };
	struct u_level_t : public object_t
	{
		[[nodiscard]] std::vector< actor_t > get_actors( ) const;
	};

	struct ftemp_rotation_t : public object_t
	{ 
		[[nodiscard]] float x_value( );

		[[nodiscard]] float fov_angle( );
	};

	struct local_player_t : public object_t
	{
		[[nodiscard]] player_controller_t get_player_controller( ) const;

		[[nodiscard]] ftemp_rotation_t get_temporary_rotation( ) const;

		[[nodiscard]] engine::structures::vector_t get_location( ) const;
	};

	struct u_game_instance_t : public object_t
	{
		[[nodiscard]] local_player_t get_local_player( ) const;
	};

	struct game_world_t : public object_t
	{
		[[nodiscard]] u_level_t get_u_level( ) const;

		[[nodiscard]] u_game_instance_t get_owning_game_instance( ) const;
	};

	namespace screen
	{
		engine::structures::matrix4x4_t to_scaled_matrix( const engine::structures::transform_t& transform );

		engine::structures::matrix4x4_t matrix_multiply( const engine::structures::matrix4x4_t& lhs, const engine::structures::matrix4x4_t& rhs );

		engine::structures::rotator_t calculate_angle( engine::structures::vector_t local_player, engine::structures::vector_t target_player );

		engine::structures::matrix3x4_t get_rotation_matrix( );

		bool world_to_screen( structures::vector_t& in, structures::vector_t& out );
	}
}