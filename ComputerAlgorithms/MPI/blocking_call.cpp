#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
	MPI_Init(&argc, &argv);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if(world_rank==0){
		//it is the master process
		int data=100;
		MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		cout<<"Process 0 snds data= "<<data<<endl;
	}else if(world_rank==1){
		int data;
		MPI_Status status;
		MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		cout<<"Process 1 received data = "<<data<<endl;
	}
	MPI_Finalize();
}
