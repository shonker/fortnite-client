#include "types.hpp"

#include <mutex>

#include "../engine.hpp"

#include "../../integrity/integrity.hpp"
#include "../../renderer/menu/config/config.hpp"

#include "../../dependencies/ida/defs.hpp"

constexpr auto M_PI = 3.14159265358979323846f;
constexpr auto bone_size = 0x60;

std::optional< engine::structures::vector_t > engine::types::mesh_t::get_bone( engine::structures::bones bone )
{
	const auto bone_array = driver.read( instance + 0x590 );

	if ( !bone_array )
		return std::nullopt;

	std::optional bone_transform = driver.read< engine::structures::transform_t >( bone_array + ( bone * bone_size ) );

	if ( !bone_transform.has_value( ) )
		return std::nullopt;

	const auto matrix = engine::types::screen::matrix_multiply( engine::types::screen::to_scaled_matrix( bone_transform.value( ) ), 
		engine::types::screen::to_scaled_matrix( get_component_to_world( ) ) );

	return std::make_optional( engine::structures::vector_t { matrix.matrix4x4[0][3], matrix.matrix4x4[1][3], matrix.matrix4x4[2][3] } );
}

std::optional< engine::structures::vector_t > engine::types::mesh_t::get_relative_location( )
{
	return std::make_optional( driver.read< engine::structures::vector_t >( instance + offsets::uscene_component::relative_location ) );
}

engine::structures::transform_t engine::types::mesh_t::get_component_to_world( ) const
{
	return driver.read< engine::structures::transform_t >( instance + offsets::skeletal_mesh_component::component_to_world );
}

engine::types::u_level_t engine::types::game_world_t::get_u_level( ) const
{
	return driver.read< u_level_t >( instance + offsets::world::u_level );
}

engine::types::u_game_instance_t engine::types::game_world_t::get_owning_game_instance( ) const
{
	return driver.read< u_game_instance_t >( instance + offsets::world::owning_game_instance );
}

std::vector< engine::types::actor_t > engine::types::u_level_t::get_actors( ) const
{
	std::vector< engine::types::actor_t > actors;

	const auto size = driver.read< std::uint32_t >( instance + offsets::u_level::actors + sizeof( void* ) );

	if ( const auto list_start = driver.read( instance + offsets::u_level::actors ) )
	{
		for ( auto i = list_start; i < list_start + size * sizeof( std::uintptr_t ); i += sizeof( std::uintptr_t ) )
		{
			auto member = driver.read< actor_t >( i );

			if ( member.instance && member.is_a< engine::types::actor_type_t::player_athena_c >( engine::values::process_base ) 
				|| member.is_a< engine::types::actor_type_t::player_pawn >( engine::values::process_base ) )	
				actors.push_back( member );		
		}
	}

	return actors;
}

engine::types::pawn_t engine::types::actor_t::get_pawn( ) const
{
	return driver.read< pawn_t >( instance + offsets::actor::instigator );
}

engine::types::local_pawn_t engine::types::player_controller_t::get_pawn( ) const
{
	return driver.read< local_pawn_t >( instance + offsets::controller::acknowledged_pawn );
}

engine::types::root_component_t engine::types::local_pawn_t::get_root_component( ) const
{
	return driver.read< root_component_t >( instance + offsets::actor::root_component );
}

engine::types::mesh_t engine::types::actor_t::get_mesh( ) const
{
	return driver.read< mesh_t >( instance + offsets::pawn::mesh );
}

engine::types::local_player_t engine::types::u_game_instance_t::get_local_player( ) const
{
	return driver.read< local_player_t >( driver.read( instance + offsets::u_game_instance::local_player_actor ) );
}

engine::types::ftemp_rotation_t engine::types::local_player_t::get_temporary_rotation( ) const
{
	return driver.read< ftemp_rotation_t >( instance + offsets::temporary_camera_manager::temp_rot_chain + offsets::temporary_camera_manager::size );
}

engine::types::player_controller_t engine::types::pawn_t::get_player_controller( ) const
{
	return driver.read< player_controller_t >( instance + offsets::pawn::a_controller );
}

