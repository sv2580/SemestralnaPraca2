#include "Priklady.h"
#include <io.h>


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

	tab_all_sorted->insert(stat->getNazov(), stat);
	for (structures::TableItem<std::wstring, Kraj*>* uzemnaJednotka : *table_kraj) {
		tab_all_sorted->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
	for (structures::TableItem<std::wstring, Okres*>* uzemnaJednotka : *table_okres) {
		tab_all_sorted->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}
	for (structures::TableItem<std::wstring, Obec*>* uzemnaJednotka : *table_obec) {
		tab_all_sorted->insert(uzemnaJednotka->accessData()->getNazov(), uzemnaJednotka->accessData());
	}

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
		wchar_t input[100];
		std::wcout << std::endl << L"Zadajte nazov uzemnej jednotky" << std::endl;
		std::wcin.ignore();
		std::wcin.getline(input, 100);
		std::wstring nazov = std::wstring(input);

		bool najdene = tab_all_sorted->tryFind(nazov, data);
		if (najdene)
			pokracuj = true;
		else {
			std::wcout << std::endl << L"Uzemna jednotka s takymto nazvom nebola najdena. Chcete pokracovat v hladani? L" << std::endl;
			std::wcout << std::endl << L"[1] Ano hladat znovu" << std::endl << L"[0] Nie prejst k ostatnym uloham." << std::endl;
			int filter;
			std::wcin >> filter;
			if (filter == 0)
				return;
		}
	}


	CriterionNazov cNazov;
	CriterionTyp cTyp;
	CriterionNadradena cNadradena;
	CriterionVekovaSkupinaPocet cSkupinaPocetPred(VekovaSkupina::Predproduktivni);
	CriterionVekovaSkupinaPocet cSkupinaPocetPro(VekovaSkupina::Produktivni);
	CriterionVekovaSkupinaPocet cSkupinaPocetPo(VekovaSkupina::Poproduktivni);

	const UzemnaJednotka* current = data;
	pokracuj = false;
	std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl
		<< L"Nazov: " << cNazov.evaluate(*current) << std::endl;

	std::wcout << L" Hodnoty vzdelania: " << std::endl;


	for (int i = 0; i < this->typyVzdelania->size(); ++i)
	{
		CriterionVzdelaniePocet cVzdelaniePocet(this->typyVzdelania->at(i));

		std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
			std::to_wstring(cVzdelaniePocet.evaluate(*current)) << std::endl;
	}


	std::wcout << L" Hodnoty vekovovych skupin: " << std::endl;

	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
		std::to_wstring(cSkupinaPocetPred.evaluate(*current)) << std::endl;
	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
		std::to_wstring(cSkupinaPocetPro.evaluate(*current)) << std::endl;
	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
		std::to_wstring(cSkupinaPocetPo.evaluate(*current)) << std::endl;

	while (!pokracuj) {


		if (current->getTyp() != TypUzemnejJednotky::Stat)
			current = cNadradena.evaluate(*current);
		else{
			pokracuj = true;
			break;
		}
		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl
			<< L"Nazov: " << cNazov.evaluate(*current) << std::endl;
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
		std::wcout << std::endl << L"[0] Pokracovat" << std::endl;
		if (!typ)
			std::wcout << L"[1] Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"[2] Filtrovat podla prislusnosti" << std::endl;
		if (!vzdelaniePocet)
			std::wcout << L"[3] Filtrovat podla poctu vzdelania" << std::endl;
		if (!vzdelaniePodiel)
			std::wcout << L"[4] Filtrovat podla podielu vzdelania" << std::endl;
		if (!vekPocet)
			std::wcout << L"[5] Filtrovat podla poctu veku a pohlavia" << std::endl;
		if (!vekPodiel)
			std::wcout << L"[6] Filtrovat podla podielu veku a pohlavia" << std::endl;
		if (!vekovaSkupinaPocet)
			std::wcout << L"[7] Filtrovat podla poctu vekovej skupiny" << std::endl;
		if (!vekovaSkupinaPodiel)
			std::wcout << L"[8] Filtrovat podla podielu vekovej skupiny" << std::endl;

		std::wcin >> filter;
		if (filter == 0) {
			pokracuj = true;
		}
		else if (filter == 1 && !typ) {
			typ = true;
			TypFiltrovanie(tabulkaNaPracovanie);
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			PrislusnostFiltrovanie(tabulkaNaPracovanie);
		}
		else if (filter == 3 && !vzdelaniePocet) {
			vzdelaniePocet = true;
			VzdelaniePocetFiltrovanie(tabulkaNaPracovanie);
		}
		else if (filter == 4 && !vzdelaniePodiel) {
			vzdelaniePodiel = true;
			VzdelaniePodielFiltrovanie(tabulkaNaPracovanie);
		}
		else if (filter == 5 && !vekPocet) {
			vekPocet = true;
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

			int minPocet = 0;
			int maxPocet = 0;
			vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
				std::wcin >> minVekPocet;
				std::wcout << L"Zadajte hornu vekovu hranicu [0 - 100]" << std::endl;
				std::wcin >> maxVekPocet;
				if (minVekPocet <= maxVekPocet && minVekPocet >= 0 &&
					minVekPocet <= 100 && maxVekPocet >= 0 && maxVekPocet <= 100)
					vybrane = true;
			}
			vybrane = false;
			while (!vybrane) {
				std::wcout << L"Zadajte dolnu hranicu poctu" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu poctu (pokial zadate 0, horna hranica bude maximalna)" << std::endl;
				std::wcin >> maxPocet;
				if (maxPocet == 0) {
					maxPocet = LLONG_MAX;
				}
				if (minPocet <= maxPocet && minPocet >= 0 && maxPocet >= 0)
					vybrane = true;
			}
			vybranePocet = Pohlavie::Zena;
			if (cislo == 2) {
				vybranePocet = Pohlavie::Muz;
			}
			else if (cislo == 3) {
				vybranePocet = Pohlavie::Obe;
			}
			FilterIntVekPocet* fPocet = new FilterIntVekPocet(vybranePocet, minPocet, maxPocet, minVekPocet, maxVekPocet);
			fPocet->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;

		}
		else if (filter == 6 && !vekPodiel) {
			vekPodiel = true;
			int minPocet = 0;
			int maxPocet = 0;
			Pohlavie vybranePocet = Pohlavie::Obe;
			VekPodielFiltrovanie(tabulkaNaPracovanie, &vybranePocet, &minPocet, &maxPocet);
		}
		else if (filter == 7 && !vekovaSkupinaPocet) {
			vekovaSkupinaPocet = true;
			VekovaSkupinaPocetFiltrovanie(tabulkaNaPracovanie);
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
				std::wcout << L"Zadajte dolnu hranicu podielu [0-100]" << std::endl;
				std::wcin >> minPocet;
				std::wcout << L"Zadajte hornu hranicu podielu [0-100]" << std::endl;
				std::wcin >> maxPocet;
				if (maxPocet == 0) {
					maxPocet == 100;
				}
				if (minPocet <= maxPocet && minPocet >= 0 && maxPocet >= 0 && minPocet <= 100 && maxPocet <= 100)
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
			FilterIntVekovaSkupinaPodiel* fPodiel = new FilterIntVekovaSkupinaPodiel(vekovaSkupina, minPocet, maxPocet);
			fPodiel->filterTable(tabulkaNaPracovanie, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPodiel;

		}
	}

	if (tabulkaNaPracovanie->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete tabulkaNaPracovanie;
		return;
	}


	CriterionNazov cNazov;
	CriterionTyp cTyp;
	CriterionVekovaSkupinaPocet cSkupinaPocetPred(VekovaSkupina::Predproduktivni);
	CriterionVekovaSkupinaPocet cSkupinaPocetPro(VekovaSkupina::Produktivni);
	CriterionVekovaSkupinaPocet cSkupinaPocetPo(VekovaSkupina::Poproduktivni);
	CriterionVekovaSkupinaPodiel cSkupinaPodielPred(VekovaSkupina::Predproduktivni);
	CriterionVekovaSkupinaPodiel cSkupinaPodielPro(VekovaSkupina::Produktivni);
	CriterionVekovaSkupinaPodiel cSkupinaPodielPo(VekovaSkupina::Poproduktivni);
	CriterionIntVekPocet cVekPocet(vybranePocet, minVekPocet, maxVekPocet);
	CriterionIntVekPodiel cVekPodiel(vybranePodiel, minVekPodiel, maxVekPodiel);
	CriterionNadradena cNadradena;


	for (structures::TableItem<std::wstring, UzemnaJednotka*>* item : *tabulkaNaPracovanie)
	{
		const UzemnaJednotka* current = item->accessData();

		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl
			<< L"Nazov: " << cNazov.evaluate(*current) << std::endl;

		if (vzdelaniePocet) {
			std::wcout << L"Hodnoty vzdelania: " << std::endl;

			for (int i = 0; i < this->typyVzdelania->size(); ++i)
			{
				CriterionVzdelaniePocet cVzdelaniePocet(this->typyVzdelania->at(i));

				std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
					std::to_wstring(cVzdelaniePocet.evaluate(*current)) << std::endl;
			}
		}


		if (vzdelaniePodiel) {
			std::wcout << L"Hodnoty vzdelania: " << std::endl;
			for (int i = 0; i < this->typyVzdelania->size(); ++i)
			{
				CriterionVzdelaniePodiel cVzdelaniePodiel(this->typyVzdelania->at(i));

				std::wcout << L" * " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) + L" : " +
					std::to_wstring(cVzdelaniePodiel.evaluate(*current)) << std::endl;
			}
		}

		if (vekovaSkupinaPocet) {
			std::wcout << L"Hodnoty vekovovych skupin: " << std::endl;

			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
				std::to_wstring(cSkupinaPodielPred.evaluate(*current)) << std::endl;
			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
				std::to_wstring(cSkupinaPodielPro.evaluate(*current)) << std::endl;
			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
				std::to_wstring(cSkupinaPodielPo.evaluate(*current)) << std::endl;
		}

		if (vekovaSkupinaPodiel) {
			std::wcout << L"Hodnoty vekovych skupin: " << std::endl;

			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
				std::to_wstring(cSkupinaPocetPred.evaluate(*current)) << std::endl;
			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
				std::to_wstring(cSkupinaPocetPro.evaluate(*current)) << std::endl;
			std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
				std::to_wstring(cSkupinaPocetPo.evaluate(*current)) << std::endl;
		}

		if (vekPocet) {
			std::wcout << L" Pocet kriteria veku medzi rokmi " + std::to_wstring(minVekPocet)
				+ L" - " + std::to_wstring(maxVekPocet) + L" pre pohlavie: " + VypisEnumPohlavie(vybranePocet)
				+ L" : " << std::to_wstring(cVekPocet.evaluate(*current)) << std::endl;
		}

		if (vekPodiel) {
			std::wcout << L" Podiel kriteria veku medzi rokmi " + std::to_wstring(minVekPodiel)
				+ L" - " + std::to_wstring(maxVekPodiel) + L" pre pohlavie: " + VypisEnumPohlavie(vybranePodiel)
				+ L" : " << std::to_wstring(cVekPodiel.evaluate(*current)) << std::endl;
		}

		pokracuj = false;
		while (!pokracuj) {

			if (current->getTyp() != TypUzemnejJednotky::Stat)
				current = cNadradena.evaluate(*current);
			else {
				pokracuj = true;
				break;
			}
			std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl
				<< L"Nazov: " << cNazov.evaluate(*current) << std::endl;
		}

		std::wcout << L"*****" << std::endl;

	}
	delete tabulkaNaPracovanie;
	std::wcin.ignore(256, '\n');

	return;
}

