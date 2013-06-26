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
#include <iomanip>
#include "matrix.h"
#include <mpi.h>
#include <math.h>
//#include <mpi/mpi.h>

using namespace std;

void master();
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
   //cout << nprocs << endl;
   //cout << myrank << endl;
   if (!myrank)
      master();
   else
      slave();
   MPI_Finalize();
   return 0;
}


void master()
{
//   MPI_Status status;
   matrix testMatrix(100,100);
   testMatrix.init();
   double* testArray = new double[testMatrix.getwidth()*testMatrix.getheight()];
   testArray = testMatrix.getpart(0,2);
   testMatrix.print();
   //cout << testMatrix.getItem(1, 3) << endl;
   //testMatrix
   int nodeCount = 4;
   //MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
   testMatrix.jacobi(0.00000005);
   testMatrix.print();
   //cout << nodeCount << endl;

   /* Schritt 1: aufteilen der matrix in teilmatrizen,
      abhaengig von der anzahl der nodes */
   /* zeilen, die jedem node zum berechnen uebergeben wird */
   int linesPerNode = testMatrix.getheight() / (nodeCount-1);
   /* anzahl der zeilen, die uebrig bleiben */
   int modLines = testMatrix.getheight() % (nodeCount-1);
   int anfang = 0;
   int ende = 0;
   /* erster node */
   if (modLines > 0)
   {
      ende = anfang+linesPerNode+1; /* +1 fuer modLines, +1 fuer ueberschneidung */
      cout << "1ich wuerde jetzt knoten nummer " << 1 << " die zeilen " << anfang << " bis " << ende << " schicken." << endl;
      int foo[2];
      testArray = testMatrix.getpart(anfang,ende);
      foo[0] = ende-anfang+1;
      foo[1] = testMatrix.getwidth();
      MPI_Send(&foo[0],2,MPI_INTEGER,1,0,MPI_COMM_WORLD);
      MPI_Send(&testArray[0],(ende-anfang)*testMatrix.getwidth(),MPI_DOUBLE,1,1, MPI_COMM_WORLD);
      anfang = ende-1;
      modLines--;
   } else
    {
      ende = anfang+linesPerNode; /* +1 fuer ueberschneidung */
      cout << "2ich wuerde jetzt knoten nummer " << 1 << " die zeilen " << anfang << " bis " << ende << " schicken." << endl;
      testArray = testMatrix.getpart(anfang,ende);
      int foo[2];
      foo[0] = ende-anfang+1;
      foo[1] = testMatrix.getwidth();
      MPI_Send(&foo[0],2,MPI_INTEGER,1,0,MPI_COMM_WORLD);
      MPI_Send(&testArray[0],(ende-anfang)*testMatrix.getwidth(),MPI_DOUBLE,1,1, MPI_COMM_WORLD);
      anfang = ende-1;
   }

   /* aufteilen auf 'mittleren' node dinger */
   for (int i = 2; i <= nodeCount-2; i++)
   {
      if (modLines > 0)
      {
         ende = anfang+linesPerNode+1+1;
         cout << "1ich wuerde jetzt knoten nummer " << i << " die zeilen " << anfang << " bis " << ende << " schicken." << endl;
         testArray = testMatrix.getpart(anfang,ende);
         int foo[2];
         foo[0] = ende-anfang+1;
         foo[1] = testMatrix.getwidth();
         MPI_Send(&foo[0],2,MPI_INTEGER,i,0,MPI_COMM_WORLD);
         MPI_Send(&testArray[0],(ende-anfang)*testMatrix.getwidth(),MPI_DOUBLE,i,1, MPI_COMM_WORLD);
         anfang = ende-1;
         modLines--;
      } else
      {
         ende = anfang+linesPerNode+1;
         cout << "2ich wuerde jetzt knoten nummer " << i << " die zeilen " << anfang << " bis " << ende << " schicken." << endl;
         testArray = testMatrix.getpart(anfang,ende);
         int foo[2];
         foo[0] = ende-anfang+1;
         foo[1] = testMatrix.getwidth();
         MPI_Send(&foo[0],2,MPI_INTEGER,i,0,MPI_COMM_WORLD);
         MPI_Send(&testArray[0],(ende-anfang)*testMatrix.getwidth(),MPI_DOUBLE,i,1, MPI_COMM_WORLD);
         anfang = ende-1;
      }
   }
   /* letzter node */
   ende = testMatrix.getheight()-1; /* +1 fuer modLines, +1 fuer ueberschneidung */
   cout << "1ich wuerde jetzt den letzten knoten " <<" die zeilen " << anfang << " bis " << ende << " schicken." << endl;
   int foo[2];
   testArray = testMatrix.getpart(anfang,ende);
   foo[0] = ende-anfang+1;
   foo[1] = testMatrix.getwidth();
   MPI_Send(&foo[0],2,MPI_INTEGER,nodeCount-1,0,MPI_COMM_WORLD);
   MPI_Send(&testArray[0],(ende-anfang)*testMatrix.getwidth(),MPI_DOUBLE,nodeCount-1,1, MPI_COMM_WORLD);
   anfang = ende-1;
   modLines--;
}


void slave()
{
   MPI_Status status;
   int myrank;
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   int info[2];
   //Initial:
   //Empfangen der Matrixgröße (Blocking)
   MPI_Recv(info,2,MPI_INTEGER,0,0,MPI_COMM_WORLD,&status);
   cout << "!!!!!!!!!!!!!!!!!!!" << info[0] << "    " << info[1] << flush << endl;
   double matrixPart[info[0]*info[1]];

   //Empfangen der InitialMatrix (Blocking)
   MPI_Recv(matrixPart,info[0]*info[1],MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   //cout << "Hallo, ich bin slave nummer " << myrank << " und habe erhalten: " << std::fixed << std::setprecision(10) << matrixPart[0] << " | " << matrixPart[1] << " | " << matrixPart[2] << " | " << matrixPart[3] << " | "<< matrixPart[4] << " | "<<  std::flush << std::endl;



}

