#include "macierz.hpp"

#include <boost\optional.hpp>
#include <mutex>
#include <algorithm>
#include <thread>
#include <vector>

// Kontroluje proces mno�enia macierzy (obiekt wsp�dzielony)
class KontrolerMnozenia
{

public:
	// Inicjuje skladowe klasy
	KontrolerMnozenia(const Macierz &m1, const Macierz &m2, Macierz &m3) :
		i(0),
		j(0),
		m1_(m1),
		m2_(m2),
		m3_(m3)
	{};
	inline const Macierz &czynnik1() const { return m1_; };
	inline const Macierz &czynnik2() const { return m2_; };
	inline Macierz &wynik() const { return m3_; };

	// Zwraca wsp�rz�dne najbli�szego elementu do obliczen
	// lub kontener niezainicjowany, gdy obliczenia dobieg�y ko�ca
	boost::optional <Polozenie> coMamLiczyc() {
		if (i < m3_.rozmiar().first && j < m3_.rozmiar().second) {
			return boost::optional<Polozenie>(Polozenie{ i,j });
		}
		else {
			return boost::optional<Polozenie>();
		}
	}

private:
	const Macierz &m1_; // pierwszy czynnik (sta�a referencja)
	const Macierz &m2_; // drugi czynnik (sta�a referencja)
	Macierz &m3_; // wynik mnozenia

public:
	int i, j;
	std::mutex blokada_; // blokada
};

class MnozycielMacierzy // funktor wykorzystywany w w�tkach mno��cych macierze
{

public:
	MnozycielMacierzy(KontrolerMnozenia &km) :  // inicjuje sk�adowe klasy
		km_{ km },
		m1_{ &km.czynnik1() },
		m2_{ &km.czynnik2() },
		m3_{ &km.wynik() }
	{};

	void operator() () { // metoda wywo�ywana przy utworzeniu w�tku

		while (km_.coMamLiczyc()) {
			int i = km_.coMamLiczyc()->first;
			int j = km_.coMamLiczyc()->second;

			for (int k = 0; k < std::max(m1_->rozmiar().first, m1_->rozmiar().second); k++) {
				km_.blokada_.lock();
				m3_->element(i, j) += m1_->element(i, k)*m2_->element(k, j);
				km_.blokada_.unlock();
			}

			km_.i++;
			if (km_.i >= m3_->rozmiar().first) {
				km_.i = 0;
				km_.j++;
			}
		}
	};

private:
	KontrolerMnozenia &km_; // referencja na kontroler danego mno�enia
	const Macierz *m1_; // wska�nik na pierwszy czynnik
	const Macierz *m2_; // wska�nik na drugi czynnik
	Macierz *m3_; // wska�nik na macierz wynikow�
};

// Funkcja mno��ca macierze m1 i m2 w p w�tkach
Macierz mnozenie_macierzy(const Macierz &m1, const Macierz &m2, int p) {
	Macierz wynik(m1.rozmiar().first, m2.rozmiar().second);

	MnozycielMacierzy mnozyciel(KontrolerMnozenia{ m1, m2, wynik});

	std::vector<std::thread> thread_grupe;

	for (int i = 0; i < p; i++) {
		thread_grupe.push_back(std::thread(mnozyciel));
	}

	for (auto& thread : thread_grupe) {
		thread.join();
	}

	return wynik;
};