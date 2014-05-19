/*
RunFunge - A Befunge-93 compatible interpreter with extensions.
Copyright (C) 2009, 2011, 2014 Dienes

This file is part of RunFunge.

RunFunge is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Author E-Mail: dienes16 [at] googlemail [dot] com
*/

#include "Stack.hpp"

void Stack::push( Stack::ValueType const value )
{
    stack.push( value );
}

Stack::ValueType Stack::pop( void )
{
    ValueType result;

    if ( stack.size() > 0 )
    {
        result = stack.top();
        stack.pop();
    }
    else
        result = 0;

    return result;
}

std::string Stack::popString( void )
{
    std::string str;

    ValueType val;
    while ( ( val = pop() ) != 0 )
        str += char( val );

    return str;
}

void Stack::clear( void )
{
    while ( !stack.empty() )
        stack.pop();
}

std::uint32_t Stack::size(void)
{
    return ( std::uint32_t )(stack.size());
}
