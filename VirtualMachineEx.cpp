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
{
#define MAP_COMMAND(CMD, FUNC) m_aCommands[CMD] = static_cast<CommandFunction>(&VirtualMachineEx::FUNC)

   MAP_COMMAND('c', cmdClearStack);
   MAP_COMMAND('x', cmdCoords);

   MAP_COMMAND('u', cmdConditionalMovementEx);
   MAP_COMMAND('d', cmdConditionalMovementEx);
   MAP_COMMAND('l', cmdConditionalMovementEx);
   MAP_COMMAND('r', cmdConditionalMovementEx);

   MAP_COMMAND('w', cmdConditionalMovementCompare);

   MAP_COMMAND('§', cmdConstantRandomMovement);

   MAP_COMMAND('[', cmdTurn);
   MAP_COMMAND(']', cmdTurn);

   MAP_COMMAND('t', cmdTurnAround);

   MAP_COMMAND('m', cmdMoveTo);

   MAP_COMMAND('S', cmdStackSize);

   MAP_COMMAND('A', cmdDigitsHex);
   MAP_COMMAND('B', cmdDigitsHex);
   MAP_COMMAND('C', cmdDigitsHex);
   MAP_COMMAND('D', cmdDigitsHex);
   MAP_COMMAND('E', cmdDigitsHex);
   MAP_COMMAND('F', cmdDigitsHex);

   MAP_COMMAND('{', cmdSubRoutine);
   MAP_COMMAND('}', cmdSubRoutine);

   MAP_COMMAND('=', cmdExecute);

#undef MAP_COMMAND
}

VirtualMachineEx::~VirtualMachineEx()
{
   //
}

bool VirtualMachineEx::cmdClearStack(char /*c*/)
{
   m_oStack.clear();

   return true;
}

bool VirtualMachineEx::cmdCoords(char /*c*/)
{
   m_oStack.push(m_oInstructionPointer.m_oPosition.x);
   m_oStack.push(m_oInstructionPointer.m_oPosition.y);

   return true;
}

bool VirtualMachineEx::cmdConditionalMovementEx(char c)
{
   if (!m_oStack.pop())
   {
      switch (c)
      {
      case 'u':
         m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionUp;
         break;

      case 'd':
         m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionDown;
         break;

      case 'l':
         m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionLeft;
         break;

      case 'r':
         m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionRight;
      }
   }

   return true;
}

bool VirtualMachineEx::cmdConditionalMovementCompare(char /*c*/)
{
   Stack::ValueType b = m_oStack.pop();
   Stack::ValueType a = m_oStack.pop();

   if (a < b)
      return cmdTurn('[');
   else if (a > b)
      return cmdTurn(']');
   else
      return true;
}

bool VirtualMachineEx::cmdConstantRandomMovement(char /*c*/)
{
   std::uint16_t randDir = std::rand() % 4;

   switch (randDir)
   {
   case 0:
      setCmdAt(m_oInstructionPointer.m_oPosition, '<');
      break;

   case 1:
      setCmdAt(m_oInstructionPointer.m_oPosition, '>');
      break;

   case 2:
      setCmdAt(m_oInstructionPointer.m_oPosition, 'v');
      break;

   case 3:
      setCmdAt(m_oInstructionPointer.m_oPosition, '^');
   }

   return false;
}

bool VirtualMachineEx::cmdTurn(char c)
{
   Vector<std::int16_t> oldDir = m_oInstructionPointer.m_oDirection;

   if (c == '[')
   {
      m_oInstructionPointer.m_oDirection.x =  oldDir.y;
      m_oInstructionPointer.m_oDirection.y = -oldDir.x;
   }
   else if (c == ']')
   {
      m_oInstructionPointer.m_oDirection.x = -oldDir.y;
      m_oInstructionPointer.m_oDirection.y =  oldDir.x;
   }

   return true;
}

bool VirtualMachineEx::cmdTurnAround(char /*c*/)
{
   m_oInstructionPointer.m_oDirection = -m_oInstructionPointer.m_oDirection;

   return true;
}

bool VirtualMachineEx::cmdMoveTo(char /*c*/)
{
   m_oInstructionPointer.m_oPosition.y = static_cast<std::uint16_t>(m_oStack.pop());
   m_oInstructionPointer.m_oPosition.x = static_cast<std::uint16_t>(m_oStack.pop());

   return false;
}

bool VirtualMachineEx::cmdStackSize(char /*c*/)
{
   m_oStack.push(Stack::ValueType(m_oStack.size()));

   return true;
}

bool VirtualMachineEx::cmdDigitsHex(char c)
{
   m_oStack.push(Stack::ValueType(c - 'A' + 10));

   return true;
}

bool VirtualMachineEx::cmdSubRoutine(char c)
{
   bool proceed = true;

   switch (c)
   {
   case '{':
      callstack.push(m_oInstructionPointer);

      m_oInstructionPointer.m_oPosition.y = static_cast<std::uint16_t>(m_oStack.pop());
      m_oInstructionPointer.m_oPosition.x = static_cast<std::uint16_t>(m_oStack.pop());

      proceed = false;

      break;

   case '}':
      if (!callstack.empty())
      {
         m_oInstructionPointer = callstack.top();

         callstack.pop();
      }
   }

   return proceed;
}

bool VirtualMachineEx::cmdExecute(char /*c*/)
{
   std::string cmd = m_oStack.popString();

   m_oStack.push(Stack::ValueType(std::system(cmd.c_str())));

   return true;
}

std::int32_t VirtualMachineEx::run()
{
   VirtualMachine::run();

   return std::int32_t(m_oStack.pop());
}
