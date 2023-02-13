#include <thread>

#include "modules.hpp"

#include "../settings/settings.hpp"

std::array< modules::module_t, 0 > modules::module_array = 
{

};

void modules::fire_modules( )
{
	while ( true )
	{
		for ( const auto& [testing, callback] : module_array )
		{
			if ( !testing || settings::values::is_testing )
				callback( );
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( module_update_interval ) );
	}
}