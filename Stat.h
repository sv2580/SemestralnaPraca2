#pragma once

#include "VyssiCelok.h";

class Stat : public VyssiCelok {
public:
	Stat(TypUzemnejJednotky typ, std::wstring nazov, VyssiCelok* celok);
	~Stat() {}

};

inline Stat::Stat(TypUzemnejJednotky typ, std::wstring nazov, VyssiCelok* celok) : VyssiCelok(typ, nazov, celok) {
}