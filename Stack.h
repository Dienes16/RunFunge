#ifndef STACK_H
#define STACK_H

#include <stack>
#include <string>

#include "Types.h"

class Stack
{
	public:
		typedef int64			Value;

	private:
		std::stack<Value>		m_stack;

	public:
		void					Push( Value const value );
		Value					Pop( void );
		std::string				PopString( void );

		void					Clear( void );

		uint32					Size( void );
};

#endif // STACK_H
