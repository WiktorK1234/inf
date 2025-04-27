/*
Struktura gry:
RPG/
├── include/
│   ├── Postac.h
│   ├── Przeciwnik.h
│   ├── EfektyStatusowe.h
│   ├── Bron.h
│   └── Pokoj.h
├── src/
│   ├── Postac.cpp
│   ├── Przeciwnik.cpp
│   ├── EfektyStatusowe.cpp
│   ├── Bron.cpp
│   ├── Pokoj.cpp
│   └── RPG.cpp
*/

#include "Postac.h"
#include "Przeciwnik.h"
#include "EfektyStatusowe.h"
#include "Bron.h"
#include "Pokoj.h"
#include "Utils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;

// Funkcja wyświetlająca menu wyboru postaci
Postac wybierzPostac() {
    clearScreen();
    cout << "Wybierz postac:\n";
    cout << "1. Opancerzony\n";
    cout << "   - HP: 140\n";
    cout << "   - Pancerz: 8%\n";
    cout << "   - Obrazenia: 10\n";
    cout << "   - Penetracja pancerza: 8%\n";
    cout << "   - Szansa na unik bierny: 15%\n";
    cout << "   - Unik manualny: 50%\n";
    cout << "   - Szansa na ucieczke: 5%\n";
    cout << "   - Bron startowa: zelazny miecz (+5 obrazen)\n";
    
    cout << "\n2. Bandyta\n";
    cout << "   - HP: 85\n";
    cout << "   - Pancerz: 0%\n";
    cout << "   - Obrazenia: 14\n";
    cout << "   - Penetracja pancerza: 15%\n";
    cout << "   - Szansa na unik bierny: 35%\n";
    cout << "   - Unik manualny: 75%\n";
    cout << "   - Szansa na ucieczke: 50%\n";
    cout << "   - Bron startowa: zatrute sztylety (+8 obrazen, +35%"<<" szansy na zatrucie)\n";
    
    cout << "\nWybor: ";
    int wybor;
    cin >> wybor;
    
    while (wybor < 1 || wybor > 2) {
        cout << "Nieprawidlowy wybor. Wybierz 1 lub 2: ";
        cin >> wybor;
    }
    
    if (wybor == 1) {
        return Opancerzony();  // Zwracamy obiekt klasy Opancerzony
    } else {
        return Bandyta();  // Zwracamy obiekt klasy Bandyta
    }
}

void wyswietlInterfejsWalki(const Postac& gracz, const vector<Przeciwnik*>& przeciwnicy) {
    clearScreen();
    cout << "================================\n";
    cout << "          WALKA\n";
    cout << "================================\n";
    cout << "\n=== " << gracz.nazwa << " (lvl " << gracz.poziom << ", HP: " << gracz.hp << "/" << gracz.maxHp << ") ===";
    if (gracz.czyPwalony()) {
        cout << " [POWALONY]";
    }
    cout << "\n";
    cout << "\nMikstury: " << gracz.mikstury << "\n";
    cout << "Przeciwnicy:\n";
    
    int licznik = 1;
    for (size_t i = 0; i < przeciwnicy.size(); ++i) {
        if (przeciwnicy[i] && przeciwnicy[i]->czyZyje()) {
            int roznica = przeciwnicy[i]->pobierzPoziom() - gracz.poziom;
            cout << (i+1) << ". " << przeciwnicy[i]->pobierzNazwe() 
                 << " (lvl " << przeciwnicy[i]->pobierzPoziom();
            
            if (roznica != 0) {
                cout << " [" << (roznica > 0 ? "+" : "") << roznica << "]";
            }
            
            cout << ", HP: " << przeciwnicy[i]->pobierzHp() 
                 << "/" << przeciwnicy[i]->pobierzMaxHp() << ")";
            
            if (przeciwnicy[i]->czyFotosyntezaAktywna()) {
                cout << " [FOTOSYNTEZA]";
            }
            
            cout << "\n";
        }
    }
}

