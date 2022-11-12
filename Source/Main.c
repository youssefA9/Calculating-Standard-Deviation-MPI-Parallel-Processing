#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"


//Name:Youssef Ahmed AbdelHafez	ID: 20180337

int main(int argc , char * argv[])
{
   	int my_rank;		/* rank of process	*/
	int p;			/* number of process	*/

	MPI_Status status;	/* return status for 	*/
				
	/* Variables */
	int numOfElements = 0;
	int globalSum = 0;	
	int localSum = 0;
	double globalMean = 0.0;
	double localSquareDiff = 0.0;
	double globalSquareDiff = 0.0;	
	int* arr;	

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	

	if(my_rank==0){
		
		printf("Enter number of elements: ");
		scanf("%d", &numOfElements);

	}
	MPI_Bcast(&numOfElements , 1 , MPI_INT, 0 , MPI_COMM_WORLD);

	if(my_rank != 0){
		int i=0;
		int randNum=0;
		arr =  malloc (numOfElements * sizeof (int));
		
		srand(time(NULL));

		for(i = 0 ; i < numOfElements ; i++){
			
			randNum = rand() % (10* (my_rank + 1)) ;
			arr[i] = randNum;
			localSum += randNum;	
		}

	}


	MPI_Allreduce(&localSum,&globalSum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

	
	if(my_rank != 0){
	
		//globalMean as in the Assignment
		
		//globalMean = globalSum / numOfElements * p;
		

		//globalMean as it should be
		globalMean = (double) globalSum / (double)(numOfElements * (p - 1));
		


		//Sum(n - mean)^2

		//If you want the localSun in the Calculations
		/*
		localSquareDiff = pow(localSum - globalMean,2); 
		*/

		//If you want the elements in each process individualy in the Calculations
		int i = 0;
		for(i = 0 ; i < numOfElements ; i++){
			
			localSquareDiff += pow(arr[i] - globalMean,2);
		}
		
		
	}


	MPI_Reduce(&localSquareDiff,&globalSquareDiff,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

	if(my_rank == 0){

		double result = 0.0;
		result = (double)sqrt(globalSquareDiff/(numOfElements*(p - 1)));
		printf("Standard Deviation: %f \n",result);

	}

	/* shutdown MPI */
	MPI_Finalize();
    return 0;
}