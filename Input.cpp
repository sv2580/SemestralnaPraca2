#include "Input.h"

#include "structures/array.h"
#include <sstream>
#include <fstream>
#include <codecvt>
#include <locale>
#include <codecvt>
#include <iostream>

#include <cstdio>
#include <io.h>
#include <fcntl.h>

structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>& Input::InputKraje(std::string filename, VyssiCelok* stat)
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>();

	/*http://www.cplusplus.com/forum/beginner/107125/*/
	std::wifstream file(filename);

	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	file.imbue(utf8_locale);

	wchar_t bom = L'\0';
	file.get(bom);


	std::wstring line;
	std::getline(file, line);

	while (!file.eof()) {
		std::wstring word;
		std::getline(file, line, L';');
		std::getline(file, line, L';');
		std::getline(file, line, L';');
		std::wstring nazov = line;

		std::getline(file, line, L';');
		std::getline(file, line, L';');
		std::getline(file, line, L'\n');
		std::wstring key = line.substr(5, 5);
		Kraj* kraj = new Kraj(TypUzemnejJednotky::Kraj, nazov, stat);

		if (key != L"*****") {
			table->insert(key, kraj);
			stat->addNizsiCelok(kraj);
		}
	}
	file.close();
	return *table;

}

structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>& Input::InputOkresy(std::string filename,
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_kraje)
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>();

	std::wifstream file(filename);

	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	file.imbue(utf8_locale);

	wchar_t bom = L'\0';
	file.get(bom);


	std::wstring line;
	std::getline(file, line);

	while (!file.eof()) {
		std::wstring word;
		std::getline(file, line, L';');
		std::getline(file, line, L';');
		std::wstring key = line;
		std::getline(file, line, L';');
		std::wstring nazov = line;

		std::getline(file, line, L';');
		std::getline(file, line, L';');
		std::getline(file, line, L'\n');

		if (key != L"SKZZZ" && key != L"SKZZZZ") {
			UzemnaJednotka* data = nullptr;
			table_kraje->tryFind(key.substr(0, 5), data);
			VyssiCelok* vyssiCelok = dynamic_cast<VyssiCelok*>(data);

			Okres* okres = new Okres(TypUzemnejJednotky::Okres, nazov, vyssiCelok);
			vyssiCelok->addNizsiCelok(okres);

			table->insert(key, okres);
		}
	}
	file.close();
	return *table;


}

structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>& Input::InputObce(std::string filename,
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_okresy)
{
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table = new structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>();
	std::wifstream file(filename);

	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	file.imbue(utf8_locale);

	wchar_t bom = L'\0';
	file.get(bom);


	std::wstring line;
	std::getline(file, line);

	while (!file.eof()) {

		structures::Array<Vzdelanie*>* pole = new structures::Array<Vzdelanie*>(8);
		std::getline(file, line, L';');
		std::wstring key = line;
		if (key != L"SKZZZZ" && key != L"SKZZZZZZZZZZ") {
			int num = 0;
			TypVzdelania typ;
			std::getline(file, line, L';');
			std::wstring nazov = line;
			for (size_t i = 0; i < 8; i++) {
				
				if(i == 7)
					std::getline(file, line, L'\n');
				else if (i < 7) {
					std::getline(file, line, L';');
				}

				switch (i)
				{
				case 0:
					typ = TypVzdelania::BezDo15;
					break;
				case 1:
					typ = TypVzdelania::Zakladne;
					break;
				case 2:
					typ = TypVzdelania::StredneOdborne;
					break;
				case 3:
					typ = TypVzdelania::UplneStredne;
					break;
				case 4:
					typ = TypVzdelania::VyssieOdborne;
					break;
				case 5:
					typ = TypVzdelania::Vysokoskolske;
					break;
				case 6:
					typ = TypVzdelania::BezNad15;
					break;
				case 7:
					typ = TypVzdelania::Nezistene;
					break;
				}
				pole->at(num) = new Vzdelanie(typ, stoi(line));

				num++;

			}
			UzemnaJednotka* data = nullptr;
			table_okresy->tryFind(key.substr(0, 6), data);
			VyssiCelok* vyssiCelok = dynamic_cast<VyssiCelok*>(data);

			Obec* obec = new Obec(TypUzemnejJednotky::Obec, nazov, vyssiCelok);
			vyssiCelok->addNizsiCelok(obec);

			obec->setVzdelanie(pole);
			table->insert(key, obec);

		}

	}
	file.close();
	return *table;
}
void Input::InputVek(std::string filename, structures::SequenceTable<std::wstring, UzemnaJednotka*>* table_obce)
{
	std::wifstream file(filename);


	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	file.imbue(utf8_locale);

	wchar_t bom = L'\0';
	file.get(bom);


	std::wstring line;
	std::getline(file, line);

	while (!file.eof()) {
		structures::Array<Vek*>* pole = new structures::Array<Vek*>(200);

		std::getline(file, line, L';');
		std::wstring key = line;

		std::getline(file, line, L';');
		
		Pohlavie pohlavie = Pohlavie::Muz;

		for (int i = 0; i < 200; i++)
		{
			if (i > 99) {
				pohlavie = Pohlavie::Zena;
			}

			if (i == 199)
				std::getline(file, line, L'\n');
			else if (i < 199) {
				std::getline(file, line, L';');
			}

			pole->at(i) = new Vek(pohlavie, stoi(line), i % 100);
		}


		UzemnaJednotka* data = nullptr;
		table_obce->tryFind(key, data);
		Obec* obec = dynamic_cast<Obec*>(data);

		obec->setVek(pole);
	}
	file.close();

}