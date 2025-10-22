#pragma once

class Utils
{
public:
	template <typename T>
	static std::string to_string_with_precision( const T a_value, const int n = 3 )
	{
		std::ostringstream out;
		out << std::setprecision( n ) << a_value;
		return out.str();
	}

	std::string CurrentPath()
	{
		char buffer[ MAX_PATH ];
		GetModuleFileName( NULL, buffer, MAX_PATH );
		std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
		return std::string( buffer ).substr( 0, pos + 1 );
	}
};
