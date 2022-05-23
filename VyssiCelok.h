#pragma once
#include "UzemnaJednotka.h"
#include "structures/array_list.h"
#include <iostream>
#include "Vzdelanie.h"





class VyssiCelok : public UzemnaJednotka {
protected:
	//structures::ArrayList<UzemnaJednotka*>* _nizsieCelky = new structures::ArrayList<UzemnaJednotka*>();
	structures::Array<int>* _vzdelanie = new structures::Array<int>(8);
	structures::Array<int>* _vek = new structures::Array<int>(200);

	int pocetObyvatelov = 0;


public:
	VyssiCelok(TypUzemnejJednotky typ, std::wstring nazov, VyssiCelok* vyssiCelok) : UzemnaJednotka(typ, nazov, vyssiCelok) {
		_vyssiCelok = vyssiCelok;

		for (int i = 0; i < POCET_TYPOV_VZDELANI; i++)
		{
			_vzdelanie->at(i) = 0;
		}

		for (int i = 0; i < POCET_ROKOV*2; i++)
		{
			_vek->at(i) = 0;
		}

	}
	~VyssiCelok() {
		delete _vek;
		_vek = nullptr;

		delete _vzdelanie;
		_vzdelanie = nullptr;
	}
	


	void addPocetObyvatelov(int pocet) {
		pocetObyvatelov += pocet;
	}



	virtual const int getVzdelaniePocet(TypVzdelania typ) const override;
	virtual const double getVzdelaniePodiel(TypVzdelania typ) const override;
	virtual const int getPocetSpolu() const override;

	virtual const int getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const override;
	virtual const double getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const override;


	virtual const int getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const override;
	virtual const double getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const override;

	void addVzdelanie(int index, int pocet) {
		_vzdelanie->at(index) += pocet;
		pocetObyvatelov += pocet;
		if (this->getVyssiCelok() != nullptr) {
			VyssiCelok* vyssiCelok = dynamic_cast<VyssiCelok*>(this->_vyssiCelok);
			vyssiCelok->addVzdelanie(index, pocet);
		}
	}

	void addVek(int index, int pocet) {
		_vek->at(index) += pocet;
		if (this->getVyssiCelok() != NULL) {
			VyssiCelok* vyssiCelok = dynamic_cast<VyssiCelok*>(this->_vyssiCelok);
			vyssiCelok->addVek(index, pocet);
		}
	}
};



inline const int VyssiCelok::getVzdelaniePocet(TypVzdelania typ) const
{
	return _vzdelanie->at(static_cast<typename std::underlying_type<TypVzdelania>::type>(typ));
}


inline const double VyssiCelok::getVzdelaniePodiel(TypVzdelania typ) const
{
	return ((double)getVzdelaniePocet(typ)/getPocetSpolu()) * 100;
}


inline const int VyssiCelok::getPocetSpolu() const
{
	return pocetObyvatelov;
}

inline const int VyssiCelok::getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const
{
	int min = 0;
	int max = 0;
	if (vekovaSkupina == VekovaSkupina::Predproduktivni) {
		min = 0;
		max = PREDPRODUKTIVNI_MAX;
	}
	else if (vekovaSkupina == VekovaSkupina::Produktivni) {
		min = PREDPRODUKTIVNI_MAX + 1;
		max = PRODUKTIVNI_MAX;
	}
	else if (vekovaSkupina == VekovaSkupina::Poproduktivni) {
		min = PRODUKTIVNI_MAX + 1;
		max = POPRODUKTIVNI_MAX;
	}

	int pocet = 0;
	for (int i = min; i < max; i++)
	{
		pocet += _vek->at(i);
		pocet += _vek->at(POCET_ROKOV + i);
	}

	return pocet;
}

inline const double VyssiCelok::getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const
{
	return ((double)getVekovaSkupinaPocet(vekovaSkupina) / getPocetSpolu()) * 100;
}

inline const int VyssiCelok::getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const
{
	int minindex = min;
	int maxindex = max;
	if (min > max || min < 0 || min > POCET_ROKOV || max < 0 || max > 100)
		return -1;

	if (pohlavie == Pohlavie::Zena) {
		minindex += POCET_ROKOV;
		maxindex += POCET_ROKOV;
	}

	int pocet = 0;
	for (int i = minindex; i < maxindex; i++)
	{
		pocet += this->_vek->at(i);
	}

	if (pohlavie == Pohlavie::Obe) {
		for (int i = minindex + POCET_ROKOV; i < maxindex + POCET_ROKOV; i++)
		{
			pocet += this->_vek->at(i);
		}
	}

	return pocet;
}

inline const double VyssiCelok::getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const
{
	return (((double)getIntervalVekPocet(pohlavie, min, max) / pocetObyvatelov) * 100);
}

