#pragma once
#include <string>
#include "EnumTyp.h"
#include "structures/array.h"

const int PREDPRODUKTIVNI_MAX = 14;
const int PRODUKTIVNI_MAX = 64;
const int POPRODUKTIVNI_MAX = 100;
const int POCET_TYPOV_VZDELANI = 8;
const int POCET_ROKOV = 100;



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
		_nazov = L"";
		_vyssiCelok = nullptr;
	};

	const std::wstring& getNazov() const;
	const TypUzemnejJednotky& getTyp() const;

	virtual void setVyssiCelok(UzemnaJednotka* celok) {
		this->_vyssiCelok = celok;
	}

	virtual const bool patriDoCelku(UzemnaJednotka* celok) const {
		bool result = false;
		const UzemnaJednotka* current = getVyssiCelok();
		while (current != NULL) {
			result = current == celok;
			if (result) {
				return result;
			}
			current = current->getVyssiCelok();
		}
		return result; 
	}


	virtual UzemnaJednotka* getVyssiCelok() const {
		return this->_vyssiCelok;
	};	
	
	virtual const int getPocetSpolu() const = 0;

	virtual const int getVzdelaniePocet(TypVzdelania typ) const = 0;
	virtual const double getVzdelaniePodiel(TypVzdelania typ) const = 0;


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


