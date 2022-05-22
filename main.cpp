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
    _setmode(_fileno(stdin), _O_U16TEXT);


    bool pokracuj = false;
    
    Priklady* input = new Priklady();
    int priklad;
    while (!pokracuj) {
        std::wcout << std::endl << L"Stlacte klavesu: " << std::endl;
        std::wcout << L"[0] Koniec " << std::endl;
        std::wcout << L"[1] Príklad - informacie o uzemnej jednotky vyhladanej podla nazvu" << std::endl;
        std::wcout << L"[2] Priklad - filtrovanie podla poctu/podielu vzdelania, typu, prislusnosti" << std::endl;
        std::wcout << L"[3] Priklad 3A - Triedenie podla nazvu" << std::endl;
        std::wcout << L"[4] Priklad 3B - Triedenie podla poctu zvoleneho typu vzdelania" << std::endl;
        std::wcout << L"[5] Priklad 3C - Triedenie podla podielu zvoleneho typu vzdelania" << std::endl;
        std::wcout << L"[6] Priklad 3A - Triedenie podla podielu zvoleneho veku" << std::endl;
        std::wcout << L"[7] Priklad 3B - Triedenie podla poctu vekovej skupiny" << std::endl;
        std::wcout << L"[8] Priklad 4A - Najdenie najlepsieho s najmensim poctom vekovej skupiny" << std::endl;
        std::wcout << L"[9]. Priklad 4B - Najdenie najlepsieho s najvacsim poctom vekovej skupiny" << std::endl;

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
            input->Priklad3cTriedeniePodielu();
        }
        else if (priklad == 6) {
            input->Priklad3aTriedenieVekPodiel();
        }
        else if (priklad == 7) {
            input->Priklad3bTriedenieVekovaSkupinaPocet();
        }
        else if (priklad == 8) {
            input->Priklad4aNajmensiaVekovaSkupina();
        }
        else if (priklad == 9) {
            input->Priklad4bNajvacsiaVekovaSkupina();
        }

    }
    delete input;
}

