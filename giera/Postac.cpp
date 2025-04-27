#include "Postac.h"
#include "Bron.h"
#include "Pokoj.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

Postac::~Postac() {
    delete aktualnaBron;
}

Postac::Postac(const Postac& other) {
    // Kopiowanie wszystkich pól
    nazwa = other.nazwa;
    hp = other.hp;
    maxHp = other.maxHp;
    pancerz = other.pancerz;
    obrazenia = other.obrazenia;
    penetracjaPancerza = other.penetracjaPancerza;
    szansaNaUnikBierny = other.szansaNaUnikBierny;
    unikManualny = other.unikManualny;
    szansaNaUcieczke = other.szansaNaUcieczke;
    poziom = other.poziom;
    exp = other.exp;
    zloto = other.zloto;
    mikstury = other.mikstury;
    efektyStatusow = other.efektyStatusow;
    powalony = other.powalony;
    pozostaleTuryPowalenia = other.pozostaleTuryPowalenia;

    // Głęboka kopia broni (jeśli istnieje)
    if (other.aktualnaBron) {
        aktualnaBron = new Bron(*other.aktualnaBron);
    } else {
        aktualnaBron = nullptr;
    }
}

Postac& Postac::operator=(const Postac& other) {
    if (this == &other) {
        return *this; // Zabezpieczenie przed samoprzypisaniem
    }

    // Zwolnienie starej broni
    delete aktualnaBron;

    // Kopiowanie pól
    nazwa = other.nazwa;
    hp = other.hp;
    maxHp = other.maxHp;
    pancerz = other.pancerz;
    obrazenia = other.obrazenia;
    penetracjaPancerza = other.penetracjaPancerza;
    szansaNaUnikBierny = other.szansaNaUnikBierny;
    unikManualny = other.unikManualny;
    szansaNaUcieczke = other.szansaNaUcieczke;
    poziom = other.poziom;
    exp = other.exp;
    zloto = other.zloto;
    mikstury = other.mikstury;
    efektyStatusow = other.efektyStatusow;
    powalony = other.powalony;
    pozostaleTuryPowalenia = other.pozostaleTuryPowalenia;

    // Głęboka kopia broni
    if (other.aktualnaBron) {
        aktualnaBron = new Bron(*other.aktualnaBron);
    } else {
        aktualnaBron = nullptr;
    }

    return *this;
}

void Postac::dodajEfektStatusu(const string& efekt, int tury) {
    efektyStatusow[efekt] = tury;
}

void Postac::otrzymajObrazenia(int ile) {
    hp = clamp(hp - ile, 0, maxHp);
    clearScreen();
    cout << "Otrzymales " << ile << " obrazen! Aktualne HP: " << hp << "/" << maxHp << endl;
    czekajNaKlawisz();
}

bool Postac::czyPowalony() const { 
    return powalony; 
}

void Postac::ustawPowalenie(int tury) {
    powalony = true;
    pozostaleTuryPowalenia = tury;
    clearScreen();
    cout << "Zostales powalony! Stracisz " << tury << " tury.\n";
    czekajNaKlawisz();
}

void Postac::aktualizujPowalenie() {
    if (powalony && --pozostaleTuryPowalenia <= 0) {
        powalony = false;
        clearScreen();
        cout << "Odzyskujesz rownowage! Juz nie jestes powalony.\n";
        czekajNaKlawisz();
    }
}

void Postac::aktualizujEfektyStatusowe() {
    for (auto it = efektyStatusow.begin(); it != efektyStatusow.end(); ) {
        const string& efekt = it->first;
        int& tury = it->second;

        if (efekt == "krwawienie") {
            hp -= 6;
            cout << "Krwawienie zadaje 6 obrazen. Pozostalo tur: " << (tury-1) << endl;
        }
        else if (efekt == "zatrucie") {
            hp -= 3;
            cout << "Trucizna zadaje 3 obrazenia. Pozostalo tur: " << (tury-1) << endl;
        }
        else if (efekt == "spowolnienie") {
            cout << "Jestes spowolniony. Pozostalo tur: " << (tury-1) << endl;
        }

        if (--tury <= 0) {
            cout << "Efekt " << efekt << " wygasl.\n";
            it = efektyStatusow.erase(it);
        } else {
            ++it;
        }
    }
}

