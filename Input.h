#pragma once
#include <string>
#include "structures/unsorted_sequence_table.h"
#include "Kraj.h"
#include "Okres.h"
#include "Obec.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "structures/sorted_sequence_table.h"
#include <iostream>

class Input
{
	public:
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>& InputKraje(std::string filename, VyssiCelok* stat);
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>& InputOkresy(std::string filename, structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_kraje);
	structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>& InputObce(std::string filename,  structures::SortedSequenceTable<std::wstring, UzemnaJednotka*>* table_okresy);
	void InputVek(std::string filename, structures::SequenceTable<std::wstring, UzemnaJednotka*>* table_obce);


};

