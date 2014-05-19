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

#ifndef VIRTUALMACHINE_HPP
#define VIRTUALMACHINE_HPP

#include <map>
#include <string>
#include <cstdint>

#include "Stack.hpp"

class VirtualMachine
{
protected:
   template<typename T>
   class Vector
   {
   public:
      T x;
      T y;

      Vector(T x, T y): x(x), y(y) { }
   };

   class IP
   {
   public:
      Vector<std::int16_t> pos;
      Vector<std::int16_t> dir;

      IP(): pos(0, 0), dir(1, 0) { }

      void move();
   };

protected:
   static const std::int16_t CODE_WIDTH = 80, CODE_HEIGHT = 25;

   char code[CODE_WIDTH][CODE_HEIGHT];

protected:
   typedef bool (VirtualMachine::*funcP)(char);

   bool cmdMovement(char c);
   bool cmdConditionalMovement(char c);
   bool cmdRandomMovement(char c);
   bool cmdMath(char c);
   bool cmdLogical(char c);
   bool cmdStackManipulation(char c);
   bool cmdOutput(char c);
   bool cmdInput(char c);
   bool cmdSkip(char c);
   bool cmdCodeManipulation(char c);
   bool cmdDigits(char c);

   std::map<char, funcP> execute;

   Stack* stack;

   IP ip;

   bool stringMode;

   char getCmdAt(std::uint16_t x, std::uint16_t y);
   void setCmdAt(std::uint16_t x, std::uint16_t y, char c);

public:
   VirtualMachine();
   virtual ~VirtualMachine();

   virtual void loadCode(const std::string& filename);

   virtual std::int32_t run();
};

#endif // VIRTUALMACHINE_HPP
