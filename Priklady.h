#pragma once
#include "structures/sorted_sequence_table.h"
#include "structures/unsorted_sequence_table.h"
#include "structures/heap_monitor.h"
#include "UzemnaJednotka.h"
#include "Vzdelanie.h"
#include "Input.h"
#include "Criterion.h"
#include "Stat.h"
#include "Filter.h"
#include "structures/quick_sort.h"
#include "SelectionCriterion.h"


class Priklady
{
private:
	Stat* stat;
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_kraj;
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_okres;
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_obec;

	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* tab_all_sorted;

	structures::Array<TypVzdelania>* typyVzdelania; // pre jednoduchsie vypisovanie typov

public: 
	Priklady();
	~Priklady();
	

	void Priklad1Vypis();

	void Priklad2Filtrovanie();

	void Priklad3aTriedenieVekPodiel();
	void Priklad3bTriedenieVekovaSkupinaPocet();

	void Priklad3aTriedenieNazvu();
	void Priklad3bTriedeniePoctu();
	void Priklad3cTriedeniePodielu();

	void Priklad4aNajmensiaVekovaSkupina();
	void Priklad4bNajvacsiaVekovaSkupina();


private:

	void FiltrovaniePreTriedenie4(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha, 
		Pohlavie* vybranePohl, int* min, int* max, VekovaSkupina* vybranaVekovaSkupina);

	void FiltrovaniePreTriedenie3(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha, TypVzdelania* vybrany);

	void FiltrovaniePreVyber(structures::SequenceTable<std::wstring, UzemnaJednotka*>* table);

	void Vypis(structures::SequenceTable<std::wstring, UzemnaJednotka*>& tabulka, TypVzdelania typNaVypis, bool vypisPocet, bool vypisPodiel);
	void spojenieTabuliek(structures::SequenceTable<std::wstring,UzemnaJednotka*>* table);
	std::wstring VypisEnumTyp(TypUzemnejJednotky typ);
	std::wstring VypisEnumVzdelanie(TypVzdelania typ);
	std::wstring VypisEnumPohlavie(Pohlavie typ);
	std::wstring VypisEnumVekovaSkupina(VekovaSkupina typ);

	std::wstring stringTowstring(std::string word) {
		std::wstring wstr(word.begin(), word.end());
		return wstr;
	}

		//void VyberUzemnejJednotky(structures::SequenceTable<std::string, UzemnaJednotka*>* table );


};