void Postac::aktualizujPoziomPrzeciwnikow(vector<Pokoj*>& wszystkiePokoje, int poziomGracza) {
    for (auto pokoj : wszystkiePokoje) {
        if (!pokoj) continue;
        
        for (auto& przeciwnik : pokoj->pobierzPrzeciwnikow()) {
            if (przeciwnik) {
                przeciwnik->skalujDoPoziomu(poziomGracza);
            }
        }
    }
}

void Postac::zdobadzDoswiadczenie(int ilosc, vector<Pokoj*>& wszystkiePokoje) {
    int staryPoziom = poziom;
    exp += ilosc;
    
    // Progi doświadczenia dla poziomów
    const vector<int> progiDoswiadczenia = {
        0,     // poziom 1: 0-100
        100,   // poziom 2: 100-250
        250,   // poziom 3: 250-450
        450,   // poziom 4: 450-650
        650,   // poziom 5: 650-900
        900,   // poziom 6: 900-1200
        1200,  // poziom 7: 1200-1500
        1500,  // poziom 8: 1500-1800
        1800,  // poziom 9: 1800-2100
        2100   // poziom 10: 2100-2500
    };
    
    // Sprawdzanie awansów
    while (poziom < 10 && exp >= progiDoswiadczenia[poziom]) {
        // Bonusy za awans
        int bonusHP = 0;
        int bonusObrazenia = 0;
        float bonusPancerz = 0.0f;
        float bonusPenetracja = 0.0f;
        float bonusUnikBierny = 0.0f;
        float bonusUnikManualny = 0.0f;
        float bonusUcieczka = 0.0f;
        
        switch(poziom) {
            case 1: // Awans na poziom 2
                bonusHP = 15;
                bonusObrazenia = 1;
                bonusPancerz = 0.02f;
                bonusPenetracja = 0.01f;
                bonusUnikBierny = 0.01f;
                bonusUnikManualny = 0.02f;
                bonusUcieczka = 0.005f;
                break;
            case 2: // Awans na poziom 3
                bonusHP = 30;
                bonusObrazenia = 3;
                bonusPancerz = 0.04f;
                bonusPenetracja = 0.02f;
                bonusUnikBierny = 0.02f;
                bonusUnikManualny = 0.04f;
                bonusUcieczka = 0.01f;
                break;
            case 3: // Awans na poziom 4
                bonusHP = 45;
                bonusObrazenia = 4;
                bonusPancerz = 0.06f;
                bonusPenetracja = 0.03f;
                bonusUnikBierny = 0.03f;
                bonusUnikManualny = 0.06f;
                bonusUcieczka = 0.015f;
                break;
            case 4: // Awans na poziom 5
                bonusHP = 60;
                bonusObrazenia = 6;
                bonusPancerz = 0.08f;
                bonusPenetracja = 0.04f;
                bonusUnikBierny = 0.04f;
                bonusUnikManualny = 0.08f;
                bonusUcieczka = 0.02f;
                break;
            case 5: // Awans na poziom 6
                bonusHP = 75;
                bonusObrazenia = 7;
                bonusPancerz = 0.10f;
                bonusPenetracja = 0.05f;
                bonusUnikBierny = 0.05f;
                bonusUnikManualny = 0.10f;
                bonusUcieczka = 0.025f;
                break;
            case 6: // Awans na poziom 7
                bonusHP = 90;
                bonusObrazenia = 9;
                bonusPancerz = 0.12f;
                bonusPenetracja = 0.06f;
                bonusUnikBierny = 0.06f;
                bonusUnikManualny = 0.12f;
                bonusUcieczka = 0.03f;
                break;
            case 7: // Awans na poziom 8
                bonusHP = 105;
                bonusObrazenia = 10;
                bonusPancerz = 0.14f;
                bonusPenetracja = 0.07f;
                bonusUnikBierny = 0.07f;
                bonusUnikManualny = 0.14f;
                bonusUcieczka = 0.035f;
                break;
            case 8: // Awans na poziom 9
                bonusHP = 120;
                bonusObrazenia = 12;
                bonusPancerz = 0.16f;
                bonusPenetracja = 0.08f;
                bonusUnikBierny = 0.08f;
                bonusUnikManualny = 0.16f;
                bonusUcieczka = 0.04f;
                break;
            case 9: // Awans na poziom 10
                bonusHP = 150;
                bonusObrazenia = 15;
                bonusPancerz = 0.20f;
                bonusPenetracja = 0.10f;
                bonusUnikBierny = 0.10f;
                bonusUnikManualny = 0.20f;
                bonusUcieczka = 0.05f;
                break;
        }
        
        // Zastosowanie bonusów
        maxHp += bonusHP;
        hp += bonusHP;
        obrazenia += bonusObrazenia;
        pancerz = min(pancerz + bonusPancerz, 0.8f); // Ograniczenie pancerza do 80%
        penetracjaPancerza = min(penetracjaPancerza + bonusPenetracja, 0.5f); // Ograniczenie penetracji do 50%
        szansaNaUnikBierny = min(szansaNaUnikBierny + bonusUnikBierny, 0.5f); // Ograniczenie uniku biernego do 50%
        unikManualny = min(unikManualny + bonusUnikManualny, 0.8f); // Ograniczenie uniku manualnego do 80%
        szansaNaUcieczke = min(szansaNaUcieczke + bonusUcieczka, 0.5f); // Ograniczenie szansy na ucieczkę do 50%
        
        poziom++;
        cout << "\n=== AWANS NA POZIOM " << poziom << " ===\n";
        cout << "Zdobyte bonusy:\n";
        cout << "+" << bonusHP << " HP\n";
        cout << "+" << bonusObrazenia << " obrazen\n";
        cout << "+" << (bonusPancerz*100) << "% pancerza\n";
        cout << "+" << (bonusPenetracja*100) << "% penetracji pancerza\n";
        cout << "+" << (bonusUnikBierny*100) << "% szansy na unik bierny\n";
        cout << "+" << (bonusUnikManualny*100) << "% uniku manualnego\n";
        cout << "+" << (bonusUcieczka*100) << "% szansy na ucieczke\n";
    }

    if (poziom > staryPoziom) {
        aktualizujPoziomPrzeciwnikow(wszystkiePokoje, poziom);
    }
}

