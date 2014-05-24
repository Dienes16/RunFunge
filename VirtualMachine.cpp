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

#include "VirtualMachine.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

const VirtualMachine::InstructionPointer::VectorType VirtualMachine::InstructionPointer::ms_koDirectionUp   { 0, -1};
const VirtualMachine::InstructionPointer::VectorType VirtualMachine::InstructionPointer::ms_koDirectionDown { 0,  1};
const VirtualMachine::InstructionPointer::VectorType VirtualMachine::InstructionPointer::ms_koDirectionLeft {-1,  0};
const VirtualMachine::InstructionPointer::VectorType VirtualMachine::InstructionPointer::ms_koDirectionRight{ 1,  0};

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

   m_aCommands['<'] = &VirtualMachine::cmdMovement;
   m_aCommands['>'] = &VirtualMachine::cmdMovement;
   m_aCommands['v'] = &VirtualMachine::cmdMovement;
   m_aCommands['^'] = &VirtualMachine::cmdMovement;

   m_aCommands['_'] = &VirtualMachine::cmdConditionalMovement;
   m_aCommands['|'] = &VirtualMachine::cmdConditionalMovement;

   m_aCommands['?'] = &VirtualMachine::cmdRandomMovement;

   m_aCommands['+'] = &VirtualMachine::cmdMath;
   m_aCommands['-'] = &VirtualMachine::cmdMath;
   m_aCommands['*'] = &VirtualMachine::cmdMath;
   m_aCommands['/'] = &VirtualMachine::cmdMath;
   m_aCommands['%'] = &VirtualMachine::cmdMath;

   m_aCommands['!'] = &VirtualMachine::cmdLogical;
   m_aCommands['`'] = &VirtualMachine::cmdLogical;

   m_aCommands[':']  = &VirtualMachine::cmdStackManipulation;
   m_aCommands['\\'] = &VirtualMachine::cmdStackManipulation;
   m_aCommands['$']  = &VirtualMachine::cmdStackManipulation;

   m_aCommands['.'] = &VirtualMachine::cmdOutput;
   m_aCommands[','] = &VirtualMachine::cmdOutput;

   m_aCommands['&'] = &VirtualMachine::cmdInput;
   m_aCommands['~'] = &VirtualMachine::cmdInput;

   m_aCommands['#'] = &VirtualMachine::cmdSkip;

   m_aCommands['g'] = &VirtualMachine::cmdCodeManipulation;
   m_aCommands['p'] = &VirtualMachine::cmdCodeManipulation;

   m_aCommands['0'] = &VirtualMachine::cmdDigits;
   m_aCommands['1'] = &VirtualMachine::cmdDigits;
   m_aCommands['2'] = &VirtualMachine::cmdDigits;
   m_aCommands['3'] = &VirtualMachine::cmdDigits;
   m_aCommands['4'] = &VirtualMachine::cmdDigits;
   m_aCommands['5'] = &VirtualMachine::cmdDigits;
   m_aCommands['6'] = &VirtualMachine::cmdDigits;
   m_aCommands['7'] = &VirtualMachine::cmdDigits;
   m_aCommands['8'] = &VirtualMachine::cmdDigits;
   m_aCommands['9'] = &VirtualMachine::cmdDigits;
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
   Stack::ValueType val = m_oStack.pop();

   switch (c)
   {
   case '_':
      m_oInstructionPointer.m_oDirection = (val) ? InstructionPointer::ms_koDirectionLeft : InstructionPointer::ms_koDirectionRight;
      break;

   case '|':
      m_oInstructionPointer.m_oDirection = (val) ? InstructionPointer::ms_koDirectionUp : InstructionPointer::ms_koDirectionDown;
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
   Stack::ValueType b = m_oStack.pop();
   Stack::ValueType a = m_oStack.pop();

   switch (c)
   {
   case '+':
      m_oStack.push(a + b);
      break;

   case '-':
      m_oStack.push(a - b);
      break;

   case '*':
      m_oStack.push(a * b);
      break;

   case '/':
      m_oStack.push(a / b);
      break;

   case '%':
      m_oStack.push(a % b);
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
      Stack::ValueType val = m_oStack.pop();

      m_oStack.push(val);
      m_oStack.push(val);
   }
      break;

   case '\\':
   {
      Stack::ValueType a = m_oStack.pop();
      Stack::ValueType b = m_oStack.pop();

      m_oStack.push(a);
      m_oStack.push(b);
   }
      break;

   case '$':
      m_oStack.pop();
   }

   return true;
}

