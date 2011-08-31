#include "Stack.h"

void Stack::Push( Stack::Value const value )
{
    m_stack.push( value );
}

Stack::Value Stack::Pop( void )
{
    Value result;

    if ( m_stack.size() > 0 )
    {
        result = m_stack.top();
        m_stack.pop();
    }
    else
        result = 0;

    return result;
}

std::string Stack::PopString( void )
{
	std::string str;

	Value val;
	while ( ( val = Pop() ) != 0 )
		str += char( val );

	return str;
}

void Stack::Clear( void )
{
	while ( !m_stack.empty() )
		m_stack.pop();
}

uint32 Stack::Size( void )
{
	return ( uint32 )( m_stack.size() );
}
