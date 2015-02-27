#include "mpi.h"
#include "iostream"

int int main(int argc, char const *argv[])
{
	
	MPI_Init(&argc,&argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::cout << "rank=" << rank << ""
	<<"size=" << size << std::endl;
	return MPI_Finalize();
}