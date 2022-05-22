#include "Priklady.h"


Priklady::Priklady()
{
	Input* input = new Input();
    stat = new Stat(TypUzemnejJednotky::Stat, L"Slovensko", nullptr);
	table_kraj = new structures::SortedSequenceTable<std::wstring, Kraj*>();
	table_okres = new structures::SortedSequenceTable<std::wstring, Okres*>();
	table_obec = new structures::SortedSequenceTable<std::wstring, Obec*>();
	input->InputKraje(table_kraj, L"csv/kraje.csv", stat);
	input->InputOkresy(table_okres, L"csv/okresy.csv", table_kraj);
	input->InputObce(table_obec, L"csv/vzdelanie.csv", table_okres);

	input->InputVek(L"csv/vek.csv", table_obec);
	this->tab_all_sorted = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>();

	this->spojenieTabuliek(this->tab_all_sorted);

	typyVzdelania = new structures::Array<TypVzdelania>(8);
	typyVzdelania->at(0) = TypVzdelania::BezDo15;
	typyVzdelania->at(1) = TypVzdelania::BezNad15;
	typyVzdelania->at(2) = TypVzdelania::Zakladne;
	typyVzdelania->at(3) = TypVzdelania::StredneOdborne;
	typyVzdelania->at(4) = TypVzdelania::UplneStredne;
	typyVzdelania->at(5) = TypVzdelania::VyssieOdborne;
	typyVzdelania->at(6) = TypVzdelania::Vysokoskolske;
	typyVzdelania->at(7) = TypVzdelania::Nezistene;
	delete input;
}

Priklady::~Priklady()
{
	delete stat;
	for (auto item : *table_kraj) {
		delete item->accessData();
	}
	delete table_kraj;
	table_kraj = nullptr;
	for (auto item : *table_okres) {
		delete item->accessData();
	}

	for (auto item : *table_obec) {
		delete item->accessData();
	}
	delete table_obec;
	delete table_okres;
	table_obec = nullptr;
	table_okres = nullptr;

	delete typyVzdelania;

	typyVzdelania = nullptr;

	delete tab_all_sorted;

	tab_all_sorted = nullptr;

}


void Priklady::Priklad1Vypis()
{
	bool pokracuj = false;
	UzemnaJednotka* data = nullptr;
	std::wstring nazov;
	while (!pokracuj) {
		std::wcout << std::endl << L"Zadajte nazov uzemnej jednotky" << std::endl;
		std::wcin >> nazov;

		bool najdene = tab_all_sorted->tryFind(nazov, data);
		if (najdene)
			pokracuj = true;
		else {
			std::wcout << std::endl << L"Uzemna jednotka s takymto nazvom nebola najdena. Chcete pokracovat v hladani? L" << std::endl;
			std::wcout << std::endl << L"[1] Ano hladat znovu" << std::endl << L"[0] Nie prejst k ostatnym uloham." << std::endl;
			int filter;
			std::wcin >> filter;
			if (filter == 0)
				pokracuj = true;
		}
	}

	const UzemnaJednotka* current = data;

	pokracuj = false;
	while (!pokracuj) {

		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;

		std::wcout << L" Hodnoty vzdelania: " << std::endl;


		for (int i = 0; i < this->typyVzdelania->size(); ++i)
		{
			std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
				std::to_wstring(current->getVzdelaniePocet(this->typyVzdelania->at(i))) << std::endl;
		}


		std::wcout << L" Hodnoty vekovovych skupin: " << std::endl;

		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;


		if (current->getTyp() != TypUzemnejJednotky::Stat)
			current = current->getVyssiCelok();
		else
			pokracuj = true;
	}

	std::wcout << std::endl;


	return;
}

