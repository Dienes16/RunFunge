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

#ifndef STACK_HPP
#define STACK_HPP

#include <stack>
#include <string>
#include <cstdint>

class Stack final
{
public:
   using ValueType = std::int64_t;

private:
   std::stack<ValueType> m_oStack;

public:
   void push(const ValueType kiValue);
   
   ValueType pop();
   
   std::string popString();

   void clear();

   std::uint32_t size();
};

#endif // STACK_HPP