engine::types::player_controller_t engine::types::local_player_t::get_player_controller( ) const
{
	return driver.read< player_controller_t >( instance + engine::offsets::u_player::player_controller );
}

engine::structures::vector_t engine::types::local_player_t::get_location( ) const
{
	const auto temp_1 = driver.read( instance + 0x70 );
		const auto temp_2 = driver.read( temp_1 + 0x98 );
		const auto temp_3 = driver.read( temp_2 + 0x180 );

	return driver.read< engine::structures::vector_t >( temp_3 + 0x20 );
}

float engine::types::root_component_t::get_temporary_rotation( ) const
{
	return driver.read< float >( instance + offsets::temporary_camera_manager::relative_rotation );
}

float engine::types::ftemp_rotation_t::x_value( )
{
	const auto temporary_x_value = driver.read< float >( instance + 0xAE0 );

	float converted = std::asin( temporary_x_value ) * ( 180.f / M_PI );

	return converted;
}

float engine::types::stemp_rotation_t::y_value( )
{
	return driver.read< float >( instance );
}

float engine::types::ftemp_rotation_t::fov_angle( )
{
	return 80.0f / ( driver.read<float>( instance + 0x610 ) / 1.19f );
}

engine::structures::matrix4x4_t engine::types::screen::to_scaled_matrix( const engine::structures::transform_t& transform )
{
	engine::structures::matrix4x4_t matrix;

	matrix.matrix4x4[0][3] = transform.translation.x;
	matrix.matrix4x4[1][3] = transform.translation.y;
	matrix.matrix4x4[2][3] = transform.translation.z;

	const auto x2 = transform.rotation.x + transform.rotation.x;
	const auto y2 = transform.rotation.y + transform.rotation.y;
	const auto z2 = transform.rotation.z + transform.rotation.z;

	const auto xx2 = transform.rotation.x * x2;
	const auto yy2 = transform.rotation.y * y2;
	const auto zz2 = transform.rotation.z * z2;

	matrix.matrix4x4[ 0 ][ 0 ] = ( 1.0f - ( yy2 + zz2 ) ) * transform.scale_3d.x;
	matrix.matrix4x4[ 1 ][ 1 ] = ( 1.0f - ( xx2 + zz2 ) ) * transform.scale_3d.y;
	matrix.matrix4x4[ 2 ][ 2 ] = ( 1.0f - ( xx2 + yy2 ) ) * transform.scale_3d.z;

	const auto yz2 = transform.rotation.y * z2;
	const auto wx2 = transform.rotation.w * x2;

	matrix.matrix4x4[ 1 ][ 2 ] = ( yz2 - wx2 ) * transform.scale_3d.z;
	matrix.matrix4x4[ 2 ][ 1 ] = ( yz2 + wx2 ) * transform.scale_3d.y;

	const auto xy2 = transform.rotation.x * y2;
	const auto wz2 = transform.rotation.w * z2;

	matrix.matrix4x4[ 0 ][ 1 ] = ( xy2 - wz2 ) * transform.scale_3d.y;
	matrix.matrix4x4[ 1 ][ 0 ] = ( xy2 + wz2 ) * transform.scale_3d.x;

	const auto xz2 = transform.rotation.x * z2;
	const auto wy2 = transform.rotation.w * y2;

	matrix.matrix4x4[ 0 ][ 2 ] = ( xz2 + wy2 ) * transform.scale_3d.z;
	matrix.matrix4x4[ 2 ][ 0 ] = ( xz2 - wy2 ) * transform.scale_3d.x;

	matrix.matrix4x4[ 3 ][ 0 ] = 0.0f;
	matrix.matrix4x4[ 3 ][ 1 ] = 0.0f;
	matrix.matrix4x4[ 3 ][ 2 ] = 0.0f;
	matrix.matrix4x4[ 3 ][ 3 ] = 1.0f;

	return matrix;
}

