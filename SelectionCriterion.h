#pragma once
#include "Criterion.h"
#include "structures/sequence_table.h"

template<typename K, typename O, typename V>
class SelectionCriterion {
private:
	Criterion<O, V>* _criterion;
protected:
	virtual bool isTestedValueBetter(V bestNow, V bestTested) = 0;
public:
	SelectionCriterion(Criterion<O, V>* criterion)
		: _criterion(criterion) {}

	~SelectionCriterion() {
		delete _criterion;
	}

	O& selectBest(structures::SequenceTable<K,O>* table);
};


template<typename K, typename O, typename V>
inline O& SelectionCriterion<K, O, V>::selectBest(structures::SequenceTable<K,O>* table)
{
	O& bestObject = nullptr;
	O& testedObject = *tableItem->accessData(); // pozor na *!!!
	V testedValue = criterion_->evaluate(testedObject);
	if (isTestedValueBetter(bestValue, testedValue)) {
		bestValue = testedValue;
		bestObject = testedObject;
	}

	return bestObject;
}

template<typename K, typename O, typename V>
class SelectionCriterionMin : public SelectionCriterion<K, O, V> {
public:
	SelectionCriterionMin(Criterion<O, V>* criterion) : SelectionCriterion(criterion) {}
protected:
	bool isTestedValueBetter(V bestNow, V bestTested) override;
};

template<typename K, typename O, typename V>
class SelectionCriterionMax : public SelectionCriterion<K, O, V> {
public:
	SelectionCriterionMax(Criterion<O, V>* criterion) : SelectionCriterion(criterion) {}
protected:
	bool isTestedValueBetter(V bestNow, V bestTested) override;

};

template<typename K, typename O, typename V>
inline bool SelectionCriterionMin<K, O, V>::isTestedValueBetter(V bestNow, V bestTested)
{
	return bestTested < bestNow;
}

template<typename K, typename O, typename V>
inline bool SelectionCriterionMax<K, O, V>::isTestedValueBetter(V bestNow, V bestTested)
{
	return bestTested > bestNow;
}
