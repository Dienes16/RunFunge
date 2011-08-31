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
*/

#include <iostream>
#include <cstring>
#include <cstdlib>

#include "VirtualMachineEx.h"

int main( int argc, char **argv )
{
    if ( argc <= 1 )
    {
        std::cerr << "Usage: RunFunge.exe <file> [-bf93]";
        return EXIT_FAILURE;
    }

    VirtualMachine* virtualMachine;

    if ( argc > 2 && !std::strcmp( argv[2], "-bf93" ) )
        virtualMachine = new VirtualMachine;
    else
        virtualMachine = new VirtualMachineEx;

    virtualMachine->loadCode( argv[1] );

    int result = virtualMachine->run();

    delete virtualMachine;

    return result;
}
