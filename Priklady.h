#pragma once
#include "structures/sorted_sequence_table.h"
#include "structures/unsorted_sequence_table.h"
#include "structures/heap_monitor.h"
#include "UzemnaJednotka.h"
#include "Vzdelanie.h"
#include "Input.h"
#include "Criterion.h"
#include "Stat.h"

class Priklady
{
private:
	Stat* stat;
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_kraj;
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_okres;
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_obec;

	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* tab_all_sorted;
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tab_all_unsorted;

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



private:

	void FiltrovaniePreTriedenie(structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha, 
		Pohlavie* vybranePohl, int* min, int* max, VekovaSkupina* vybranaVekovaSkupina);

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
	void Filtrovanie(structures::UnsortedSequenceTable<std::wstring,UzemnaJednotka*>* table, char uloha, TypVzdelania* vybrany);

		//void VyberUzemnejJednotky(structures::SequenceTable<std::string, UzemnaJednotka*>* table );


};