void walka(Postac &gracz, vector<Przeciwnik*> &przeciwnicy, vector<Pokoj*>& wszystkiePokoje) {
    int expSuma = 0;
    int zlotoSuma = 0;
    bool ucieczka = false;

    map<Przeciwnik*, pair<string, int>> efektyStatusow;
    map<Przeciwnik*, pair<float, float>> bonusyUnikow; // <przeciwnik, <bonusUnikBierny, bonusUnikManualny>>
    vector<Przeciwnik*> stunowaniPrzeciwnicy;

    auto czyUnikBierny = [&](Przeciwnik* przeciwnik) {
        float bazowaSzansa = gracz.szansaNaUnikBierny;
        if (bonusyUnikow.count(przeciwnik)) {
            bazowaSzansa += bonusyUnikow[przeciwnik].first;
        }
        return (static_cast<float>(rand()) / RAND_MAX) < bazowaSzansa;
    };

    try {
        if (przeciwnicy.empty()) {
            cout << "Brak przeciwnikow do walki!\n";
            czekajNaKlawisz();
            return;
        }

        // Przeskaluj przeciwników do aktualnego poziomu gracza przed walką
        for (auto& przeciwnik : przeciwnicy) {
            if (przeciwnik) {
                przeciwnik->skalujDoPoziomu(gracz.poziom);
            }
        }

        while (gracz.hp > 0 && !przeciwnicy.empty() && !ucieczka) {
            // --- TURA GRACZA ---
            if (gracz.czyPwalony()) {
                cout << "Jestes powalony i tracisz te ture!\n";
                gracz.aktualizujPowalenie();
            }
            else {
                przeciwnicy.erase(
                    remove_if(przeciwnicy.begin(), przeciwnicy.end(),
                        [](Przeciwnik* p) {
                            if (p && p->pobierzHp() <= 0) {
                                delete p;
                                return true;
                            }
                            return false;
                        }),
                    przeciwnicy.end()
                );

                if (przeciwnicy.empty()) break;

                wyswietlInterfejsWalki(gracz, przeciwnicy);

                // Menu akcji
                int wybor = 0;
                bool akcjaWykonana = false;
                
                while (!akcjaWykonana) {
                    cout << "\nWybierz akcje:\n";
                    cout << "1. Atakuj\n";
                    cout << "2. Unik (szansa: " << gracz.unikManualny*100 << "%)\n";
                    cout << "3. Ucieczka (szansa: " << gracz.szansaNaUcieczke*100 << "%)\n";
                    cout << "4. Statystyki\n";
                    cout << "5. Uzyj mikstury (+" << static_cast<int>(gracz.maxHp * 0.2) << " HP)\n";
                    cout << "Wybor: ";
                    
                    if (!(cin >> wybor)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Nieprawidlowy wybor!\n";
                        continue;
                    }

                    switch (wybor) {
                        case 1: { // Atak
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Wybierz przeciwnika (1-" << przeciwnicy.size() << "): ";
                            int wyborPrzeciwnika;
                            if (!(cin >> wyborPrzeciwnika)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Nieprawidlowy wybor!\n";
                                break;
                            }
                            
                            if (wyborPrzeciwnika < 1 || wyborPrzeciwnika > static_cast<int>(przeciwnicy.size())) {
                                cout << "Nie ma takiego przeciwnika!\n";
                                break;
                            }
                            
                            Przeciwnik* cel = przeciwnicy[wyborPrzeciwnika-1];
                            if (cel->czyUnik()) {
                                cout << cel->pobierzNazwe() << " unika ataku!\n";
                            } else {
                                int obrazenia = gracz.obliczObrazenia(gracz.obliczCalkowiteObrazenia());
                                cel->otrzymajObrazenia(obrazenia);
                                cout << "Zadales " << obrazenia << " obrazen " << cel->pobierzNazwe() << "!\n";
                            }

                            // Sprawdzenie czy bron ma efekt statusu
                            if (gracz.aktualnaBron && !gracz.aktualnaBron->efekt.empty()) {
                                float szansa = static_cast<float>(rand()) / RAND_MAX;
                                if (szansa < gracz.aktualnaBron->szansaNaEfekt) {
                                    string efekt = gracz.aktualnaBron->efekt;
                                    
                                    if (efekt == "zatrucie") {
                                        zatrucie(cel);
                                        efektyStatusow[cel] = {"zatrucie", 4};
                                    } 
                                    else if (efekt == "zamrozenie") {
                                        zamrozenie(cel);
                                        efektyStatusow[cel] = {"zamrozenie", 1};
                                    }
                                    else if (efekt == "podpalenie") {
                                        podpalenie(cel);
                                        efektyStatusow[cel] = {"podpalenie", 3};
                                    }
                                    else if (efekt == "krwawienie") {
                                        krwawienie(cel);
                                        efektyStatusow[cel] = {"krwawienie", 2};
                                    }
                                    else if (efekt == "powalenie") {
                                        powalenie(cel);
                                        efektyStatusow[cel] = {"powalenie", 2};
                                        cel->zmniejszObrazenia(0.75f);
                                        cel->zmniejszPancerz(0.75f);
                                    }
                                    else if (efekt == "ogluszenie") {
                                        ogluszenie(cel);
                                        efektyStatusow[cel] = {"ogluszenie", 1};
                                    }
                                    else if (efekt == "kradziez") {
                                        kradziez(cel);
                                        int skradzioneZloto = rand() % (cel->pobierzPoziom() * 5) + 5;
                                        gracz.zloto += skradzioneZloto;
                                        cout << "Ukradles " << skradzioneZloto << " zlota!\n";
                                    }
                                    else if (efekt == "oslepienie") {
                                        oslepienie(cel);
                                        efektyStatusow[cel] = {"oslepienie", 3};
                                        float bonusUniku = 0.15f;
                                        bonusyUnikow[cel] = {bonusUniku, bonusUniku};
                                        cout << "Twoja szansa na unik przeciwko " << cel->pobierzNazwe() << " wzrosla o " << (bonusUniku*100) << "%!\n";
                                    }
                                    else if (efekt == "strach") {
                                        strach(cel);
                                        efektyStatusow[cel] = {"strach", 1};
                                    }
                                    else if (efekt == "porazenie") {
                                        porazenie(cel);
                                        efektyStatusow[cel] = {"porazenie", 2};
                                        cel->zmniejszObrazenia(0.75f);
                                        cel->zmniejszPancerz(0.75f);
                                    }
                                    else if (efekt == "chaos") {
                                        chaos(cel);
                                        vector<string> efekty = {
                                            "zatrucie", "zamrozenie", "podpalenie", "krwawienie",
                                            "powalenie", "ogluszenie", "oslepienie", "strach", "porazenie", "kradziez"
                                        };
                                        string losowyEfekt = efekty[rand() % efekty.size()];
                                        
                                        if (losowyEfekt == "zatrucie") {
                                            zatrucie(cel);
                                            efektyStatusow[cel] = {"zatrucie", 4};
                                        }
                                        else if (losowyEfekt == "zamrozenie") {
                                            zamrozenie(cel);
                                            efektyStatusow[cel] = {"zamrozenie", 1};
                                        }
                                        else if (losowyEfekt == "podpalenie") {
                                            podpalenie(cel);
                                            efektyStatusow[cel] = {"podpalenie", 3};
                                        }
                                        else if (losowyEfekt == "krwawienie") {
                                            krwawienie(cel);
                                            efektyStatusow[cel] = {"krwawienie", 2};
                                        }
                                        else if (losowyEfekt == "powalenie") {
                                            powalenie(cel);
                                            efektyStatusow[cel] = {"powalenie", 2};
                                            cel->zmniejszObrazenia(0.75f);
                                            cel->zmniejszPancerz(0.75f);
                                        }
                                        else if (losowyEfekt == "ogluszenie") {
                                            ogluszenie(cel);
                                            efektyStatusow[cel] = {"ogluszenie", 1};
                                        }
                                        else if (losowyEfekt == "oslepienie") {
                                            oslepienie(cel);
                                            efektyStatusow[cel] = {"oslepienie", 3};
                                            float bonusUniku = 0.2f;
                                            gracz.szansaNaUnikBierny += bonusUniku;
                                            gracz.unikManualny += bonusUniku;
                                            cout << "Twoja szansa na unik wzrosla o " << (bonusUniku*100) << "%!\n";
                                        }
                                        else if (losowyEfekt == "strach") {
                                            strach(cel);
                                            efektyStatusow[cel] = {"strach", 1};
                                        }
                                        else if (losowyEfekt == "porazenie") {
                                            porazenie(cel);
                                            efektyStatusow[cel] = {"porazenie", 2};
                                            cel->zmniejszObrazenia(0.75f);
                                            cel->zmniejszPancerz(0.75f);
                                        }
                                        else if (losowyEfekt == "kradziez") {
                                            kradziez(cel);
                                            int skradzioneZloto = rand() % (cel->pobierzPoziom() * 5) + 5;
                                            gracz.zloto += skradzioneZloto;
                                            cout << "Ukradles " << skradzioneZloto << " zlota!\n";
                                        }
                                    }
                                }
                            }
                            
                            if (cel->pobierzHp() <= 0) {
                                cout << cel->pobierzNazwe() << " pokonany!\n";
                                efektyStatusow.erase(cel);
                                
                                int losowyBonusExp = rand() % 11 + 5;
                                int bazaExp = cel->pobierzPoziom() * (cel->pobierzObrazenia() * 2 + cel->pobierzMaxHp() / 3);
                                expSuma += bazaExp + (bazaExp * losowyBonusExp / 100);
                                
                                int losowyBonusZloto = rand() % 11 + 5;
                                int bazaZloto = cel->pobierzPoziom() * (cel->pobierzObrazenia() + cel->pobierzMaxHp() / 2);
                                zlotoSuma += bazaZloto + (bazaZloto * losowyBonusZloto / 100);
                                
                                if (cel->pobierzNazwe() == "Straznik korzeni") {
                                    expSuma += 100;
                                    zlotoSuma += 50;
                                }
                            }
                            akcjaWykonana = true;
                            czekajNaKlawisz();
                            break;
                        }
                        case 2: { // Unik
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            if (gracz.czyUnikManualny()) {
                                cout << "Przygotowales sie do uniku w nastepnej turze!\n";
                            } else {
                                cout << "Nie udalo sie przygotowac do uniku!\n";
                            }
                            akcjaWykonana = true;
                            czekajNaKlawisz();
                            break;
                        }
                        case 3: { // Ucieczka
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            if (gracz.czyUcieczka()) {
                                cout << "Uciekles z walki!\n";
                                ucieczka = true;
                            } else {
                                cout << "Nie udalo sie uciec!\n";
                            }
                            akcjaWykonana = true;
                            czekajNaKlawisz();
                            break;
                        }
                        case 4: { // Statystyki
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            clearScreen();
                            gracz.wyswietlStatystyki(false);
                            
                            cout << "\n=== STATYSTYKI PRZECIWNIKOW ===\n";
                            for (auto p : przeciwnicy) {
                                if (p) {
                                    cout << "Przeciwnik: " << p->pobierzNazwe() << "\n";
                                    cout << "HP: " << p->pobierzHp() << "/" << p->pobierzMaxHp() << "\n";
                                    cout << "Pancerz: " << static_cast<int>(p->pobierzPancerz() * 100) << "%\n";
                                    cout << "Obrazenia: " << p->pobierzObrazenia() << "\n\n";
                                }
                            }
                            
                            czekajNaKlawisz();
                            wyswietlInterfejsWalki(gracz, przeciwnicy);
                            break;
                        }
                        case 5: { // Mikstura
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            if (gracz.mikstury > 0) {
                                int leczenie = gracz.maxHp * 0.3;
                                int noweHP = gracz.hp + leczenie;
                                
                                if (noweHP > gracz.maxHp) {
                                    leczenie = gracz.maxHp - gracz.hp;
                                    noweHP = gracz.maxHp;
                                }
                                
                                gracz.hp = noweHP;
                                gracz.mikstury--;
                                
                                cout << "Uzyles mikstury! (+" << leczenie << " HP)\n";
                                cout << "Aktualne HP: " << gracz.hp << "/" << gracz.maxHp << "\n";
                                cout << "Pozostalo mikstur: " << gracz.mikstury << "\n";
                            } else {
                                cout << "Nie masz mikstur!\n";
                            }
                            akcjaWykonana = true;
                            czekajNaKlawisz();
                            break;
                        }
                        default: {
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Nieprawidlowy wybor!\n";
                            break;
                        }
                    }
                }

                if (gracz.hp <= 0 || przeciwnicy.empty() || ucieczka) break;

                // --- TURA PRZECIWNIKA ---
                auto czyUnikManualny = [&](Przeciwnik* przeciwnik) {
                    float bazowaSzansa = gracz.unikManualny;
                    if (bonusyUnikow.count(przeciwnik)) {
                        bazowaSzansa += bonusyUnikow[przeciwnik].second;
                    }
                    return (static_cast<float>(rand()) / RAND_MAX) < bazowaSzansa;
                };

                // --- OBSŁUGA EFEKTÓW STATUSÓW PRZED TURĄ PRZECIWNIKA ---
                stunowaniPrzeciwnicy.clear();

                for (auto it = efektyStatusow.begin(); it != efektyStatusow.end(); ) {
                    auto& [przeciwnik, efekt] = *it;
                    
                    if (find(przeciwnicy.begin(), przeciwnicy.end(), przeciwnik) == przeciwnicy.end()) {
                        if (efekt.first == "oslepienie") {
                            bonusyUnikow.erase(przeciwnik);
                        }
                        it = efektyStatusow.erase(it);
                        continue;
                    }
                    
                    string typEfektu = efekt.first;
                    int& pozostaleTury = efekt.second;
                    
                    if (typEfektu == "zatrucie") {
                        przeciwnik->ustawHp(przeciwnik->pobierzHp() - 3);
                        cout << przeciwnik->pobierzNazwe() << " otrzymal 3 obrazenia od zatrucia! (pozostalo tur: " << (pozostaleTury-1) << ")\n";
                    }
                    else if (typEfektu == "podpalenie") {
                        przeciwnik->ustawHp(przeciwnik->pobierzHp() - 4);
                        cout << przeciwnik->pobierzNazwe() << " otrzymal 4 obrazenia od podpalenia! (pozostalo tur: " << (pozostaleTury-1) << ")\n";
                    }
                    else if (typEfektu == "krwawienie") {
                        przeciwnik->ustawHp(przeciwnik->pobierzHp() - 6);
                        cout << przeciwnik->pobierzNazwe() << " otrzymal 6 obrazen od krwawienia! (pozostalo tur: " << (pozostaleTury-1) << ")\n";
                    }    
                    else if (typEfektu == "zamrozenie" || typEfektu == "ogluszenie" || typEfektu == "strach") {
                        stunowaniPrzeciwnicy.push_back(przeciwnik);
                        cout << przeciwnik->pobierzNazwe() << " jest " << typEfektu << " i traci ture! (pozostalo tur: " << (pozostaleTury-1) << ")\n";
                    }
                    else if (typEfektu == "oslepienie") {
                        cout << przeciwnik->pobierzNazwe() << " jest oslepiony! (pozostalo tur: " << (pozostaleTury-1) << ")\n";
                    }
                    
                    pozostaleTury--;
                    
                    if (pozostaleTury <= 0) {
                        if (typEfektu == "powalenie" || typEfektu == "porazenie") {
                            przeciwnik->ustawObrazenia(przeciwnik->pobierzObrazenia() / 0.75f);
                            przeciwnik->ustawPancerz(przeciwnik->pobierzPancerz() / 0.75f);
                            cout << przeciwnik->pobierzNazwe() << " wraca do normalnego stanu po " << typEfektu << "!\n";
                        }
                        else if (typEfektu == "oslepienie") {
                            bonusyUnikow.erase(przeciwnik);
                            cout << przeciwnik->pobierzNazwe() << " odzyskuje wzrok!\n";
                            cout << "Twoja szansa na unik przeciwko " << przeciwnik->pobierzNazwe() << " wraca do normy!\n";
                        }
                        
                        it = efektyStatusow.erase(it);
                        continue;
                    }
                    
                    it++;
                }

                for (size_t i = 0; i < przeciwnicy.size(); ++i) {
                    if (przeciwnicy[i] && przeciwnicy[i]->pobierzHp() > 0) {
                        if (find(stunowaniPrzeciwnicy.begin(), stunowaniPrzeciwnicy.end(), przeciwnicy[i]) != stunowaniPrzeciwnicy.end()) {
                            continue;
                        }

                        if (przeciwnicy[i]->czyFotosyntezaAktywna()) {
                            przeciwnicy[i]->resetujFotosynteze();
                            continue;
                        }

                        if (czyUnikBierny(przeciwnicy[i])) {
                            cout << "Unikles ataku " << przeciwnicy[i]->pobierzNazwe() << "!\n";
                        } else {
                            if (przeciwnicy[i]->pobierzNazwe() == "Straznik korzeni" && rand() % 100 < 40) {
                                int obrazenia = przeciwnicy[i]->obliczObrazenia(przeciwnicy[i]->pobierzObrazenia() * 0.75f);
                                gracz.hp -= obrazenia;
                                gracz.ustawPowalenie();
                                cout << przeciwnicy[i]->pobierzNazwe() << " atakuje gwaltownymi korzeniami!\n";
                                cout << "Zadaje " << obrazenia << " obrazen i powala cie!\n";
                            } 
                            else {
                                int obrazenia = przeciwnicy[i]->obliczObrazenia(przeciwnicy[i]->pobierzObrazenia());
                                gracz.hp -= obrazenia;
                                cout << przeciwnicy[i]->pobierzNazwe() << " zadal ci " << obrazenia << " obrazen!\n";
                            }

                            if (gracz.hp <= 0) {
                                cout << "Zostales pokonany!\n";
                                break;
                            }
                        }
                    }
                }
                
                przeciwnicy.erase(
                    remove_if(przeciwnicy.begin(), przeciwnicy.end(),
                        [](Przeciwnik* p) {
                            if (p && p->pobierzHp() <= 0) {
                                delete p;
                                return true;
                            }
                            return false;
                        }),
                    przeciwnicy.end()
                );
                
                czekajNaKlawisz();
            }
        }

        clearScreen();
        if (gracz.hp > 0 && !ucieczka) {
            cout << "\n=== ZWYCIESTWO ===\n";
            gracz.zdobadzDoswiadczenie(expSuma, wszystkiePokoje);
            gracz.zloto += zlotoSuma;
            cout << "Zdobyto " << expSuma << " EXP i " << zlotoSuma << " zlota!\n";
        } else if (ucieczka) {
            cout << "\n=== UCIECZKA ===\n";
        } else {
            cout << "\n=== PORAZKA ===\n";
        }
        gracz.wyswietlStatystyki(false);
        czekajNaKlawisz();
    }
    catch (const exception& e) {
        cerr << "Krytyczny blad walki: " << e.what() << endl;
        for (auto p : przeciwnicy) {
            if (p != nullptr) {
                delete p;
            }
        }
        przeciwnicy.clear();
    }
}