void Priklady::Priklad3aTriedenieNazvu() {
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	FiltrovaniePreTriedenie3(table, 'a');
	if (table->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete table;
		return;
	}
	bool pokracovat = false;

	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}
	delete table;


	std::wcout << L"[1] Vzostupne " << std::endl << L"[2] Zostupne" << std::endl;

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
		std::wcout << cNazov->evaluate(*uzemnaJednotka->accessData()) << std::endl;
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
	FiltrovaniePreTriedenie3(table, 'b');

	if (table->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete table;
		return;
	}
	bool pokracovat = false;
	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}

	delete table;

	int cisloTypu;
	while (!pokracovat) {
		std::wcout << std::endl << L"Vyberte typ vzdelania podla ktoreho poctu chcete triedit" << std::endl;
		for (int i = 0; i < this->typyVzdelania->size(); ++i)
		{
			std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
		}

		std::wcin >> cisloTypu;
		if (cisloTypu > 0 && cisloTypu <= this->typyVzdelania->size())
		{
			vzdelanie = typyVzdelania->at(cisloTypu - 1);
			pokracovat = true;
		}
	}


	SortCriterion<int>* sort = new SortCriterion<int>();
	CriterionVzdelaniePocet* cPocet = new CriterionVzdelaniePocet(vzdelanie);
	CriterionNazov cNazov;

	std::wcout << L"[1] Vzostupne " << std::endl << L"[2] Zostupne" << std::endl;
	pokracovat = false;
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
		std::wcout << cNazov.evaluate(*uzemnaJednotka->accessData()) + L" po�et " + VypisEnumVzdelanie(vzdelanie) +
			L" : " + std::to_wstring(cPocet->evaluate(*uzemnaJednotka->accessData())) << std::endl;
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
	FiltrovaniePreTriedenie3(table, 'c');

	if (table->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete table;
		return;
	}

	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}
	delete table;

	bool pokracovat = false;
	int cisloTypu;
	while (!pokracovat) {
		std::wcout << std::endl << L"Vyberte typ vzdelania podla ktoreho poctu chcete triedit" << std::endl;
		for (int i = 0; i < this->typyVzdelania->size(); ++i)
		{
			std::wcout << std::to_wstring(i + 1) + L". " + VypisEnumVzdelanie(this->typyVzdelania->at(i)) << std::endl;
		}

		std::wcin >> cisloTypu;
		if (cisloTypu > 0 && cisloTypu <= this->typyVzdelania->size())
		{
			vzdelanie = typyVzdelania->at(cisloTypu - 1);
			pokracovat = true;
		}
	}
	pokracovat = false;
	std::wcout << L"[1] Vzostupne " << std::endl << L"[2] Zostupne" << std::endl;

	CriterionNazov cNazov;
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
		std::wcout << cNazov.evaluate(*uzemnaJednotka->accessData()) + L" podiel " + VypisEnumVzdelanie(vzdelanie) +
			L" : " + std::to_wstring(cPodiel->evaluate(*uzemnaJednotka->accessData())) << std::endl;


	}

	delete sort;
	delete cPodiel;
	delete tabulkaNaPracovanie;
	return;
}

