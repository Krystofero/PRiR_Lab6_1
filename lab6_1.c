#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h

double Leibniz(int n, double wartosc_procesu)
{
    int i;
    double wynik = 0;
	wartosc_procesu = wartosc_rocesu / 4;

    wynik = (pow(-1, n - 1)/(2 * n - 1));
    wynik += wartosc_procesu;
    wynik *= 4;
    return wynik;
}

int main(int argc, char *argv[]) {
	int l_procesow, nr_akt_proc, tag = 20;
	double wynik = 0.0;
	MPI_Status status;
	MPI_Init(&argc, &argv); //inicjalizacja œrodowiska MPI
	MPI_Comm_size(MPI_COMM_WORLD, &l_procesow);  // okresla liczbê uruchomonych procesow tworz¹cych sieæ
	MPI_Comm_rank(MPI_COMM_WORLD, &nr_akt_proc); // okresla numer aktualnego procesu
	
	if(nr_akt_proc == 0) //sprawdzamy czy to pierwszy proces
	{
		MPI_Send(&wynik, 1, MPI_DOUBLE, nr_akt_proc+1, tag, MPI_COMM_WORLD); //wysy³a komunikat
	} 
	if((nr_akt_proc > 1) && (nr_akt_proc < l_procesow-1)) 
	{
		MPI_Recv(&wynik, 1, MPI_DOUBLE, nr_akt_proc-1, tag, MPI_COMM_WORLD, &status); //odbiera komunikat
		wynik = Lebinz(nr_akt_proc, wynik);
		printf("Nr Procesu = %d ( Aktualny wynik liczby PI = %f )", nr_akt_proc, wynik);
		if(nr_akt_proc != l_procesow-1) //sprawdzamy czy to nie ostatni proces
		{
			MPI_Send(&wynik, 1, MPI_DOUBLE, nr_akt_proc+1, tag, MPI_COMM_WORLD); //wysy³a komunikat
		}
		else //jeœli to ostatni proces
		{
			printf("Nr ostatniego procesu = %d ( Ostateczny wynik przybli¿enia liczby PI = %f )", nr_akt_proc, wynik);
		}
	}
	
	MPI_Finalize(); //funkcja koñczy pracê w trybie MPI
	return 0;
}