void Priklady::Priklad2Filtrovanie()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	int filter = -1;
	bool pokracuj = false;

	bool typ = false;
	bool prislusnost = false;
	bool vzdelaniePocet = false;
	bool vzdelaniePodiel = false;
	bool vekPocet = false;
	bool vekPodiel = false;
	bool vekovaSkupinaPocet = false;
	bool vekovaSkupinaPodiel = false;


	int minVekPocet = 0;
	int maxVekPocet = 0;
	int minVekPodiel = 0;
	int maxVekPodiel = 0;

	Pohlavie vybranePocet = Pohlavie::Obe;
	Pohlavie vybranePodiel = Pohlavie::Obe;

	while (!pokracuj) {
		std::wcout << L"0 - Pokracovat" << std::endl;

		if (!typ)
			std::wcout << L"1  - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"2 - Filtrovat podla prislusnosti" << std::endl;
		if (!vzdelaniePocet)
			std::wcout << L"3 - Filtrovat podla poctu vzdelania" << std::endl;
		if (!vzdelaniePodiel)
			std::wcout << L"4 - Filtrovat podla podielu vzdelania" << std::endl;
		if (!vekPocet)
			std::wcout << L"5 - Filtrovat podla poctu veku" << std::endl;
		if (!vekPodiel)
			std::wcout << L"6 - Filtrovat podla podielu veku" << std::endl;
		if (!vekovaSkupinaPocet)
			std::wcout << L"7 - Filtrovat podla poctu vekovej skupiny" << std::endl;
		if (!vekovaSkupinaPodiel)
			std::wcout << L"8 - Filtrovat podla podielu vekovej skupiny" << std::endl;

		std::wcin >> filter;
		if (filter == 0) {
			pokracuj = true;
		}
		else if (filter == 1 && !typ) {
			typ = true;
			std::wcout << L"Vyberte si uzemnu jednotku: " << std::endl;
			std::wcout << L"1. Stat" << std::endl << L"2. Kraj" << std::endl << L"3. Okres" << std::endl << L"4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte jedno z cisiel: " << std::endl;
				std::wcin >> vyber;
				switch (vyber)
				{
				case 1:
					vybranyTyp = TypUzemnejJednotky::Stat;
					break;
				case 2:
					vybranyTyp = TypUzemnejJednotky::Kraj;
					vybrane = true;
					break;
				case 3:
					vybranyTyp = TypUzemnejJednotky::Okres;
					vybrane = true;
					break;
				case 4:
					vybranyTyp = TypUzemnejJednotky::Obec;
					vybrane = true;
					break;
				}
				FilterTyp* fTyp = new FilterTyp(vybranyTyp);
				fTyp->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
				delete fTyp;
				vybrane = true;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl <<L"Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
			std::wcin >> nazov;
			FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
			fPrislusnost->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPrislusnost;

		}
		else if (filter == 3 && !vzdelaniePocet) {
			vzdelaniePocet = true;
			bool vybrane = false;
			int cisloTypu;
			TypVzdelania vybranyTyp;

			while (!vybrane) {
				std::wcout << std::endl <<L"Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::wcin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					vybranyTyp = typyVzdelania->at(cisloTypu - 1);

					vybrane = true;
				}
			}
			int min;
			int max;
			std::wcout << L"Zadajte dolnu hranicu poctu" << std::endl;
			std::wcin >> min;
			std::wcout << L"Zadajte hornu hranicu poctu (pokial zadate 0, horna hranica bude maximalna)";
			std::wcin >> max;
			if (max == 0) {
				max = INT_MAX;
			}
			FilterIntPocet* fPocet = new FilterIntPocet(vybranyTyp, min, max);
			fPocet->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;
		}
		else if (filter == 4 && !vzdelaniePodiel) {
			vzdelaniePodiel = false;
			bool vybrane = false;
			int cisloTypu;
			TypVzdelania vybranyTyp;

			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::wcin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					vybranyTyp = typyVzdelania->at(cisloTypu - 1);

					vybrane = true;
				}
			}
			double min;
			double max;
			std::wcout << L"Zadajte dolnu hranicu podielu" << std::endl;
			std::wcin >> min;
			std::wcout << L"Zadajte hornu hranicu podielu (pokial zadate 0, horna hranica bude 100%)";
			std::wcin >> max;
			if (max == 0) {
				max = LLONG_MAX;
			}
			FilterIntPodiel* fPodiel = new FilterIntPodiel(vybranyTyp, min, max);
			fPodiel->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPodiel;
		}
		else if (filter == 5 && !vekPocet) {
			vekPocet = true;
			bool vybrane = false;
			int cislo;

			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte pohlavie ktore chcete filtrovat: " << std::endl <<
				L"[1.] Zeny " << std::endl << L"[2.] Muzi "  << std::endl << std::endl << L"[3.] Zeny aj muzi " << std::endl;
				
				std::wcin >> cislo;
				if (cislo > 0 && cislo <= 3)
				{
					vybrane = true;
				}
			}

			int minPocet = 0;
			int maxPocet = 0;
			vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
				std::wcin >> minVekPocet;
				std::wcout << L"Zadajte hornu vekovu hranicu [0 - 100]";
				std::wcin >> maxVekPocet;
				if (minVekPocet <= maxVekPocet && minVekPocet >= 0 && 
					minVekPocet <= 100 && maxVekPocet >= 0 && maxVekPocet <= 100)
					vybrane = true;
			}
			vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte dolnu hranicu poctu" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu poctu";
				std::wcin >> maxPocet;
				if (minPocet < maxPocet && minPocet >= 0 && maxPocet >= 0)
					vybrane = true;
			}
			vybranePocet = Pohlavie::Zena;
			if (cislo == 2) {
				vybranePocet = Pohlavie::Muz;
			}
			else if (cislo == 3) {
				vybranePocet = Pohlavie::Obe;
			}
			FilterIntVekPocet* fPocet = new FilterIntVekPocet(vybranePocet, minPocet,maxPocet,minVekPocet,maxVekPocet);
			fPocet->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;

		}
		else if (filter == 6 && !vekPodiel) {
			vekPodiel = true;
			bool vybrane = false;
			int cislo;

			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte pohlavie ktore chcete filtrovat: L" << std::endl <<
					L"[1.] Zeny" << std::endl << L"[2.] Muzi" << std::endl <<  L"[3.] Zeny aj muzi" << std::endl;

				std::wcin >> cislo;
				if (cislo > 0 && cislo <= 3)
				{
					vybrane = true;
				}
			}

			int minPocet = 0;
			int maxPocet = 0;
			vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
				std::wcin >> minVekPodiel;
				std::wcout << L"Zadajte hornu vekovu hranicu [0 - 100]";
				std::wcin >> maxVekPodiel;
				if (minVekPodiel < maxVekPodiel && minVekPodiel >= 0 && minVekPodiel <= 100 && maxVekPodiel >= 0 && maxVekPodiel <= 100)
					vybrane = true;
			}
			vybrane = false;

			while (!vybrane) {
				std::wcout << L"Zadajte dolnu hranicu podielu" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu podielu";
				std::wcin >> maxPocet;
				if (minPocet < maxPocet && minPocet >= 0 && maxPocet >= 0 && minPocet <= 100 && maxPocet <= 100)
					vybrane = true;
			}
			vybranePodiel = Pohlavie::Zena;
			if (cislo == 2) {
				vybranePodiel = Pohlavie::Muz;
			}
			else if (cislo == 3) {
				vybranePodiel = Pohlavie::Obe;
			}
			FilterIntVekPodiel* fPodiel = new FilterIntVekPodiel(vybranePodiel, minPocet, minPocet, minVekPodiel, maxVekPodiel);
			fPodiel->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPodiel;
		}
		else if (filter == 7 && !vekovaSkupinaPocet) {
			vekovaSkupinaPocet = true;
			bool vybrane = false;
			int cislo;
			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
					L"[1.] Predproduktivni " << std::endl << L"[2.] Produktivni " << std::endl << std::endl << L"[3.] Poproduktivni " << std::endl;

				std::wcin >> cislo;
				if (cislo > 0 && cislo <= 3)
				{
					vybrane = true;
				}
			}

			int minPocet = 0;
			int maxPocet = 0;
			vybrane = false;

			while (!vybrane) {
				std::wcout << L"Zadajte dolnu hranicu poctu" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu poctu";
				std::wcin >> maxPocet;
				if (minPocet < maxPocet && minPocet >= 0 && maxPocet >= 0 )
					vybrane = true;
			}
			VekovaSkupina vekovaSkupina;
			switch (cislo)
			{
			case 1:
				vekovaSkupina = VekovaSkupina::Predproduktivni;
				break;
			case 2:
				vekovaSkupina = VekovaSkupina::Produktivni;
				break;
			default:
				vekovaSkupina = VekovaSkupina::Poproduktivni;
				break;
			}
			FilterIntVekovaSkupinaPocet* fPocet = new FilterIntVekovaSkupinaPocet(vekovaSkupina, minPocet, minPocet);
			fPocet->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;

		}
		else if (filter == 8 && !vekovaSkupinaPodiel)
		{
			vekovaSkupinaPodiel = true;
			bool vybrane = false;
			int cislo;

			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
					L"[1.] Predproduktivni " << std::endl << L"[2.] Produktivni " << std::endl << std::endl << L"[3.] Poproduktivni " << std::endl;

				std::wcin >> cislo;
				if (cislo > 0 && cislo <= 3)
				{
					vybrane = true;
				}
			}

			int minPocet = 0;
			int maxPocet = 0;
			vybrane = false;

			while (!vybrane) {
				std::wcout << L"Zadajte dolnu hranicu podielu" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu podielu";
				std::wcin >> maxPocet;
				if (minPocet < maxPocet && minPocet >= 0 && maxPocet >= 0 && minPocet <= 1000 && maxPocet <= 100)
					vybrane = true;
			}
			VekovaSkupina vekovaSkupina;
			switch (cislo)
			{
			case 1:
				vekovaSkupina = VekovaSkupina::Predproduktivni;
				break;
			case 2:
				vekovaSkupina = VekovaSkupina::Produktivni;
				break;
			default:
				vekovaSkupina = VekovaSkupina::Poproduktivni;
				break;
			}
			FilterIntVekovaSkupinaPodiel* fPodiel = new FilterIntVekovaSkupinaPodiel(vekovaSkupina, minPocet, minPocet);
			fPodiel->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPodiel;

		}
	}

	for (structures::TableItem<std::wstring, UzemnaJednotka*>* item : *tabulkaNaPracovanie) 
	{
		const UzemnaJednotka* current = item->accessData();

		pokracuj = false;
		while (!pokracuj) {

			std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << L"Nazov: ";
			std::wcout << current->getNazov() << std::endl;

			if (vzdelaniePocet) {
				std::wcout << L"Hodnoty vzdelania: " << std::endl;

				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
						std::to_wstring(current->getVzdelaniePocet(this->typyVzdelania->at(i))) << std::endl;
				}
			}


			if (vzdelaniePodiel) {
				std::wcout <<  L"Hodnoty vzdelania: " << std::endl;

				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
						std::to_wstring(current->getVzdelaniePodiel(this->typyVzdelania->at(i))) << std::endl;
				}
			}

			if (vekovaSkupinaPocet) {
				std::wcout << L"Hodnoty vekovovych skupin: " << std::endl;

				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

			}

			if (vekovaSkupinaPodiel) {
				std::wcout << L"Hodnoty vekovovych skupin: " << std::endl;

				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

			}

			if (vekPocet) {
				std::wcout << L" Pocet kriteria veku medzi rokmi "  + std::to_wstring(minVekPocet)  
					+ L" - " + std::to_wstring(maxVekPocet) + L" pre pohlavie: " + VypisEnumPohlavie(vybranePocet) 
					+ L" : " << current->getIntervalVekPocet(vybranePocet,minVekPocet,maxVekPocet) << std::endl;

			}

			if (vekPodiel) {
				std::wcout << L" Podiel kriteria veku medzi rokmi " + std::to_wstring(minVekPodiel)
					+ L" - " + std::to_wstring(maxVekPodiel) + L" pre pohlavie: " + VypisEnumPohlavie(vybranePodiel)
					+ L" : " << current->getIntervalVekPodiel(vybranePodiel, minVekPodiel, minVekPodiel) << std::endl;
			}
			
			std::wcout << L"*****" << std::endl;

			if (current->getTyp() != TypUzemnejJednotky::Stat)
				current = current->getVyssiCelok();
			else
				pokracuj = true;
		}
	}
	delete tabulkaNaPracovanie;
	return;
}