// Funkcja do losowania jednego przeciwnika
vector<Przeciwnik*> losujPrzeciwnikow(int poziomGracza, bool czyCzaszka1 = false) {
    vector<Przeciwnik*> przeciwnicy;
    int liczbaPrzeciwnikow = 2 + rand() % 2; // 2-3 przeciwników

    for (int i = 0; i < liczbaPrzeciwnikow; ++i) {
        int typ;
        // Jeśli to nie jest pokój Czaszka1, wyklucz Strażnika Korzeni (typ 6)
        if (czyCzaszka1) {
            typ = rand() % 7; // Wszystkie typy włącznie ze Strażnikiem
        } else {
            typ = rand() % 6; // Tylko typy 0-5 (bez Strażnika)
        }

        // Tworzenie nowego przeciwnika
        Przeciwnik* p = stworzPrzeciwnika(typ, poziomGracza);
        if (p) {
            przeciwnicy.push_back(p);
        }
    }

    // Skalowanie przeciwników do poziomu gracza
    for (auto p : przeciwnicy) {
        if (p) {
            p->skalujDoPoziomu(poziomGracza);
        }
    }

    // Zwracamy wektor przeciwników
    return przeciwnicy;
}


Pokoj* aktualnyPokoj = nullptr;

void rozszerzMape(Pokoj* skrzyzowanie2, vector<Pokoj*>& wszystkiePokoje, int poziomGracza) {
    try {
        Pokoj* fontanna = new Pokoj("Fontanna Uzdrawiajaca");
        Pokoj* komnata1 = new Pokoj("Komnata 1");
        Pokoj* komnata2 = new Pokoj("Komnata 2");
        Pokoj* czaszka1 = new Pokoj("Czaszka 1");
        Pokoj* komnata3 = new Pokoj("Komnata 3");
        Pokoj* boss = new Pokoj("Komnata Bossowa");

        // Łączenie pokoi
        fontanna->dodajPolaczenie("powrot", skrzyzowanie2);
        skrzyzowanie2->dodajPolaczenie("tyl", fontanna);
        skrzyzowanie2->dodajPolaczenie("lewo", komnata1);
        skrzyzowanie2->dodajPolaczenie("prawo", komnata2);
        
        komnata1->dodajPolaczenie("powrot", skrzyzowanie2);
        komnata1->dodajPolaczenie("przod", czaszka1);
        
        czaszka1->dodajPolaczenie("powrot", komnata1);
        
        komnata2->dodajPolaczenie("powrot", skrzyzowanie2);
        komnata2->dodajPolaczenie("przod", komnata3);
        
        komnata3->dodajPolaczenie("powrot", komnata2);
        komnata3->dodajPolaczenie("przod", boss);
        
        boss->dodajPolaczenie("powrot", komnata3);

        // Funkcja specjalna fontanny
        fontanna->specjalnaAkcja = [](Postac& gracz) {
            clearScreen();
            cout << "================================\n";
            cout << "   FONTANNA UZDRAWIAJACA\n";
            cout << "================================\n\n";
            cout << "Twoje obecne HP: " << gracz.hp << "/" << gracz.maxHp << "\n\n";
            
            if (gracz.hp == gracz.maxHp) {
                cout << "Jestes juz w pelni wyleczony!\n";
                czekajNaKlawisz();
                return false;
            }
            
            cout << "Czy chcesz sie uleczyc? (T/N): ";
            char wybor;
            cin >> wybor;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (tolower(wybor) == 't') {
                gracz.hp = gracz.maxHp;
                clearScreen();
                cout << "ZOSTALES W PELNI UZDROWIONY!\n";
                cout << "Twoje HP: " << gracz.maxHp << "/" << gracz.maxHp << "\n";
                czekajNaKlawisz();
                return true;
            }
            return false;
        };

        // Dodawanie przeciwników
        auto dodajPrzeciwnikow = [poziomGracza](Pokoj* pokoj, bool miniboss = false) {
            if (miniboss) {
                pokoj->przeciwnicy = {new StraznikKorzeni()};
                pokoj->przeciwnicy[0]->ustawPoziom(poziomGracza + 2);
                pokoj->przeciwnicy[0]->skalujDoPoziomu(poziomGracza);
            } else {
                pokoj->przeciwnicy = losujPrzeciwnikow(poziomGracza, false);
            }
        };

        dodajPrzeciwnikow(komnata1);
        dodajPrzeciwnikow(komnata2);
        dodajPrzeciwnikow(komnata3);
        dodajPrzeciwnikow(czaszka1, true); // Miniboss
        boss->przeciwnicy.clear(); // Początkowo pusty

        wszystkiePokoje.insert(wszystkiePokoje.end(), {fontanna, komnata1, komnata2, czaszka1, komnata3, boss});
    }
    catch (const exception& e) {
        cerr << "Blad podczas rozszerzania mapy: " << e.what() << endl;
    }
}

