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

#ifndef STACK_IPP
#define STACK_IPP

#include <type_traits>

template<typename T>
inline void Stack<T>::push(const T& rkxValue)
{
   m_oStack.push(rkxValue);
}

template<typename T>
template<typename U>
U Stack<T>::pop()
{
   static_assert(std::is_convertible<T, U>::value, "T is not convertible to U");

   if (m_oStack.empty())
      return {};

   T xResult = m_oStack.top();

   m_oStack.pop();

   return static_cast<U>(xResult);
}

template<typename T>
std::string Stack<T>::popString()
{
   std::string sString;

   char cValue;

   while ((cValue = pop<char>()) != 0)
      sString += static_cast<char>(cValue);

   return sString;
}

template<typename T>
inline bool Stack<T>::isEmpty()
{
   return m_oStack.empty();
}

template<typename T>
inline void Stack<T>::clear()
{
   while (m_oStack.empty() == false)
      m_oStack.pop();
}

template<typename T>
inline std::uint32_t Stack<T>::getSize()
{
   return static_cast<std::uint32_t>(m_oStack.size());
}

#endif // STACK_IPP