void Priklady::Priklad3aTriedenieVekPodiel()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	Pohlavie pohlavie = Pohlavie::Obe;
	int min = 0;
	int max = 0;

	FiltrovaniePreTriedenie4(table, 'a', &pohlavie, &min, &max, nullptr);

	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}
	delete table;

	bool pokracovat = false;
	std::wcout << L"1. Vzostupne " << std::endl << L"2. Zostupne" << std::endl;

	SortCriterion<double>* sort = new SortCriterion<double>();
	CriterionIntVekPodiel* cVek = new CriterionIntVekPodiel(pohlavie,min,max);

	int filter;
	while (!pokracovat) {
		std::wcin >> filter;
		if (filter == 1) {
			sort->SortVzostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cVek);
			pokracovat = true;
		}
		else if (filter == 2) {
			sort->SortZostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cVek);
			pokracovat = true;
		}
	}

	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *tabulkaNaPracovanie)
	{
		std::wcout << uzemnaJednotka->accessData()->getNazov() << std::endl;
		std::wcout << L"Pocet muzov" + uzemnaJednotka->accessData()->getIntervalVekPocet(Pohlavie::Muz, min, max) << std::endl;
		std::wcout << L"Pocet zien" + uzemnaJednotka->accessData()->getIntervalVekPocet(Pohlavie::Zena, min, max) << std::endl;
		std::wcout << L"Podiel " + VypisEnumPohlavie(pohlavie) + L" : " + 
			std::to_wstring(uzemnaJednotka->accessData()->getIntervalVekPodiel(pohlavie, min, max)) << std::endl;

	}

	delete sort;
	delete cVek;
	delete tabulkaNaPracovanie;
	return;
}
void Priklady::Priklad3bTriedenieVekovaSkupinaPocet()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);


	VekovaSkupina vekSkupina = VekovaSkupina::Produktivni;
	int min = 0;
	int max = 0;

	FiltrovaniePreTriedenie4(table, 'b', nullptr, &min, &max, &vekSkupina);
	
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}
	delete table;

	bool pokracovat = false;
	std::wcout << L"1. Vzostupne " << std::endl << L"2. Zostupne" << std::endl;

	SortCriterion<int>* sort = new SortCriterion<int>();
	CriterionVekovaSkupinaPocet* cVek = new CriterionVekovaSkupinaPocet(vekSkupina);

	int filter;
	while (!pokracovat) {
		std::wcin >> filter;
		if (filter == 1) {
			sort->SortVzostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cVek);
			pokracovat = true;
		}
		else if (filter == 2) {
			sort->SortZostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cVek);
			pokracovat = true;
		}
	}

	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *tabulkaNaPracovanie)
	{
		std::wcout << uzemnaJednotka->accessData()->getNazov() << std::endl;
		std::wcout << L"Podiel " + uzemnaJednotka->accessData()->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni) << std::endl;
		std::wcout << L"Podiel " + uzemnaJednotka->accessData()->getVekovaSkupinaPocet(VekovaSkupina::Produktivni) << std::endl;
		std::wcout << L"Podiel " + uzemnaJednotka->accessData()->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni) << std::endl;

	}

	delete sort;
	delete tabulkaNaPracovanie;
	return;
}

