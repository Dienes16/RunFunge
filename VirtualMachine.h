#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <map>
#include <string>

#include "Stack.h"
#include "Types.h"

#define CODE_WIDTH	80
#define CODE_HEIGHT	25

template<typename T>
class Vector
{
	public:
		T x, y;

		Vector( T x, T y ): x( x ), y( y ) { }
};

class IP
{
	public:
		Vector<int16> pos;
		Vector<int16> dir;

		IP( void ): pos( 0, 0 ), dir( 1, 0 ) { }

		void Move( void );
};

class VirtualMachine
{
	private:
		char					code[CODE_WIDTH][CODE_HEIGHT];

	protected:
		typedef bool ( VirtualMachine::*func_p )( char );

		bool					_movement( char c );
		bool					_conditional_movement( char c );
		bool					_random_movement( char c );
		bool					_math( char c );
		bool					_logical( char c );
		bool					_stack_manipulation( char c );
		bool					_output( char c );
		bool					_input( char c );
		bool					_skip( char c );
		bool					_code_manipulation( char c );
		bool					_digits( char c );

		std::map<char, func_p>	execute;

		Stack					*stack;

		IP						ip;

		bool					string_mode;

		virtual char			get_cmd_at( uint16 x, uint16 y );
		virtual void			set_cmd_at( uint16 x, uint16 y, char c );

	public:
		/*ctor*/				VirtualMachine( void );
		/*dtor*/ virtual		~VirtualMachine( void );

		virtual void			LoadCode( const std::string &filename );

		virtual int32			Run( void );
};

#endif // VIRTUALMACHINE_H