void Postac::uzyjMikstury() {
    if (mikstury > 0) {
        int ileLeczenia = maxHp * 0.2;
        hp = min(hp + ileLeczenia, maxHp);
        mikstury--;
        clearScreen();
        cout << "Uzyles mikstury. Odzyskales " << ileLeczenia << " HP. Pozostalo mikstur: " << mikstury << "\n";
        czekajNaKlawisz();
    } else {
        clearScreen();
        cout << "Nie masz mikstur!\n";
        czekajNaKlawisz();
    }
}

void Postac::wyswietlStatystyki(bool czyscEkran) {
    if (czyscEkran) {
        clearScreen();
    }
        
        // Nagłówek
        cout << "================================\n";
        cout << "   STATYSTYKI POSTACI (" << nazwa << ")\n";
        cout << "================================\n\n";
        
        // Poziom i doświadczenie
        cout << "Poziom: " << poziom << "\n";
        
        if (poziom < 10) {
            const vector<int> progi = {0, 100, 250, 450, 650, 900, 1200, 1500, 1800, 2100};
            int expDoNastepnego = progi[poziom] - exp;
            cout << "EXP: " << exp << "/" << progi[poziom] << " (do nastepnego poziomu: " << expDoNastepnego << ")\n";
        } else {
            cout << "EXP: " << exp << " (MAKSYMALNY POZIOM)\n";
        }
        
        // Statystyki podstawowe
        cout << "\n=== PODSTAWOWE ===\n";
        cout << "HP: " << hp << "/" << maxHp << "\n";
        cout << "Obrazenia: " << obrazenia << "\n";
        if(aktualnaBron) {
            cout << " (+" << aktualnaBron->pobierzBonusObrazen() << " z " << aktualnaBron->pobierzNazwe() << ")\n";
        }
        cout << "Pancerz: " << static_cast<int>(pancerz * 100) << "%\n";
        cout << "Penetracja pancerza: " << static_cast<int>(penetracjaPancerza * 100) << "%\n";

        // Statystyki specjalne
        cout << "\n=== SPECJALNE ===\n";
        cout << "Szansa na unik bierny: " << static_cast<int>(szansaNaUnikBierny * 100) << "%\n";
        cout << "Unik manualny: " << static_cast<int>(unikManualny * 100) << "%\n";
        cout << "Szansa na ucieczke: " << static_cast<int>(szansaNaUcieczke * 100) << "%\n";

        // Ekwipunek
        cout << "\n=== EKWIPUNEK ===\n";
        if(aktualnaBron) {
            cout << "Bron: " << aktualnaBron->pobierzNazwe() << " (+" 
                 << aktualnaBron->pobierzBonusObrazen() << " obrazen)";
    
            if(aktualnaBron->pobierzEfekt() != EfektStatusu::BRAK) {
                cout << " [Efekt: ";
                switch(aktualnaBron->pobierzEfekt()) {
                    case EfektStatusu::ZATRUCIE: cout << "zatrucie"; break;
                    case EfektStatusu::PODPALENIE: cout << "podpalenie"; break;
                    case EfektStatusu::ZAMROZENIE: cout << "zamrozenie"; break;
                    case EfektStatusu::POWALENIE: cout << "powalenie"; break;
                    case EfektStatusu::KRWAWIENIE: cout << "krwawienie"; break;
                    case EfektStatusu::OGLUSZENIE: cout << "ogluszenie"; break;
                    case EfektStatusu::PORAZENIE: cout << "porazenie"; break;
                    case EfektStatusu::OSLEPIENIE: cout << "oslepienie"; break;
                    case EfektStatusu::SMIERC: cout << "smierc"; break;
                    case EfektStatusu::KRADZIEZ: cout << "kradziez"; break;
                    default: cout << "nieznany";
                }
                cout << " - " << static_cast<int>(aktualnaBron->pobierzSzansaNaEfekt() * 100) << "% szansy]";
            }
            cout << "\n";
        }
        cout << "Zloto: " << zloto << "\n";
        cout << "Mikstury: " << mikstury << "\n";
        cout << "================================\n";
    czekajNaKlawisz();
}
bool Postac::czyUnikBierny() {
    if (szansaNaUnikBierny + unikManualny > 0.8f) {
        szansaNaUnikBierny = 0.8f - unikManualny;
    }
    return static_cast<float>(rand()) / RAND_MAX < szansaNaUnikBierny;
}

