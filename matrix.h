/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  PhilmacFLy <philmacfly@binary-kitchen.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef MATRIX_H
#define MATRIX_H

class matrix
{
//*************************************************************************************************************************************************
// Private Definitions
//*************************************************************************************************************************************************
   private:
      int width, height;
      double *werte;

//*************************************************************************************************************************************************
// Public Definitions
//*************************************************************************************************************************************************
   public:
   //Konstruktore:
      //erzeugt eine mit Randbedingungen vorbelegte Matrix
      matrix(int inwidth, int inheight);
      //erzeuge eine Matrix mit dem Inhalt von array;
      matrix(double* array, int inwidth, int inheight);

   //Destruktor
      ~matrix ();

   //Methoden
      void copyM(matrix toCopy);
      void print();
      int jacobi(double acc);
      //void init();

   //Get-Methoden:
      int getHeight() {return (height);}
      int getWidth() {return (width);}
      double getItem(int column, int row);
      //!Vorsicht "roher Pointer"
      double* getRowPtr(int row, double*targetPtr);
      //double * getRowPtr(int startLine, int endLine, double* targetPtr);

   //Set-Methoden:
      void setItem(int column, int row, double item);
      //Achtung items muss mindestens width*Elemente lang sein!
      void setRow(int row, double* items);
};

#endif // MATRIX_H
