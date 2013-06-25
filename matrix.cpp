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
matrix::matrix(int inheight, int inwidth)
{
   this->height = inheight;
   this->width = inwidth;
   //memory allocated for elements of rows.
   werte = new double [inwidth*inheight];
}

int matrix::getwidth()
{
return this->width;
}

int matrix::getheight()
{
return this->height;
}

void matrix::init()
{
   for(int i=0; i<height*width; i++)
   {
      werte[i]=0;
   }
   werte[0] = 1;
   werte[this->width*this->height-1] = 1;

   for (int j = 1; j < this->width; j++)
   {
      double Rand = werte[j-1]-(double)1/(this->width-1);
      //zeilen, von vorne und hinten
      werte[j] = Rand;
      werte[(this->width*this->height)-j-1] = Rand;
      werte[this->width*j] = Rand;
      werte[this->width*this->height-(j*this->width)-1] = Rand;

      //spalten
   }
  /* for (int b=1; b<this->width*this->height-1; b++)
   {
      double Rand = werte[0][b-1]-(double)1/(this->width-1);
      werte[0][b] = Rand;
      werte[b][0] = Rand;
      werte[width-1][(width-1)-b] = Rand;
      werte[(width-1)-b][width-1] = Rand;
   }*/

}
void matrix::print()
{
   for (int i = 0; i < this->width*this->height; i++)
   {
      std::cout << werte[i] << "\t";
      if ( ((i+1) % this->width == 0) && i != 0)
         std::cout << std::endl;
   }
   std::cout << std::endl;
}


double * matrix::getpart(int beginLine, int endLine)
{
   matrix result(endLine-beginLine+1, this->width);
   int cnt = 0;
   for (int ii = beginLine*this->width; ii <= ((endLine+1)*this->width)-1; ii++)
   {
      result.werte[cnt] = this->werte[ii];
      //std::cout << "ich setze jetzt den wert " << this->werte[ii] << " an die stelle " << cnt << std::endl;
      cnt++;
   }
   return result.werte;
}

void matrix::copyM(matrix toCopy)
{
   //TODO: memcpy() waere schoener, aber da toCopy.matrix private ist, meh...
   for (int i = 0; i < toCopy.getheight(); i++)
   {
      for (int j = 0; j < toCopy.getwidth(); j++)
      {
	 //this->werte[i][j] = toCopy.getItem(i,j);
	 this->setItem(i, j, toCopy.getItem(i,j));
      }
   }
}

/*
double * matrix::getMatrix(double * matrixArray)
{
   int cnt = 0;
   for (int i = 0; i < height; i++)
   {
      for (int j = 0; j < width; j++)
      {
         matrixArray[cnt] = werte[j][i];
         cnt++;
      }
   }
   return matrixArray;
}
double ** matrix::getpart(int i, int j)
{
matrix result(i, j);

   for (int ii = 0; ii < i; ii++)
   {
      for (int jj = 0; jj < j; jj++)
      {
	 result.werte[ii][jj] = this->werte[ii][jj];
      }
   }
return result.werte;
}


void matrix::init()
{
   for(int i=0; i<height; i++)
   {
      for(int j=0; j<width; j++)
      {
	 werte[i][j]=0;
      }
   }

   werte[0][0] = 1;
   werte[this->width-1][this->height-1] = 1;

   for (int b=1; b<this->width-1; b++)
   {
      double Rand = werte[0][b-1]-(double)1/(this->width-1);
      werte[0][b] = Rand;
      werte[b][0] = Rand;
      werte[width-1][(width-1)-b] = Rand;
      werte[(width-1)-b][width-1] = Rand;
   }
}


void matrix::print()
{
   for (int i = 0; i < this->width; i++)
   {
      for (int j = 0; j < this->height; j++)
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
      for(int i=1; i<width-1; i++)
      {
	 for(int j=1; j<height-1; j++)
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
*/

void matrix::jacobi(double Accuracy)
{
   matrix tmpmatrix(this->width, this->height);
   tmpmatrix.init();


   bool isInaccurate = true;

   while(isInaccurate )
   {
      //Ist der Wert genau?
      isInaccurate = false;
      for(int i=1; i<width-1; i++)
      {
	 for(int j=1; j<height-1; j++)
	 {
	    //werte[i][j]=0.25*(tmpmatrix.werte[i-1][j]+tmpmatrix.werte[i+1][j]+tmpmatrix.werte[i][j+1]+tmpmatrix.werte[i][j-1]);
	    this->setItem(i,j, (0.25* (tmpmatrix.getItem(i-1, j)+tmpmatrix.getItem(i+1,j)+tmpmatrix.getItem(i, j+1)+tmpmatrix.getItem(i, j-1))));
	    if (Accuracy < std::abs(this->getItem(i,j)-tmpmatrix.getItem(i,j)))
	    {
	       isInaccurate=true;
	    }
	 }
      }
      tmpmatrix.copyM(*this);
   }
}


double matrix::getItem(int x, int y)
{
   return werte[(x)*width+y];
}

void matrix::setItem(int x, int y, double item)
{
   werte[(x)*width+y] = item;
}