void Priklady::Priklad3aTriedenieVekPodiel()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	int min = 0;
	int max = 0;

	FiltrovaniePreTriedenie4(table, 'a');

	if (table->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete table;
		return;
	}

	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}
	delete table;

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

	vybrane = false;
	while (!vybrane) {
		std::wcout << L"Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
		std::wcin >> min;
		std::wcout << L"Zadajte hornu vekovu hranicu [0 - 100]" << std::endl;
		std::wcin >> max;
		if (min < max && min >= 0 && min <= 100 && max >= 0 && max <= 100)
			vybrane = true;
	}

	Pohlavie pohlavie = Pohlavie::Zena;
	if (cislo == 2) {
		pohlavie = Pohlavie::Muz;
	}
	else if (cislo == 3) {
		pohlavie = Pohlavie::Obe;
	}


	bool pokracovat = false;
	std::wcout << L"[1] Vzostupne " << std::endl << L"[2] Zostupne" << std::endl;

	SortCriterion<double>* sort = new SortCriterion<double>();
	CriterionIntVekPodiel* cVek = new CriterionIntVekPodiel(pohlavie, min, max);
	CriterionNazov cNazov;
	CriterionIntVekPocet cPocetZeny(Pohlavie::Zena, min, max);
	CriterionIntVekPocet cPocetMuzi(Pohlavie::Muz, min, max);


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
		std::wcout << cNazov.evaluate(*uzemnaJednotka->accessData()) << std::endl;
		std::wcout << L"Pocet muzov medzi rokmi " + std::to_wstring(min) + L" - " + std::to_wstring(max) + L" : " +
			std::to_wstring(cPocetMuzi.evaluate(*uzemnaJednotka->accessData())) << std::endl;
		std::wcout << L"Pocet zien medzi rokmi " + std::to_wstring(cPocetZeny.evaluate(*uzemnaJednotka->accessData()))
			<< std::endl;
		std::wcout << L"Podiel " + VypisEnumPohlavie(pohlavie) + L" : " +
			std::to_wstring(cVek->evaluate(*uzemnaJednotka->accessData())) << std::endl;

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

	FiltrovaniePreTriedenie4(table, 'b');

	if (table->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete table;
		return;
	}

	structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::UnsortedSequenceTable<std::wstring, UzemnaJednotka*>;
	for (structures::TableItem<std::wstring, UzemnaJednotka*>* tableItem : *table)
	{
		tabulkaNaPracovanie->insert(tableItem->getKey(), tableItem->accessData());
	}
	delete table;

	bool vybrane = false;
	int cislo;
	while (!vybrane) {
		std::wcout << std::endl << L"Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
			L"[1.] Predproduktivni " << std::endl << L"[2.] Produktivni " << std::endl << L"[3.] Poproduktivni " << std::endl;

		std::wcin >> cislo;
		if (cislo > 0 && cislo <= 3)
		{
			vybrane = true;
		}
	}

	switch (cislo)
	{
	case 1:
		vekSkupina = VekovaSkupina::Predproduktivni;
		break;
	case 2:
		vekSkupina = VekovaSkupina::Produktivni;
		break;
	default:
		vekSkupina = VekovaSkupina::Poproduktivni;
		break;
	}

	bool pokracovat = false;
	std::wcout << L"[1] Vzostupne " << std::endl << L"[2] Zostupne" << std::endl;
	CriterionNazov cNazov;
	SortCriterion<int>* sort = new SortCriterion<int>();
	CriterionVekovaSkupinaPocet* cVek = new CriterionVekovaSkupinaPocet(vekSkupina);
	CriterionVekovaSkupinaPodiel cSkupinaPodielPred(VekovaSkupina::Predproduktivni);
	CriterionVekovaSkupinaPodiel cSkupinaPodielPro(VekovaSkupina::Produktivni);
	CriterionVekovaSkupinaPodiel cSkupinaPodielPo(VekovaSkupina::Poproduktivni);
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
		std::wcout << cNazov.evaluate(*uzemnaJednotka->accessData()) << std::endl;
		std::wcout << L"Po�et zvolenej vekovej skupiny " + VypisEnumVekovaSkupina(vekSkupina) + L" : " +
			std::to_wstring(cVek->evaluate(*uzemnaJednotka->accessData())) << std::endl;

		std::wcout << L"Podiel po�tu vekovej skupiny predproduktivni " +
			std::to_wstring(cSkupinaPodielPred.evaluate(*uzemnaJednotka->accessData())) << std::endl;
		std::wcout << L"Podiel po�tu vekovej skupiny produktivni " +
			std::to_wstring(cSkupinaPodielPro.evaluate(*uzemnaJednotka->accessData())) << std::endl;
		std::wcout << L"Podiel po�tu vekovej skupiny ppproduktivni " +
			std::to_wstring(cSkupinaPodielPo.evaluate(*uzemnaJednotka->accessData())) << std::endl;
	}

	delete sort;
	delete cVek;
	delete tabulkaNaPracovanie;
	return;
}