void Priklady::Priklad3aTriedenieNazvu() {
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	FiltrovaniePreTriedenie3(table, 'a', nullptr);
	bool pokracovat = false;

	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}
	delete table;


	std::wcout << L"1. Vzostupne " << std::endl << L"2. Zostupne" << std::endl;

	SortCriterion<std::wstring>* sort = new SortCriterion<std::wstring>();
	CriterionNazov* cNazov = new CriterionNazov();

	int filter;
	while (!pokracovat) {
		std::wcin >> filter;
		if (filter == 1) {
			sort->SortVzostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cNazov);
			pokracovat = true;
		}
		else if (filter == 2) {
			sort->SortZostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cNazov);
			pokracovat = true;
		}
	}

	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *tabulkaNaPracovanie)
	{
		std::wcout << uzemnaJednotka->accessData()->getNazov() << std::endl;
	}

	delete sort;
	delete cNazov;
	delete tabulkaNaPracovanie;
	return;
}
void Priklady::Priklad3bTriedeniePoctu()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	TypVzdelania vzdelanie = TypVzdelania::Null;
	FiltrovaniePreTriedenie3(table, 'b', &vzdelanie);
	bool pokracovat = false;
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}

	delete table;

	std::wcout << L"1. Vzostupne " << std::endl << L"2. Zostupne" << std::endl;

	SortCriterion<int>* sort = new SortCriterion<int>();
	CriterionVzdelaniePocet* cPocet = new CriterionVzdelaniePocet(vzdelanie);

	int filter;
	while (!pokracovat) {
		std::wcin >> filter;
		if (filter == 1) {
			sort->SortVzostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cPocet);
			pokracovat = true;
		}
		else if (filter == 2) {
			sort->SortZostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cPocet);
			pokracovat = true;
		}
	}

	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *tabulkaNaPracovanie)
	{
		std::wcout << uzemnaJednotka->accessData()->getNazov();
		std::wcout << L" " + std::to_wstring(uzemnaJednotka->accessData()->getVzdelaniePocet(vzdelanie));
	}

	delete sort;
	delete cPocet;
	delete tabulkaNaPracovanie;
	return;
}
void Priklady::Priklad3cTriedeniePodielu()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	TypVzdelania vzdelanie = TypVzdelania::Null;
	FiltrovaniePreTriedenie3(table, 'c', &vzdelanie);
	bool pokracovat = false;
	
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}

	
	std::wcout << L"1. Vzostupne " << std::endl << L"2. Zostupne" << std::endl;


	SortCriterion<double>* sort = new SortCriterion<double>();
	CriterionVzdelaniePodiel* cPodiel = new CriterionVzdelaniePodiel(vzdelanie);

	int filter;
	while (!pokracovat) {
		std::wcin >> filter;
		if (filter == 1) {
			sort->SortVzostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cPodiel);
			pokracovat = true;
		}
		else if (filter == 2) {
			sort->SortZostupne(0, tabulkaNaPracovanie->size() - 1, *tabulkaNaPracovanie, cPodiel);
			pokracovat = true;
		}
	}

	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *tabulkaNaPracovanie)
	{
		std::wcout << uzemnaJednotka->accessData()->getNazov();
		std::wcout << L" " + std::to_wstring(uzemnaJednotka->accessData()->getVzdelaniePodiel(vzdelanie));
	}

	delete sort;
	delete cPodiel;
	delete tabulkaNaPracovanie;
	return;
}

