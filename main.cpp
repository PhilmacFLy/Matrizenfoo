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
<<<<<<< HEAD
#include <mpi.h>
/* anzahl node */
#define N 42
=======
//#include <mpi/mpi.h>

>>>>>>> 1b70aa98995f00412a14b08451a009c345fa1561
using namespace std;

void master(matrix &testMatrix);
void slave();

int main(int argc, char** argv)
{
<<<<<<< HEAD
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
=======
   matrix testMatrix(5, 5);
>>>>>>> 1b70aa98995f00412a14b08451a009c345fa1561
   testMatrix.init();
   matrix testMatrix2(5, 5);
   testMatrix2.init();
   //testMatrix.print();
<<<<<<< HEAD
   testMatrix.gaussseidel(0.000000005);
   testMatrix2.jacobi(0.000000005);
   testMatrix.print();
   testMatrix2.print();
   //cout << "kopieren..." << endl << endl;
   //matrix test2Matrix(5);
   //test2Matrix.copyM(testMatrix);
   //test2Matrix.print();
   */
=======
   testMatrix.gaussseidel(0.000005);
   testMatrix2.jacobi(0.000005);
   testMatrix.print();
   testMatrix2.print();
   cout << "kopieren..." << endl << endl;
   matrix test2Matrix(5, 5);
   test2Matrix.copyM(testMatrix);
   test2Matrix.print();
   //matrix blub = test2Matrix.getpart(3,4);
   //blub.print();
>>>>>>> 1b70aa98995f00412a14b08451a009c345fa1561
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
