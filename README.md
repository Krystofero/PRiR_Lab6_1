Projekt przedstawia zadanie 1 z Labolatorium nr 6 PRiR. Program napisano w języku C. Równolegle oblicza i wyświetla na ekranie przybliżenie liczby PI z wzoru Leibniz-a wykorzystując n procesow. Procesy przekazują między sobą aktualną wartość przybliżenia PI i wypisują ją na ekranie.

Import bibliotek ( w tym biblioteki MPI umożliwiającej pisanie programów wykonywanych równolegle):

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include "mpi.h
    
Funkcja wykonująca obliczenia ze wzroru Leibnitza dla pojedynczego procesu ( przyjmuje liczbę wszystkich procesów oraz wartość jaką przyjmuje aktualny proces) :

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
    
W mainie najpierw inicjalizujemy wszystkie potrzebne zmienne ( w tym funkcję MPI_Init - ma ona za zadanie zainicjalizowanie środowiska MPI dla procesu, który 
ją wywołuje Inicjalizacja informuje środowisko uruchomieniowe MPI o nowym procesie):

    int l_procesow, nr_akt_proc, tag = 20;
    double wynik = 0.0;
    MPI_Status status;
    MPI_Init(&argc, &argv); //inicjalizacja środowiska MPI
    MPI_Comm_size(MPI_COMM_WORLD, &l_procesow);  // okresla liczbę uruchomionych procesow tworzących sieć
    MPI_Comm_rank(MPI_COMM_WORLD, &nr_akt_proc); // okresla numer aktualnego procesu
	
Sprawdzamy czy to pierwszy proces( wysyłamy komunikat ze zmiennej wyniki - to adres początkowy, o długości bufora 1, typie pojedyńczego elementu double, proces docelowy to proces następny, a komunikator globalny to MPI_COMM_WORLD, zawierający wszystkie procesy):

    if(nr_akt_proc == 0) //sprawdzamy czy to pierwszy proces
    {
      MPI_Send(&wynik, 1, MPI_DOUBLE, nr_akt_proc+1, tag, MPI_COMM_WORLD); //wysyła komunikat
    } 
    
Jeżeli numer aktualnego procesu znajduje się w przedziale liczby procesów <0, n-1> to odbieramy komunikat wypisujemy wynik funkcji Leibnitz, jeżeli to ostatni rekord to również to wypisujemy, informując o tym użytkownika:

    if((nr_akt_proc > 1) && (nr_akt_proc < l_procesow-1)) 
      {
        MPI_Recv(&wynik, 1, MPI_DOUBLE, nr_akt_proc-1, tag, MPI_COMM_WORLD, &status); //odbiera komunikat
        wynik = Lebinz(nr_akt_proc, wynik);
        printf("Nr Procesu = %d ( Aktualny wynik liczby PI = %f )", nr_akt_proc, wynik);
        if(nr_akt_proc != l_procesow-1) //sprawdzamy czy to nie ostatni proces
        {
          MPI_Send(&wynik, 1, MPI_DOUBLE, nr_akt_proc+1, tag, MPI_COMM_WORLD); //wysyła komunikat
        }
        else //jeśli to ostatni proces
        {
          printf("Nr ostatniego procesu = %d ( Ostateczny wynik przybliżenia liczby PI = %f )", nr_akt_proc, wynik);
        }
      }
      
Na koniec funkcja, która kończy pracę w trybie MPI:

	  MPI_Finalize(); //funkcja kończy pracę w trybie MPI
    
