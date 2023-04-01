#include <windows.h>

#include <cstdint>

#include "io/io.hpp"

#include "driver/driver.hpp"
#include "engine/engine.hpp"

#include "integrity/integrity.hpp"

#include "renderer/menu/menu.hpp"
#include "renderer/renderer.hpp"

#include "settings/settings.hpp"

#include "modules/modules.hpp"

#include "modules/aimbot/aimbot.hpp"

#include "dependencies/xorstring/xorstring.hpp"

int core( )
{
	/*bool integrity_status = true;

	if constexpr ( !settings::values::using_auth )
		integrity_status = true;

	if ( integrity_status )
	{*/
		while ( !FindWindowA( xs( "UnrealWindow" ), nullptr ) )
			std::this_thread::sleep_for( std::chrono::seconds( 20 ) );

		if constexpr ( settings::values::debug )
			io::open_console( xs( "NZXT CAM" ) );

		driver.initiate( xs( "UnrealWindow" ) );

		std::thread{ engine::functions::initiate }.detach( );

		std::thread{ engine::functions::update_aim_actors_vector }.detach();

		std::thread{ engine::functions::update_visual_actors_vector }.detach( );

		std::thread( []( )
			{
				while ( true )
				{
					modules::aimbot::callback( );

					std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
				}
			} ).detach( );

		std::thread{ modules::fire_modules }.detach( );

		std::thread( []( )
			{ renderer::start( xs( "UnrealWindow" ) ); } ).detach( );
	/*}
	else
	{
		_Exit( 0 );
	}*/

	return 0;
}

bool DllMain( void*, const std::uint32_t injection_reason, void* )
{
	if ( injection_reason == DLL_PROCESS_ATTACH )
		std::thread{ core }.detach( );

	return true;
}