engine::structures::matrix4x4_t engine::types::screen::matrix_multiply( const engine::structures::matrix4x4_t& lhs, const engine::structures::matrix4x4_t& rhs )
{
	engine::structures::matrix4x4_t matrix;

	const auto a00 = lhs.matrix4x4[ 0 ][ 0 ]; const auto a01 = lhs.matrix4x4[ 0 ][ 1 ]; const auto a02 = lhs.matrix4x4[ 0 ][ 2 ]; const auto a03 = lhs.matrix4x4[ 0 ][ 3 ];
	const auto a10 = lhs.matrix4x4[ 1 ][ 0 ]; const auto a11 = lhs.matrix4x4[ 1 ][ 1 ]; const auto a12 = lhs.matrix4x4[ 1 ][ 2 ]; const auto a13 = lhs.matrix4x4[ 1 ][ 3 ];
	const auto a20 = lhs.matrix4x4[ 2 ][ 0 ]; const auto a21 = lhs.matrix4x4[ 2 ][ 1 ]; const auto a22 = lhs.matrix4x4[ 2 ][ 2 ]; const auto a23 = lhs.matrix4x4[ 2 ][ 3 ];
	const auto a30 = lhs.matrix4x4[ 3 ][ 0 ]; const auto a31 = lhs.matrix4x4[ 3 ][ 1 ]; const auto a32 = lhs.matrix4x4[ 3 ][ 2 ]; const auto a33 = lhs.matrix4x4[ 3 ][ 3 ];

	const auto b00 = rhs.matrix4x4[ 0 ][ 0 ]; const auto b01 = rhs.matrix4x4[ 0 ][ 1 ]; const auto b02 = rhs.matrix4x4[ 0 ][ 2 ]; const auto b03 = rhs.matrix4x4[ 0 ][ 3 ];
	const auto b10 = rhs.matrix4x4[ 1 ][ 0 ]; const auto b11 = rhs.matrix4x4[ 1 ][ 1 ]; const auto b12 = rhs.matrix4x4[ 1 ][ 2 ]; const auto b13 = rhs.matrix4x4[ 1 ][ 3 ];
	const auto b20 = rhs.matrix4x4[ 2 ][ 0 ]; const auto b21 = rhs.matrix4x4[ 2 ][ 1 ]; const auto b22 = rhs.matrix4x4[ 2 ][ 2 ]; const auto b23 = rhs.matrix4x4[ 2 ][ 3 ];
	const auto b30 = rhs.matrix4x4[ 3 ][ 0 ]; const auto b31 = rhs.matrix4x4[ 3 ][ 1 ]; const auto b32 = rhs.matrix4x4[ 3 ][ 2 ]; const auto b33 = rhs.matrix4x4[ 3 ][ 3 ];

	matrix.matrix4x4[ 0 ][ 0 ] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
	matrix.matrix4x4[ 0 ][ 1 ] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
	matrix.matrix4x4[ 0 ][ 2 ] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
	matrix.matrix4x4[ 0 ][ 3 ] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
 
	matrix.matrix4x4[ 1 ][ 0 ] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
	matrix.matrix4x4[ 1 ][ 1 ] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
	matrix.matrix4x4[ 1 ][ 2 ] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
	matrix.matrix4x4[ 1 ][ 3 ] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;

	matrix.matrix4x4[ 2 ][ 0 ] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
	matrix.matrix4x4[ 2 ][ 1 ] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
	matrix.matrix4x4[ 2 ][ 2 ] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
	matrix.matrix4x4[ 2 ][ 3 ] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;

	matrix.matrix4x4[ 3 ][ 0 ] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
	matrix.matrix4x4[ 3 ][ 1 ] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
	matrix.matrix4x4[ 3 ][ 2 ] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
	matrix.matrix4x4[ 3 ][ 3 ] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;

	return matrix;
}

engine::structures::rotator_t engine::types::screen::calculate_angle( engine::structures::vector_t local_player, engine::structures::vector_t target_player )
{
	engine::structures::rotator_t new_view_angle;

	const auto rotation = local_player - target_player;

	const auto hypotenuse = std::sqrt( rotation.x * rotation.x + rotation.y * rotation.y );

	new_view_angle.pitch = -std::atan( rotation.z / hypotenuse ) * ( 180.f / M_PI );
	new_view_angle.yaw = std::atan( rotation.y / rotation.x ) * ( 180.f / M_PI );
	new_view_angle.roll = 0.f;

	if ( rotation.x >= 0.f )
		new_view_angle.yaw += 180.0f;

	return new_view_angle.clamp( );
}

