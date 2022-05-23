#pragma once
#include "UzemnaJednotka.h"
#include "VyssiCelok.h"
template <typename O, typename R>
class Criterion {

public: 
	virtual R evaluate(const O& object) = 0;

};

template <typename R>
class CriterionUzemnaJednotka: public Criterion<UzemnaJednotka, R> {
public:
	virtual R evaluate(const UzemnaJednotka& object) override = 0;
};


class CriterionNazov : public CriterionUzemnaJednotka<std::wstring>{
public:

	virtual std::wstring evaluate(const UzemnaJednotka& object) override {
		return object.getNazov();
	}
};


class CriterionNadradena: public CriterionUzemnaJednotka<UzemnaJednotka*> {
public:

	virtual UzemnaJednotka* evaluate(const UzemnaJednotka& object) override {
		return object.getVyssiCelok();
	}
};


class CriterionTyp : public CriterionUzemnaJednotka<TypUzemnejJednotky> {
public:
	TypUzemnejJednotky evaluate(const UzemnaJednotka& object) override;
};

inline TypUzemnejJednotky CriterionTyp::evaluate(const UzemnaJednotka& object) {
	return object.getTyp();
}

class CriterionPrislusnost : public CriterionUzemnaJednotka<bool> {
public:
	CriterionPrislusnost(UzemnaJednotka* celok);
private: 
	UzemnaJednotka* _vyssiCelok;
	virtual bool evaluate(const UzemnaJednotka& object) override;
};

inline CriterionPrislusnost::CriterionPrislusnost(UzemnaJednotka* celok)
{
	_vyssiCelok = celok;
}

inline bool CriterionPrislusnost::evaluate(const UzemnaJednotka& object) {
	return object.patriDoCelku(_vyssiCelok);
}

class CriterionVzdelaniePocet : public CriterionUzemnaJednotka<int> {
private:
	TypVzdelania _typ;
public:
	CriterionVzdelaniePocet(TypVzdelania typ) : _typ(typ) {};
	int evaluate(const UzemnaJednotka& object) override;
};

inline int CriterionVzdelaniePocet::evaluate(const UzemnaJednotka& object) {
	return object.getVzdelaniePocet(_typ);
}


class CriterionVzdelaniePodiel : public CriterionUzemnaJednotka<double> {
private:
	TypVzdelania _typ;
public:
	CriterionVzdelaniePodiel(TypVzdelania typ) : _typ(typ) {};
	double evaluate(const UzemnaJednotka& object) override;
};

inline double CriterionVzdelaniePodiel::evaluate(const UzemnaJednotka& object) {
	return object.getVzdelaniePodiel(_typ);
}



class CriterionIntVekPocet : public CriterionUzemnaJednotka<int> {
private:
	Pohlavie _pohlavie;
	int _min;
	int _max;

public:
	CriterionIntVekPocet(Pohlavie pohlavie, int min, int max) : _pohlavie(pohlavie),
		_min(min), _max(max) {};
	int evaluate(const UzemnaJednotka& object) override;
};

inline int CriterionIntVekPocet::evaluate(const UzemnaJednotka& object) {
	return object.getIntervalVekPocet(_pohlavie, _min, _max);
}

class CriterionIntVekPodiel : public CriterionUzemnaJednotka<double> {
private:
	Pohlavie _pohlavie;
	int _min;
	int _max;
public:
	CriterionIntVekPodiel(Pohlavie pohlavie, int min, int max) : _pohlavie(pohlavie),
		_min(min), _max(max) {};
	double evaluate(const UzemnaJednotka& object) override;
};

inline double CriterionIntVekPodiel::evaluate(const UzemnaJednotka& object) {
	return object.getIntervalVekPodiel(_pohlavie, _min, _max);
}


class CriterionVekovaSkupinaPocet : public CriterionUzemnaJednotka<int> {
private:
	VekovaSkupina _skupina;
public:
	CriterionVekovaSkupinaPocet(VekovaSkupina skupina) : _skupina(skupina){};
	int evaluate(const UzemnaJednotka& object) override;
};

inline int CriterionVekovaSkupinaPocet::evaluate(const UzemnaJednotka& object)
{
	return object.getVekovaSkupinaPocet(_skupina);

}


class CriterionVekovaSkupinaPodiel: public CriterionUzemnaJednotka<double>{
private:
	VekovaSkupina _skupina;


public:
	CriterionVekovaSkupinaPodiel(VekovaSkupina skupina) : _skupina(skupina) {};
	double evaluate(const UzemnaJednotka& object) override;
};

inline double CriterionVekovaSkupinaPodiel::evaluate(const UzemnaJednotka& object) {
	return object.getVekovaSkupinaPodiel(_skupina);
}

