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

int main(int iArgCount, char **asz8ArgValues)
{
   if (iArgCount <= 1)
   {
      std::cerr << "Usage: RunFunge.exe <file> [-bf93]";
      return EXIT_FAILURE;
   }

   const std::string ksScriptFilename = asz8ArgValues[1];

   enum class Mode
   {
      BF93,
      EXT
   };

   Mode eMode = Mode::EXT;

   if (iArgCount > 2)
   {
      if (std::strcmp(asz8ArgValues[2], "-bf93") == false)
      {
         eMode = Mode::BF93;
      }
   }   

   const std::unique_ptr<rf::VirtualMachine> kupoVirtualMachine =
      (eMode == Mode::BF93) ? std::make_unique<rf::VirtualMachine  >()
                            : std::make_unique<rf::VirtualMachineEx>();

   kupoVirtualMachine->loadCode(ksScriptFilename);

   return kupoVirtualMachine->run();
}
