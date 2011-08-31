#include "VirtualMachineEx.h"

#include <cmath>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cctype>

VirtualMachineEx::VirtualMachineEx( void ):
	VirtualMachine(),  stack1( stack ), stack2( new Stack )
{
	execute['c'] = func_p( &VirtualMachineEx::_clear_stack );
	execute['x'] = func_p( &VirtualMachineEx::_coords );

	execute['u'] = func_p( &VirtualMachineEx::_conditional_movement_ex );
	execute['d'] = func_p( &VirtualMachineEx::_conditional_movement_ex );
	execute['l'] = func_p( &VirtualMachineEx::_conditional_movement_ex );
	execute['r'] = func_p( &VirtualMachineEx::_conditional_movement_ex );

	execute['w'] = func_p( &VirtualMachineEx::_conditional_movement_compare );

	execute['§'] = func_p( &VirtualMachineEx::_constant_random_movement );

	execute['['] = func_p( &VirtualMachineEx::_turn );
	execute[']'] = func_p( &VirtualMachineEx::_turn );

	execute['t'] = func_p( &VirtualMachineEx::_turn_around );

	execute['m'] = func_p( &VirtualMachineEx::_move_to );

	execute['s'] = func_p( &VirtualMachineEx::_swap_stack );

	execute['S'] = func_p( &VirtualMachineEx::_stack_size );

	execute[';'] = func_p( &VirtualMachineEx::_swap_stack_top );

	execute['A'] = func_p( &VirtualMachineEx::_digits_hex );
	execute['B'] = func_p( &VirtualMachineEx::_digits_hex );
	execute['C'] = func_p( &VirtualMachineEx::_digits_hex );
	execute['D'] = func_p( &VirtualMachineEx::_digits_hex );
	execute['E'] = func_p( &VirtualMachineEx::_digits_hex );
	execute['F'] = func_p( &VirtualMachineEx::_digits_hex );

	execute['{'] = func_p( &VirtualMachineEx::_function );
	execute['}'] = func_p( &VirtualMachineEx::_function );

	execute['='] = func_p( &VirtualMachineEx::_execute );
}

VirtualMachineEx::~VirtualMachineEx( void )
{
	delete stack2;
}

bool VirtualMachineEx::_clear_stack( char c )
{
	stack->Clear();

	return true;
}

bool VirtualMachineEx::_coords( char c )
{
	stack->Push( ip.pos.x );
	stack->Push( ip.pos.y );

	return true;
}

bool VirtualMachineEx::_conditional_movement_ex( char c )
{
	if ( !stack->Pop() )
	{
		switch ( c )
		{
			case 'u':
				ip.dir.x = 0; ip.dir.y = -1; break;

			case 'd':
				ip.dir.x = 0; ip.dir.y = 1; break;

			case 'l':
				ip.dir.x = -1; ip.dir.y = 0; break;

			case 'r':
				ip.dir.x = 1; ip.dir.y = 0;
		}
	}

	return true;
}

bool VirtualMachineEx::_conditional_movement_compare( char c )
{
	Stack::Value b = stack->Pop(), a = stack->Pop();

	if ( a < b )
		return _turn( '[' );
	else if ( a > b )
		return _turn( ']' );
	else
		return true;
}

bool VirtualMachineEx::_constant_random_movement( char c )
{
	uint16 rand_dir = std::rand() % 4;

	switch ( rand_dir )
	{
		case 0:
			set_cmd_at( ip.pos.x, ip.pos.y, '<' ); break;

		case 1:
			set_cmd_at( ip.pos.x, ip.pos.y, '>' ); break;

		case 2:
			set_cmd_at( ip.pos.x, ip.pos.y, 'v' ); break;

		case 3:
			set_cmd_at( ip.pos.x, ip.pos.y, '^' );
	}

	return false;
}

bool VirtualMachineEx::_turn( char c )
{
	/*
	double	direction = c == '[' ? ( M_PI_2 * -1.0 ) : M_PI_2,
			atan_of_dir = std::atan2( double( ip.dir.y ), double( ip.dir.x ) );

	ip.dir.x = ( int64 )( std::cos( atan_of_dir + direction ) );
	ip.dir.y = ( int64 )( std::sin( atan_of_dir + direction ) );
	*/

	Vector<int16> old_dir = ip.dir;

	if ( c == '[' )
	{
		ip.dir.x = old_dir.y;
		ip.dir.y = -old_dir.x;
	}
	else if ( c == ']' )
	{
		ip.dir.x = -old_dir.y;
		ip.dir.y = old_dir.x;
	}

	return true;
}

bool VirtualMachineEx::_turn_around( char c )
{
	ip.dir.x *= -1;
	ip.dir.y *= -1;

	return true;
}

bool VirtualMachineEx::_move_to( char c )
{
	ip.pos.y = stack->Pop();
	ip.pos.x = stack->Pop();

	return false;
}

bool VirtualMachineEx::_swap_stack( char c )
{
	stack = stack == stack1 ? stack2 : stack1;

	return true;
}

bool VirtualMachineEx::_stack_size( char c )
{
	stack->Push( Stack::Value( stack->Size() ) );

	return true;
}

bool VirtualMachineEx::_swap_stack_top( char c )
{
	Stack	*inactive_stack = stack == stack1 ? stack2 : stack1;

	inactive_stack->Push( stack->Pop() );

	return true;
}

bool VirtualMachineEx::_digits_hex( char c )
{
	stack->Push( Stack::Value( c - 'A' + 10 ) );

	return true;
}

bool VirtualMachineEx::_function( char c )
{
	bool proceed = true;

	switch ( c )
	{
		case '{':
			callstack.push( ip );

			ip.pos.y = stack->Pop();
			ip.pos.x = stack->Pop();

			proceed = false;

			break;

		case '}':
			if ( !callstack.empty() )
			{
				ip = callstack.top();

				callstack.pop();
			}
	}

	return proceed;
}

bool VirtualMachineEx::_execute( char c )
{
	std::string cmd = stack->PopString();

	stack->Push( Stack::Value( system( cmd.c_str() ) ) );

	return true;
}
/*
void VirtualMachineEx::LoadCode( const std::string &filename )
{
    std::ifstream file;
    uint16 x = 0, y = 0;
    char cmd;

    file.open( filename.c_str() );

    while ( file.good() )
    {
    	file.get( cmd );

    	if ( cmd == '\n' )
    	{
    		x = 0;
    		++y;
    	}
    	else
    	{
    		if ( !std::isspace( cmd ) )
    		{
    			set_cmd_at( x, y, cmd );
    		}

    		++x;
    	}
    }

    file.close();
}
*/

int32 VirtualMachineEx::Run( void )
{
	VirtualMachine::Run();

	return int32( stack->Pop() );
}

/*
char VirtualMachineEx::get_cmd_at( uint16 x, uint16 y )
{
	char cmd = ' ';

	std::map<uint16, std::map<uint16, char> >::iterator it_x = code.find( x );

	if ( it_x != code.end() )
	{
		std::map<uint16, char>::iterator it_y = it_x->second.find( y );

		if ( it_y != it_x->second.end() )
		{
			cmd = it_y->second;
		}
	}

	return cmd;
}

void VirtualMachineEx::set_cmd_at( uint16 x, uint16 y, char c )
{
	code[x];
	code[x][y] = c;
}
*/
