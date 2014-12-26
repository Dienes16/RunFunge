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

#include "VirtualMachine.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cctype>

namespace rf
{
const VirtualMachine::InstructionPointer::DirectionVectorType VirtualMachine::InstructionPointer::ms_koDirectionUp   { 0, -1};
const VirtualMachine::InstructionPointer::DirectionVectorType VirtualMachine::InstructionPointer::ms_koDirectionDown { 0,  1};
const VirtualMachine::InstructionPointer::DirectionVectorType VirtualMachine::InstructionPointer::ms_koDirectionLeft {-1,  0};
const VirtualMachine::InstructionPointer::DirectionVectorType VirtualMachine::InstructionPointer::ms_koDirectionRight{ 1,  0};

void VirtualMachine::InstructionPointer::advance()
{
   m_oPosition.x += m_oDirection.x;
   if (m_oPosition.x >= ms_ki16CodeWidth)
      m_oPosition.x = 0;
   else if (m_oPosition.x < 0)
      m_oPosition.x = ms_ki16CodeWidth - 1;

   m_oPosition.y += m_oDirection.y;
   if (m_oPosition.y >= ms_ki16CodeHeight)
      m_oPosition.y = 0;
   else if (m_oPosition.y < 0)
      m_oPosition.y = ms_ki16CodeHeight - 1;
}

VirtualMachine::VirtualMachine()
   : m_bStringMode(false)
{
   std::srand(static_cast<unsigned int>(std::time(NULL)));

#define MAP_COMMAND(CMD, FUNC) m_amfpCommands[CMD] = &VirtualMachine::FUNC

   MAP_COMMAND('<', cmdMovement);
   MAP_COMMAND('>', cmdMovement);
   MAP_COMMAND('v', cmdMovement);
   MAP_COMMAND('^', cmdMovement);

   MAP_COMMAND('_', cmdConditionalMovement);
   MAP_COMMAND('|', cmdConditionalMovement);

   MAP_COMMAND('?', cmdRandomMovement);

   MAP_COMMAND('+', cmdMath);
   MAP_COMMAND('-', cmdMath);
   MAP_COMMAND('*', cmdMath);
   MAP_COMMAND('/', cmdMath);
   MAP_COMMAND('%', cmdMath);

   MAP_COMMAND('!', cmdLogical);
   MAP_COMMAND('`', cmdLogical);

   MAP_COMMAND(':', cmdStackManipulation);
   MAP_COMMAND('\\', cmdStackManipulation);
   MAP_COMMAND('$', cmdStackManipulation);

   MAP_COMMAND('.', cmdOutput);
   MAP_COMMAND(',', cmdOutput);

   MAP_COMMAND('&', cmdInput);
   MAP_COMMAND('~', cmdInput);

   MAP_COMMAND('#', cmdSkip);

   MAP_COMMAND('g', cmdCodeManipulation);
   MAP_COMMAND('p', cmdCodeManipulation);

   MAP_COMMAND('0', cmdDigits);
   MAP_COMMAND('1', cmdDigits);
   MAP_COMMAND('2', cmdDigits);
   MAP_COMMAND('3', cmdDigits);
   MAP_COMMAND('4', cmdDigits);
   MAP_COMMAND('5', cmdDigits);
   MAP_COMMAND('6', cmdDigits);
   MAP_COMMAND('7', cmdDigits);
   MAP_COMMAND('8', cmdDigits);
   MAP_COMMAND('9', cmdDigits);

#undef MAP_COMMAND
}

VirtualMachine::~VirtualMachine()
{
   //
}

bool VirtualMachine::cmdMovement(char c)
{
   switch (c)
   {
   case '<':
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionLeft;
      break;

   case '>':
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionRight;
      break;

   case 'v':
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionDown;
      break;

   case '^':
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionUp;
   }

   return true;
}

bool VirtualMachine::cmdConditionalMovement(char c)
{
   StackValueType xValue = m_oStack.pop();

   switch (c)
   {
   case '_':
      m_oInstructionPointer.m_oDirection = (xValue) ? InstructionPointer::ms_koDirectionLeft : InstructionPointer::ms_koDirectionRight;
      break;

   case '|':
      m_oInstructionPointer.m_oDirection = (xValue) ? InstructionPointer::ms_koDirectionUp : InstructionPointer::ms_koDirectionDown;
   }

   return true;
}

bool VirtualMachine::cmdRandomMovement(char /*c*/)
{
   short randDir = std::rand() % 4;

   switch (randDir)
   {
   case 0:
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionLeft;
      break;

   case 1:
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionRight;
      break;

   case 2:
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionDown;
      break;

   case 3:
      m_oInstructionPointer.m_oDirection = InstructionPointer::ms_koDirectionUp;
   }

   return true;
}

bool VirtualMachine::cmdMath(char c)
{
   StackValueType xB = m_oStack.pop();
   StackValueType xA = m_oStack.pop();

   switch (c)
   {
   case '+':
      m_oStack.push(xA + xB);
      break;

   case '-':
      m_oStack.push(xA - xB);
      break;

   case '*':
      m_oStack.push(xA * xB);
      break;

   case '/':
      m_oStack.push(xA / xB);
      break;

   case '%':
      m_oStack.push(xA % xB);
   }

   return true;
}

bool VirtualMachine::cmdLogical(char c)
{
   switch (c)
   {
   case '!':
      m_oStack.push(!(m_oStack.pop()));
      break;

   case '`':
      m_oStack.push(m_oStack.pop() < m_oStack.pop());
   }

   return true;
}

bool VirtualMachine::cmdStackManipulation(char c)
{
   switch (c)
   {
   case ':':
   {
      StackValueType xValue = m_oStack.pop();

      m_oStack.push(xValue);
      m_oStack.push(xValue);
   }
      break;

   case '\\':
   {
      StackValueType xA = m_oStack.pop();
      StackValueType xB = m_oStack.pop();

      m_oStack.push(xA);
      m_oStack.push(xB);
   }
      break;

   case '$':
      m_oStack.pop();
   }

   return true;
}

bool VirtualMachine::cmdOutput(char c)
{
   switch (c)
   {
   case '.':
      std::cout << m_oStack.pop() << ' ';
      break;

   case ',':
      std::cout << m_oStack.pop<char>();
   }

   return true;
}

bool VirtualMachine::cmdInput(char c)
{
   switch (c)
   {
   case '&':
   {
      std::string sNumber;
      std::getline(std::cin, sNumber);

      m_oStack.push(std::stoll(sNumber));
   }
      break;

   case '~':
      m_oStack.push(std::getchar());
   }

   return true;
}

bool VirtualMachine::cmdSkip(char /*c*/)
{
   m_oInstructionPointer.advance();

   return true;
}

bool VirtualMachine::cmdCodeManipulation(char c)
{
   InstructionPointer::PositionVectorType oPosition;
   
   oPosition.y = m_oStack.pop<InstructionPointer::PositionVectorType::ValueType>();
   oPosition.x = m_oStack.pop<InstructionPointer::PositionVectorType::ValueType>();

   switch (c)
   {
   case 'g':
      m_oStack.push(getCmdAt(oPosition));
      break;

   case 'p':
      setCmdAt(oPosition, m_oStack.pop<char>());
   }

   return true;
}

bool VirtualMachine::cmdDigits(char c)
{
   if (std::isdigit(c))
      m_oStack.push(static_cast<StackValueType>(c - '0'));

   return true;
}

void VirtualMachine::loadCode(const std::string& rksFilename)
{
   std::ifstream oFile;

   oFile.open(rksFilename.c_str());

   InstructionPointer::PositionVectorType oPosition;

   for (oPosition.y = 0; oPosition.y < ms_ki16CodeHeight; ++oPosition.y)
      for (oPosition.x = 0; oPosition.x < ms_ki16CodeWidth; ++oPosition.x)
         setCmdAt(oPosition, ' ');

   char cCmd;

   for (oPosition.y = 0; oPosition.y < ms_ki16CodeHeight; ++oPosition.y)
   {
      for (oPosition.x = 0; oPosition.x < ms_ki16CodeWidth; ++oPosition.x)
      {
         if (oFile.get(cCmd))
         {
            if (cCmd != '\n')
               setCmdAt(oPosition, cCmd);
            else
               break;
         }
         else
         {
            goto DONE; // Break both loops (no raptors involved)
         }
      }
   }
   DONE:

   oFile.close();
}

int VirtualMachine::run()
{
   std::ofstream oLogfile;
   oLogfile.open("oLogfile.txt", std::ios::out);

   const bool kbLog = true;

   for (;;)
   {
      char cCmd = getCmdAt(m_oInstructionPointer.m_oPosition);

      if (kbLog && oLogfile.is_open())
      {
         if ( cCmd != ' ')
            oLogfile << cCmd << " pos: " << m_oInstructionPointer.m_oPosition.x << "," << m_oInstructionPointer.m_oPosition.y << " dir: " << m_oInstructionPointer.m_oDirection.x << "," << m_oInstructionPointer.m_oDirection.y << std::endl;
      }

      bool bProceed = true;

      if (cCmd == '"')
      {
         m_bStringMode = !m_bStringMode;
      }
      else if (m_bStringMode)
      {
         m_oStack.push(cCmd);
      }
      else if (cCmd == '@')
      {
         break;
      }
      else
      {
         std::map<char, CommandFunction>::iterator cmdIt;

         if ((cmdIt = m_amfpCommands.find(cCmd)) != m_amfpCommands.end())
            bProceed = (this->*cmdIt->second)(cCmd);
      }

      if (bProceed)
         m_oInstructionPointer.advance();
   }

   oLogfile.close();

   return 0;
}

char VirtualMachine::getCmdAt(const InstructionPointer::PositionVectorType& rkoPosition)
{
   return m_acCode[rkoPosition.x][rkoPosition.y];
}

void VirtualMachine::setCmdAt(const InstructionPointer::PositionVectorType& rkoPosition, char c)
{
   m_acCode[rkoPosition.x][rkoPosition.y] = c;
}
} // namespace rf