engine::structures::matrix3x4_t engine::types::screen::get_rotation_matrix( )
{
	structures::matrix3x4_t rotation_matrix;

	auto owning_game_instance = engine::values::game_world.get_owning_game_instance( );

	auto local_player = owning_game_instance.get_local_player( );	

	auto y_val = local_player.get_player_controller( ).get_pawn( ).get_root_component( ).get_temporary_rotation( );

	std::printf( "y_val %lf\n", y_val );

	if ( y_val < 0 )
		y_val += 360;

	engine::structures::vector_t camera_rotation { local_player.get_temporary_rotation( ).x_value( ), 
		y_val, 0  };

	const auto p = camera_rotation.x * M_PI / 180;
	const auto y = camera_rotation.y * M_PI / 180;
	const auto r = camera_rotation.z * M_PI / 180;

	const auto sp = std::sinf( p ), cp = std::cosf( p ), sy = std::sinf( y ), cy = std::cosf( y ), sr = std::sinf( r ), cr = std::cosf( r );

	rotation_matrix.matrix3x4[ 0 ][ 0 ] = cp * cy; rotation_matrix.matrix3x4[ 0 ][ 1 ] = cp * sy; rotation_matrix.matrix3x4[ 0 ][ 2 ] = sp; rotation_matrix.matrix3x4[ 0 ][ 3 ] = 0.f;
	rotation_matrix.matrix3x4[ 1 ][ 0 ] = sr * sp * cy - cr * sy; rotation_matrix.matrix3x4[ 1 ][ 1 ] = sr * sp * sy + cr * cy; rotation_matrix.matrix3x4[ 1 ][ 2 ] = -sr * cp; rotation_matrix.matrix3x4[ 1 ][ 3 ] = 0.f;
	rotation_matrix.matrix3x4[ 2 ][ 0 ] = -(cr * sp * cy + sr * sy); rotation_matrix.matrix3x4[ 2 ][ 1 ] = cy * sr - cr * sp * sy; rotation_matrix.matrix3x4[ 2 ] [2 ] = cr * cp; rotation_matrix.matrix3x4[ 2 ][ 3 ] = 0.f;

	return rotation_matrix;
}

bool engine::types::screen::world_to_screen( structures::vector_t& in, structures::vector_t& out )
{
	const auto rotation_matrix = engine::types::screen::get_rotation_matrix( );

	auto owning_game_instance = engine::values::game_world.get_owning_game_instance( );

	auto local_player = owning_game_instance.get_local_player( );

	const auto camera_fov = local_player.get_temporary_rotation( ).fov_angle( );

	std::printf( "camera_fov %lf\n", camera_fov );

	structures::vector_t x{ rotation_matrix.matrix3x4[ 0 ][ 0 ], rotation_matrix.matrix3x4[ 0 ][ 1 ], rotation_matrix.matrix3x4[ 0 ][ 2 ] };
	structures::vector_t y{ rotation_matrix.matrix3x4[ 1 ][ 0 ], rotation_matrix.matrix3x4[ 1 ][ 1 ], rotation_matrix.matrix3x4[ 1 ][ 2 ] };
	structures::vector_t z{ rotation_matrix.matrix3x4[ 2 ][ 0 ], rotation_matrix.matrix3x4[ 2 ][ 1 ], rotation_matrix.matrix3x4[ 2 ][ 2 ] };

	std::printf( "location %lf, %lf, %lf\n", local_player.get_location( ).x, local_player.get_location( ).y, local_player.get_location( ).z );

	const auto delta = in - local_player.get_location( );

	structures::vector_t transformed{ delta.dot( y ), delta.dot( z ), delta.dot( x ) };

	transformed.z = transformed.z < 1.f ? 1.f : transformed.z;

	const auto [ width, height ] = engine::tools::get_screen_size( );

	const auto center_x = width / 2.f;
	const auto center_y = height / 2.f;

	const auto fov = std::tanf( camera_fov * M_PI / 360.f );

	out.x = center_x + transformed.x * center_x / fov / transformed.z;
	out.y = center_y - transformed.y * center_x / fov / transformed.z;

	if ( out.x > width || out.x < 0 || out.y > height || out.y < 0 )
		return false;

	return true;
}