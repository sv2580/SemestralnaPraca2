#pragma once
#include "EnumTyp.h"

class Vzdelanie {
private:
	TypVzdelania _typVzdelania;
	int _pocet;
	

public:
	Vzdelanie(TypVzdelania typ, int pocet);
	~Vzdelanie() {}

	const int& const getPocet();
	const TypVzdelania& const getTyp();
};

inline Vzdelanie::Vzdelanie(TypVzdelania typ, int pocet)
{
	this->_typVzdelania = typ;
	this->_pocet = pocet;
}

inline const int& const Vzdelanie::getPocet() {
	return _pocet;
}
inline const TypVzdelania& const Vzdelanie::getTyp() {
	return _typVzdelania;
}