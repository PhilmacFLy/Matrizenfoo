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
void slave(int nodeCount);
int main(int argc, char** argv)
{
   int nodeCount = 4;
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
   if (!myrank)
      master();
   else
      slave(nodeCount);
   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Finalize();
   return 0;
}


void master()
{
//   MPI_Status status;
   matrix testMatrix(20,20);
   testMatrix.init();
   double* testArray = new double[testMatrix.getwidth()*testMatrix.getheight()];
   testArray = testMatrix.getpart(0,2);
   //testMatrix.print();
   //cout << testMatrix.getItem(1, 3) << endl;
   //testMatrix
   int nodeCount = 4;
   //MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);
   //testMatrix.jacobi(0.00000005);
   //testMatrix.print();
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
      MPI_Send(&testArray[0],(ende+1-anfang)*testMatrix.getwidth(),MPI_DOUBLE,1,1, MPI_COMM_WORLD);
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
      MPI_Send(&testArray[0],(ende+1-anfang)*testMatrix.getwidth(),MPI_DOUBLE,1,1, MPI_COMM_WORLD);
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
         MPI_Send(&testArray[0],(ende+1-anfang)*testMatrix.getwidth(),MPI_DOUBLE,i,1, MPI_COMM_WORLD);
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
         MPI_Send(&testArray[0],(ende+1-anfang)*testMatrix.getwidth(),MPI_DOUBLE,i,1, MPI_COMM_WORLD);
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
   MPI_Send(&testArray[0],(ende+1-anfang)*testMatrix.getwidth(),MPI_DOUBLE,nodeCount-1,1, MPI_COMM_WORLD);
   anfang = ende-1;
   modLines--;
}


void slave(int nodeCount)
{
   MPI_Status status;
   MPI_Request req;
   int myrank;
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   int info[2];

   /* mpiXInfo[2]
    * mpiXInfo[0] == 0,1; 1: node hat genauigkeit noch nicht erreicht.
    *                         d.h.: node wird nach der naechsten iteration nochmal daten liefern
    * mpiXInfo[1] == laenge der gesendeten zeile. redundant afaik, aber was solls...
    */
   int mpiSendInfo[2];  /* infos, die bei der interNodeKommunikation gesendet werden */
   int mpiGetInfo[2];   /* infos, die bei der interNodeKommunikation empfangen werden */

   //Initial:
   //Empfangen der Matrixgröße (Blocking)
   MPI_Recv(info,2,MPI_INTEGER,0,0,MPI_COMM_WORLD,&status);
   //cout << "!!!!!!!!!!!!!!!!!!!" << info[0] << "    " << info[1] << flush << endl;
   double matrixPart[info[0]*info[1]];

   //Empfangen der InitialMatrix (Blocking)
   MPI_Recv(matrixPart,info[0]*info[1],MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   //cout << "Hallo, ich bin slave nummer " << myrank << " und habe erhalten: " << std::fixed << std::setprecision(10) << matrixPart[0] << " | " << matrixPart[1] << " | " << matrixPart[2] << " | " << matrixPart[3] << " | "<< matrixPart[4] << " | "<<  std::flush << std::endl;
   matrix nodeM(matrixPart,info[0], info[1]);
   int doneStatus = nodeM.jacobi(0.00000001);
   //nodeM.print();
   cout << endl << endl << "node " << myrank << " ist fertig!" << endl;


   /* node1 ist fertig, muss an node 2 senden */
   if (myrank == 1)
   {
      int neighborDone = 0;
      int nodeDone = 0;
      int jacobiReturn = 0;
      double getLine[nodeM.getwidth()];

      while (nodeDone == 0)
      {
         jacobiReturn = nodeM.jacobi(0.000001);
         cout << "ich, Node1, werde jetzt zeile " << info[0]-2 << "/" << info[0] << " an Node2 schicken" << endl;
         /* info array aufbauen */
         mpiSendInfo[0] = jacobiReturn;
         mpiSendInfo[1] = nodeM.getwidth();
         /* nonblocking senden */
         MPI_Isend(&mpiSendInfo[0],2,MPI_INTEGER,myrank+1,42,MPI_COMM_WORLD,&req);
         MPI_Wait(&req,MPI_STATUSES_IGNORE);
         /* status des nachbarn, ausser er hat im letzten schritt schon ende bekannt gegeben  */
         if (neighborDone != 1)
         {
            /* info array abholen */
            MPI_Recv(&mpiGetInfo[0],2,MPI_INTEGER,myrank+1,42,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
            /* nachbar ist noch nicht fertig, line wird erwartet */
            if (mpiGetInfo[0] == 1)
            {
               MPI_Recv(&getLine[0],nodeM.getwidth(),MPI_DOUBLE,2,43,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
            /* sonst: letztes mal zeile einlesen */
            } else
            {
               MPI_Recv(&getLine[0],nodeM.getwidth(),MPI_DOUBLE,2,43,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
               neighborDone = 1;
            }
            cout << "lo, ich, 1, habe was geGETet: " << mpiGetInfo[0] << " " << mpiGetInfo[1] << flush << endl;
         }
         /* genauigkeit erreicht? dann abbrechen */
         if (jacobiReturn == 0)
         {
            nodeDone = 1;
         }

      }
      //TODO: zeugs an master senden.
   /* letzter node */
   } else if (myrank == nodeCount-1)
   {
      cout << "ich, Node" << nodeCount-1 << " werde jetzt Node " << myrank-1 << "zeile 1" << "/" << info[0] << " schicken" << endl;
      //MPI_Recv(mpiInfo,2,MPI_INTEGER,myrank-1,42,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
      //cout << "lo, hab mpiInfo erhalten, inhalt: " << mpiInfo[0] << " " << mpiInfo[1] << flush << endl;
   /* restliche nodes */
   } else
   {
      cout << "ich, Node " << myrank << " werde Zeile " << info[0]-2 << "/" << info[0] << " an Node " << myrank+1 << "senden" << endl;
      cout << "ich, Node " << myrank << " werde Zeile " << 2 << "/" << info[0] << " an Node " << myrank-1 << "senden" << endl;
      mpiSendInfo[0] = 1;
      mpiSendInfo[1] = nodeM.getwidth();
      MPI_Recv(mpiGetInfo,2,MPI_INTEGER,myrank-1,42,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
      cout << "lo, hab mpiInfo erhalten, inhalt: " << mpiGetInfo[0] << " " << mpiGetInfo[1] << flush << endl;
      MPI_Isend(&mpiSendInfo[0],2,MPI_INTEGER,myrank-1,42,MPI_COMM_WORLD,&req);
      MPI_Wait(&req,MPI_STATUSES_IGNORE);
   }
}

