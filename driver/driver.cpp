#include "driver.hpp"
#include "../integrity/integrity.hpp"

#include <tlhelp32.h>
#include <iostream>

driver_t driver;

namespace types
{
	template < class type_t, auto deleter_t > using managed_t = std::unique_ptr< std::remove_pointer_t< type_t >, std::integral_constant< decltype( deleter_t ), deleter_t > >;
}

std::uintptr_t driver_t::get_base( ) const
{
	const auto data = data_t{ data_t::command_t::get_base, to };

	if ( !data.from_address )
		NtDCompositionSetChildRootVisual( data, nullptr );

	return data.from_address;
}

std::string driver_t::read_string( std::uintptr_t address, const bool inner ) const
{
	if ( inner )
		address = driver.read( address );

	std::string string;

	do
		string.push_back( driver.read< char >( address++ ) );
	while ( driver.read< char >( address ) != '\0' );

	return string;
}

std::string driver_t::read_raw_bytes( std::uintptr_t address, const size_t size )
{
	std::unique_ptr< char[] > temporary( new char[size] );
	
	driver.read_arr<char>( address, temporary.get( ), size );
	
	return std::string( temporary.get( ) );
}

std::string driver_t::read_unicode_string( std::uintptr_t address, const int size ) const
{
	char16_t temporary[128] = { '\0' };

	driver.read_arr<char16_t>( address, temporary, size * sizeof( std::uint16_t ) );
	std::string buffer = 
		integrity::utilities::to_utf8( temporary );

	return buffer;
}

bool driver_t::initiate( const char* const window_name )
{
	target_window = lf( FindWindowA )( window_name, nullptr );

	lf( GetWindowThreadProcessId )( target_window, &target_id );

	return lf( GetWindowThreadProcessId )( lf( FindWindowA )( window_name, nullptr ), &to );
}