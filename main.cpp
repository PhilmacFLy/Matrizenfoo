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
//#include <mpi/mpi.h>

using namespace std;

void master(matrix &testMatrix);
void slave();

int main(int argc, char** argv)
{
   int nprocs, myrank;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   /* das lassen wir jetzt erstmal drin....
      Folgendes: ohne die schelife terminiert das programm zu schnell
      Es wird nur ein "node" erzeugt... durch das warten (plus -np 4 switch)
      funktionierts auch wie gewuenscht...
      quick'n'dirty
   */
   for (int i = 0; i < 20000; i++)
   {
      cout;
   }
   cout << nprocs << endl;
   cout << myrank << endl;

   MPI_Finalize();
   return 0;
}


void master(matrix &testMatrix)
{
//   MPI_Status status;
   int nodeCount = 4;
   //MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
   //testMatrix.jacobi(0.00000005);
   //cout << nodeCount << endl;


   /* Schritt 1: aufteilen der matrix in teilmatrizen,
      abhaengig von der anzahl der nodes */
   /* zeilen, die jedem node zum berechnen uebergeben wird */
   int linesPerNode = testMatrix.getheight() / nodeCount;

   /* anzahl der zeilen, die uebrig bleiben */
   int modLines = testMatrix.getheight() % nodeCount;
   int anfang = 0;
   int ende = 0;

   /* aufteilen auf node dinger */
   for (int i = 0; i < nodeCount; i++)
   {
      if (modLines > 0)
      {
         ende = anfang+linesPerNode+1;
         cout << "1ich wuerde jetzt knoten nummer " << i << " die zeilen " << anfang << " bis " << ende-1 << " schicken." << endl;
         MPI_Send(testMatrix.getpart(anfang,ende),(ende-anfang)*testMatrix.getwidth(),MPI_DOUBLE,i,1, MPI_COMM_WORLD);
         anfang = ende;
         modLines--;
      } else
      {
         ende = anfang+linesPerNode;
         cout << "2ich wuerde jetzt knoten nummer " << i << " die zeilen " << anfang << " bis " << ende-1 << " schicken." << endl;
         MPI_Send(testMatrix.getpart(anfang,ende),(ende-anfang)*testMatrix.getwidth(),MPI_DOUBLE,i,1, MPI_COMM_WORLD);
         anfang = ende;
      }
       // MPI_Send (testMatrix.getPart(), 2, MPI_DOUBLE, i+1, MSG_DATA, MPI_COMM_WORLD);
   }

}


void slave()
{
   cout << "slave" << endl;
}
