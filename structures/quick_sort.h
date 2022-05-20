#pragma once

#include "sort.h"
#include "unsorted_sequence_table.h"

namespace structures
{
	/// <summary> Triedenie Quick sort. </summary>
	/// <typeparam name = "K"> Kluc prvkov v tabulke. </typepram>
	/// <typeparam name = "T"> Typ dat ukladanych v tabulke. </typepram>
	template <typename K, typename T, typename R>
	class QuickSort : public Sort<K, T>
	{
	public:
		/// <summary> Utriedi tabulku triedenim Quick sort. </summary>
		/// <param name = "table"> NonortedSequenceTable, ktoru ma utriedit. </param>
		void sort(UnsortedSequenceTable<K, T>& table) override;
		void quick(int min, int max, UnsortedSequenceTable<K, T>& table);
		void quickCriterion(int min, int max, structures::UnsortedSequenceTable<K, T*>* table, Criterion<T, R>* criterion);

	};

	template <typename K, typename T, typename R>
	inline void QuickSort<K, T, R>::sort(UnsortedSequenceTable<K, T>& table)
	{
		quick(0, table.size() - 1, table);
	}

	template <typename K, typename T, typename R>
	inline void QuickSort<K, T, R>::quick(int min, int max, UnsortedSequenceTable<K, T>& table)
	{
		K pivot = table.getItemAtIndex((min + max) / 2).getKey();
		int left = min;
		int right = max;

		while (table.getItemAtIndex(left).getKey() < pivot)
			left++;
		while (table.getItemAtIndex(left).getKey() > pivot)
			right--;
		if (left <= right) {
			table.swap(left, right);
			left++;
			right--;
		}

		while (left <= right) {
			if (min < left) {
				quick(min, left, table);
			}

			if (left < max) {
				quick(left, max, table);
			}
		}
	}

	template<typename K, typename T, typename R>
	inline void QuickSort<K, T, R>::quickCriterion(int min, int max, structures::UnsortedSequenceTable<K, T*>* table, Criterion<T, R>* criterion)
	{
		R pivot = criterion->evaluate(dynamic_cast<T&>(*(table->getItemAtIndex((min + max) / 2).accessData())));
		int left = min;
		int right = max;

		while (criterion->evaluate(dynamic_cast<T&>(*(table->getItemAtIndex(left).accessData()))) < pivot)
			left++;
		while (criterion->evaluate(dynamic_cast<T&>(*(table->getItemAtIndex(right).accessData()))) < pivot)
			right--;
		if (left <= right) {
			table->swap(left, right);
			left++;
			right--;
		}

		while (left <= right) {
			if (min < left) {
				quickCriterion(min, left, table, criterion);
			}

			if (left < max) {
				quickCriterion(left, max, table, criterion);
			}
		}
	};


};

template <typename R>
class SortCriterion {
public:
	void SortVzostupne(int min, int max, structures::UnsortedSequenceTable<std::string, UzemnaJednotka*>& table, CriterionUzemnaJednotka<R>* criterion) {
		int index = (min + max) / 2;
		UzemnaJednotka* data = table.getItemAtIndex((min + max) / 2).accessData();
		R pivot = criterion->evaluate(*data);
		int left = min;
		int right = max;
		UzemnaJednotka* leftData = table.getItemAtIndex(left).accessData();
		UzemnaJednotka* rightData = table.getItemAtIndex(right).accessData();

		while (left <= right) {
			leftData = table.getItemAtIndex(left).accessData();
			rightData = table.getItemAtIndex(right).accessData();

			while (criterion->evaluate(*leftData) < pivot) {
				left++;
				leftData = table.getItemAtIndex(left).accessData();
			}
			while (criterion->evaluate(*rightData) > pivot) {
				right--;
				rightData = table.getItemAtIndex(right).accessData();
			}

			if (left <= right)
			{
				table.swap(left, right);
				left++;
				right--;
			}
		}

		if (min < right) {
			SortVzostupne(min, left, table, criterion);
		}

		if (left < max) {
			SortVzostupne(left, max, table, criterion);
		}

	}

	void SortZostupne(int min, int max, structures::UnsortedSequenceTable<std::string, UzemnaJednotka*>& table, CriterionUzemnaJednotka<R>* criterion) {
		int index = (min + max) / 2;
		UzemnaJednotka* data = table.getItemAtIndex((min + max) / 2).accessData();
		R pivot = criterion->evaluate(*data);
		int left = min;
		int right = max;
		UzemnaJednotka* leftData = table.getItemAtIndex(left).accessData();
		UzemnaJednotka* rightData = table.getItemAtIndex(right).accessData();

		while (left <= right) {
			leftData = table.getItemAtIndex(left).accessData();
			rightData = table.getItemAtIndex(right).accessData();

			while (criterion->evaluate(*leftData) > pivot) {
				left++;
				leftData = table.getItemAtIndex(left).accessData();
			}
			while (criterion->evaluate(*rightData) < pivot) {
				right--;
				rightData = table.getItemAtIndex(right).accessData();
			}

			if (left <= right)
			{
				table.swap(left, right);
				left++;
				right--;
			}
		}

		if (min < right) {
			SortZostupne(min, left, table, criterion);
		}

		if (left < max) {
			SortZostupne(left, max, table, criterion);
		}

	}

};