void Priklady::Priklad4aNajmensiaVekovaSkupina()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	FiltrovaniePreVyber(tabulkaNaPracovanie);
	bool pokracuj = false;
	int cislo;
	while (!pokracuj)
	{
		std::wcout << L"Zadajte ktoru vekovu skupinu chcete pouzit: " << std::endl;
		std::wcout << L"[1.] Predproduktivni" << std::endl;
		std::wcout << L"[2.] Produktivni" << std::endl;
		std::wcout << L"[3.] Poproduktivni" << std::endl;

		std::wcin >> cislo;
		if (cislo >= 1 && cislo <= 3)
			pokracuj = true;
	}
	VekovaSkupina vekovaSkupina = VekovaSkupina::Predproduktivni;

	if(cislo == 2)
		vekovaSkupina = VekovaSkupina::Produktivni;

	if (cislo == 2)
		vekovaSkupina = VekovaSkupina::Poproduktivni;
	
	SelectionMinVekovaSkupina<std::wstring>* select = new SelectionMinVekovaSkupina<std::wstring>(vekovaSkupina);
	UzemnaJednotka* najlepsiaUzemnaJednotka = select->selectBest(tabulkaNaPracovanie, INT_MAX);
	const UzemnaJednotka* current = najlepsiaUzemnaJednotka;

	pokracuj = false;
	while (!pokracuj) {

		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;

		std::wcout << L"Hodnoty vekovovych skupin: " << std::endl;

		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

		if (current->getTyp() != TypUzemnejJednotky::Stat)
			current = current->getVyssiCelok();
		else
			pokracuj = true;
	}


	delete select;
	delete tabulkaNaPracovanie;
}

