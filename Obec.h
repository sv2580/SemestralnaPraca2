#pragma once

#include "UzemnaJednotka.h";
#include "VyssiCelok.h";
#include "structures/array.h"
#include "Vzdelanie.h"
#include "Vek.h"


class Obec : public UzemnaJednotka {
private:
	structures::Array<Vzdelanie*>* _vzdelanie;
	structures::Array<Vek*>* _vek;


public:
	~Obec();
	Obec(TypUzemnejJednotky typ, std::wstring nazov, UzemnaJednotka* celok);
	bool patriDoCelku(VyssiCelok* celok);


	// Inherited via UzemnaJednotka
	virtual const bool patriDoCelku(UzemnaJednotka* celok) const override;


	virtual const int getVzdelaniePocet(TypVzdelania typ) const override;

	virtual const double getVzdelaniePodiel(TypVzdelania typ) const override;


	// Inherited via UzemnaJednotka
	virtual const int getPocetSpolu() const override;


	void const setVzdelanie(structures::Array<Vzdelanie*>* vzdelanie);
	void const setVek(structures::Array<Vek*>* vek);



	// Inherited via UzemnaJednotka
	virtual const int getVekPocet(Pohlavie pohlavie, int vek) const override;

	virtual const double getVekPodiel(Pohlavie pohlavie, int vek) const override;

	virtual const int getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const override;

	virtual const double getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const override;


	// Inherited via UzemnaJednotka
	virtual const int getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const override;

	virtual const double getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const override;

};


inline bool Obec::patriDoCelku(VyssiCelok* celok) {
	return this->_vyssiCelok == celok;
}

inline const bool Obec::patriDoCelku(UzemnaJednotka* celok) const
{
	return _vyssiCelok == celok || _vyssiCelok->getVyssiCelok() == celok || _vyssiCelok->getVyssiCelok()->getVyssiCelok() == celok; 
}

inline const int Obec::getVzdelaniePocet(TypVzdelania typ) const
{
	for (int i = 0; i < this->_vzdelanie->size(); i++)
	{
		if (_vzdelanie->at(i)->getTyp() == typ) {
			return _vzdelanie->at(i)->getPocet();
		}
	}
}

inline const double Obec::getVzdelaniePodiel(TypVzdelania typ) const
{
	
	return ((double)getVzdelaniePocet(typ) / getPocetSpolu()) * 100;
}

inline const int Obec::getPocetSpolu() const
{
	int spolu = 0; 
	for (int i = 0; i < this->_vzdelanie->size(); i++)
	{
		spolu += _vzdelanie->at(i)->getPocet();
	}
	return spolu;

}

inline const void Obec::setVzdelanie(structures::Array<Vzdelanie*>* vzdelanie)  
{
	this->_vzdelanie =  new structures::Array<Vzdelanie*>(*vzdelanie);
}

inline void const Obec::setVek(structures::Array<Vek*>* vek)
{
	this->_vek = new structures::Array<Vek*>(*vek);
}

inline const int Obec::getVekPocet(Pohlavie pohlavie, int vek) const
{
	int index = vek;
	if(pohlavie == Pohlavie::Zena){
		index += 100;
	}
	return _vek->at(index)->getPocet();
}

inline const double Obec::getVekPodiel(Pohlavie pohlavie, int vek) const
{
	int index = vek;
	if (pohlavie == Pohlavie::Zena) {
		index += 100;
	}
	return (double)( _vek->at(index)->getPocet() / this->getPocetSpolu())*100;
}

inline const int Obec::getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const
{
	int min = 0;
	int max = 0;
	if (vekovaSkupina == VekovaSkupina::Predproduktivni) {
		min = 0;
		max = 14;
	} else if (vekovaSkupina == VekovaSkupina::Produktivni) {
		min = 15;
		max = 64;
	} else if (vekovaSkupina == VekovaSkupina::Poproduktivni) {
		min = 65;
		max = 100;
	}

	int pocet = 0;
	for (int i = min; i < max; i++)
	{
		pocet += _vek->at(i)->getPocet();
		pocet += _vek->at(100 + i)->getPocet();
	}

	return pocet;

}

inline const double Obec::getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const
{
	return (double)(this->getVekovaSkupinaPocet(vekovaSkupina) / this->getPocetSpolu())*100;
}

inline const int Obec::getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const
{
	int minindex = min;
	int maxindex = max;
	if (minindex > maxindex || min < 0 || min > 100 || max < 0 || max > 100)
		return -1;

	if (pohlavie == Pohlavie::Zena) {
		minindex += 100;
		maxindex += 100;
	}

	int pocet = 0;
	for (int i = minindex; i < maxindex; i++)
	{
		pocet += this->_vek->at(i)->getPocet();
	}


	return pocet;
}

inline const double Obec::getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const
{
	return (double)(this->getIntervalVekPocet(pohlavie,min,max) / this->getPocetSpolu()) * 100;
}

inline Obec::~Obec()
{
	delete _vek;
	delete _vzdelanie;
	_vek = nullptr;
	_vzdelanie = nullptr;
}

inline Obec::Obec(TypUzemnejJednotky typ, std::wstring nazov, UzemnaJednotka* celok) : UzemnaJednotka(typ,nazov, celok)
{
	this->_typ = typ;
	this->_nazov = nazov;
	this->_vyssiCelok = celok;
}