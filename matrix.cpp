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

#define __MAXHEAT 1

#include <iostream>
#include "matrix.h"
#include <cmath>
#include <iomanip>

//*************************************************************************************************************************************************
// Public Implementations
//*************************************************************************************************************************************************

//Konstruktore:
matrix::matrix(int inwidth, int inheight)
{
   height = inheight;
   width = inwidth;
   werte = new double [inwidth*inheight];
   //innere Nullen
   for (int i=1; i<height-1; i++)
   {
      for (int j=0; j<width; j++)
      {
         this->setItem(j,i,0);
      }
   }
   
   //Erste/Letze Zeile
   this->setItem(0,0,1);
   this->setItem(width-1,height-1,1);
   for (int k=1; k<width-1; k++)
   {
      double Rand = std::abs(werte[k-1]-(double)1/(width-1));
      this->setItem(k,0,Rand);
      this->setItem(width-k-1,height-1,Rand);
   }
   
   //Erste letze Spalte
   for (int n=1; n<height-1; n++)
   {
      double Rand = std::abs(getItem(0,n-1)-(double)1/(height-1));
      this->setItem(0,n,Rand);
      this->setItem(width-1,height-n-1,Rand);
   }
}

matrix::matrix(double* array, int inheight, int inwidth)
{
   height = inheight;
   width = inwidth;
   werte = new double [inwidth*inheight];
   for (int i=0; i<(height*width); i++)
   {
      werte[i] = array[i];
   }
}

//Destruktor:
matrix::~matrix()
{
   delete[] werte;
}

//Methoden
void matrix::copyM(matrix toCopy)
{
   width = toCopy.getWidth();
   height = toCopy.getHeight();
   delete[] werte;
   werte = new double [width*height];
   for (int i=0; i<(height*width); i++)
   {
      werte[i] = toCopy.getItem((i%width),(i/width));
   }
}

void matrix::print()
{
   for (int i = 0; i < this->width*this->height; i++)
   {
      std::cout << std::fixed << std::setprecision(10) << werte[i] << "\t";
      if ( ((i+1) % this->width == 0) && i != 0)
         std::cout << std::endl;
   }
   std::cout << std::endl;
}

int matrix::jacobi(double acc)
{
   double neuerWert = -1;
   bool isAccurate = true;
   //Erzeuge neue tmpMatrix
   double* werte;
   matrix tmpMatrix(getRowPtr(0,werte),height,width);
   
   for (int row = 1; row < getHeight()-1; row++)
   {
      for (int column =1; column < getWidth()-1; column++)
      {
         double neuerWert = tmpMatrix.getItem(column,(row-1));
         neuerWert += tmpMatrix.getItem(column,(row+1));
         neuerWert += tmpMatrix.getItem((column-1),row);
         neuerWert += tmpMatrix.getItem((column+1),row);
         neuerWert *= .25;
         setItem(column,row,neuerWert);
         //Sobald ein Wert ausserhalb der Genauigkeit ist.
         if ( (std::abs(acc) > std::abs(getItem(column,row) - tmpMatrix.getItem(column,row))) || (!isAccurate) )
         {
            isAccurate = false;
         }
      }
   }
   
   if (isAccurate)
      return 0;
   else
      return 1;
}


//Getter
double matrix::getItem(int column, int row)
{
   int oneDimPos = (row*width)+column;
   return (werte[oneDimPos]);
}

double * matrix::getRowPtr(int row, double* targetPtr)
{
   if(row >= height)
   {
      std::cout << "!!!!getRowPtr: Zugriff auf Zeile: "<<row<<" nicht möglich. maximale Zeile der Matrix ist: "<< height-1 << std::endl;
      return 0;     
   }
   int oneDimPos = (row*width);
   targetPtr = (&werte[oneDimPos]);
   if(row < height)
   return (targetPtr);
}

/*
double * matrix::getRowPtr(int startLine, int endLine, double* targetPtr)
{
   if( (startLine>=height) || (endLine>=height) )
   {
      std::cout << "!!!!getRowPtr: Unzulässiger Zugriff (" << startLine << "," << endLine << ") Maximale Höhe ist" << height-1 << std::endl;
      return 0;     
   }
   int oneDimPos = (row*width);
   targetPtr = (&werte[oneDimPos]);
   if(row < height)
   return (targetPtr);
}
*/  

//Setter
void matrix::setItem(int column, int row, double item)
{
   int oneDimPos = (row*width)+column;
   //Fange falsche Zugriffe ab
   if( (column<(width)) || (row<(height)) )
      werte[oneDimPos] = item;
   else
      std::cout << "!!!!setItem: Zugriff auf ("<<column<<","<<row<<") nicht möglich. Grenzen der Matrix sind("<<width-1<<","<<height-1<<")" << std::endl;
}

void matrix::setRow(int row, double* items)
{
   for (int i=0; i<width; i++)
   {
      setItem(i,row,items[i]);
   }
}