void Priklady::Priklad4bNajvacsiaVekovaSkupina()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	FiltrovaniePreVyber(tabulkaNaPracovanie);


	bool pokracuj = false;
	int cislo;
	while (!pokracuj)
	{
		std::wcout << L"Zadajte ktoru vekovu skupinu chcete pouzit: " << std::endl;
		std::wcout << L"[1.] Predproduktivni" << std::endl;
		std::wcout << L"[2.] Produktivni" << std::endl;
		std::wcout << L"[3.] Poproduktivni" << std::endl;

		std::wcin >> cislo;
		if (cislo >= 1 && cislo <= 3)
			pokracuj = true;
	}
	VekovaSkupina vekovaSkupina = VekovaSkupina::Predproduktivni;

	if (cislo == 2)
		vekovaSkupina = VekovaSkupina::Produktivni;

	if (cislo == 2)
		vekovaSkupina = VekovaSkupina::Poproduktivni;
	

	SelectionMaxVekovaSkupina<std::wstring>* select = new SelectionMaxVekovaSkupina<std::wstring>(vekovaSkupina);
	UzemnaJednotka* najlepsiaUzemnaJednotka = (select->selectBest(tabulkaNaPracovanie, 0));
	const UzemnaJednotka* current = najlepsiaUzemnaJednotka;

	pokracuj = false;
	while (!pokracuj) {

		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;

		std::wcout << L" Hodnoty vekovovych skupin: " << std::endl;

		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

		if (current->getTyp() != TypUzemnejJednotky::Stat)
			current = current->getVyssiCelok();
		else
			pokracuj = true;
	}


	delete select;
	delete tabulkaNaPracovanie;
}




void Priklady::spojenieTabuliek(structures::SequenceTable<std::wstring, UzemnaJednotka*>* table)
{
	table->insert(stat->getNazov(), stat);
	for (structures::TableItem<std::wstring, Kraj*>* uzemnaJednotka : *table_kraj) {
		table->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
	for (structures::TableItem<std::wstring, Okres*>* uzemnaJednotka : *table_okres) {
		table->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
	for (structures::TableItem<std::wstring, Obec*>* uzemnaJednotka : *table_obec) {
		table->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
}


void Priklady::FiltrovaniePreTriedenie4(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha,
	Pohlavie* vybranePohl, int* min, int* max, VekovaSkupina* vybranaVekovaSkupina) {

	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;
	bool filterNavyse = false;

	while (!pokracuj) {
		if (!typ)
			std::wcout << L"1 - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"2 - Filtrovat podla prislusnosti" << std::endl;
		if (uloha == 'a' && !filterNavyse)
			std::wcout << L"3 - Filtrovat podla podielu veku" << std::endl;
		if (uloha == 'b' && !filterNavyse)
			std::wcout << L"4 - Filtrovat podla poctu vekovej skupiny" << std::endl;
		if(filterNavyse)
			std::wcout << L"0 - Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			std::wcout << L"Vyberte si uzemnu jednotku:" << std::endl;
			std::wcout << L"1. Stat: L" << std::endl << L"2. Kraj" << std::endl << L"3. Okres" << std::endl << L"4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte jedno z cisiel: " << std::endl;
				std::wcin >> vyber;
				switch (vyber)
				{
				case 1:
					vybranyTyp = TypUzemnejJednotky::Stat;
					break;
				case 2:
					vybranyTyp = TypUzemnejJednotky::Kraj;
					vybrane = true;
					break;
				case 3:
					vybranyTyp = TypUzemnejJednotky::Okres;
					vybrane = true;
					break;
				case 4:
					vybranyTyp = TypUzemnejJednotky::Obec;
					vybrane = true;
					break;
				}
				FilterTyp* fTyp = new FilterTyp(vybranyTyp);
				fTyp->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
				vybrane = true;
				delete fTyp;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl << L"Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
			std::wcin >> nazov;
			FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
			fPrislusnost->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPrislusnost;


		}
		else if (filter == 3 && (uloha == 'a') && !filterNavyse) {
			filterNavyse = false;
			bool vybrane = false;
			int cislo;

			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte pohlavie ktore chcete filtrovat: " << std::endl <<
					L"[1.] Zeny " << std::endl << L"[2.] Muzi " << std::endl << L"[3.] Zeny aj muzi " << std::endl;

				std::wcin >> cislo;
				if (cislo > 0 && cislo <= 3)
				{
					vybrane = true;
				}
			}
			int minVekPodiel = 0;
			int maxVekPodiel = 0;
			int minPocet = 0;
			int maxPocet = 0;
			vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
				std::wcin >> minVekPodiel;
				std::wcout << L"Zadajte hornu vekovu hranicu [0 - 100]" << std::endl;
				std::wcin >> maxVekPodiel;
				if (minVekPodiel < maxVekPodiel && minVekPodiel >= 0 && minVekPodiel <= 100 && maxVekPodiel >= 0 && maxVekPodiel <= 100)
					vybrane = true;
			}
			vybrane = false;
			*min = minPocet;
			*max = maxPocet;

			while (!vybrane) {
				std::wcout << L"Zadajte dolnu hranicu podielu" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu podielu";
				std::wcin >> maxPocet;
				if (minPocet < maxPocet && minPocet >= 0 && maxPocet >= 0 && minPocet <= 100 && maxPocet <= 100)
					vybrane = true;
			}
			Pohlavie vybranePodiel = Pohlavie::Zena;
			if (cislo == 2) {
				vybranePodiel = Pohlavie::Muz;
			}
			else if (cislo == 3) {
				vybranePodiel = Pohlavie::Obe;
			}

			*vybranePohl = vybranePodiel;
			FilterIntVekPodiel* fPodiel = new FilterIntVekPodiel(vybranePodiel, minPocet, minPocet, minVekPodiel, maxVekPodiel);
			fPodiel->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPodiel;
		}
		else if (filter == 4 && uloha == 'b' && !filterNavyse) {
			filterNavyse = true;
			bool vybrane = false;
			int cislo;
			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
					L"[1.] Predproduktivni " << std::endl << L"[2.] Produktivni " << std::endl << std::endl << L"[3.] Poproduktivni " << std::endl;

				std::wcin >> cislo;
				if (cislo > 0 && cislo <= 3)
				{
					vybrane = true;
				}
			}

			int minPocet = 0;
			int maxPocet = 0;
			vybrane = false;

			while (!vybrane) {
				std::wcout << L"Zadajte dolnu hranicu poctu" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu poctu";
				std::wcin >> maxPocet;
				if (minPocet < maxPocet && minPocet >= 0 && maxPocet >= 0)
					vybrane = true;
			}
			VekovaSkupina vekovaSkupina;
			switch (cislo)
			{
			case 1:
				vekovaSkupina = VekovaSkupina::Predproduktivni;
				break;
			case 2:
				vekovaSkupina = VekovaSkupina::Produktivni;
				break;
			default:
				vekovaSkupina = VekovaSkupina::Poproduktivni;
				break;
			}

			*vybranaVekovaSkupina = vekovaSkupina;
			FilterIntVekovaSkupinaPocet* fPocet = new FilterIntVekovaSkupinaPocet(vekovaSkupina, minPocet, minPocet);
			fPocet->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;
		}
		else if (filter == 0) {
			pokracuj = true;
		}
	}

	return;
}


