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

#include <iostream>
#include "matrix.h"
#include <cmath>
#include <iomanip>

//*************************************************************************************************************************************************
// Private Implementations
//*************************************************************************************************************************************************

/*matrix matrix::fork()
{
matrix forkedMatrix;
forkedMatrix = new matrix(this->size);
	for (int i = 0; i < this->size; i++)
	{
		for (int j = 0; j < this->size; j++)
		{
			forkedMatrix.werte[i][j] = this->werte[i][j];
		}
	}
return forkedMatrix;
}*/



//*************************************************************************************************************************************************
// Public Implementations
//*************************************************************************************************************************************************
void matrix::copyM(matrix toCopy)
{
   //TODO: memcpy() waere schoener, aber da toCopy.matrix private ist, meh...
   for (int i = 0; i < toCopy.getSize(); i++)
   {
      for (int j = 0; j < toCopy.getSize(); j++)
      {
	 this->werte[i][j] = toCopy.getItem(i,j);
      }
   }
}

void matrix::init()
{
   for(int i=0; i<size; i++)
   {
      for(int j=0; j<size; j++)
      {
	 werte[i][j]=0;
      }
   }

   werte[0][0] = 1;
   werte[this->size-1][this->size-1] = 1;

   for (int b=1; b<this->size-1; b++)
   {
      double Rand = werte[0][b-1]-(double)1/(this->size-1);
      werte[0][b] = Rand;
      werte[b][0] = Rand;
      werte[size-1][(size-1)-b] = Rand;
      werte[(size-1)-b][size-1] = Rand;
   }
}
matrix::matrix(int insize)
{
   this->size = insize;
   //memory allocated for elements of rows.
   werte = new double *[size] ;

   //memory allocated for  elements of each column.
   for( int i = 0 ; i < insize ; i++ )
   {
      werte[i] = new double[size];
   }
}

void matrix::print()
{
   for (int i = 0; i < this->size; i++)
   {
      for (int j = 0; j < this->size; j++)
      {
	 std::cout << std::fixed << std::setprecision(10) <<werte[i][j] << "\t";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

void matrix::gaussseidel(double Accuracy)
{
   double buffer = 0;
   bool isInaccurate = true;

   while(isInaccurate)
   {
      isInaccurate = false;
      for(int i=1; i<size-1; i++)
      {
	 for(int j=1; j<size-1; j++)
	 {
	    buffer = werte[i][j];
	    werte[i][j]=0.25*(werte[i-1][j]+werte[i+1][j]+werte[i][j+1]+werte[i][j-1]);
	    if (Accuracy < std::abs(werte[i][j]-buffer))
	    {
	       isInaccurate=true;
	    }
	 }
      }
  }
}


void matrix::jacobi(double Accuracy)
{
   matrix tmpmatrix(this->size);
   tmpmatrix.init();


   bool isInaccurate = true;

   while(isInaccurate )
   {
      //Ist der Wert genau?
      isInaccurate = false;
      for(int i=1; i<size-1; i++)
      {
	 for(int j=1; j<size-1; j++)
	 {
	    werte[i][j]=0.25*(tmpmatrix.werte[i-1][j]+tmpmatrix.werte[i+1][j]+tmpmatrix.werte[i][j+1]+tmpmatrix.werte[i][j-1]);
	    if (Accuracy < std::abs(werte[i][j]-tmpmatrix.werte[i][j]))
	    {
	       isInaccurate=true;
	    }
	 }
      }
      tmpmatrix.copyM(*this);
   }
}


int matrix::getSize()
{
   return this->size;
}

double matrix::getItem(int x, int y)
{
   return werte[x][y];
}

