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

#ifndef VIRTUALMACHINEEX_H
#define VIRTUALMACHINEEX_H

#include "VirtualMachine.hpp"

#include <stack>

class VirtualMachineEx: public VirtualMachine
{
    protected:

        bool            cmdClearStack( char c );
        bool            cmdCoords( char c );
        bool            cmdConditionalMovementEx( char c );
        bool            cmdConditionalMovementCompare( char c );
        bool            cmdConstantRandomMovement( char c );
        bool            cmdTurn( char c );
        bool            cmdTurnAround( char c );
        bool            cmdMoveTo( char c );
        bool            cmdSwapStack( char c );
        bool            cmdStackSize( char c );
        bool            cmdSwapStackTop( char c );
        bool            cmdDigitsHex( char c );
        bool            cmdSubRoutine( char c );
        bool            cmdExecute( char c );

        Stack           *stack1, *stack2;

        std::stack<IP>  callstack;

    public:
                        VirtualMachineEx( void );
                        ~VirtualMachineEx( void );

        int32           run( void );
};

#endif // VIRTUALMACHINEEX_H