void Priklady::FiltrovaniePreTriedenie3(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha, TypVzdelania* vybrany) {
	bool pouzityPocet = false;
	bool pouzityPodiel = false;
	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;


	while (!pokracuj) {
		if (!typ)
			std::wcout << L"1 - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"2 - Filtrovat podla prislusnosti" << std::endl;
		if (uloha == 'b' && !pouzityPocet)
			std::wcout << L"3 - Filtrovat podla poctu" << std::endl;
		if (uloha == 'c' && !pouzityPodiel)
			std::wcout << L"4 - Filtrovat podla podielu" << std::endl;
		if((pouzityPocet && uloha == 'b') || (pouzityPodiel && uloha == 'c'))
			std::wcout << L"0 - Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			std::wcout << L"Vyberte si uzemnu jednotku: " << std::endl;
			std::wcout << L"1. Stat: " << std::endl << L"2. Kraj" << std::endl << L"3. Okres" << std::endl << L"4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte jedno z cisiel: " << std::endl;
				std::wcin >> vyber;
				switch (vyber)
				{
				case 1:
					vybranyTyp = TypUzemnejJednotky::Stat;
					break;
				case 2:
					vybranyTyp = TypUzemnejJednotky::Kraj;
					vybrane = true;
					break;
				case 3:
					vybranyTyp = TypUzemnejJednotky::Okres;
					vybrane = true;
					break;
				case 4:
					vybranyTyp = TypUzemnejJednotky::Obec;
					vybrane = true;
					break;
				}
				FilterTyp* fTyp = new FilterTyp(vybranyTyp);
				fTyp->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
				vybrane = true;
				delete fTyp;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl << L"Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
			std::wcin >> nazov;
			FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
			fPrislusnost->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPrislusnost;


		}
		else if (filter == 3 && (uloha == 'b')) {
			pouzityPocet = true;
			bool vybrane = false;
			int cisloTypu;

			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::wcin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					*vybrany = typyVzdelania->at(cisloTypu - 1);
					vybrane = true;
				}
			}
			int min;
			int max;
			std::wcout << L"Zadajte dolnu hranicu poctu" << std::endl;
			std::wcin >> min;
			std::wcout << L"Zadajte hornu hranicu poctu (pokial zadate 0, horna hranica bude maximalna)";
			std::wcin >> max;
			if (max == 0) { ///TO-DO
				max = INT16_MAX;
			}
			FilterIntPocet* fPocet = new FilterIntPocet(*vybrany, min, max);
			fPocet->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;
		}
		else if (filter == 4 && uloha == 'c') {
			pouzityPodiel = true;
			bool vybrane = false;
			int cisloTypu;

			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::wcin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					*vybrany = typyVzdelania->at(cisloTypu - 1);
					vybrane = true;
				}
			}
			double min;
			double max;
			std::wcout << L"Zadajte dolnu hranicu podielu" << std::endl;
			std::wcin >> min;
			std::wcout << L"Zadajte hornu hranicu podielu (pokial zadate 0, horna hranica bude 100%)" << std::endl;;
			std::wcin >> max;
			if (max == 0) {
				max = LLONG_MAX;
			}
			FilterIntPodiel* fPodiel = new FilterIntPodiel(*vybrany, min, max);
			fPodiel->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPodiel;
		}
		else if (filter == 0) {
			pokracuj = true;
		}
	}


	return;

}

