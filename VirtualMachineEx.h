#ifndef VIRTUALMACHINEEX_H
#define VIRTUALMACHINEEX_H

#include "VirtualMachine.h"

#include <stack>
#include <map>

#include "Types.h"

class VirtualMachineEx: public VirtualMachine
{
	private:
		//std::map<uint16, std::map<uint16, char> > code;
		char						code[CODE_WIDTH][CODE_HEIGHT];

	protected:

		bool						_clear_stack( char c );
		bool						_coords( char c );
		bool						_conditional_movement_ex( char c );
		bool						_conditional_movement_compare( char c );
		bool						_constant_random_movement( char c );
		bool						_turn( char c );
		bool						_turn_around( char c );
		bool						_move_to( char c );
		bool						_swap_stack( char c );
		bool						_stack_size( char c );
		bool						_swap_stack_top( char c );
		bool						_digits_hex( char c );
		bool						_function( char c );
		bool						_execute( char c );

		Stack						*stack1, *stack2;

		std::stack<IP>				callstack;

		//virtual char				get_cmd_at( uint16 x, uint16 y );
		//virtual void				set_cmd_at( uint16 x, uint16 y, char c );

	public:
		/*ctor*/					VirtualMachineEx( void );
		/*dtor*/ virtual			~VirtualMachineEx( void );

		//virtual void				LoadCode( const std::string &filename );

		virtual int32				Run( void );
};

#endif // VIRTUALMACHINEEX_H
