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

#ifndef STACK_HPP
#define STACK_HPP

#include <stack>
#include <string>
#include <cstdint>

namespace rf
{
template<typename T>
class Stack final
{
public:
   using ValueType = T;

private:
   std::stack<ValueType> m_oStack;

public:
   inline void push(const T& rkxValue);
   
   template<typename U = T>
   U pop();

   std::string popString();

   inline bool isEmpty();

   inline void clear();

   inline std::uint32_t getSize();
};
} // namespace rf

#ifndef STACK_HPP_NOIMPL
#   include "Stack.ipp"
#endif // STACK_HPP_NOIMPL

#endif // STACK_HPP
