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
#include <array>

#include "Stack.hpp"

class VirtualMachine
{
protected:
   template<typename T>
   class Vector
   {
   public:
      using ValueType = T;

   public:
      T x;
      T y;

   public:
      inline Vector& operator=(const Vector& rkoOther)
      {
         x = rkoOther.x;
         y = rkoOther.y;

         return *this;
      }

      inline Vector& operator=(Vector&& rroOther)
      {
         x = rroOther.x;
         y = rroOther.y;

         return *this;
      }

      inline Vector operator+()
      {
         return *this;
      }

      inline Vector operator-()
      {
         return
         {
              -x
            , -y
         };
      }

      inline Vector& operator+=(const Vector& rkoRhs)
      {
         x += rkoRhs.x;
         y += rkoRhs.y;

         return *this;
      }

      inline Vector& operator-=(const Vector& rkoRhs)
      {
         x -= rkoRhs.x;
         y -= rkoRhs.y;

         return *this;
      }
   };

   class InstructionPointer
   {
   public:
      using VectorType = Vector<std::int16_t>;

   public:
      static const VectorType ms_koDirectionUp;
      static const VectorType ms_koDirectionDown;
      static const VectorType ms_koDirectionLeft;
      static const VectorType ms_koDirectionRight;

   public:
      VectorType m_oPosition;
      VectorType m_oDirection;

   public:
      void advance();
   };

protected:
   static const std::int16_t ms_ki16CodeWidth = 80, ms_ki16CodeHeight = 25;

   std::array<std::array<char, ms_ki16CodeHeight>, ms_ki16CodeWidth> code;

protected:
   using CommandFunction = bool(VirtualMachine::*)(char);

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

   std::map<char, CommandFunction> m_aCommands;

   Stack<std::uint64_t> m_oStack;

   InstructionPointer m_oInstructionPointer;

   bool m_bStringMode;

   char getCmdAt(const InstructionPointer::VectorType& rkoPosition);
   void setCmdAt(const InstructionPointer::VectorType& rkoPosition, char c);

public:
   VirtualMachine();
   virtual ~VirtualMachine();

   virtual void loadCode(const std::string& rksFilename);

   virtual std::int32_t run();
};

template<typename T>
inline VirtualMachine::Vector<T> operator+(const VirtualMachine::Vector<T>& rkoLhs, const VirtualMachine::Vector<T>& rkoRhs)
{
   return
   {
        rkoLhs.x + rkoRhs.x
      , rkoLhs.y + rkoRhs.y
   };
}

template<typename T>
inline VirtualMachine::Vector<T> operator-(const VirtualMachine::Vector<T>& rkoLhs, const VirtualMachine::Vector<T>& rkoRhs)
{
   return
   {
        rkoLhs.x - rkoRhs.x
      , rkoLhs.y - rkoRhs.y
   };
}

#endif // VIRTUALMACHINE_HPP
