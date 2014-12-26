/*
RunFunge - A Befunge-93 compatible interpreter with extensions.
Copyright (C) 2009, 2011, 2014 Dienes

This file is part of RunFunge.

RunFunge is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see
<http://www.gnu.org/licenses/>.

Author E-Mail: dienes16 [at] googlemail [dot] com
*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <memory>

#include "VirtualMachineEx.hpp"

int main(int iArgCount, char **ppcArgValues)
{
   if (iArgCount <= 1)
   {
      std::cerr << "Usage: RunFunge.exe <file> [-bf93]";
      return EXIT_FAILURE;
   }

   std::unique_ptr<rf::VirtualMachine> upoVirtualMachine;

   if (iArgCount > 2 && !std::strcmp(ppcArgValues[2], "-bf93"))
      upoVirtualMachine = std::make_unique<rf::VirtualMachine>();
   else
      upoVirtualMachine = std::make_unique<rf::VirtualMachineEx>();

   upoVirtualMachine->loadCode(ppcArgValues[1]);

   return upoVirtualMachine->run();
}
