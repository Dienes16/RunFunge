#include "VirtualMachine.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

void IP::Move( void )
{
    pos.x += dir.x;
    if ( pos.x >= CODE_WIDTH )
        pos.x = 0;
    else if ( pos.x < 0 )
        pos.x = CODE_WIDTH - 1;

    pos.y += dir.y;
    if ( pos.y >= CODE_HEIGHT )
        pos.y = 0;
    else if ( pos.y < 0 )
        pos.y = CODE_HEIGHT - 1;
}

VirtualMachine::VirtualMachine( void ):
	stack( new Stack ), string_mode( false )
{
	std::srand( std::time( NULL ) );

	execute['<'] = &VirtualMachine::_movement;
	execute['>'] = &VirtualMachine::_movement;
	execute['v'] = &VirtualMachine::_movement;
	execute['^'] = &VirtualMachine::_movement;

	execute['_'] = &VirtualMachine::_conditional_movement;
	execute['|'] = &VirtualMachine::_conditional_movement;

	execute['?'] = &VirtualMachine::_random_movement;

	execute['+'] = &VirtualMachine::_math;
	execute['-'] = &VirtualMachine::_math;
	execute['*'] = &VirtualMachine::_math;
	execute['/'] = &VirtualMachine::_math;
	execute['%'] = &VirtualMachine::_math;

	execute['!'] = &VirtualMachine::_logical;
	execute['`'] = &VirtualMachine::_logical;

	execute[':']  = &VirtualMachine::_stack_manipulation;
	execute['\\'] = &VirtualMachine::_stack_manipulation;
	execute['$']  = &VirtualMachine::_stack_manipulation;

	execute['.'] = &VirtualMachine::_output;
	execute[','] = &VirtualMachine::_output;

	execute['&'] = &VirtualMachine::_input;
	execute['~'] = &VirtualMachine::_input;

	execute['#'] = &VirtualMachine::_skip;

	execute['g'] = &VirtualMachine::_code_manipulation;
	execute['p'] = &VirtualMachine::_code_manipulation;

	execute['0'] = &VirtualMachine::_digits;
	execute['1'] = &VirtualMachine::_digits;
	execute['2'] = &VirtualMachine::_digits;
	execute['3'] = &VirtualMachine::_digits;
	execute['4'] = &VirtualMachine::_digits;
	execute['5'] = &VirtualMachine::_digits;
	execute['6'] = &VirtualMachine::_digits;
	execute['7'] = &VirtualMachine::_digits;
	execute['8'] = &VirtualMachine::_digits;
	execute['9'] = &VirtualMachine::_digits;
}

VirtualMachine::~VirtualMachine( void )
{
	delete stack;
}

bool VirtualMachine::_movement( char c )
{
	switch ( c )
	{
		case '<':
			ip.dir.x = -1; ip.dir.y = 0; break;

		case '>':
			ip.dir.x = 1; ip.dir.y = 0; break;

		case 'v':
			ip.dir.x = 0; ip.dir.y = 1; break;

		case '^':
			ip.dir.x = 0; ip.dir.y = -1;
	}

	return true;
}

bool VirtualMachine::_conditional_movement( char c )
{
	Stack::Value val = stack->Pop();

	switch ( c )
	{
		case '_':
			ip.dir.y = 0; ip.dir.x = val ? -1 : 1; break;

		case '|':
			ip.dir.x = 0; ip.dir.y = val ? -1 : 1;
	}

	return true;
}

bool VirtualMachine::_random_movement( char c )
{
	short rand_dir = std::rand() % 4;

	switch ( rand_dir )
	{
		case 0:
			ip.dir.x = -1; ip.dir.y = 0; break;

		case 1:
			ip.dir.x = 1; ip.dir.y = 0; break;

		case 2:
			ip.dir.x = 0; ip.dir.y = 1; break;

		case 3:
			ip.dir.x = 0; ip.dir.y = -1;
	}

	return true;
}

bool VirtualMachine::_math( char c )
{
	Stack::Value b = stack->Pop(), a = stack->Pop();

	switch ( c )
	{
		case '+':
			stack->Push( a + b ); break;

		case '-':
			stack->Push( a - b ); break;

		case '*':
			stack->Push( a * b ); break;

		case '/':
			stack->Push( a / b ); break;

		case '%':
			stack->Push( a % b );
	}

	return true;
}