void Priklady::Priklad4aNajmensiaVekovaSkupina()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	FiltrovaniePreVyber(tabulkaNaPracovanie);
	if (tabulkaNaPracovanie->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete tabulkaNaPracovanie;
		return;
	}
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

	SelectionMinVekovaSkupina<std::wstring>* select = new SelectionMinVekovaSkupina<std::wstring>(vekovaSkupina);
	UzemnaJednotka* najlepsiaUzemnaJednotka = select->selectBest(tabulkaNaPracovanie, INT_MAX);
	const UzemnaJednotka* current = najlepsiaUzemnaJednotka;
	CriterionNazov cNazov;
	CriterionTyp cTyp;
	CriterionVekovaSkupinaPocet cSkupinaPocetPred(VekovaSkupina::Predproduktivni);
	CriterionVekovaSkupinaPocet cSkupinaPocetPro(VekovaSkupina::Produktivni);
	CriterionVekovaSkupinaPocet cSkupinaPocetPo(VekovaSkupina::Poproduktivni);
	CriterionNadradena cNadradena;
	pokracuj = false;
	std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl << L"Nazov: ";
	std::wcout << current->getNazov() << std::endl;

	std::wcout << L"Hodnoty vekovovych skupin: " << std::endl;

	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
		std::to_wstring(cSkupinaPocetPred.evaluate(*current)) << std::endl;
	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
		std::to_wstring(cSkupinaPocetPro.evaluate(*current)) << std::endl;
	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
		std::to_wstring(cSkupinaPocetPo.evaluate(*current)) << std::endl;

	while (!pokracuj) {

		if (current->getTyp() != TypUzemnejJednotky::Stat)
			current = cNadradena.evaluate(*current);
		else {
			pokracuj = true;
			break;
		}
		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;
	}


	delete select;
	delete tabulkaNaPracovanie;
}

