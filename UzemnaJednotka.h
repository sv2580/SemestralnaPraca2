#pragma once
#include <string>
#include "EnumTyp.h"
#include "structures/array.h"
class UzemnaJednotka {
protected:
	std::wstring _nazov;
	TypUzemnejJednotky _typ;
	UzemnaJednotka* _vyssiCelok;


public:

	UzemnaJednotka(TypUzemnejJednotky typ, std::wstring nazov, UzemnaJednotka* vyssiCelok) :
		_typ(typ),
		_nazov(nazov),
		_vyssiCelok(vyssiCelok)
	{
	}
	~UzemnaJednotka() {
		if (_vyssiCelok) {
			delete _vyssiCelok;
		}
	};
	const std::wstring& getNazov() const;
	const TypUzemnejJednotky& getTyp() const;

	virtual void setVyssiCelok(UzemnaJednotka* celok) {
		this->_vyssiCelok = celok;
	}


	virtual const bool patriDoCelku(UzemnaJednotka* celok) const = 0;
	virtual const UzemnaJednotka* getVyssiCelok() const {
		return this->_vyssiCelok;
	};	
	
	virtual const int getPocetSpolu() const = 0;

	virtual const int getVzdelaniePocet(TypVzdelania typ) const = 0;
	virtual const double getVzdelaniePodiel(TypVzdelania typ) const = 0;

	virtual const int getVekPocet(Pohlavie pohlavie, int vek) const = 0;
	virtual const double getVekPodiel(Pohlavie pohlavie, int vek) const = 0;

	virtual const int getIntervalVekPocet(Pohlavie pohlavie, int min, int max) const = 0;
	virtual const double getIntervalVekPodiel(Pohlavie pohlavie, int min, int max) const = 0;

	virtual const int getVekovaSkupinaPocet(VekovaSkupina vekovaSkupina) const = 0;
	virtual const double getVekovaSkupinaPodiel(VekovaSkupina vekovaSkupina) const = 0;

	
};


inline const std::wstring& UzemnaJednotka::getNazov() const
{
	return this->_nazov;
}

inline const TypUzemnejJednotky& UzemnaJednotka::getTyp() const {
	return this->_typ;
}
