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

#include <stdexcept>

void Stack::push(const Stack::ValueType kiValue)
{
   m_oStack.push(kiValue);
}

std::string Stack::popString()
{
   std::string sString;

   ValueType iValue;

   while ((iValue = pop()) != 0)
   {
      if (iValue > 255)
         throw std::runtime_error("Stack value is not a byte");

      sString += static_cast<char>(iValue);
   }

   return sString;
}

void Stack::clear()
{
   while (m_oStack.empty() == false)
      m_oStack.pop();
}

std::uint32_t Stack::size()
{
   return static_cast<std::uint32_t>(m_oStack.size());
}
