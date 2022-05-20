#pragma once
#include "UzemnaJednotka.h"
#include "structures/array_list.h"
#include "Vzdelanie.h"

class VyssiCelok : public UzemnaJednotka {
protected:
	structures::ArrayList<UzemnaJednotka*>* _nizsieCelky = new structures::ArrayList<UzemnaJednotka*>();
	structures::Array<Vzdelanie*>* _vzdelania = new structures::Array<Vzdelanie*>(8);

public:
	VyssiCelok(TypUzemnejJednotky typ, std::wstring nazov, VyssiCelok* vyssiCelok) : UzemnaJednotka(typ, nazov, vyssiCelok) {
		_vyssiCelok = vyssiCelok;
	}
	~VyssiCelok() {
		delete _nizsieCelky;
		_nizsieCelky = nullptr;
		delete _vzdelania;
		_vzdelania = nullptr;
	}
	
	void VypocitajVzdelania();

	structures::ArrayList<UzemnaJednotka*>& getZoznamNizsieCelky() {
		return *_nizsieCelky;
	}

	void const setNizsieCelky(structures::ArrayList<UzemnaJednotka*>* zoznam) {
		this->_nizsieCelky = zoznam;
	}

	void addNizsiCelok(UzemnaJednotka* nizsiCelok) {
		this->_nizsieCelky->add(nizsiCelok);
	}

	// Inherited via UzemnaJednotka
	virtual const bool patriDoCelku(UzemnaJednotka* celok) const override;


	virtual const int getVzdelaniePocet(TypVzdelania typ) const override;
	virtual const double getVzdelaniePodiel(TypVzdelania typ) const override;
	virtual const int getPocetSpolu() const override;

	


	// Inherited via UzemnaJednotka
	virtual const int getVekPocet(Pohlavie pohlavie, int vek) const override;

	virtual const double getVekPodiel(Pohlavie pohlavie, int vek) const override;

	virtual const int getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const override;

	virtual const double getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const override;


	// Inherited via UzemnaJednotka
	virtual const int getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const override;

	virtual const double getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const override;

};

inline void VyssiCelok::VypocitajVzdelania()
{
}

inline const bool VyssiCelok::patriDoCelku(UzemnaJednotka* celok) const
{
	return _vyssiCelok == celok;
}

inline const int VyssiCelok::getVzdelaniePocet(TypVzdelania typ) const
{
	int result = 0;

	for (auto item : *this->_nizsieCelky) {
		result += item->getVzdelaniePocet(typ);
	}

	return result;
}



inline const double VyssiCelok::getVzdelaniePodiel(TypVzdelania typ) const
{
	return ((double)getVzdelaniePocet(typ)/getPocetSpolu()) * 100;
}



inline const int VyssiCelok::getPocetSpolu() const
{
	int result = 0;

	for (auto item : *this->_nizsieCelky) {
		result += item->getPocetSpolu();
	}

	return result;
}

inline const int VyssiCelok::getVekPocet(Pohlavie pohlavie, int vek) const
{
	int result = 0;

	for (auto item : *this->_nizsieCelky) {
		result += item->getVekPocet(pohlavie,vek);
	}

	return result;
}

inline const double VyssiCelok::getVekPodiel(Pohlavie pohlavie, int vek) const
{
	return ((double)getVekPocet(pohlavie,vek) / getPocetSpolu()) * 100;
}

inline const int VyssiCelok::getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const
{

	int result = 0;

	for (auto item : *this->_nizsieCelky) {
		result += item->getVekovaSkupinaPocet(vekovaSkupina);
	}

	return result;
}

inline const double VyssiCelok::getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const
{
	return ((double)getVekovaSkupinaPocet(vekovaSkupina) / getPocetSpolu()) * 100;
}

inline const int VyssiCelok::getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const
{
	int result = 0;

	for (auto item : *this->_nizsieCelky) {
		result += item->getIntervalVekPocet(pohlavie, min, max);
	}

	return result;
}

inline const double VyssiCelok::getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const
{
	return (double)(this->getIntervalVekPocet(pohlavie, min, max) / this->getPocetSpolu()) * 100;
}

