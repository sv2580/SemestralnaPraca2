#pragma once
#include "VyssiCelok.h";

class Kraj : public VyssiCelok {
public:
	Kraj(TypUzemnejJednotky typ, std::wstring nazov, VyssiCelok* celok) :
		VyssiCelok(typ, nazov, celok)
	{
	}

	~Kraj() {
		_vyssiCelok = nullptr;

	}
};