bool VirtualMachine::cmdOutput(char c)
{
   Stack::ValueType val = m_oStack.pop();

   switch (c)
   {
   case '.':
      std::cout << val << ' ';
      break;

   case ',':
      std::cout << char(val);
   }

   return true;
}

bool VirtualMachine::cmdInput(char c)
{
   switch (c)
   {
   case '&':
   {
      std::string s;
      std::getline(std::cin, s);

      std::stringstream ss;
      ss << s;

      Stack::ValueType val;
      ss >> val;

      m_oStack.push(val);
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
   Stack::ValueType y = m_oStack.pop();
   Stack::ValueType x = m_oStack.pop();

   switch (c)
   {
   case 'g':
      m_oStack.push(getCmdAt({static_cast<InstructionPointer::VectorType::ValueType>(x), static_cast<InstructionPointer::VectorType::ValueType>(y)}));
      break;

   case 'p':
      setCmdAt({static_cast<InstructionPointer::VectorType::ValueType>(x), static_cast<InstructionPointer::VectorType::ValueType>(y)}, m_oStack.pop<char>());
   }

   return true;
}

bool VirtualMachine::cmdDigits(char c)
{
   if (std::isdigit(c))
      m_oStack.push(Stack::ValueType(c - '0'));

   return true;
}

void VirtualMachine::loadCode(const std::string& rksFilename)
{
   std::ifstream file;
   std::uint16_t x;
   std::uint16_t y;
   char cmd;
   bool done = false;

   file.open(rksFilename.c_str());

   for (y = 0; y < ms_ki16CodeHeight; ++y)
      for (x = 0; x < ms_ki16CodeWidth; ++x)
         setCmdAt({x, y}, ' ');

   for (y = 0; y < ms_ki16CodeHeight; ++y)
   {
      for (x = 0; x < ms_ki16CodeWidth; ++x)
      {
         if (file.get(cmd))
         {
            if (cmd != '\n')
               setCmdAt({x, y}, cmd);
            else
               break;
         }
         else
         {
            done = true;
            break;
         }
      }

      if (done)
         break;
   }

   file.close();
}

std::int32_t VirtualMachine::run()
{
   char cmd = ' ';
   char prevCmd;
   std::map<char, CommandFunction>::iterator cmdIt;
   bool proceed;
   bool doLog = true;
   std::int32_t result = 0;

   std::ofstream logfile;
   logfile.open("logfile.txt", std::ios::out);

   for (;;)
   {
      prevCmd = cmd;
      cmd = getCmdAt(m_oInstructionPointer.m_oPosition);

      if (doLog && logfile.is_open())
      {
         if ( /*cmd != prev_cmd &&*/ cmd != ' ')
            logfile << cmd << " pos: " << m_oInstructionPointer.m_oPosition.x << "," << m_oInstructionPointer.m_oPosition.y << " dir: " << m_oInstructionPointer.m_oDirection.x << "," << m_oInstructionPointer.m_oDirection.y << std::endl;
      }

      proceed = true;

      if (cmd == '"')
         m_bStringMode = !m_bStringMode;
      else if (m_bStringMode)
         m_oStack.push(cmd);
      else if (cmd == '@')
         break;
      else
         if ((cmdIt = m_aCommands.find(cmd)) != m_aCommands.end())
            proceed = (this->*cmdIt->second)(cmd);

      if (proceed)
         m_oInstructionPointer.advance();
   }

   logfile.close();

   return result;
}

char VirtualMachine::getCmdAt(const InstructionPointer::VectorType& rkoPosition)
{
   return code[rkoPosition.x][rkoPosition.y];
}

void VirtualMachine::setCmdAt(const InstructionPointer::VectorType& rkoPosition, char c)
{
   code[rkoPosition.x][rkoPosition.y] = c;
}