void rozszerzMapeDalej(Pokoj* skrzyzowanie1, vector<Pokoj*>& wszystkiePokoje, int poziomGracza) {
    try {
        // Tworzenie pokoi
        Pokoj* komnata4 = new Pokoj("Komnata 4");
        Pokoj* komnata5 = new Pokoj("Komnata 5");
        Pokoj* komnata6 = new Pokoj("Komnata 6");
        Pokoj* sklep = new Pokoj("Sklep");
        Pokoj* komnata7 = new Pokoj("Komnata 7");
        Pokoj* czaszka2 = new Pokoj("Czaszka 2");

        // Łączenie pokoi
        skrzyzowanie1->dodajPolaczenie("lewo", komnata4);
        skrzyzowanie1->dodajPolaczenie("przod", komnata5);
        skrzyzowanie1->dodajPolaczenie("prawo", komnata6);
        
        komnata4->dodajPolaczenie("powrot", skrzyzowanie1);
        komnata5->dodajPolaczenie("powrot", skrzyzowanie1);
        komnata6->dodajPolaczenie("powrot", skrzyzowanie1);
        
        komnata6->dodajPolaczenie("przod", sklep);
        sklep->dodajPolaczenie("powrot", komnata6);
        sklep->dodajPolaczenie("prawo", komnata7);
        
        komnata7->dodajPolaczenie("powrot", sklep);
        komnata7->dodajPolaczenie("przod", czaszka2);
        czaszka2->dodajPolaczenie("powrot", komnata7);

        // Dodawanie przeciwników
        auto dodajPrzeciwnikow = [poziomGracza](Pokoj* pokoj, bool miniboss = false) {
            if (miniboss) {
                pokoj->przeciwnicy = {new StraznikKorzeni()};
                pokoj->przeciwnicy[0]->ustawPoziom(poziomGracza + 2);
                pokoj->przeciwnicy[0]->skalujDoPoziomu(poziomGracza);
            } else {
                pokoj->przeciwnicy = losujPrzeciwnikow(poziomGracza, false);
            }
        };

        dodajPrzeciwnikow(komnata4);
        dodajPrzeciwnikow(komnata5);
        dodajPrzeciwnikow(komnata6);
        dodajPrzeciwnikow(komnata7);
        dodajPrzeciwnikow(czaszka2, true); // Miniboss

        // Dodanie do listy pokoi
        wszystkiePokoje.insert(wszystkiePokoje.end(), {komnata4, komnata5, komnata6, sklep, komnata7, czaszka2});

    } catch (const exception& e) {
        cerr << "Blad podczas rozszerzania mapy: " << e.what() << endl;
    }
}

