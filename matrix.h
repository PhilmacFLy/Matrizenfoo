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
      //matrix fork();

//*************************************************************************************************************************************************
// Public Definitions
//*************************************************************************************************************************************************
   public:
      matrix(int inheight, int inwidth);
      double *werte;
      //void copyM(matrix toCopy);
      double * getpart(int height, int width);
      void print();
      //void gaussseidel(double Accuracy);
      //void jacobi(double Accuracy);
      void init();
      //double * getMatrix(double * testArray);
      int getheight();
      int getwidth();
      //double getItem(int x, int y);
};

#endif // MATRIX_H