void Priklady::Priklad4bNajvacsiaVekovaSkupina()
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* tabulkaNaPracovanie = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>(*tab_all_sorted);
	FiltrovaniePreVyber(tabulkaNaPracovanie);

	if (tabulkaNaPracovanie->size() == 0) {
		std::wcout << L"Filtrovaniu nevyhovuju ziadne uzemne jednotky." << std::endl;
		delete tabulkaNaPracovanie;
		return;
	}


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
	CriterionVekovaSkupinaPocet cSkupinaPodielPred(VekovaSkupina::Predproduktivni);
	CriterionVekovaSkupinaPocet cSkupinaPodielPro(VekovaSkupina::Produktivni);
	CriterionVekovaSkupinaPocet cSkupinaPodielPo(VekovaSkupina::Poproduktivni);
	CriterionNazov cNazov;
	CriterionTyp cTyp;
	CriterionNadradena cNadradena;
	SelectionMaxVekovaSkupina<std::wstring>* select = new SelectionMaxVekovaSkupina<std::wstring>(vekovaSkupina);
	UzemnaJednotka* najlepsiaUzemnaJednotka = (select->selectBest(tabulkaNaPracovanie, 0));
	const UzemnaJednotka* current = najlepsiaUzemnaJednotka;
	std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl << L"Nazov: ";
	std::wcout << current->getNazov() << std::endl;


	std::wcout << L" Hodnoty vekovovych skupin: " << std::endl;


	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Predproduktivni) + L" : " +
		std::to_wstring(cSkupinaPodielPred.evaluate(*current)) << std::endl;
	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Produktivni) + L" : " +
		std::to_wstring(cSkupinaPodielPro.evaluate(*current)) << std::endl;
	std::wcout << L" * " + this->VypisEnumVekovaSkupina(VekovaSkupina::Poproduktivni) + L" : " +
		std::to_wstring(cSkupinaPodielPo.evaluate(*current)) << std::endl;

	pokracuj = false;
	while (!pokracuj) {



		if (current->getTyp() != TypUzemnejJednotky::Stat)
			current = cNadradena.evaluate(*current);
		else {
			pokracuj = true;
			break;
		}
		std::wcout << std::endl << L"Typ: " + VypisEnumTyp(cTyp.evaluate(*current)) << std::endl << L"Nazov: ";
		std::wcout << current->getNazov() << std::endl;
	}


	delete select;
	delete tabulkaNaPracovanie;
}

