#include "Priklady.h"


Priklady::Priklady()
{
	Input* input = new Input();
	this->stat = new Stat(TypUzemnejJednotky::Stat, L"Slovensko", NULL);
	this->table_kraj = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(input->InputKraje("csv/kraje.csv", stat));
	this->table_okres = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(input->InputOkresy("csv/okresy.csv", table_kraj));
	this->table_obec = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(input->InputObce("csv/vzdelanie.csv", table_okres));

	input->InputVek("csv/vek.csv", table_obec);

	this->tab_all_sorted = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>();
	this->tab_all_unsorted = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>();

	this->spojenieTabuliek(this->tab_all_sorted);
	this->spojenieTabuliek(this->tab_all_unsorted);



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
	delete table_kraj;
	delete table_obec;
	delete table_okres;
	delete typyVzdelania;
	delete tab_all_sorted;
	delete tab_all_unsorted;
}


void Priklady::Priklad1Vypis()
{
	bool pokracuj = false;
	UzemnaJednotka* data = nullptr;
	std::wstring nazov;
	while (!pokracuj) {
		std::wcout << std::endl << "Zadajte nazov uzemnej jednotky" << std::endl;
		std::wcin >> nazov;

		bool najdene = tab_all_sorted->tryFind(nazov, data);
		if (najdene)
			pokracuj = true;
		else {
			std::wcout << std::endl << "Uzemna jednotka s takymto nazvom nebola najdena. Chcete pokracovat v hladani? " << std::endl;
			std::wcout << std::endl << "[1] Ano hladat znovu" << std::endl << "[0] Nie prejst k ostatnym uloham." << std::endl;
			int filter;
			std::cin >> filter;
			if (filter == 0)
				pokracuj = true;
		}
	}

	const UzemnaJednotka* current = data;

	pokracuj = false;
	while (!pokracuj) {

		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;

		std::wcout << " Hodnoty vzdelania: " << std::endl;


		for (int i = 0; i < this->typyVzdelania->size(); ++i)
		{
			std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
				std::to_wstring(current->getVzdelaniePocet(this->typyVzdelania->at(i))) << std::endl;
		}


		std::wcout << " Hodnoty vekovovych skupin: " << std::endl;

		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
			std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

		std::wcout << L"*****" << std::endl;

		if (current->getTyp() != TypUzemnejJednotky::Stat)
			current = current->getVyssiCelok();
		else
			pokracuj = true;
	}


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
		std::wcout << "0 - Pokracovat" << std::endl;

		if (!typ)
			std::wcout << "1  - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << "2 - Filtrovat podla prislusnosti" << std::endl;
		if (!vzdelaniePocet)
			std::wcout << "3 - Filtrovat podla poctu vzdelania" << std::endl;
		if (!vzdelaniePodiel)
			std::wcout << "4 - Filtrovat podla podielu vzdelania" << std::endl;
		if (!vekPocet)
			std::wcout << "5 - Filtrovat podla poctu veku" << std::endl;
		if (!vekPodiel)
			std::wcout << "6 - Filtrovat podla podielu veku" << std::endl;
		if (!vekovaSkupinaPocet)
			std::wcout << "7 - Filtrovat podla poctu vekovej skupiny" << std::endl;
		if (!vekovaSkupinaPodiel)
			std::wcout << "8 - Filtrovat podla podielu vekovej skupiny" << std::endl;

		std::cin >> filter;
		if (filter == 0) {
			pokracuj = true;
		}
		else if (filter == 1 && !typ) {
			typ = true;
			std::wcout << "Vyberte si uzemnu jednotku: " << std::endl;
			std::wcout << "1. Stat: " << std::endl << "2. Kraj" << std::endl << "3. Okres" << std::endl << "4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << "Zadajte jedno z cisiel: " << std::endl;
				std::cin >> vyber;
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
				vybrane = true;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl << "Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
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
				std::wcout << std::endl << "Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::cin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					vybranyTyp = typyVzdelania->at(cisloTypu - 1);

					vybrane = true;
				}
			}
			int min;
			int max;
			std::wcout << "Zadajte dolnu hranicu poctu" << std::endl;
			std::cin >> min;
			std::wcout << "Zadajte hornu hranicu poctu (pokial zadate 0, horna hranica bude maximalna)";
			std::cin >> max;
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
				std::wcout << std::endl << "Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::cin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					vybranyTyp = typyVzdelania->at(cisloTypu - 1);

					vybrane = true;
				}
			}
			double min;
			double max;
			std::wcout << "Zadajte dolnu hranicu podielu" << std::endl;
			std::cin >> min;
			std::wcout << "Zadajte hornu hranicu podielu (pokial zadate 0, horna hranica bude 100%)";
			std::cin >> max;
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
				std::wcout << std::endl << "Vyberte pohlavie ktore chcete filtrovat: " << std::endl <<
				"[1.] Zeny " << std::endl << "[2.] Muzi "  << std::endl << std::endl << "[3.] Zeny aj muzi " << std::endl;
				
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
				std::wcout << "Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
				std::cin >> minVekPocet;
				std::wcout << "Zadajte hornu vekovu hranicu [0 - 100]";
				std::cin >> maxVekPocet;
				if (minVekPocet <= maxVekPocet && minVekPocet >= 0 && 
					minVekPocet <= 100 && maxVekPocet >= 0 && maxVekPocet <= 100)
					vybrane = true;
			}
			vybrane = false;
			while (!vybrane) {
				std::wcout << "Zadajte dolnu hranicu poctu" << std::endl;
				std::cin >> minPocet;
				std::wcout << "Zadajte hornu hranicu poctu";
				std::cin >> maxPocet;
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
				std::wcout << std::endl << "Vyberte pohlavie ktore chcete filtrovat: " << std::endl <<
					"[1.] Zeny " << std::endl << "[2.] Muzi " << std::endl << std::endl << "[3.] Zeny aj muzi " << std::endl;

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
				std::wcout << "Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
				std::cin >> minVekPodiel;
				std::wcout << "Zadajte hornu vekovu hranicu [0 - 100]";
				std::cin >> maxVekPodiel;
				if (minVekPodiel < maxVekPodiel && minVekPodiel >= 0 && minVekPodiel <= 100 && maxVekPodiel >= 0 && maxVekPodiel <= 100)
					vybrane = true;
			}
			vybrane = false;

			while (!vybrane) {
				std::wcout << "Zadajte dolnu hranicu podielu" << std::endl;
				std::cin >> minPocet;
				std::wcout << "Zadajte hornu hranicu podielu";
				std::cin >> maxPocet;
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
				std::wcout << std::endl << "Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
					"[1.] Predproduktivni " << std::endl << "[2.] Produktivni " << std::endl << std::endl << "[3.] Poproduktivni " << std::endl;

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
				std::wcout << "Zadajte dolnu hranicu poctu" << std::endl;
				std::cin >> minPocet;
				std::wcout << "Zadajte hornu hranicu poctu";
				std::cin >> maxPocet;
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
				std::wcout << std::endl << "Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
					"[1.] Predproduktivni " << std::endl << "[2.] Produktivni " << std::endl << std::endl << "[3.] Poproduktivni " << std::endl;

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
				std::wcout << "Zadajte dolnu hranicu podielu" << std::endl;
				std::cin >> minPocet;
				std::wcout << "Zadajte hornu hranicu podielu";
				std::cin >> maxPocet;
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
				std::wcout << " Hodnoty vzdelania: " << std::endl;

				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
						std::to_wstring(current->getVzdelaniePocet(this->typyVzdelania->at(i))) << std::endl;
				}
			}


			if (vzdelaniePodiel) {
				std::wcout << " Hodnoty vzdelania: " << std::endl;

				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
						std::to_wstring(current->getVzdelaniePodiel(this->typyVzdelania->at(i))) << std::endl;
				}
			}

			if (vekovaSkupinaPocet) {
				std::wcout << " Hodnoty vekovovych skupin: " << std::endl;

				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
				std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
					std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

			}

			if (vekovaSkupinaPodiel) {
				std::wcout << " Hodnoty vekovovych skupin: " << std::endl;

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
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_unsorted);
	Pohlavie pohlavie = Pohlavie::Obe;
	int min = 0;
	int max = 0;

	FiltrovaniePreTriedenie(tabulkaNaPracovanie, 'a', &pohlavie, &min, &max, nullptr);
	bool pokracovat = false;
	std::wcout << "1. Vzostupne " << std::endl << "2. Zostupne" << std::endl;

	SortCriterion<double>* sort = new SortCriterion<double>();
	CriterionIntVekPodiel* cVek = new CriterionIntVekPodiel(pohlavie,min,max);

	int filter;
	while (!pokracovat) {
		std::cin >> filter;
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
		std::wcout << "Poèet mužov" + uzemnaJednotka->accessData()->getIntervalVekPocet(Pohlavie::Muz, min, max) << std::endl;
		std::wcout << "Poèet žien" + uzemnaJednotka->accessData()->getIntervalVekPocet(Pohlavie::Zena, min, max) << std::endl;
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
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_unsorted);
	VekovaSkupina vekSkupina = VekovaSkupina::Produktivni;
	int min = 0;
	int max = 0;

	FiltrovaniePreTriedenie(tabulkaNaPracovanie, 'a', nullptr, nullptr, nullptr, &vekSkupina);
	bool pokracovat = false;
	std::wcout << "1. Vzostupne " << std::endl << "2. Zostupne" << std::endl;

	SortCriterion<int>* sort = new SortCriterion<int>();
	CriterionVekovaSkupinaPocet* cVek = new CriterionVekovaSkupinaPocet(vekSkupina);

	int filter;
	while (!pokracovat) {
		std::cin >> filter;
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
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_unsorted);
	Filtrovanie(tabulkaNaPracovanie, 'a', nullptr);
	bool pokracovat = false;
	std::wcout << "1. Vzostupne " << std::endl << "2. Zostupne" << std::endl;

	SortCriterion<std::wstring>* sort = new SortCriterion<std::wstring>();
	CriterionNazov* cNazov = new CriterionNazov();

	int filter;
	while (!pokracovat) {
		std::cin >> filter;
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
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_unsorted);
	TypVzdelania vzdelanie = TypVzdelania::Null;
	Filtrovanie(tabulkaNaPracovanie, 'b', &vzdelanie);
	bool pokracovat = false;
	std::wcout << "1. Vzostupne " << std::endl << "2. Zostupne" << std::endl;

	SortCriterion<int>* sort = new SortCriterion<int>();
	CriterionVzdelaniePocet* cPocet = new CriterionVzdelaniePocet(vzdelanie);

	int filter;
	while (!pokracovat) {
		std::cin >> filter;
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
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_unsorted);
	TypVzdelania vzdelanie = TypVzdelania::Null;
	Filtrovanie(tabulkaNaPracovanie, 'c', &vzdelanie);
	bool pokracovat = false;
	std::wcout << "1. Vzostupne " << std::endl << "2. Zostupne" << std::endl;

	SortCriterion<double>* sort = new SortCriterion<double>();
	CriterionVzdelaniePodiel* cPodiel = new CriterionVzdelaniePodiel(vzdelanie);

	int filter;
	while (!pokracovat) {
		std::cin >> filter;
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
		std::wcout << "Zadajte ktoru vekovu skupinu chcete pouzit: " << std::endl;
		std::wcout << "[1.] Predproduktivni" << std::endl;
		std::wcout << "[2.] Produktivni" << std::endl;
		std::wcout << "[3.] Poproduktivni" << std::endl;

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
	UzemnaJednotka* najlepsiaUzemnaJednotka = select->selectBest(tabulkaNaPracovanie);	const UzemnaJednotka* current = najlepsiaUzemnaJednotka;

	pokracuj = false;
	while (!pokracuj) {

		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;

		std::wcout << " Hodnoty vekovovych skupin: " << std::endl;

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
		std::wcout << "Zadajte ktoru vekovu skupinu chcete pouzit: " << std::endl;
		std::wcout << "[1.] Predproduktivni" << std::endl;
		std::wcout << "[2.] Produktivni" << std::endl;
		std::wcout << "[3.] Poproduktivni" << std::endl;

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
	UzemnaJednotka* najlepsiaUzemnaJednotka = (select->selectBest(tabulkaNaPracovanie));	const UzemnaJednotka* current = najlepsiaUzemnaJednotka;

	pokracuj = false;
	while (!pokracuj) {

		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;

		std::wcout << " Hodnoty vekovovych skupin: " << std::endl;

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



void Priklady::Vypis(structures::SequenceTable<std::wstring, UzemnaJednotka*>& tabulka, TypVzdelania typNaVypis, bool vypisPocet, bool vypisPodiel)
{
	for (auto item : tabulka) {
		std::wstring typ = VypisEnumTyp(item->accessData()->getTyp());
		std::wcout << std::endl << L"Typ: " + typ << std::endl << L"Nazov: ";
		std::wcout << item->accessData()->getNazov() << std::endl;
		if (vypisPocet) {
			std::wcout << " Hodnoty vzdelania: " << std::endl;

			if (typNaVypis == TypVzdelania::Null) {
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
						std::to_wstring(item->accessData()->getVzdelaniePocet(this->typyVzdelania->at(i))) << std::endl;
				}
			}
			else {
				std::wcout << L" * " + VypisEnumVzdelanie(typNaVypis) + L" : " +
					std::to_wstring(item->accessData()->getVzdelaniePocet(typNaVypis)) << std::endl;
			}
		}
		if (vypisPodiel) {
			for (int i = 0; i < this->typyVzdelania->size(); ++i)
			{
				std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
					std::to_wstring(item->accessData()->getVzdelaniePodiel(this->typyVzdelania->at(i))) << std::endl;
			}
		}

		std::wcout << L" Hodnoty vekovovych skupin: " << std::endl;

		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
			std::to_wstring(item->accessData()->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
			std::to_wstring(item->accessData()->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
		std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
			std::to_wstring(item->accessData()->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

		const UzemnaJednotka* current = item->accessData();

		while (current->getVyssiCelok() != nullptr) {
			current = current->getVyssiCelok();

			std::wcout << std::endl << L"Typ: " + VypisEnumTyp(current->getTyp()) << std::endl << "Nazov: ";
			std::wcout << current->getNazov() << std::endl;
			std::wcout << " Hodnoty vzdelania: " << std::endl;

			if (vypisPocet) {
				if (typNaVypis == TypVzdelania::Null) {
					for (int i = 0; i < this->typyVzdelania->size(); ++i)
					{
						std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
							std::to_wstring(current->getVzdelaniePocet(this->typyVzdelania->at(i))) << std::endl;
					}
				}
				else {
					std::wcout << L" * " + VypisEnumVzdelanie(typNaVypis) + L" : " +
						std::to_wstring(current->getVzdelaniePocet(typNaVypis)) << std::endl;
				}
			}
			if (vypisPodiel) {
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
						std::to_wstring(current->getVzdelaniePodiel(this->typyVzdelania->at(i))) << std::endl;
				}
			}
			std::wcout << " Hodnoty vekovovych skupin: " << std::endl;

			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
				std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Predproduktivni)) << std::endl;
			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
				std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Produktivni)) << std::endl;
			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
				std::to_wstring(current->getVekovaSkupinaPocet(VekovaSkupina::Poproduktivni)) << std::endl;

		}
		std::wcout << "*****" << std::endl;
	}

	return;
}
/*
void Priklady::VyberUzemnejJednotky(structures::SequenceTable<std::string, UzemnaJednotka*>* table )
{
	std::wcout << "Vyberte si uzemnu jednotku: " << std::endl;
	std::wcout << "1. Stat: " << std::endl << "2. Kraj" << std::endl << "3. Okres" << std::endl << "4. Obec" << std::endl;
	int vyber;
	TypUzemnejJednotky typ;
	bool vybrane = false;

	while (!vybrane) {
		std::wcout << "Zadajte jedno z cisiel: " << std::endl;
		std::cin >> vyber;
		switch (vyber)
		{
		case 1:
			typ = TypUzemnejJednotky::Stat;
			table = nullptr;
			break;
		case 2:
			typ = TypUzemnejJednotky::Kraj;
			table->assign(*this->table_kraj);
			vybrane = true;
			break;
		case 3:
			typ = TypUzemnejJednotky::Okres;
			table->assign(*this->table_okres);

			table = this->table_okres;
			vybrane = true;
			break;
		case 4:
			typ = TypUzemnejJednotky::Obec;
			table->assign(*this->table_obec);

			table = this->table_obec;
			vybrane = true;
			break;
		}
	}
	return;
}
*/

