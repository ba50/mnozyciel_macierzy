#include <iostream>
#include <memory>
#include <fstream>
#include <chrono>
#include <ctime>

#include "mnozenie_macierzy.hpp"

int main(int argc, char *argv[]) {

	if (argc < 4)
	{
		std::cerr << "Za ma³o parametrów!" << std::endl;
		return (1);
	}	std::chrono::time_point<std::chrono::system_clock> start, koniec;
	//const clockid_t ZegarID = CLOCK_REALTIME;
	//timespec start, koniec; // Struktury przechowuj¹ce czas
	std::cout << "Mnozenie macierzy..." << std::endl;
	std::cout.flush();

	std::ifstream file_in;
	file_in.open(argv[1]);
	Macierz a(file_in);
	file_in.close();
	file_in.open(argv[2]);
	Macierz b(file_in);
	file_in.close();

	//clock_gettime(ZegarID, &start); // Zapisuje czas pocz¹tku mno¿enia
	start = std::chrono::system_clock::now();
	// Mno¿y macierze a i b (liczba oznacza liczbê równoleg³ych w¹tków)
	std::unique_ptr<Macierz> wynik = std::make_unique<Macierz>(mnozenie_macierzy(a, b, 1));
	//clock_gettime(ZegarID, &koniec); // Zapisuje czas koñca mno¿enia
	koniec = std::chrono::system_clock::now();
	std::chrono::duration<double> delta_time = koniec - start;
	std::cout << " zajelo " <<  delta_time.count() << " s" << std::endl;

	std::ofstream file_out;
	file_out.open(argv[3]);
	file_out << *wynik;
	file_out.close();

	system("pause");
	return 0;
}