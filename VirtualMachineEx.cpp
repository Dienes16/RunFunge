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

#include "VirtualMachineEx.hpp"

#include <cmath>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cctype>

namespace rf
{
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
   StackValueType xB = m_oStack.pop();
   StackValueType xA = m_oStack.pop();

   if (xA < xB)
      return cmdTurn('[');
   else if (xA > xB)
      return cmdTurn(']');
   else
      return true;
}

bool VirtualMachineEx::cmdConstantRandomMovement(char /*c*/)
{
   int iRandomDirection = std::rand() % 4;

   switch (iRandomDirection)
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
   InstructionPointer::DirectionVectorType oPrevDir = m_oInstructionPointer.m_oDirection;

   switch (c)
   {
   case '[':
      m_oInstructionPointer.m_oDirection.x =  oPrevDir.y;
      m_oInstructionPointer.m_oDirection.y = -oPrevDir.x;
      break;

   case ']':
      m_oInstructionPointer.m_oDirection.x = -oPrevDir.y;
      m_oInstructionPointer.m_oDirection.y =  oPrevDir.x;
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
   m_oInstructionPointer.m_oPosition.y = m_oStack.pop<InstructionPointer::PositionVectorType::ValueType>();
   m_oInstructionPointer.m_oPosition.x = m_oStack.pop<InstructionPointer::PositionVectorType::ValueType>();

   return false;
}

bool VirtualMachineEx::cmdStackSize(char /*c*/)
{
   m_oStack.push(static_cast<StackValueType>(m_oStack.getSize()));

   return true;
}

bool VirtualMachineEx::cmdDigitsHex(char c)
{
   m_oStack.push(static_cast<StackValueType>(c - 'A' + 10));

   return true;
}

bool VirtualMachineEx::cmdSubRoutine(char c)
{
   bool bProceed = true;

   switch (c)
   {
   case '{':
      m_oCallstack.push(m_oInstructionPointer);

      m_oInstructionPointer.m_oPosition.y = m_oStack.pop<InstructionPointer::PositionVectorType::ValueType>();
      m_oInstructionPointer.m_oPosition.x = m_oStack.pop<InstructionPointer::PositionVectorType::ValueType>();

      bProceed = false;

      break;

   case '}':
      if (!m_oCallstack.isEmpty())
         m_oInstructionPointer = m_oCallstack.pop();
   }

   return bProceed;
}

bool VirtualMachineEx::cmdExecute(char /*c*/)
{
   std::string sCommand = m_oStack.popString();

   m_oStack.push(StackValueType(std::system(sCommand.c_str())));

   return true;
}

int VirtualMachineEx::run()
{
   VirtualMachine::run();

   return m_oStack.pop<int>();
}
} // namespace rf