void Priklady::spojenieTabuliek(structures::SequenceTable<std::wstring, UzemnaJednotka*>* table)
{
	table->insert(stat->getNazov(), stat);
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *table_kraj) {
		table->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *table_okres) {
		table->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* uzemnaJednotka : *table_obec) {
		table->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
}


void Priklady::FiltrovaniePreTriedenie(structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha, Pohlavie* vybranePohl, int* min, int* max, VekovaSkupina* vybranaVekovaSkupina) {

	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;
	bool filterNavyse = false;

	while (!pokracuj) {
		if (!typ)
			std::wcout << "1 - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << "2 - Filtrovat podla prislusnosti" << std::endl;
		if (uloha == 'a' && !filterNavyse)
			std::wcout << "3 - Filtrovat podla podielu veku" << std::endl;
		if (uloha == 'b' && !filterNavyse)
			std::wcout << "4 - Filtrovat podla poctu vekovej skupiny" << std::endl;
		std::wcout << "0 - Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			std::wcout << "Vyberte si uzemnu jednotku: " << std::endl;
			std::wcout << "1. Stat: " << std::endl << "2. Kraj" << std::endl << "3. Okres" << std::endl << "4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << "Zadajte jedno z cisiel: " << std::endl;
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
				fTyp->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
				vybrane = true;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl << "Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
			std::wcin >> nazov;
			FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
			fPrislusnost->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPrislusnost;


		}
		else if (filter == 3 && (uloha == 'a') && !filterNavyse) {
			filterNavyse = false;
			bool vybrane = false;
			int cislo;

			while (!vybrane) {
				std::wcout << std::endl << "Vyberte pohlavie ktore chcete filtrovat: " << std::endl <<
					"[1.] Zeny " << std::endl << "[2.] Muzi " << std::endl << "[3.] Zeny aj muzi " << std::endl;

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
				std::wcout << "Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
				std::cin >> minVekPodiel;
				std::wcout << "Zadajte hornu vekovu hranicu [0 - 100]" << std::endl;
				std::cin >> maxVekPodiel;
				if (minVekPodiel < maxVekPodiel && minVekPodiel >= 0 && minVekPodiel <= 100 && maxVekPodiel >= 0 && maxVekPodiel <= 100)
					vybrane = true;
			}
			vybrane = false;
			*min = minPocet;
			*max = maxPocet;

			while (!vybrane) {
				std::wcout << "Zadajte dolnu hranicu podielu" << std::endl;
				std::cin >> minPocet;
				std::wcout << "Zadajte hornu hranicu podielu";
				std::cin >> maxPocet;
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
			fPodiel->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPodiel;
		}
		else if (filter == 4 && uloha == 'b' && !filterNavyse) {
			filterNavyse = true;
			bool vybrane = false;
			int cislo;
			while (!vybrane) {
				std::wcout << std::endl << "Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
					"[1.] Predproduktivni " << std::endl << "[2.] Produktivni " << std::endl << std::endl << "[3.] Poproduktivni " << std::endl;

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
				std::wcout << "Zadajte dolnu hranicu poctu" << std::endl;
				std::cin >> minPocet;
				std::wcout << "Zadajte hornu hranicu poctu";
				std::cin >> maxPocet;
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
			fPocet->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;
		}
		else if (filter == 0) {
			pokracuj = true;
		}
	}

	return;
}


void Priklady::Filtrovanie(structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha, TypVzdelania* vybrany) {
	bool nepouzityPocet = true;
	bool nepouzityPodiel = true;
	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;


	while (!pokracuj) {
		if (!typ)
			std::wcout << "1 - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << "2 - Filtrovat podla prislusnosti" << std::endl;
		if (uloha == 'b')
			std::wcout << "3 - Filtrovat podla poctu" << std::endl;
		if (uloha == 'c')
			std::wcout << "4 - Filtrovat podla podielu" << std::endl;
		std::wcout << "0 - Pokracovat" << std::endl;

		std::cin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			std::wcout << "Vyberte si uzemnu jednotku: " << std::endl;
			std::wcout << "1. Stat: " << std::endl << "2. Kraj" << std::endl << "3. Okres" << std::endl << "4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << "Zadajte jedno z cisiel: " << std::endl;
				std::cin >> vyber;
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
				fTyp->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
				vybrane = true;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl << "Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
			std::wcin >> nazov;
			FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
			fPrislusnost->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPrislusnost;


		}
		else if (filter == 3 && (uloha == 'b')) {
			nepouzityPocet = false;
			bool vybrane = false;
			int cisloTypu;

			while (!vybrane) {
				std::wcout << std::endl << "Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::cin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					*vybrany = typyVzdelania->at(cisloTypu - 1);
					vybrane = true;
				}
			}
			int min;
			int max;
			std::wcout << "Zadajte dolnu hranicu poctu" << std::endl;
			std::cin >> min;
			std::wcout << "Zadajte hornu hranicu poctu (pokial zadate 0, horna hranica bude maximalna)";
			std::cin >> max;
			if (max == 0) { ///TO-DO
				max = INT16_MAX;
			}
			FilterIntPocet* fPocet = new FilterIntPocet(*vybrany, min, max);
			fPocet->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;
		}
		else if (filter == 4 && uloha == 'c') {
			nepouzityPodiel = false;
			bool vybrane = false;
			int cisloTypu;

			while (!vybrane) {
				std::wcout << std::endl << "Vyberte typ vzdelania ktory chcete filtrovat" << std::endl;
				for (int i = 0; i < this->typyVzdelania->size(); ++i)
				{
					std::wcout << i + 1 + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
				}
				std::cin >> cisloTypu;
				if (cisloTypu > 0 && cisloTypu <= 8)
				{
					*vybrany = typyVzdelania->at(cisloTypu - 1);
					vybrane = true;
				}
			}
			double min;
			double max;
			std::wcout << "Zadajte dolnu hranicu podielu" << std::endl;
			std::cin >> min;
			std::wcout << "Zadajte hornu hranicu podielu (pokial zadate 0, horna hranica bude 100%)";
			std::cin >> max;
			if (max == 0) {
				max = LLONG_MAX;
			}
			FilterIntPodiel* fPodiel = new FilterIntPodiel(*vybrany, min, max);
			fPodiel->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
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
			std::wcout << "1 - Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << "2 - Filtrovat podla prislusnosti" << std::endl;
		
		std::wcout << "0 - Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			std::wcout << "Vyberte si uzemnu jednotku: " << std::endl;
			std::wcout << "1. Stat: " << std::endl << "2. Kraj" << std::endl << "3. Okres" << std::endl << "4. Obec" << std::endl;
			int vyber;
			TypUzemnejJednotky vybranyTyp;
			bool vybrane = false;
			while (!vybrane) {
				std::wcout << "Zadajte jedno z cisiel: " << std::endl;
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
				fTyp->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
				vybrane = true;
			}
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			std::wstring nazov = L"";
			std::wcout << std::endl << "Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
			std::wcin >> nazov;
			FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
			fPrislusnost->filterTable(table, new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPrislusnost;
		}
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

/*std::wcout << "Vyberte si uzemnu jednotku: " << std::endl;
	std::wcout << "1. Kraj" << std::endl << "2. Okres" << std::endl << "3. Obec" << std::endl;
	int vyber;
	bool vybrane = false;

	while (!vybrane) {
		std::wcout << "Zadajte jedno z cisiel: " << std::endl;
		std::cin >> vyber;
		switch (vyber)
		{
		case 1:
			vybrane = true;
			for (structures::TableItem<std::string, UzemnaJednotka*>* uzemnaJednotka : *table_kraj) {
				tabulkaNaPracovanie->insert(uzemnaJednotka->getKey(), uzemnaJednotka->accessData());
			}
			break;
		case 2:
			vybrane = true;
			for (structures::TableItem<std::string, UzemnaJednotka*>* uzemnaJednotka : *table_okres) {
				tabulkaNaPracovanie->insert(uzemnaJednotka->getKey(), uzemnaJednotka->accessData());
			}
			break;
		case 3:
			vybrane = true; for (structures::TableItem<std::string, UzemnaJednotka*>* uzemnaJednotka : *table_obec) {
				tabulkaNaPracovanie->insert(uzemnaJednotka->getKey(), uzemnaJednotka->accessData());
			}
			break;
		}
	}

	int filter = 0;
	std::wcout << "1. Filtrovat podla prislusnosti " << std::endl << "Ine tlacidla - Pokracovat" << std::endl;
	std::cin >> filter;

	if (filter == 1) {
		std::string nazov = "";
		std::wcout << std::endl << "Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
		std::cin >> nazov;
		FilterPrislusnost* fPrislusnost = new FilterPrislusnost(nazov, true);
		fPrislusnost->filterTable(tabulkaNaPracovanie, new structures::UnsortedSequenceTable<std::string,UzemnaJednotka*>);
		delete fPrislusnost;
	}*/