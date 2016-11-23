#include <utility>

typedef std::pair <int, int> RozmiarMacierzy;
typedef std::pair <int, int> Polozenie;

class Macierz
{

public:
	Macierz(std::istream &is) { // tworzy macierz na podstawie strumienia
		is >> n_;
		is >> m_;

		mac_ = std::make_unique<int[]>(n_*m_);

		for (int i = 0; i < n_*m_; i++) {
			is >> mac_[i];
		}

	};
	Macierz(int n, int m) :  // tworzy pust¹ (niezainicjowan¹) macierz n x m
		n_(n),
		m_(m),
		mac_{std::make_unique<int[]>(n_*m_)}
	{
		for (int i = 0; i < n_*m_; i++) {
			mac_[i] = 0;
		}
	};
	Macierz(const Macierz &mac) :
		n_{mac.n_},
		m_{mac.m_},
		mac_{std::make_unique<int[]>(n_*m_)}
	{
		for (int i = 0; i < n_*m_; i++) {
			mac_[i] = mac.mac_[i];
		}
		
	};
	int element(int i, int j) const { // zwraca element macierzy (kopia przez wartoœæ)
		return mac_[i*m_ + j];
	};
	int &element(int i, int j) { // zwraca ref. (mo¿na modyfikowaæ!) do el. macierzy
		return mac_[i*m_ + j];
	};
	RozmiarMacierzy rozmiar() const { // zwraca rozmiar macierzy
		return RozmiarMacierzy(n_, m_);
	};

private:
	int n_;
	int m_;
	std::unique_ptr<int[]> mac_;

	friend std::ostream &operator<< (std::ostream &os, const Macierz &m);
};

std::ostream &operator<< (std::ostream &os, const Macierz &m) {
	os << m.n_ << " " << m.m_ << std::endl;
	for (int i = 0; i < m.rozmiar().first; i++) {
		for (int j = 0; j < m.rozmiar().second; j++) {
			os << m.element(i, j) << "\t";
		}
		os << std::endl;
	}
	return os;
};