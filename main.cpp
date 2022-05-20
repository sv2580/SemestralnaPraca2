// Student.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "Student.h"
#include "Filter.h"
#include "structures/sorted_sequence_table.h"
#include "Priklady.h"
#include "structures/heap_monitor.h"
#include <io.h> 
#include <fcntl.h> 
int main()
{
    initHeapMonitor();
    _setmode(_fileno(stdout), _O_U16TEXT);

    bool pokracuj = false;
    Priklady* input = new Priklady();
    int priklad;
    while (!pokracuj) {
        std::wcout << "0. Koniec " << std::endl;
        std::wcout << "1. Pr�klad - vypis poctu vzdelania, typu, vekovych skupin podla nazvu" << std::endl;
        std::wcout << "2. Priklad - filtrovanie podla poctu/podielu vzdelania, typu, prislusnosti" << std::endl;
        std::wcout << "3. Priklad 3A - Triedenie podla nazvu" << std::endl;
        std::wcout << "4. Priklad 3B - Triedenie podla poctu" << std::endl;
        std::wcout << "5. Priklad 3C - Triedenie podla podielu" << std::endl;

        std::wcin >> priklad;
        if (priklad == 0) {
            pokracuj = true;
        }
        else if (priklad == 1) {
            input->Priklad1Vypis();
        }
        else if (priklad == 2) {
            input->Priklad2Filtrovanie();
        }
        else if (priklad == 3) {
            input->Priklad3aTriedenieNazvu();
        }
        else if (priklad == 4 ) {
            input->Priklad3bTriedeniePoctu();
        } 
        else if (priklad == 5) {
            input->Priklad3bTriedeniePoctu();
        }

    }
    delete input;
    return 0;
}

