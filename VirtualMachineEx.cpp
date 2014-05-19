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

#include "VirtualMachineEx.hpp"

#include <cmath>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cctype>

VirtualMachineEx::VirtualMachineEx()
   : VirtualMachine()
   , stack1(stack)
   , stack2(new Stack)
{
   execute['c'] = funcP(&VirtualMachineEx::cmdClearStack);
   execute['x'] = funcP(&VirtualMachineEx::cmdCoords);

   execute['u'] = funcP(&VirtualMachineEx::cmdConditionalMovementEx);
   execute['d'] = funcP(&VirtualMachineEx::cmdConditionalMovementEx);
   execute['l'] = funcP(&VirtualMachineEx::cmdConditionalMovementEx);
   execute['r'] = funcP(&VirtualMachineEx::cmdConditionalMovementEx);

   execute['w'] = funcP(&VirtualMachineEx::cmdConditionalMovementCompare);

   execute['§'] = funcP(&VirtualMachineEx::cmdConstantRandomMovement);

   execute['['] = funcP(&VirtualMachineEx::cmdTurn);
   execute[']'] = funcP(&VirtualMachineEx::cmdTurn);

   execute['t'] = funcP(&VirtualMachineEx::cmdTurnAround);

   execute['m'] = funcP(&VirtualMachineEx::cmdMoveTo);

   execute['s'] = funcP(&VirtualMachineEx::cmdSwapStack);

   execute['S'] = funcP(&VirtualMachineEx::cmdStackSize);

   execute[';'] = funcP(&VirtualMachineEx::cmdSwapStackTop);

   execute['A'] = funcP(&VirtualMachineEx::cmdDigitsHex);
   execute['B'] = funcP(&VirtualMachineEx::cmdDigitsHex);
   execute['C'] = funcP(&VirtualMachineEx::cmdDigitsHex);
   execute['D'] = funcP(&VirtualMachineEx::cmdDigitsHex);
   execute['E'] = funcP(&VirtualMachineEx::cmdDigitsHex);
   execute['F'] = funcP(&VirtualMachineEx::cmdDigitsHex);

   execute['{'] = funcP(&VirtualMachineEx::cmdSubRoutine);
   execute['}'] = funcP(&VirtualMachineEx::cmdSubRoutine);

   execute['='] = funcP(&VirtualMachineEx::cmdExecute);
}

VirtualMachineEx::~VirtualMachineEx()
{
   delete stack2;
}

bool VirtualMachineEx::cmdClearStack(char c)
{
   stack->clear();

   return true;
}

bool VirtualMachineEx::cmdCoords(char c)
{
   stack->push(ip.pos.x);
   stack->push(ip.pos.y);

   return true;
}

bool VirtualMachineEx::cmdConditionalMovementEx(char c)
{
   if (!stack->pop())
   {
      switch (c)
      {
      case 'u':
         ip.dir.x =  0;
         ip.dir.y = -1;
         break;

      case 'd':
         ip.dir.x = 0;
         ip.dir.y = 1;
         break;

      case 'l':
         ip.dir.x = -1;
         ip.dir.y =  0;
         break;

      case 'r':
         ip.dir.x = 1;
         ip.dir.y = 0;
      }
   }

   return true;
}

bool VirtualMachineEx::cmdConditionalMovementCompare(char c)
{
   Stack::ValueType b = stack->pop();
   Stack::ValueType a = stack->pop();

   if (a < b)
      return cmdTurn('[');
   else if (a > b)
      return cmdTurn(']');
   else
      return true;
}

bool VirtualMachineEx::cmdConstantRandomMovement(char c)
{
   std::uint16_t randDir = std::rand() % 4;

   switch (randDir)
   {
   case 0:
      setCmdAt(ip.pos.x, ip.pos.y, '<');
      break;

   case 1:
      setCmdAt(ip.pos.x, ip.pos.y, '>');
      break;

   case 2:
      setCmdAt(ip.pos.x, ip.pos.y, 'v');
      break;

   case 3:
      setCmdAt(ip.pos.x, ip.pos.y, '^');
   }

   return false;
}

bool VirtualMachineEx::cmdTurn(char c)
{
   Vector<std::int16_t> oldDir = ip.dir;

   if (c == '[')
   {
      ip.dir.x =  oldDir.y;
      ip.dir.y = -oldDir.x;
   }
   else if (c == ']')
   {
      ip.dir.x = -oldDir.y;
      ip.dir.y =  oldDir.x;
   }

   return true;
}

bool VirtualMachineEx::cmdTurnAround(char c)
{
   ip.dir.x *= -1;
   ip.dir.y *= -1;

   return true;
}

bool VirtualMachineEx::cmdMoveTo(char c)
{
   ip.pos.y = stack->pop();
   ip.pos.x = stack->pop();

   return false;
}

bool VirtualMachineEx::cmdSwapStack(char c)
{
   stack = (stack == stack1) ? stack2 : stack1;

   return true;
}

bool VirtualMachineEx::cmdStackSize(char c)
{
   stack->push(Stack::ValueType(stack->size()));

   return true;
}

bool VirtualMachineEx::cmdSwapStackTop(char c)
{
   Stack* inactiveStack = (stack == stack1) ? stack2 : stack1;

   inactiveStack->push(stack->pop());

   return true;
}

bool VirtualMachineEx::cmdDigitsHex(char c)
{
   stack->push(Stack::ValueType(c - 'A' + 10));

   return true;
}

bool VirtualMachineEx::cmdSubRoutine(char c)
{
   bool proceed = true;

   switch (c)
   {
   case '{':
      callstack.push(ip);

      ip.pos.y = stack->pop();
      ip.pos.x = stack->pop();

      proceed = false;

      break;

   case '}':
      if (!callstack.empty())
      {
         ip = callstack.top();

         callstack.pop();
      }
   }

   return proceed;
}

bool VirtualMachineEx::cmdExecute(char c)
{
   std::string cmd = stack->popString();

   stack->push(Stack::ValueType(std::system(cmd.c_str())));

   return true;
}

std::int32_t VirtualMachineEx::run()
{
   VirtualMachine::run();

   return std::int32_t(stack->pop());
}