void Priklady::FiltrovaniePreTriedenie3(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha) {
	bool pouzityPocet = false;
	bool pouzityPodiel = false;
	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;


	while (!pokracuj) {
		if (!typ)
			std::wcout << L"[1] Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"[2] Filtrovat podla prislusnosti" << std::endl;
		if (uloha == 'b' && !pouzityPocet)
			std::wcout << L"[3] Filtrovat podla poctu vzdelania" << std::endl;
		if (uloha == 'c' && !pouzityPodiel)
			std::wcout << L"[4] Filtrovat podla podielu vzdelania" << std::endl;
		std::wcout << L"[0] - Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			TypFiltrovanie(table);

		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			PrislusnostFiltrovanie(table);
		}
		else if (filter == 3 && (uloha == 'b')) {
			pouzityPocet = true;
			VzdelaniePocetFiltrovanie(table);
		}
		else if (filter == 4 && uloha == 'c') {
			pouzityPodiel = true;
			VzdelaniePodielFiltrovanie(table);
		}
		else if (filter == 0) {
			pokracuj = true;
		}
	}


	return;

}

void Priklady::FiltrovaniePreTriedenie4(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table, char uloha) {

	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;
	bool filterNavyse = false;

	while (!pokracuj) {
		if (!typ)
			std::wcout << L"[1] Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"[2] Filtrovat podla prislusnosti" << std::endl;
		if (uloha == 'a' && !filterNavyse)
			std::wcout << L"[3] Filtrovat podla podielu veku" << std::endl;
		if (uloha == 'b' && !filterNavyse)
			std::wcout << L"[4] Filtrovat podla poctu vekovej skupiny" << std::endl;
		std::wcout << L"[0] Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			TypFiltrovanie(table);
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			PrislusnostFiltrovanie(table);

		}
		else if (filter == 3 && (uloha == 'a') && !filterNavyse) {
			filterNavyse = true;
			int min;
			int max;
			Pohlavie pohlavie;
			VekPodielFiltrovanie(table, &pohlavie, &min, &max);
		}
		else if (filter == 4 && uloha == 'b' && !filterNavyse) {
			filterNavyse = true;
			bool vybrane = false;
			int cislo;
			while (!vybrane) {
				std::wcout << std::endl << L"Vyberte vekovu hranicu ktore chcete filtrovat: " << std::endl <<
					L"[1.] Predproduktivni " << std::endl << L"[2.] Produktivni " << std::endl << L"[3.] Poproduktivni " << std::endl;

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
				std::wcout << L"Zadajte hornu hranicu poctu" << std::endl;
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

			FilterIntVekovaSkupinaPocet* fPocet = new FilterIntVekovaSkupinaPocet(vekovaSkupina, minPocet, maxPocet);
			fPocet->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
			delete fPocet;
		}
		else if (filter == 0) {
			pokracuj = true;
		}
	}

	return;
}

