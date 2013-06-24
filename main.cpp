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
#include <mpi.h>
/* anzahl node */
#define N 42
using namespace std;

void master(matrix &testMatrix);
void slave();

int main(int argc, char** argv)
{
   int nprocs, myrank;
   MPI_Init(&argc, &argv);
   matrix testMatrix(5);

   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   if (!myrank)
      master(testMatrix);
   else
      slave();
   MPI_Finalize();
   testMatrix.print();
   /*
   matrix testMatrix(5);
   testMatrix.init();
   matrix testMatrix2(5);
   testMatrix2.init();
   //testMatrix.print();
   testMatrix.gaussseidel(0.000000005);
   testMatrix2.jacobi(0.000000005);
   testMatrix.print();
   testMatrix2.print();
   //cout << "kopieren..." << endl << endl;
   //matrix test2Matrix(5);
   //test2Matrix.copyM(testMatrix);
   //test2Matrix.print();
   */
   return 0;
}


void master(matrix &testMatrix)
{
   MPI_Status status;
   int nodeCount;
   MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
   cout << size << endl;


   /* Schritt 1: aufteilen der matrix in teilmatrizen,
      abhaengig von der anzahl der nodes

   */
   int linesPerNode;
   /* zeilen, die jedem node zum berechnen uebergeben wird */
   int linesPerNode = testMatrix.getHeight() / nodeCount;
   /* zeilen, die uebrig bleiben */
   int modLines = testMatrix.getHeight() % nodeCount;
   int anfang,ende;
   for (int i = 0; i < nodeCount; i++)
   {
      if (modLines > 0)
        MPI_Send (testMatrix.getPart(), 2, MPI_DOUBLE, i+1, MSG_DATA, MPI_COMM_WORLD);

   }

}


void slave()
{
   cout << "slave" << endl;
}
