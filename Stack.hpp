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
#include <type_traits>

template<typename T>
class Stack final
{
public:
   using ValueType = T;

private:
   std::stack<ValueType> m_oStack;

public:
   void push(const T& rkxValue)
   {
      m_oStack.push(rkxValue);
   }
   
   template<typename U = ValueType>
   U pop()
   {
      static_assert(std::is_convertible<T, U>::value, "T is not convertible to U");
      
      if (m_oStack.empty())
         return {};
      
      T xResult = m_oStack.top();
      
      m_oStack.pop();

      return static_cast<U>(xResult);
   }
   
   std::string popString()
   {
      std::string sString;

      char cValue;

      while ((cValue = pop<char>()) != 0)
         sString += static_cast<char>(cValue);

      return sString;
   }

   bool empty()
   {
      return m_oStack.empty();
   }

   void clear()
   {
      while (m_oStack.empty() == false)
         m_oStack.pop();
   }

   std::uint32_t size()
   {
      return static_cast<std::uint32_t>(m_oStack.size());
   }
};

#endif // STACK_HPP
