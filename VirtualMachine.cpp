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

#include "VirtualMachine.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

void VirtualMachine::IP::move( void )
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
    stack( new Stack ), stringMode( false )
{
    std::srand( std::time( NULL ) );

    execute['<'] = &VirtualMachine::cmdMovement;
    execute['>'] = &VirtualMachine::cmdMovement;
    execute['v'] = &VirtualMachine::cmdMovement;
    execute['^'] = &VirtualMachine::cmdMovement;

    execute['_'] = &VirtualMachine::cmdConditionalMovement;
    execute['|'] = &VirtualMachine::cmdConditionalMovement;

    execute['?'] = &VirtualMachine::cmdRandomMovement;

    execute['+'] = &VirtualMachine::cmdMath;
    execute['-'] = &VirtualMachine::cmdMath;
    execute['*'] = &VirtualMachine::cmdMath;
    execute['/'] = &VirtualMachine::cmdMath;
    execute['%'] = &VirtualMachine::cmdMath;

    execute['!'] = &VirtualMachine::cmdLogical;
    execute['`'] = &VirtualMachine::cmdLogical;

    execute[':']  = &VirtualMachine::cmdStackManipulation;
    execute['\\'] = &VirtualMachine::cmdStackManipulation;
    execute['$']  = &VirtualMachine::cmdStackManipulation;

    execute['.'] = &VirtualMachine::cmdOutput;
    execute[','] = &VirtualMachine::cmdOutput;

    execute['&'] = &VirtualMachine::cmdInput;
    execute['~'] = &VirtualMachine::cmdInput;

    execute['#'] = &VirtualMachine::cmdSkip;

    execute['g'] = &VirtualMachine::cmdCodeManipulation;
    execute['p'] = &VirtualMachine::cmdCodeManipulation;

    execute['0'] = &VirtualMachine::cmdDigits;
    execute['1'] = &VirtualMachine::cmdDigits;
    execute['2'] = &VirtualMachine::cmdDigits;
    execute['3'] = &VirtualMachine::cmdDigits;
    execute['4'] = &VirtualMachine::cmdDigits;
    execute['5'] = &VirtualMachine::cmdDigits;
    execute['6'] = &VirtualMachine::cmdDigits;
    execute['7'] = &VirtualMachine::cmdDigits;
    execute['8'] = &VirtualMachine::cmdDigits;
    execute['9'] = &VirtualMachine::cmdDigits;
}

VirtualMachine::~VirtualMachine( void )
{
    delete stack;
}

bool VirtualMachine::cmdMovement( char c )
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

bool VirtualMachine::cmdConditionalMovement( char c )
{
    Stack::ValueType val = stack->pop();

    switch ( c )
    {
        case '_':
            ip.dir.y = 0; ip.dir.x = val ? -1 : 1; break;

        case '|':
            ip.dir.x = 0; ip.dir.y = val ? -1 : 1;
    }

    return true;
}

bool VirtualMachine::cmdRandomMovement( char c )
{
    short randDir = std::rand() % 4;

    switch ( randDir )
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

bool VirtualMachine::cmdMath( char c )
{
    Stack::ValueType b = stack->pop(), a = stack->pop();

    switch ( c )
    {
        case '+':
            stack->push( a + b ); break;

        case '-':
            stack->push( a - b ); break;

        case '*':
            stack->push( a * b ); break;

        case '/':
            stack->push( a / b ); break;

        case '%':
            stack->push( a % b );
    }

    return true;
}

bool VirtualMachine::cmdLogical( char c )
{
    switch ( c )
    {
        case '!':
            stack->push( !( stack->pop() ) ); break;

        case '`':
            stack->push( stack->pop() < stack->pop() );
    }

    return true;
}

bool VirtualMachine::cmdStackManipulation( char c )
{
    switch ( c )
    {
        case ':':
            {
                Stack::ValueType val = stack->pop();

                stack->push( val );
                stack->push( val );
            }
            break;

        case '\\':
            {
                Stack::ValueType a = stack->pop(),
                                 b = stack->pop();

                stack->push( a );
                stack->push( b );
            }
            break;

        case '$':
            stack->pop();
    }

    return true;
}

bool VirtualMachine::cmdOutput( char c )
{
    Stack::ValueType val = stack->pop();

    switch ( c )
    {
        case '.':
            std::cout << val << ' '; break;

        case ',':
            std::cout << char( val );
    }

    return true;
}

bool VirtualMachine::cmdInput( char c )
{
    switch ( c )
    {
        case '&':
            {
                std::string s;
                std::getline( std::cin, s );

                std::stringstream ss;
                ss << s;

                Stack::ValueType val;
                ss >> val;

                stack->push( val );
            }
            break;

        case '~':
            stack->push( std::getchar() );
    }

    return true;
}

bool VirtualMachine::cmdSkip( char c )
{
    ip.move();

    return true;
}

bool VirtualMachine::cmdCodeManipulation( char c )
{
    Stack::ValueType y = stack->pop(), x = stack->pop();

    switch ( c )
    {
        case 'g':
            stack->push( getCmdAt( x, y ) ); break;

        case 'p':
            setCmdAt( x, y, char( stack->pop() ) );
    }

    return true;
}

bool VirtualMachine::cmdDigits( char c )
{
    if ( std::isdigit( c ) )
        stack->push( Stack::ValueType( c - '0' ) );

    return true;
}

void VirtualMachine::loadCode( const std::string &filename )
{
    std::ifstream file;
    std::uint16_t x, y;
    char cmd;
    bool done = false;

    file.open( filename.c_str() );

    for ( y = 0; y < CODE_HEIGHT; ++y )
        for ( x = 0; x < CODE_WIDTH; ++x )
            setCmdAt( x, y, ' ' );

    for ( y = 0; y < CODE_HEIGHT; ++y )
    {
        for ( x = 0; x < CODE_WIDTH; ++x )
        {
            if ( file.get( cmd ) )
            {
                if ( cmd != '\n' )
                    setCmdAt( x, y, cmd );
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

std::int32_t VirtualMachine::run(void)
{
    char cmd = ' ', prevCmd;
    std::map<char, funcP>::iterator cmdIt;
    bool proceed, doLog = true;
    std::int32_t result = 0;

    std::ofstream logfile;
    logfile.open( "logfile.txt", std::ios::out );

    while ( true )
    {
        prevCmd = cmd;
        cmd = getCmdAt( ip.pos.x, ip.pos.y );

        if ( doLog && logfile.is_open() )
        {
            if ( /*cmd != prev_cmd &&*/ cmd != ' ' )
                logfile << cmd << " pos: " << ip.pos.x << "," << ip.pos.y << " dir: " << ip.dir.x << "," << ip.dir.y << std::endl;
        }

        proceed = true;

        if ( cmd == '"' )
            stringMode = 1 - stringMode;
        else if ( stringMode )
            stack->push( cmd );
        else if ( cmd == '@' )
            break;
        else
            if ( ( cmdIt = execute.find( cmd ) ) != execute.end() )
                proceed = ( this->*cmdIt->second )( cmd );

        if ( proceed )
            ip.move();
    }

    logfile.close();

    return result;
}

char VirtualMachine::getCmdAt( std::uint16_t x, std::uint16_t y)
{
    return code[x][y];
}
void VirtualMachine::setCmdAt( std::uint16_t x, std::uint16_t y, char c)
{
    code[x][y] = c;
}