void Priklady::FiltrovaniePreVyber(structures::SequenceTable<std::wstring, UzemnaJednotka*>* table)
{
	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;


	while (!pokracuj) {
		if (!typ)
			std::wcout << L"1 - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"2 - Filtrovat podla prislusnosti" << std::endl;
		
		std::wcout << L"0 - Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			std::wcout << L"Vyberte si uzemnu jednotku:" << std::endl;
			std::wcout << L"1. Stat: L" << std::endl << L"2. Kraj" << std::endl << L"3. Okres" << std::endl << L"4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte jedno z cisiel: " << std::endl;
				std::wcin >> vyber;
				if(vyber >= 1 && vyber <= 4)
					vybrane = true;


				switch (vyber)
				{
				case 1:
					vybranyTyp = TypUzemnejJednotky::Stat;
					break;
				case 2:
					vybranyTyp = TypUzemnejJednotky::Kraj;
					vybrane = true;
					break;
				case 3:
					vybranyTyp = TypUzemnejJednotky::Okres;
					vybrane = true;
					break;
				case 4:
					vybranyTyp = TypUzemnejJednotky::Obec;
					vybrane = true;
					break;
				}
				FilterTyp* fTyp = new FilterTyp(vybranyTyp);
				fTyp->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
				delete fTyp;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl << L"Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
			std::wcin >> nazov;
			FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
			fPrislusnost->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPrislusnost;
		}
		else if (filter == 0)
			pokracuj = true;
	}

	return;
}

std::wstring Priklady::VypisEnumTyp(TypUzemnejJednotky typ)
{
	std::wstring result;
	switch (typ) {
	case TypUzemnejJednotky::Kraj:
		result = L"Kraj";
		break;
	case TypUzemnejJednotky::Obec:
		result = L"Obec";
		break;
	case TypUzemnejJednotky::Stat:
		result = L"Stat";
		break;
	case TypUzemnejJednotky::Okres:
		result = L"Okres";
		break;
	}

	return result;
}

std::wstring Priklady::VypisEnumPohlavie(Pohlavie typ)
{
	std::wstring result;
	switch (typ) {
	case Pohlavie::Zena:
		result = L"Zena";
		break;
	case Pohlavie::Muz:
		result = L"Muz";
		break;
	case Pohlavie::Obe:
		result = L"Zeny aj muzi";
		break;
	}

	return result;
}

std::wstring Priklady::VypisEnumVekovaSkupina(VekovaSkupina typ)
{
	std::wstring result;
	switch (typ) {
	case VekovaSkupina::Predproduktivni:
		result = L"Predproduktivni";
		break;
	case VekovaSkupina::Produktivni:
		result = L"Produktivni";
		break;
	case VekovaSkupina::Poproduktivni:
		result = L"Poproduktivni";
		break;
	}
	return result;
}

std::wstring Priklady::VypisEnumVzdelanie(TypVzdelania typ)
{
	std::wstring result;
	switch (typ)
	{
	case TypVzdelania::BezDo15:
		result = L"bez ukonceneho vzdelania - osoby vo veku 0-14 rokov";
		break;
	case TypVzdelania::BezNad15:
		result = L"bez skolskeho vzdelania - osoby vo veku 15 rokov a viac";
		break;
	case TypVzdelania::Zakladne:
		result = L"zakladne vzdelanie";
		break;
	case TypVzdelania::StredneOdborne:
		result = L"stredne odborne (ucnovske) vzdelanie (bez maturity)";
		break;
	case TypVzdelania::UplneStredne:
		result = L"uplne stredne vzdelanie (s maturitou)";
		break;
	case TypVzdelania::VyssieOdborne:
		result = L"vyssie odborne vzdelanie";
		break;
	case TypVzdelania::Vysokoskolske:
		result = L"vysokoskolske vzdelanie";
		break;
	case TypVzdelania::Nezistene:
		result = L"nezistene";
		break;
	}
	return result;
}

