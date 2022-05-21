#pragma once
#include <string>
#include "structures/unsorted_sequence_table.h"
#include "Kraj.h"
#include "Okres.h"
#include "Obec.h"
#include <iostream> //
#include "structures/sorted_sequence_table.h"
#include "structures/array.h"
#include <fstream> //
#include <codecvt> //


class Input
{
	public:
	void InputKraje(structures::SortedSequenceTable<std::wstring, Kraj*>* table, std::wstring filename, VyssiCelok* stat);
	void InputOkresy(structures::SortedSequenceTable<std::wstring, Okres*>* table, std::wstring filename, structures::SortedSequenceTable<std::wstring, Kraj*>* table_kraje);
	void InputObce(structures::SortedSequenceTable<std::wstring, Obec*>* table, std::wstring filename,  structures::SortedSequenceTable<std::wstring, Okres*>* table_okresy);
	void InputVek(std::wstring filename, structures::SequenceTable<std::wstring, Obec*>* table_obce);


};

