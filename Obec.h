#pragma once

#include "UzemnaJednotka.h";
#include "VyssiCelok.h";
#include "structures/array.h"
#include "Vzdelanie.h"
#include "Vek.h"

const int PREDPRODUKTIVNI_MAX = 14;
const int PRODUKTIVNI_MAX = 64;
const int POPRODUKTIVNI_MAX = 100;
const int POCET_TYPOV_VZDELANI = 8;
const int POCET_ROKOV = 100;


class Obec : public UzemnaJednotka {
private:
	structures::Array<Vzdelanie*>* _vzdelanie;
	structures::Array<Vek*>* _vek;
	int pocetObyvatelov = 0;


public:
	~Obec();
	Obec(TypUzemnejJednotky typ, std::wstring nazov, UzemnaJednotka* celok);


	// Inherited via UzemnaJednotka


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


inline const int Obec::getVzdelaniePocet(TypVzdelania typ) const
{
	return _vzdelanie->at(static_cast<typename std::underlying_type<TypVzdelania>::type>(typ))->getPocet();
}

inline const double Obec::getVzdelaniePodiel(TypVzdelania typ) const
{
	if (pocetObyvatelov == 0)
		return 0;

	return ((double)getVzdelaniePocet(typ) / getPocetSpolu()) * 100;
}

inline const int Obec::getPocetSpolu() const
{
	return pocetObyvatelov;
}

inline const void Obec::setVzdelanie(structures::Array<Vzdelanie*>* vzdelanie)  
{
	this->_vzdelanie =  new structures::Array<Vzdelanie*>(*vzdelanie);

	pocetObyvatelov = 0;

	for (int i = 0; i < this->_vzdelanie->size(); i++)
	{
		pocetObyvatelov += _vzdelanie->at(i)->getPocet();
		VyssiCelok* vyssiCelok = dynamic_cast<VyssiCelok*>(this->_vyssiCelok);
		vyssiCelok->addVzdelanie(_vzdelanie->at(i)->getTyp(), _vzdelanie->at(i)->getPocet());
	}

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
	if (pocetObyvatelov == 0) {
		return 0;
	}
	return ((double) _vek->at(index)->getPocet() / this->getPocetSpolu())*100;
}

inline const int Obec::getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const
{
	int min = 0;
	int max = 0;
	if (vekovaSkupina == VekovaSkupina::Predproduktivni) {
		min = 0;
		max = PREDPRODUKTIVNI_MAX;
	} else if (vekovaSkupina == VekovaSkupina::Produktivni) {
		min = PREDPRODUKTIVNI_MAX + 1;
		max = PRODUKTIVNI_MAX;
	} else if (vekovaSkupina == VekovaSkupina::Poproduktivni) {
		min = PRODUKTIVNI_MAX + 1;
		max = POPRODUKTIVNI_MAX;
	}

	int pocet = 0;
	for (int i = min; i < max; i++)
	{
		pocet += _vek->at(i)->getPocet();
		pocet += _vek->at(POCET_ROKOV + i)->getPocet();
	}

	return pocet;

}

inline const double Obec::getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const
{
	if (pocetObyvatelov == 0) {
		return 0;
	}
	return ((double)this->getVekovaSkupinaPocet(vekovaSkupina) / this->getPocetSpolu())*100;
}

inline const int Obec::getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const
{
	int minindex = min;
	int maxindex = max;
	if (min > max || min < 0 || min > 100 || max < 0 || max > 100)
		return -1;

	if (pohlavie == Pohlavie::Zena) {
		minindex += POCET_ROKOV;
		maxindex += POCET_ROKOV;
	}

	int pocet = 0;
	for (int i = minindex; i < maxindex; i++)
	{
		pocet += this->_vek->at(i)->getPocet();
	}

	if (pohlavie == Pohlavie::Obe) {
		for (int i = minindex+ POCET_ROKOV; i < maxindex+100; i++)
		{
			pocet += this->_vek->at(i)->getPocet();
		}
	}

	return pocet;
}

inline const double Obec::getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const

{
	if (pocetObyvatelov == 0) {
		return 0;
	}

	return ((double)this->getIntervalVekPocet(pohlavie,min,max) / this->getPocetSpolu()) * 100;
}

inline Obec::~Obec()
{
	for (int i = 0; i < POCET_ROKOV*2; i++)
	{
		delete _vek->at(i);
	}
	delete _vek;
	_vek = nullptr;
	for (int i = 0; i < POCET_TYPOV_VZDELANI; i++)
	{
		delete _vzdelanie->at(i);
	}
	delete _vzdelanie;
	_vzdelanie = nullptr;
}

inline Obec::Obec(TypUzemnejJednotky typ, std::wstring nazov, UzemnaJednotka* celok) : UzemnaJednotka(typ,nazov, celok)
{
	this->_typ = typ;
	this->_nazov = nazov;
	this->_vyssiCelok = celok;

	
}