void inicjalizujMape(Pokoj*& startowy, vector<Pokoj*>& wszystkiePokoje, int poziomGracza = 1) {
    // Podstawowe pokoje
    startowy = new Pokoj("Start");
    Pokoj* lewy = new Pokoj("Lewy");
    Pokoj* prawy = new Pokoj("Prawy");
    Pokoj* przod = new Pokoj("Przod");
    Pokoj* relikwie = new Pokoj("Relikwie");
    Pokoj* skrzyzowanie1 = new Pokoj("Skrzyzowanie1");
    Pokoj* skrzyzowanie2 = new Pokoj("Skrzyzowanie2");
    
    // Łączenie pokoi
    startowy->dodajPolaczenie("lewo", lewy);
    startowy->dodajPolaczenie("przod", przod);
    startowy->dodajPolaczenie("prawo", prawy);
    
    lewy->dodajPolaczenie("powrot", startowy);
    lewy->dodajPolaczenie("przod", relikwie);
    lewy->dodajPolaczenie("prawo", skrzyzowanie1);
    
    prawy->dodajPolaczenie("powrot", startowy);
    prawy->dodajPolaczenie("przod", skrzyzowanie2);
    
    przod->dodajPolaczenie("powrot", startowy);
    przod->dodajPolaczenie("przod", skrzyzowanie1);
    
    relikwie->dodajPolaczenie("powrot", lewy);
    
    skrzyzowanie1->dodajPolaczenie("powrot", lewy);
    skrzyzowanie1->dodajPolaczenie("lewo", przod);
    
    skrzyzowanie2->dodajPolaczenie("powrot", prawy);

    // Przeciwnicy
    lewy->przeciwnicy = losujPrzeciwnikow(poziomGracza,false);
    prawy->przeciwnicy = losujPrzeciwnikow(poziomGracza,false);
    przod->przeciwnicy = losujPrzeciwnikow(poziomGracza,false);
    skrzyzowanie1->przeciwnicy = losujPrzeciwnikow(poziomGracza,false);
    skrzyzowanie2->przeciwnicy = losujPrzeciwnikow(poziomGracza,false);

    // Inicjalizacja
    wszystkiePokoje = {startowy, lewy, prawy, przod, relikwie, skrzyzowanie1, skrzyzowanie2};
    
    // Rozszerzenia
    rozszerzMape(skrzyzowanie2, wszystkiePokoje, poziomGracza);
    rozszerzMapeDalej(skrzyzowanie1, wszystkiePokoje, poziomGracza);
}