bool VirtualMachine::_logical( char c )
{
	switch ( c )
	{
		case '!':
			stack->Push( !( stack->Pop() ) ); break;

		case '`':
			stack->Push( stack->Pop() < stack->Pop() );
	}

	return true;
}

bool VirtualMachine::_stack_manipulation( char c )
{
	switch ( c )
	{
		case ':':
			{
				Stack::Value val = stack->Pop();

				stack->Push( val );
				stack->Push( val );
			}
			break;

		case '\\':
			{
				Stack::Value a = stack->Pop(),
							 b = stack->Pop();

				stack->Push( a );
				stack->Push( b );
			}
			break;

		case '$':
			stack->Pop();
	}

	return true;
}

bool VirtualMachine::_output( char c )
{
	Stack::Value val = stack->Pop();

	switch ( c )
	{
		case '.':
			std::cout << val << ' '; break;

		case ',':
			std::cout << char( val );
	}

	return true;
}

bool VirtualMachine::_input( char c )
{
	switch ( c )
	{
		case '&':
			{
				std::string s;
				std::getline( std::cin, s );

				std::stringstream ss;
				ss << s;

				Stack::Value val;
				ss >> val;

				stack->Push( val );
			}
			break;

		case '~':
			{
				//char c = GetUnbufferedInput();
				//stack.Push( Stack::Value( c ) );
				//std::cout << c;

				//char c;
				//std::cin >> c;
				//stack.Push( Stack::Value( c ) );

				stack->Push( std::getchar() );
			}
	}

	return true;
}

bool VirtualMachine::_skip( char c )
{
	ip.Move();

	return true;
}

bool VirtualMachine::_code_manipulation( char c )
{
	Stack::Value y = stack->Pop(), x = stack->Pop();

	switch ( c )
	{
		case 'g':
			stack->Push( get_cmd_at( x, y ) ); break;

		case 'p':
			set_cmd_at( x, y, char( stack->Pop() ) );
	}

	return true;
}

bool VirtualMachine::_digits( char c )
{
	if ( std::isdigit( c ) )
		stack->Push( Stack::Value( c - '0' ) );

	return true;
}

void VirtualMachine::LoadCode( const std::string &filename )
{
    std::ifstream file;
    uint16 x, y;
    char cmd;
    bool done = false;

    file.open( filename.c_str() );

    for ( y = 0; y < CODE_HEIGHT; ++y )
        for ( x = 0; x < CODE_WIDTH; ++x )
            set_cmd_at( x, y, ' ' );

    for ( y = 0; y < CODE_HEIGHT; ++y )
    {
        for ( x = 0; x < CODE_WIDTH; ++x )
        {
            if ( file.get( cmd ) )
            {
                if ( cmd != '\n' )
                    set_cmd_at( x, y, cmd );
                else
                    break;
            }
            else
            {
                done = true;
                break;
            }
        }

        if ( done )
            break;
    }

    file.close();
}

int32 VirtualMachine::Run( void )
{
	char cmd, prev_cmd;
	std::map<char, func_p>::iterator cmd_it;
	bool proceed, do_log = true;
	int32 result = 0;

	std::ofstream logfile;
	logfile.open( "logfile.txt", std::ios::out );

	while ( true )
	{
		prev_cmd = cmd;
		cmd = get_cmd_at( ip.pos.x, ip.pos.y );

		if ( do_log && logfile.is_open() )
		{
			if ( /*cmd != prev_cmd &&*/ cmd != ' ' )
				logfile << cmd << " pos: " << ip.pos.x << "," << ip.pos.y << " dir: " << ip.dir.x << "," << ip.dir.y << std::endl;
		}

		proceed = true;

		if ( cmd == '"' )
			string_mode = 1 - string_mode;
		else if ( string_mode )
			stack->Push( cmd );
		else if ( cmd == '@' )
			break;
		else
			if ( ( cmd_it = execute.find( cmd ) ) != execute.end() )
				proceed = ( this->*cmd_it->second )( cmd );

		if ( proceed )
			ip.Move();
	}

	logfile.close();

	return result;
}

char VirtualMachine::get_cmd_at( uint16 x, uint16 y )
{
	return code[x][y];
}
void VirtualMachine::set_cmd_at( uint16 x, uint16 y, char c )
{
	code[x][y] = c;
}
