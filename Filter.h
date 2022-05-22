#pragma once
#include "Criterion.h"
#include "structures/sequence_table.h"
#include "structures/sorted_sequence_table.h"

#include <string>
template <typename O>
class Filter {
public:
	virtual bool pass(const O& object) = 0;
};



template <typename O, typename V>
class FilterWithCriterion : public Filter<O> {

public:
	FilterWithCriterion(Criterion<O, V>* pcriterion);
	~FilterWithCriterion();
	bool pass(const O& object) override;

	void filterTable(structures::SequenceTable<std::wstring, O*>* table, structures::SequenceTable<std::wstring, O*>* result);

protected:
	virtual bool passFilter(V value) = 0;


private:
	Criterion<O, V>* criterion;
};

template<typename O, typename V>
inline FilterWithCriterion<O, V>::FilterWithCriterion(Criterion<O, V>* pcriterion) {
	criterion = pcriterion;
}

template<typename O, typename V>
inline FilterWithCriterion<O, V>::~FilterWithCriterion() {
	delete criterion;
}

template<typename O, typename V>
inline bool FilterWithCriterion<O, V>::pass(const O& object) {
	return passFilter(criterion->evaluate(object));
}

template<typename O, typename V>
inline void FilterWithCriterion<O, V>::filterTable(structures::SequenceTable<std::wstring, O*>* table, structures::SequenceTable<std::wstring, O*>* result)
{

	for (structures::TableItem<std::wstring, O*>* item : *table) {
		//cout << item->accessData()->getNazov() << endl;

		if (this->pass(*item->accessData())) {
			result->insert(item->getKey(), item->accessData());
		}
	}

	table->assign(*result);
	delete result;
}


template <typename O, typename V>
class Filter_value : public FilterWithCriterion<O, V> {
public:
	Filter_value(Criterion<O, V>* pcriterion, V pvalue);
protected:
	bool passFilter(V pvalue) override;

private:
	V value;
};


template<typename O, typename V>
inline Filter_value<O, V>::Filter_value(Criterion<O, V>* pcriterion, V pvalue)
	: FilterWithCriterion<O, V>(pcriterion) {
	value = pvalue;
}

template<typename O, typename V>
inline bool Filter_value<O, V>::passFilter(V pvalue) {
	return value == pvalue;
}



template <typename O, typename V>
class Filter_interval : public FilterWithCriterion<O, V> {
public:
	Filter_interval(Criterion<O, V>* pcriterion, V pmin, V pmax);

protected:
	bool passFilter(V value) override;
private:
	V min;
	V max;
};


template<typename O, typename V>
inline  Filter_interval<O, V>::Filter_interval(Criterion<O, V>* pcriterion, V pmin, V pmax)
	: FilterWithCriterion<O, V>(pcriterion) {
	min = pmin;
	max = pmax;
}


template<typename O, typename V>
inline bool Filter_interval<O, V>::passFilter(V value) {
	return value >= min && value <= max;
}


class FilterNazov : public Filter_value<UzemnaJednotka, std::wstring> {
public:
	FilterNazov(std::wstring nazov) :
		Filter_value(new CriterionNazov(), nazov) {}
};

class FilterTyp : public Filter_value<UzemnaJednotka, TypUzemnejJednotky> {
public:
	FilterTyp(TypUzemnejJednotky typ) :
		Filter_value(new CriterionTyp(), typ) {}
};


class FilterPrislusnost : public Filter_value<UzemnaJednotka, bool> {
public:
	FilterPrislusnost(UzemnaJednotka* celok, bool prislusnost) :
		Filter_value(new CriterionPrislusnost(celok), prislusnost) {}
};


class FilterPocet : public Filter_value<UzemnaJednotka, int> {
public:
	FilterPocet(TypVzdelania typ, int pocet) :
		Filter_value(new CriterionVzdelaniePocet(typ), pocet) {}
};

class FilterPodiel: public Filter_value<UzemnaJednotka, double> {
public:
	FilterPodiel(TypVzdelania typ, double pocet) :
		Filter_value(new CriterionVzdelaniePodiel(typ), pocet) {}
};

class FilterIntPocet : public Filter_interval<UzemnaJednotka, int> {
public:
	FilterIntPocet(TypVzdelania typ, int min, int max) :
		Filter_interval(new CriterionVzdelaniePocet(typ), min, max) {}
};

class FilterIntPodiel : public Filter_interval<UzemnaJednotka, double> {
public:
	FilterIntPodiel(TypVzdelania typ, double min, double max) :
		Filter_interval(new CriterionVzdelaniePodiel(typ), min, max) {}
};
/*
class FilterIntVekPocet : public Filter_interval<UzemnaJednotka, int> {
public:
	FilterIntVekPocet(Pohlavie pohlavie, int vek, int min, int max) :
		Filter_interval(new CriterionVekPocet(pohlavie,vek), min, max) {}
};

class FilterIntVekPodiel : public Filter_interval<UzemnaJednotka, double> {
public:
	FilterIntVekPodiel(Pohlavie pohlavie, int vek, double min, double max) :
		Filter_interval(new CriterionVekPodiel(pohlavie, vek), min, max) {}
};*/

class FilterIntVekPocet : public Filter_interval<UzemnaJednotka, int> {
public:
	FilterIntVekPocet(Pohlavie pohlavie, int minPocet, int maxPocet, int minVek, int maxVek) :
		Filter_interval(new CriterionIntVekPocet(pohlavie, minVek, maxVek), minPocet, maxPocet) {}
};

class FilterIntVekPodiel : public Filter_interval<UzemnaJednotka, double> {
public:
	FilterIntVekPodiel(Pohlavie pohlavie, int minPodiel, int maxPodiel, int minVek, int maxVek)  :
		Filter_interval(new CriterionIntVekPodiel(pohlavie, minVek, maxVek), minPodiel, maxPodiel) {}
};


class FilterIntVekovaSkupinaPocet : public Filter_interval<UzemnaJednotka, int> {
public:
	FilterIntVekovaSkupinaPocet(VekovaSkupina skupina, int min, int max) :
		Filter_interval(new CriterionVekovaSkupinaPocet(skupina), min, max) {}
};

class FilterIntVekovaSkupinaPodiel : public Filter_interval<UzemnaJednotka, double> {
public:
	FilterIntVekovaSkupinaPodiel(VekovaSkupina skupina, double min, double max) :
		Filter_interval(new CriterionVekovaSkupinaPodiel(skupina), min, max) {}
};

