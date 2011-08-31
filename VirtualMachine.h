/*
 * RunFunge - A Befunge-93 compatible interpreter with extensions.
 * Copyright (C) 2009, 2011 Dienes
 *
 * This file is part of RunFunge.
 *
 * RunFunge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author E-Mail: dienes16 [at] googlemail [dot] com
*/

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <map>
#include <string>

#include "Stack.h"
#include "Types.h"

class VirtualMachine
{
    protected:
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

                void move( void );
        };

    protected:
        static const int16      CODE_WIDTH = 80, CODE_HEIGHT = 25;

        char                    code[CODE_WIDTH][CODE_HEIGHT];

    protected:
        typedef bool ( VirtualMachine::*funcP )( char );

        bool                    cmdMovement( char c );
        bool                    cmdConditionalMovement( char c );
        bool                    cmdRandomMovement( char c );
        bool                    cmdMath( char c );
        bool                    cmdLogical( char c );
        bool                    cmdStackManipulation( char c );
        bool                    cmdOutput( char c );
        bool                    cmdInput( char c );
        bool                    cmdSkip( char c );
        bool                    cmdCodeManipulation( char c );
        bool                    cmdDigits( char c );

        std::map<char, funcP>   execute;

        Stack                   *stack;

        IP                      ip;

        bool                    stringMode;

        char                    getCmdAt( uint16 x, uint16 y );
        void                    setCmdAt( uint16 x, uint16 y, char c );

    public:
                                VirtualMachine( void );
        virtual                 ~VirtualMachine( void );

        virtual void            loadCode( const std::string &filename );

        virtual int32           run( void );
};

#endif // VIRTUALMACHINE_H
