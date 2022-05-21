#pragma once

#include "VyssiCelok.h";

class Okres : public VyssiCelok {
public:
	Okres(TypUzemnejJednotky typ, std::wstring nazov, VyssiCelok* celok);

	~Okres() {
		_vyssiCelok = nullptr;
	}

};

inline Okres::Okres(TypUzemnejJednotky typ, std::wstring nazov, VyssiCelok* celok) : VyssiCelok(typ, nazov, celok)
{

}