bool Postac::czyUnikManualny() {
    return static_cast<float>(rand()) / RAND_MAX < unikManualny;
}

bool Postac::czyUcieczka() {
    return static_cast<float>(rand()) / RAND_MAX < szansaNaUcieczke;
}

int Postac::obliczObrazenia(int obrazeniaAtakujacego) {
    float redukcja = pancerz;
    int obrazenia = obrazeniaAtakujacego * (1.0f - redukcja);
    return max(1, obrazenia);
}

int Postac::obliczCalkowiteObrazenia() {
    int podstawoweObrazenia = obrazenia;
    if (aktualnaBron) {
        podstawoweObrazenia += aktualnaBron->pobierzBonusObrazen();
    }
    return podstawoweObrazenia;
}

// Zastąp obecną deklarację tablicy tym kodem:
vector<Bron> Postac::dostepneBronie = {
    Bron("zelazny miecz", 5),          // brak efektu
    Bron("zatrute sztylety", 8, 0.35f, EfektStatusu::ZATRUCIE)
};

Opancerzony::Opancerzony(Bron* bron) {
    nazwa = "Opancerzony";
    hp = 140;
    maxHp = 140;
    pancerz = 0.08f;
    obrazenia = 10;
    penetracjaPancerza = 0.08f;
    szansaNaUnikBierny = 0.15f;
    unikManualny = 0.50f;
    szansaNaUcieczke = 0.05f;
    aktualnaBron = &dostepneBronie[0];  // Przypisanie broni z tablicy statycznej
}

Bandyta::Bandyta(Bron* bron) {
    nazwa = "Bandyta";
    hp = 85;
    maxHp = 85;
    pancerz = 0.0f;
    obrazenia = 14;
    penetracjaPancerza = 0.15f;
    szansaNaUnikBierny = 0.35f;
    unikManualny = 0.75f;
    szansaNaUcieczke = 0.50f;
    aktualnaBron = &dostepneBronie[1];  // Przypisanie broni z tablicy statycznej
}