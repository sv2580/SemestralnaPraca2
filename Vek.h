#pragma once

#pragma once
#include "EnumTyp.h"

class Vek {
private:
	Pohlavie _pohlavie;
	int _vek;
	int _pocet;


public:
	Vek(Pohlavie pohlavie , int pocet, int vek);
	const int& const getPocet();
	const int& const getVek();
	const Pohlavie& const getPohlavie();
	~Vek() {};
};

inline Vek::Vek(Pohlavie pohlavie, int pocet, int vek) :
	_pohlavie(pohlavie),
	_pocet(pocet),
	_vek(vek)
{
}

inline const int& const Vek::getPocet() {
	return _pocet;
}
inline const int& const Vek::getVek()
{
	return _vek;
}
inline const Pohlavie& const Vek::getPohlavie() {
	return _pohlavie;
}