void Priklady::FiltrovaniePreVyber(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table)
{
	int filter = -1;
	bool pokracuj = false;
	bool typ = false;
	bool prislusnost = false;


	while (!pokracuj) {
		if (!typ)
			std::wcout << L"[1] Filtrovat podla typu" << std::endl;
		if (!prislusnost)
			std::wcout << L"[2] - Filtrovat podla prislusnosti" << std::endl;

		std::wcout << L"0 - Pokracovat" << std::endl;

		std::wcin >> filter;

		if (filter == 1 && !typ) {
			typ = true;
			TypFiltrovanie(table);
		}
		else if (filter == 2 && !prislusnost) {
			prislusnost = true;
			PrislusnostFiltrovanie(table);
		}
		else if (filter == 0)
			pokracuj = true;
	}

	return;
}

void Priklady::TypFiltrovanie(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table)
{
	std::wcout << L"Vyberte si uzemnu jednotku: " << std::endl;
	std::wcout << L"[1] Stat" << std::endl << L"[2] Kraj" << std::endl << L"[3] Okres" << std::endl << L"[4] Obec" << std::endl;
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
		delete fTyp;
		vybrane = true;
	}
}

void Priklady::PrislusnostFiltrovanie(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table)
{
	std::wstring nazov;
	std::wcout << std::endl << L"Zadajte nazov vyssej uzemnej jednotky ku ktorej maju vybrane uzemne jednotky prisluchat" << std::endl;
	std::wcin.ignore();
	getline(std::wcin, nazov);
	UzemnaJednotka* data;
	bool najdene = tab_all_sorted->tryFind(nazov, data);
	if (!najdene) {
		std::wcout << std::endl << L"Uzemna jednotka s takymto nazvom nebola najdena." << std::endl;
	}
	else {
		FilterPrislusnost* fPrislusnost = new FilterPrislusnost(data, true);
		fPrislusnost->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
		delete fPrislusnost;
	}

}