void rozpocznijGre() {
    srand(time(nullptr));
    Postac gracz = wybierzPostac();
    bool graTrwa = true;

    // Inicjalizacja mapy
    Pokoj* startowy = nullptr;
    vector<Pokoj*> wszystkiePokoje;
    inicjalizujMape(startowy, wszystkiePokoje, gracz.poziom);
    aktualnyPokoj = startowy;

    while (graTrwa && gracz.hp > 0) {
        clearScreen();
        
        // Nagłówek
        cout << "================================\n";
        cout << "  " << aktualnyPokoj->nazwa << "\n";
        cout << "================================\n";
        cout << "HP: " << gracz.hp << "/" << gracz.maxHp << " | ";
        cout << "Poziom: " << gracz.poziom << " | ";
        cout << "Zloto: " << gracz.zloto << " | ";
        cout << "Mikstury: " << gracz.mikstury << "\n\n";

        // Sprawdź czy pokój ma przeciwników
        if (!aktualnyPokoj->pokonany && aktualnyPokoj->przeciwnicy.empty()) {
            aktualnyPokoj->przeciwnicy = losujPrzeciwnikow(gracz.poziom, false);
        }

        // Specjalna akcja pokoju
        if (aktualnyPokoj->specjalnaAkcja && !aktualnyPokoj->odwiedzony) {
            if (aktualnyPokoj->specjalnaAkcja(gracz)) {
                aktualnyPokoj->odwiedzony = true;
            }
        }

        // Menu nawigacji
        bool wybranoAkcje = false;
        while (!wybranoAkcje && graTrwa) {
            cout << "Dostepne akcje:\n";
            int opcja = 1;
            map<int, string> kierunki;

            // Opcje nawigacji
            for (const auto& polaczenie : aktualnyPokoj->polaczenia) {
                cout << opcja << ". Idz na " << polaczenie.first << " (" << polaczenie.second->nazwa << ")\n";
                kierunki[opcja] = polaczenie.first;
                opcja++;
            }

            // Inne opcje
            cout << opcja++ << ". Statystyki postaci\n";
            if (gracz.mikstury > 0) {
                cout << opcja++ << ". Uzyj mikstury (+" << static_cast<int>(gracz.maxHp * 0.2) << " HP)\n";
            }
            cout << opcja << ". Wyjdz z gry\n";

            // Wybór gracza
            cout << "\nWybor: ";
            int wybor;
            cin >> wybor;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nieprawidlowy wybor!\n";
                czekajNaKlawisz();
                clearScreen();
                continue;
            }

            // Obsługa wyboru
            if (wybor >= 1 && wybor <= kierunki.size()) {
                // Nawigacja
                string wybranyKierunek = kierunki[wybor];
                aktualnyPokoj = aktualnyPokoj->polaczenia.at(wybranyKierunek);
                wybranoAkcje = true;

                // Walka tylko jeśli są przeciwnicy i pokój niepokonany
                if (!aktualnyPokoj->pokonany && !aktualnyPokoj->przeciwnicy.empty()) {
                    walka(gracz, aktualnyPokoj->przeciwnicy, wszystkiePokoje);
                    if (gracz.hp <= 0) {
                        graTrwa = false;
                    } else if (aktualnyPokoj->przeciwnicy.empty()) {
                        aktualnyPokoj->pokonany = true;
                        int zdobyteZloto = 20 + rand() % 30;
                        gracz.zloto += zdobyteZloto;
                        cout << "\nZdobyto " << zdobyteZloto << " zlota!\n";
                        czekajNaKlawisz();
                    }
                }
            }
            else if (wybor == kierunki.size() + 1) {
                // Statystyki
                clearScreen();
                gracz.wyswietlStatystyki();
                czekajNaKlawisz();
                clearScreen();
            }
            else if (wybor == kierunki.size() + 2 && gracz.mikstury > 0) {
                // Mikstura
                gracz.uzyjMikstury();
                czekajNaKlawisz();
                wybranoAkcje = true;
            }
            else if (wybor == kierunki.size() + (gracz.mikstury > 0 ? 3 : 2)) {
                // Wyjście z gry
                cout << "\nCzy na pewno chcesz wyjsc? (T/N): ";
                char potwierdzenie;
                cin >> potwierdzenie;
                if (tolower(potwierdzenie) == 't') {
                    graTrwa = false;
                    wybranoAkcje = true;
                }
                clearScreen();
            }
            else {
                cout << "Nieprawidlowy wybor!\n";
                czekajNaKlawisz();
                clearScreen();
            }
        }
    }

    // Zakończenie gry
    clearScreen();
    if (gracz.hp <= 0) {
        cout << "================================\n";
        cout << "         GAME OVER\n";
        cout << "================================\n\n";
        cout << "Twoja postac zginala...\n";
        cout << "Osiagniety poziom: " << gracz.poziom << "\n";
    } else {
        cout << "================================\n";
        cout << "        KONIEC GRY\n";
        cout << "================================\n\n";
        cout << "Twoje osiagniecia:\n";
        cout << "- Poziom: " << gracz.poziom << "\n";
        cout << "- Zloto: " << gracz.zloto << "\n";
        cout << "- Pokonane lokacje: " << count_if(wszystkiePokoje.begin(), wszystkiePokoje.end(), 
            [](Pokoj* p) { return p->pokonany; }) << "/" << wszystkiePokoje.size() << "\n";
    }
    czekajNaKlawisz();

    // Sprzątanie
    for (auto pokoj : wszystkiePokoje) {
        delete pokoj;
    }
}

int main() {
    // Inicjalizacja generatora losowego
    srand(static_cast<unsigned>(time(nullptr)));

    // Inicjalizacja mapy
    vector<Pokoj*> wszystkiePokoje;
    Pokoj* startowy = nullptr;
    inicjalizujMape(startowy, wszystkiePokoje);
    
    // Sprawdzenie poprawności inicjalizacji
    if (!startowy) {
        cerr << "Błąd inicjalizacji mapy!" << endl;
        return 1;
    }

    // Rozpoczęcie gry
    rozpocznijGre();
    
    // Sprzątanie
    for (auto pokoj : wszystkiePokoje) {
        delete pokoj;
    }
    return 0;
}