void Priklady::VzdelaniePocetFiltrovanie(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table)
{
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
		if (cisloTypu > 0 && cisloTypu <= this->typyVzdelania->size())
		{
			vybranyTyp = typyVzdelania->at(cisloTypu - 1);
			vybrane = true;
		}
	}
	int min;
	int max;
	std::wcout << L"Zadajte dolnu hranicu poctu" << std::endl;
	std::wcin >> min;
	std::wcout << L"Zadajte hornu hranicu poctu (pokial zadate 0, horna hranica bude maximalna)" << std::endl;
	std::wcin >> max;
	if (max == 0) {
		max = INT_MAX;
	}

	if (min <= max && min >= 0 && max >= 0)
		vybrane = true;
	FilterIntPocet* fPocet = new FilterIntPocet(vybranyTyp, min, max);
	fPocet->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
	delete fPocet;
}

void Priklady::VzdelaniePodielFiltrovanie(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table)
{
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
	vybrane = false;
	while (!vybrane) {
		std::wcout << L"Zadajte dolnu hranicu podielu [0-100]" << std::endl;
		std::wcin >> min;
		std::wcout << L"Zadajte hornu hranicu podielu [0-100] (pokial zadate 0, horna hranica bude 100%)";
		std::wcin >> max;
		if (max == 0) {
			max = 100;
		}
		if (min <= max && min >= 0 && max >= 0 && min <= 100 && max <= 100)
			vybrane = true;
	}
	FilterIntPodiel* fPodiel = new FilterIntPodiel(vybranyTyp, min, max);
	fPodiel->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
	delete fPodiel;
}

void Priklady::VekovaSkupinaPocetFiltrovanie(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table)
{
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
		std::wcout << L"Zadajte hornu hranicu poctu (pokial zadate 0, horna hranica bude maximalna)" << std::endl;
		std::wcin >> maxPocet;
		if (maxPocet == 0) {
			maxPocet = INT_MAX;
		}
		if (minPocet <= maxPocet && minPocet >= 0 && maxPocet >= 0)
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
	FilterIntVekovaSkupinaPocet* fPocet = new FilterIntVekovaSkupinaPocet(vekovaSkupina, minPocet, maxPocet);
	fPocet->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
	delete fPocet;
}

void Priklady::VekPodielFiltrovanie(structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table, Pohlavie* pohlavie, int* min, int* max)
{
	bool vybrane = false;
	int cislo;

	while (!vybrane) {
		std::wcout << std::endl << L"Vyberte pohlavie ktore chcete filtrovat: L" << std::endl <<
			L"[1.] Zeny" << std::endl << L"[2.] Muzi" << std::endl << L"[3.] Zeny aj muzi" << std::endl;

		std::wcin >> cislo;
		if (cislo > 0 && cislo <= 3)
		{
			vybrane = true;
		}
	}
	Pohlavie vybranePohlavie;
	int minPocet = 0;
	int maxPocet = 0;
	int minVek = 0;
	int maxVek = 0;
	vybrane = false;
	while (!vybrane) {
		std::wcout << L"Zadajte dolnu vekovu hranicu [0 - 100]" << std::endl;
		std::wcin >> minVek;
		std::wcout << L"Zadajte hornu vekovu hranicu [0 - 100]" << std::endl;
		std::wcin >> maxVek;
		if (minVek <= maxVek && minVek >= 0 && minVek <= 100 && maxVek >= 0 && maxVek <= 100)
			vybrane = true;
	}
	vybrane = false;

	while (!vybrane) {
		std::wcout << L"Zadajte dolnu hranicu podielu [0 - 100]" << std::endl;
		std::wcin >> minPocet;
		std::wcout << L"Zadajte hornu hranicu podielu [0 - 100]" << std::endl;
		std::wcin >> maxPocet;
		if (minPocet <= maxPocet && minPocet >= 0 && maxPocet >= 0 && minPocet <= 100 && maxPocet <= 100)
			vybrane = true;
	}
	if (cislo == 2) {
		vybranePohlavie = Pohlavie::Muz;
	}
	else if (cislo == 3) {
		vybranePohlavie = Pohlavie::Obe;
	}

	*pohlavie = Pohlavie::Zena;
	*min = minVek;
	*max = maxVek;
	FilterIntVekPodiel* fPodiel = new FilterIntVekPodiel(vybranePohlavie, minPocet, maxPocet, minVek, maxVek);
	fPodiel->filterTable(table, new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>);
	delete fPodiel;
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

