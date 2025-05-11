/*
Struktura gry:
RPG/
├── include/
|   ├── Utils.h
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
#include <memory>
#include <stdexcept>
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

// Funkcja konwertująca enum EfektStatusu na string
string efektToString(EfektStatusu efekt) {
    switch(efekt) {
        case EfektStatusu::ZATRUCIE: return "zatrucie";
        case EfektStatusu::PODPALENIE: return "podpalenie";
        case EfektStatusu::ZAMROZENIE: return "zamrozenie";
        case EfektStatusu::KRWAWIENIE: return "krwawienie";
        case EfektStatusu::POWALENIE: return "powalenie";
        case EfektStatusu::OGLUSZENIE: return "ogluszenie";
        case EfektStatusu::OSLEPIENIE: return "oslepienie";
        case EfektStatusu::SMIERC: return "smierc";
        case EfektStatusu::STRACH: return "strach";
        case EfektStatusu::PORAZENIE: return "porazenie";
        case EfektStatusu::KRADZIEZ: return "kradziez";
        case EfektStatusu::CHAOS: return "chaos";
        default: return "nieznany";
    }
}

// Funkcja wyświetlająca menu wyboru postaci
unique_ptr<Postac> wybierzPostac() {
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
        return make_unique<Opancerzony>(nullptr);  // lub nowa Bron(...)
    } else {
        return make_unique<Bandyta>(nullptr);     // lub nowa Bron(...)
    }
}

void wyswietlInterfejsWalki(const Postac& gracz, const vector<unique_ptr<Przeciwnik>>& przeciwnicy) {
    clearScreen();
    cout << "================================\n";
    cout << "          WALKA\n";
    cout << "================================\n";
    cout << "\n=== " << gracz.pobierzNazwe() << " (lvl " << gracz.poziom << ", HP: " << gracz.hp << "/" << gracz.maxHp << ") ===";
    
    // Wyświetlanie efektów statusowych gracza
    if (gracz.czyPowalony()) {
        cout << " [POWALONY]";
    }
    if (gracz.maEfekt("spowolnienie")) {
        cout << " [SPOWOLNIONY]";
    }
    if (gracz.maEfekt("zatrucie")) {
        cout << " [ZATRUTY]";
    }
    if (gracz.maEfekt("krwawienie")) {
        cout << " [KRWAWI]";
    }
    if (gracz.maEfekt("ogluszenie")) {
        cout << " [OGŁUSZONY]";
    }
    
    cout << "\n";
    cout << "\nMikstury: " << gracz.mikstury << "\n";
    cout << "Przeciwnicy:\n";
    
    for (size_t i = 0; i < przeciwnicy.size(); ++i) {
        if (przeciwnicy[i] && przeciwnicy[i]->czyZyje()) {
            cout << (i+1) << ". " << przeciwnicy[i]->pobierzNazwe() 
                 << " (lvl " << przeciwnicy[i]->pobierzPoziom() << ", HP: "
                 << przeciwnicy[i]->pobierzHp() << "/" << przeciwnicy[i]->pobierzMaxHp() << ")";
            
            if (przeciwnicy[i]->czyPowalony()) {
                cout << " [POWALONY]";
            }
            if (przeciwnicy[i]->maEfekt("spowolnienie")) {
                cout << " [SPOWOLNIONY]";
            }
            if (przeciwnicy[i]->maEfekt("zatrucie")) {
                cout << " [ZATRUTY]";
            }
            if (przeciwnicy[i]->maEfekt("podpalenie")) {
                cout << " [PODPALONY]";
            }
            if (przeciwnicy[i]->maEfekt("zamrozenie")) {
                cout << " [ZAMROŻONY]";
            }
            if (przeciwnicy[i]->maEfekt("oslepienie")) {
                cout << " [OŚLEPIONY]";
            }
            
            if (przeciwnicy[i]->czyFotosyntezaAktywna()) {
                cout << " [FOTOSYNTEZA]";
            }
            
            cout << "\n";
        }
    }
}

void walka(Postac& gracz, vector<unique_ptr<Przeciwnik>>& przeciwnicy, vector<unique_ptr<Pokoj>>& wszystkiePokoje) {
    // Inicjalizacja walki
    int expSuma = 0;
    int zlotoSuma = 0;
    bool ucieczka = false;
    int poziomPrzedWalka = gracz.poziom;

    // Struktury do śledzenia efektów statusowych
    map<Przeciwnik*, pair<string, int>> efektyStatusow; // <przeciwnik, <efekt, tury>>
    map<Przeciwnik*, pair<float, float>> bonusyUnikow;  // <przeciwnik, <bonusUnikBierny, bonusUnikManualny>>

    // Obliczanie nagród za przeciwników
    for (auto& p : przeciwnicy) {
        if (p) {
            expSuma += 20 * p->pobierzPoziom();
            zlotoSuma += 10 * p->pobierzPoziom();
        }
    }

    // Główna pętla walki
    while (gracz.hp > 0 && !przeciwnicy.empty() && !ucieczka) {
        // --- TURA GRACZA ---
        wyswietlInterfejsWalki(gracz, przeciwnicy);

        if (gracz.czyPowalony()) {
            cout << "Jestes powalony i tracisz te ture!\n";
            gracz.aktualizujPowalenie();
            czekajNaKlawisz();
        } else {
            // Menu akcji w walce
            int wybor = 0;
            bool akcjaWykonana = false;
            
            while (!akcjaWykonana) {
                cout << "\nWybierz akcje:\n";
                cout << "1. Atakuj\n";
                cout << "2. Unik (szansa: " << gracz.unikManualny * 100 << "%)\n";
                cout << "3. Ucieczka (szansa: " << gracz.szansaNaUcieczke * 100 << "%)\n";
                cout << "4. Statystyki\n";
                if (gracz.mikstury > 0) {
                    cout << "5. Uzyj mikstury (+" << static_cast<int>(gracz.maxHp * 0.2) << " HP)\n";
                }
                cout << "Wybor: ";
                
                if (!(cin >> wybor)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Nieprawidlowy wybor!\n";
                    continue;
                }

                switch (wybor) {
                    case 1: { // Atak
                        cout << "Wybierz przeciwnika (1-" << przeciwnicy.size() << "): ";
                        int wyborPrzeciwnika;
                        if (!(cin >> wyborPrzeciwnika) || wyborPrzeciwnika < 1 || wyborPrzeciwnika > przeciwnicy.size()) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Nieprawidlowy wybor przeciwnika!\n";
                            break;
                        }

                        auto& przeciwnik = przeciwnicy[wyborPrzeciwnika - 1];
                        if (przeciwnik->czyUnik()) {
                            cout << przeciwnik->pobierzNazwe() << " unika ataku!\n";
                        } else {
                            int obrazenia = gracz.obliczCalkowiteObrazenia();
                            int obrazeniaZadane = przeciwnik->obliczObrazenia(obrazenia);
                            przeciwnik->otrzymajObrazenia(obrazeniaZadane);
                            
                            cout << "Zadales " << obrazeniaZadane << " obrazen " 
                                 << przeciwnik->pobierzNazwe() << "!\n";
                            
                            // Sprawdzenie efektów broni
                            if (gracz.aktualnaBron && gracz.aktualnaBron->pobierzEfekt() != EfektStatusu::BRAK) {
                                float szansaNaEfekt = gracz.aktualnaBron->pobierzSzansaNaEfekt();
                                if (static_cast<float>(rand()) / RAND_MAX < szansaNaEfekt) {
                                    EfektStatusu efekt = gracz.aktualnaBron->pobierzEfekt();
                                    Przeciwnik* cel = przeciwnik.get();
                                    
                                    switch(efekt) {
                                        case EfektStatusu::ZATRUCIE:
                                            zatrucie(cel);
                                            efektyStatusow[cel] = {"zatrucie", 4};
                                            break;
                                        case EfektStatusu::PODPALENIE:
                                            podpalenie(cel);
                                            efektyStatusow[cel] = {"podpalenie", 3};
                                            break;
                                        case EfektStatusu::ZAMROZENIE:
                                            zamrozenie(cel);
                                            efektyStatusow[cel] = {"zamrozenie", 1};
                                            break;
                                        case EfektStatusu::KRWAWIENIE:
                                            krwawienie(cel);
                                            efektyStatusow[cel] = {"krwawienie", 2};
                                            break;
                                        case EfektStatusu::POWALENIE:
                                            powalenie(cel);
                                            efektyStatusow[cel] = {"powalenie", 2};
                                            break;
                                        case EfektStatusu::OGLUSZENIE:
                                            ogluszenie(cel);
                                            efektyStatusow[cel] = {"ogluszenie", 1};
                                            break;
                                        case EfektStatusu::OSLEPIENIE:
                                            oslepienie(cel);
                                            efektyStatusow[cel] = {"oslepienie", 3};
                                            bonusyUnikow[cel] = {0.15f, 0.15f};
                                            break;
                                        case EfektStatusu::STRACH:
                                            strach(cel);
                                            efektyStatusow[cel] = {"strach", 1};
                                            break;
                                        case EfektStatusu::PORAZENIE:
                                            porazenie(cel);
                                            efektyStatusow[cel] = {"porazenie", 2};
                                            break;
                                        case EfektStatusu::KRADZIEZ:
                                            kradziez(cel, &gracz);
                                            efektyStatusow[cel] = {"kradziez", 1};
                                            break;
                                        case EfektStatusu::CHAOS:
                                            chaos(cel, &gracz);
                                            break;
                                        case EfektStatusu::SMIERC:
                                            smierc(cel);
                                            break;
                                        default: break;
                                    }
                                    cout << "Efekt " << efektToString(efekt) << " zadzialal na " << cel->pobierzNazwe() << "!\n";
                                }
                            }
                        }
                        akcjaWykonana = true;
                        czekajNaKlawisz();
                        break;
                    }
                    case 2: { // Unik
                        if (gracz.czyUnikManualny()) {
                            cout << "Przygotowales sie do uniku w nastepnej turze!\n";
                            gracz.szansaNaUnikBierny += 0.3f; // Tymczasowy bonus do uniku
                        } else {
                            cout << "Nie udalo sie przygotowac do uniku!\n";
                        }
                        akcjaWykonana = true;
                        czekajNaKlawisz();
                        break;
                    }
                    case 3: { // Ucieczka
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
                        gracz.wyswietlStatystyki(false);
                        czekajNaKlawisz();
                        wyswietlInterfejsWalki(gracz, przeciwnicy);
                        break;
                    }
                    case 5: { // Mikstura
                        if (gracz.mikstury > 0) {
                            gracz.uzyjMikstury();
                            akcjaWykonana = true;
                        } else {
                            cout << "Nie masz mikstur!\n";
                            czekajNaKlawisz();
                        }
                        break;
                    }
                    default: {
                        cout << "Nieprawidlowy wybor!\n";
                        czekajNaKlawisz();
                        break;
                    }
                }
            }
        }

        if (gracz.hp <= 0 || przeciwnicy.empty() || ucieczka) break;

        // --- TURA PRZECIWNIKA ---
vector<Przeciwnik*> stunowaniPrzeciwnicy;

// Aplikowanie efektów statusowych
for (auto it = efektyStatusow.begin(); it != efektyStatusow.end(); ) {
    Przeciwnik* p = it->first;
    string efekt = it->second.first;
    int& tury = it->second.second;

    // Sprawdź czy przeciwnik nadal żyje
    bool przeciwnikIstnieje = any_of(przeciwnicy.begin(), przeciwnicy.end(),
        [p](const unique_ptr<Przeciwnik>& ptr) { return ptr.get() == p; });
    
    if (!przeciwnikIstnieje) {
        it = efektyStatusow.erase(it);
        continue;
    }

    // Efekty zadające obrażenia
    if (efekt == "zatrucie") {
        p->otrzymajObrazenia(3);
        cout << p->pobierzNazwe() << " otrzymal 3 obrazenia od zatrucia\n";
    }
    else if (efekt == "podpalenie") {
        p->otrzymajObrazenia(4);
        cout << p->pobierzNazwe() << " otrzymal 4 obrazenia od podpalenia\n";
    }
    else if (efekt == "krwawienie") {
        p->otrzymajObrazenia(6);
        cout << p->pobierzNazwe() << " krwawi (6 obrazen)\n";
    }
    else if (efekt == "kradziez") {
        int skradzioneZloto = rand() % 10 + 5; // 5-14 złota
        gracz.ustawZloto(gracz.pobierzZloto() + skradzioneZloto);
        cout << "Okradles " << p->pobierzNazwe() << " z " << skradzioneZloto << " zlota!\n";
    }

    // Efekty ogłuszające
    if (efekt == "zamrozenie" || efekt == "ogluszenie" || efekt == "strach") {
        stunowaniPrzeciwnicy.push_back(p);
        cout << p->pobierzNazwe() << " jest " << efekt << " i traci ture\n";
    }

    // Zmniejsz licznik tur i usuń wygasłe efekty
    if (--tury <= 0) {
        if (efekt == "powalenie" || efekt == "porazenie") {
            p->resetujBazoweStatystyki();
            cout << p->pobierzNazwe() << " wraca do normalnych statystyk\n";
        }
        else if (efekt == "oslepienie") {
            bonusyUnikow.erase(p);
            cout << p->pobierzNazwe() << " odzyskuje wzrok\n";
        }
        it = efektyStatusow.erase(it);
    } else {
        ++it;
    }
}

        // Ataki przeciwników
        for (auto& przeciwnik : przeciwnicy) {
            if (przeciwnik->pobierzHp() <= 0) continue;
            if (find(stunowaniPrzeciwnicy.begin(), stunowaniPrzeciwnicy.end(), przeciwnik.get()) != stunowaniPrzeciwnicy.end()) {
                continue;
            }

            if (gracz.czyUnikBierny()) {
                cout << "Unikles ataku " << przeciwnik->pobierzNazwe() << "!\n";
            } else {
                przeciwnik->atakuj(gracz);
                
                if (gracz.hp <= 0) {
                    cout << "Zostales pokonany!\n";
                    break;
                }
            }
        }
        czekajNaKlawisz();
        
        // Reset tymczasowego bonusa do uniku
        gracz.szansaNaUnikBierny = min(gracz.szansaNaUnikBierny - 0.3f, 0.0f);
        
        // Usuwanie pokonanych przeciwników
        przeciwnicy.erase(
            remove_if(przeciwnicy.begin(), przeciwnicy.end(),
                [](const unique_ptr<Przeciwnik>& p) {
                    return p->pobierzHp() <= 0;
                }),
            przeciwnicy.end()
        );
    }

    // --- ZAKOŃCZENIE WALKI ---
    clearScreen();
    
    if (gracz.hp <= 0) {
        // Przegrana
        cout << "================================\n";
        cout << "         PORAZKA\n"; 
        cout << "================================\n\n";
        cout << "Twoja postac zostala pokonana...\n";
        
        // Odejmij część złota przy przegranej (30% szans)
        if (rand() % 100 < 30) {
            int utraconeZloto = gracz.pobierzZloto() * 0.3;
            gracz.ustawZloto(gracz.pobierzZloto() - utraconeZloto);
            cout << "Straciles " << utraconeZloto << " zlota!\n";
        }
    } 
    else if (ucieczka) {
        // Ucieczka
        cout << "================================\n";
        cout << "        UCIECZKA\n";
        cout << "================================\n\n";
        cout << "Udalo ci sie uciec z walki!\n";
        
        // Utrata części EXP przy ucieczce
        int utraconyExp = expSuma * 0.5;
        cout << "Tracisz " << utraconyExp << " EXP za ucieczke!\n";
    }
    else {
        // Zwycięstwo
        cout << "================================\n";
        cout << "         ZWYCIESTWO\n";
        cout << "================================\n\n";
        
        // Bonus za pokonanie wszystkich przeciwników
        if (przeciwnicy.empty()) {
            int bonus = zlotoSuma * 0.2;
            zlotoSuma += bonus;
            cout << "Bonus za czyste zwyciestwo: +" << bonus << " zlota!\n";
        }

        // Nagrody
        cout << "Zdobyto:\n";
        cout << "- " << expSuma << " doswiadczenia\n";
        cout << "- " << zlotoSuma << " zlota\n\n";

        // Dodanie nagród do postaci
        gracz.zdobadzDoswiadczenie(expSuma, wszystkiePokoje);
        gracz.ustawZloto(gracz.pobierzZloto() + zlotoSuma);

        // Sprawdzenie czy poziom się zwiększył
        if (gracz.poziom > poziomPrzedWalka) {
            cout << "! AWANSOWALES DO POZIOMU " << gracz.poziom << " !\n";
        }

        // 15% szansy na miksturę po walce
        if (rand() % 100 < 15) {
            gracz.mikstury++;
            cout << "Znalazles miksture zdrowia w rzeczach przeciwnikow!\n";
        }
    }

    // Czyszczenie efektów statusowych
    efektyStatusow.clear();
    bonusyUnikow.clear();

    // Wyświetlenie aktualnych statystyk
    cout << "\nTwoje aktualne statystyki:\n";
    cout << "HP: " << gracz.hp << "/" << gracz.maxHp << " | ";
    cout << "Zloto: " << gracz.pobierzZloto() << " | ";
    cout << "Mikstury: " << gracz.mikstury << "\n";

    czekajNaKlawisz();
}

// Funkcja do losowania jednego przeciwnika
vector<unique_ptr<Przeciwnik>> losujPrzeciwnikow(int poziomGracza, bool czyCzaszka1 = false) {
    vector<unique_ptr<Przeciwnik>> przeciwnicy;
    if (poziomGracza <= 0) {
        cerr << "Nieprawidłowy poziom gracza: " << poziomGracza << endl;
        return przeciwnicy;
    }

    int liczbaPrzeciwnikow = 2 + rand() % 2; // 2-3 przeciwników

    for (int i = 0; i < liczbaPrzeciwnikow; ++i) {
        int typ = czyCzaszka1 ? rand() % 7 : rand() % 6;
        try {
            unique_ptr<Przeciwnik> p(stworzPrzeciwnika(typ, poziomGracza));
            if (p) {
                przeciwnicy.push_back(move(p));
            }
        } catch (const exception& e) {
            cerr << "Błąd tworzenia przeciwnika: " << e.what() << endl;
        }
    }
    
    return przeciwnicy;
}

Pokoj* aktualnyPokoj = nullptr;

void rozszerzMape(Pokoj* skrzyzowanie2, vector<unique_ptr<Pokoj>>& wszystkiePokoje, int poziomGracza) {
    try {
        // Tworzenie nowych pokoi z użyciem smart pointerów
        auto fontanna = make_unique<Pokoj>("Fontanna Uzdrawiajaca");
        auto komnata1 = make_unique<Pokoj>("Komnata 1");
        auto komnata2 = make_unique<Pokoj>("Komnata 2");
        auto czaszka1 = make_unique<Pokoj>("Czaszka 1");
        auto komnata3 = make_unique<Pokoj>("Komnata 3");
        auto boss = make_unique<Pokoj>("Komnata Bossowa");

        // Łączenie pokoi
        fontanna->dodajPolaczenie("powrot", skrzyzowanie2);
        skrzyzowanie2->dodajPolaczenie("tyl", fontanna.get());
        skrzyzowanie2->dodajPolaczenie("lewo", komnata1.get());
        skrzyzowanie2->dodajPolaczenie("prawo", komnata2.get());
        
        komnata1->dodajPolaczenie("powrot", skrzyzowanie2);
        komnata1->dodajPolaczenie("przod", czaszka1.get());
        
        czaszka1->dodajPolaczenie("powrot", komnata1.get());
        
        komnata2->dodajPolaczenie("powrot", skrzyzowanie2);
        komnata2->dodajPolaczenie("przod", komnata3.get());
        
        komnata3->dodajPolaczenie("powrot", komnata2.get());
        komnata3->dodajPolaczenie("przod", boss.get());
        
        boss->dodajPolaczenie("powrot", komnata3.get());

        // Funkcja specjalna fontanny
        auto fontanna = make_unique<Pokoj>("Fontanna Uzdrawiajaca");
        
        fontanna->ustawSpecjalnaAkcje([&](Postac& gracz) -> bool {
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
        });

        // Dodawanie przeciwników
        auto dodajPrzeciwnikow = [poziomGracza](unique_ptr<Pokoj>& pokoj, bool miniboss = false) {
            if (miniboss) {
                auto straznik = make_unique<StraznikKorzeni>();
                straznik->ustawPoziom(poziomGracza + 2);
                straznik->skalujDoPoziomu(poziomGracza);
                pokoj->dodajPrzeciwnika(move(straznik));
            } else {
                auto przeciwnicy = losujPrzeciwnikow(poziomGracza);
                for (auto& p : przeciwnicy) {
                    pokoj->dodajPrzeciwnika(move(p));
                }
            }
        };

        dodajPrzeciwnikow(komnata1);
        dodajPrzeciwnikow(komnata2);
        dodajPrzeciwnikow(komnata3);
        dodajPrzeciwnikow(czaszka1, true); // Miniboss
        auto boss = make_unique<Pokoj>("Komnata Bossowa");
        boss->wyczyscPrzeciwnikow(); // Początkowo pusty

        // Dodanie do listy pokoi
        wszystkiePokoje.push_back(move(fontanna));
        wszystkiePokoje.push_back(move(komnata1));
        wszystkiePokoje.push_back(move(komnata2));
        wszystkiePokoje.push_back(move(czaszka1));
        wszystkiePokoje.push_back(move(komnata3));
        wszystkiePokoje.push_back(move(boss));
    }
    catch (const exception& e) {
        cerr << "Blad podczas rozszerzania mapy: " << e.what() << endl;
        throw;
    }
}

void rozszerzMapeDalej(Pokoj* skrzyzowanie1, vector<unique_ptr<Pokoj>>& wszystkiePokoje, int poziomGracza) {
    try {
        // Tworzenie nowych pokoi
        auto komnata4 = make_unique<Pokoj>("Komnata 4");
        auto komnata5 = make_unique<Pokoj>("Komnata 5");
        auto komnata6 = make_unique<Pokoj>("Komnata 6");
        auto sklep = make_unique<Pokoj>("Sklep");
        auto komnata7 = make_unique<Pokoj>("Komnata 7");
        auto czaszka2 = make_unique<Pokoj>("Czaszka 2");

        // Łączenie pokoi
        skrzyzowanie1->dodajPolaczenie("lewo", komnata4.get());
        skrzyzowanie1->dodajPolaczenie("przod", komnata5.get());
        skrzyzowanie1->dodajPolaczenie("prawo", komnata6.get());
        
        komnata4->dodajPolaczenie("powrot", skrzyzowanie1);
        komnata5->dodajPolaczenie("powrot", skrzyzowanie1);
        komnata6->dodajPolaczenie("powrot", skrzyzowanie1);
        
        komnata6->dodajPolaczenie("przod", sklep.get());
        sklep->dodajPolaczenie("powrot", komnata6.get());
        sklep->dodajPolaczenie("prawo", komnata7.get());
        
        komnata7->dodajPolaczenie("powrot", sklep.get());
        komnata7->dodajPolaczenie("przod", czaszka2.get());
        czaszka2->dodajPolaczenie("powrot", komnata7.get());

        // Dodawanie przeciwników
        auto dodajPrzeciwnikow = [poziomGracza](unique_ptr<Pokoj>& pokoj, bool miniboss = false) {
            if (miniboss) {
                auto straznik = make_unique<StraznikKorzeni>();
                straznik->ustawPoziom(poziomGracza + 2);
                straznik->skalujDoPoziomu(poziomGracza);
                pokoj->dodajPrzeciwnika(move(straznik));
            } else {
                auto przeciwnicy = losujPrzeciwnikow(poziomGracza);
                for (auto& p : przeciwnicy) {
                    pokoj->dodajPrzeciwnika(move(p));
                }
            }
        };

        dodajPrzeciwnikow(komnata4);
        dodajPrzeciwnikow(komnata5);
        dodajPrzeciwnikow(komnata6);
        dodajPrzeciwnikow(komnata7);
        dodajPrzeciwnikow(czaszka2, true); // Miniboss

        // Dodanie do listy pokoi
        wszystkiePokoje.push_back(move(komnata4));
        wszystkiePokoje.push_back(move(komnata5));
        wszystkiePokoje.push_back(move(komnata6));
        wszystkiePokoje.push_back(move(sklep));
        wszystkiePokoje.push_back(move(komnata7));
        wszystkiePokoje.push_back(move(czaszka2));

    } catch (const exception& e) {
        cerr << "Blad podczas rozszerzania mapy: " << e.what() << endl;
        throw;
    }
}

void inicjalizujMape(unique_ptr<Pokoj>& startowy, vector<unique_ptr<Pokoj>>& wszystkiePokoje, int poziomGracza) {
    startowy = make_unique<Pokoj>("Start");
    wszystkiePokoje.push_back(move(startowy));
    try {
        // Tworzenie podstawowych pokoi z użyciem smart pointerów
        startowy = make_unique<Pokoj>("Start");
        auto lewy = make_unique<Pokoj>("Lewy");
        auto prawy = make_unique<Pokoj>("Prawy");
        auto przod = make_unique<Pokoj>("Przod");
        auto relikwie = make_unique<Pokoj>("Relikwie");
        auto skrzyzowanie1 = make_unique<Pokoj>("Skrzyzowanie1");
        auto skrzyzowanie2 = make_unique<Pokoj>("Skrzyzowanie2");

        // Łączenie pokoi
        startowy->dodajPolaczenie("lewo", lewy.get());
        startowy->dodajPolaczenie("przod", przod.get());
        startowy->dodajPolaczenie("prawo", prawy.get());

        lewy->dodajPolaczenie("powrot", startowy.get());
        lewy->dodajPolaczenie("przod", relikwie.get());
        lewy->dodajPolaczenie("prawo", skrzyzowanie1.get());

        prawy->dodajPolaczenie("powrot", startowy.get());
        prawy->dodajPolaczenie("przod", skrzyzowanie2.get());

        przod->dodajPolaczenie("powrot", startowy.get());
        przod->dodajPolaczenie("przod", skrzyzowanie1.get());

        relikwie->dodajPolaczenie("powrot", lewy.get());

        skrzyzowanie1->dodajPolaczenie("powrot", lewy.get());
        skrzyzowanie1->dodajPolaczenie("lewo", przod.get());

        skrzyzowanie2->dodajPolaczenie("powrot", prawy.get());

        // Dodawanie przeciwników
        auto dodajPrzeciwnikow = [poziomGracza](unique_ptr<Pokoj>& pokoj, bool miniboss = false) {
            if (miniboss) {
                auto straznik = make_unique<StraznikKorzeni>();
                straznik->skalujDoPoziomu(poziomGracza);
                straznik->ustawPoziom(poziomGracza + 2);
                pokoj->dodajPrzeciwnika(move(straznik));
            } else {
                auto przeciwnicy = losujPrzeciwnikow(poziomGracza);
                for (auto& p : przeciwnicy) {
                    pokoj->dodajPrzeciwnika(move(p));
                }
            }
        };

        dodajPrzeciwnikow(lewy);
        dodajPrzeciwnikow(prawy);
        dodajPrzeciwnikow(przod);
        dodajPrzeciwnikow(skrzyzowanie1);
        dodajPrzeciwnikow(skrzyzowanie2);

        // Dodanie pokoi do wektora wszystkich pokoi
        wszystkiePokoje.push_back(move(startowy));
        wszystkiePokoje.push_back(move(lewy));
        wszystkiePokoje.push_back(move(prawy));
        wszystkiePokoje.push_back(move(przod));
        wszystkiePokoje.push_back(move(relikwie));
        wszystkiePokoje.push_back(move(skrzyzowanie1));
        wszystkiePokoje.push_back(move(skrzyzowanie2));

        // Rozszerzenia mapy
        rozszerzMape(wszystkiePokoje.back().get(), wszystkiePokoje, poziomGracza);
        rozszerzMapeDalej((*(wszystkiePokoje.end()-2)).get(), wszystkiePokoje, poziomGracza);
    }
    catch (const exception& e) {
        cerr << "Blad podczas inicjalizacji mapy: " << e.what() << endl;
        throw; // Przekazujemy wyjątek dalej
    }
}

void rozpocznijGre(unique_ptr<Postac>& gracz, unique_ptr<Pokoj>& startowy, vector<unique_ptr<Pokoj>>& wszystkiePokoje) {
    srand(static_cast<unsigned>(time(nullptr)));

    // Zmiana: unique_ptr zamiast Postac
    unique_ptr<Postac> gracz = wybierzPostac();
    bool graTrwa = true;

    // Zmiana: unique_ptr dla pokoi
    unique_ptr<Pokoj> startowy;
    vector<unique_ptr<Pokoj>> wszystkiePokoje;
    
    try {
        inicjalizujMape(startowy, wszystkiePokoje, gracz->pobierzPoziom());
        
        // Upewnij się, że startowy istnieje
        if (!startowy) {
            throw runtime_error("Pokój startowy nie został zainicjalizowany");
        }
        
        // Poprawne ustawienie aktualnego pokoju
        Pokoj* aktualnyPokoj = startowy.get();
        
        while (graTrwa && gracz->pobierzHp() > 0) {
        clearScreen();
        cout << "================================\n";
        cout << "  " << aktualnyPokoj->pobierzNazwe() << "\n";
        cout << "================================\n";
        cout << "HP: " << gracz->hp << "/" << gracz->maxHp << " | "; // Użycie ->
        cout << "Poziom: " << gracz->poziom << " | "; // Użycie ->
        cout << "Zloto: " << gracz->zloto << " | "; // Użycie ->
        cout << "Mikstury: " << gracz->mikstury << "\n\n"; // Użycie ->

        if (!aktualnyPokoj->czyPokonany() && !aktualnyPokoj->czySaPrzeciwnicy()) {
            aktualnyPokoj->pobierzPrzeciwnikow() == losujPrzeciwnikow(gracz->poziom);
        }

        if (aktualnyPokoj->maSpecjalnaAkcje() && !aktualnyPokoj->czyOdwiedzony()) {
            if (aktualnyPokoj->pobierzSpecjalnaAkcje()(*gracz)) {
                aktualnyPokoj->ustawOdwiedzony(true);
            }
        }

                // Menu nawigacji
                bool wybranoAkcje = false;
                while (!wybranoAkcje && graTrwa) {
                    // Wyświetlenie dostępnych kierunków
                    cout << "Dostepne akcje:\n";
                    int opcja = 1;
                    map<int, pair<string, Pokoj*>> kierunki;  // Przechowuje numery, kierunki i cele
                    
                    // Generowanie opcji nawigacji
                    for (const auto& polaczenie : aktualnyPokoj->pobierzPolaczenia()) {
                        cout << opcja << ". Idz na " << polaczenie.first 
                             << " (" << polaczenie.second->pobierzNazwe() << ")\n";
                        kierunki[opcja] = polaczenie;
                        opcja++;
                    }
        
                    // Dodanie dodatkowych opcji
                    cout << opcja++ << ". Statystyki postaci\n";
                    if (gracz->mikstury > 0) {
                        cout << opcja++ << ". Uzyj mikstury (+" << static_cast<int>(gracz->maxHp * 0.2) << " HP)\n";
                    }
                    cout << opcja << ". Wyjdz z gry\n";
        
                    // Obsługa wyboru gracza
                    cout << "\nWybor: ";
                    int wybor;
                    if (!(cin >> wybor)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Nieprawidlowy wybor!\n";
                        czekajNaKlawisz();
                        clearScreen();
                        continue;
                    }
        
                    // Obsługa nawigacji
                    if (aktualnyPokoj) {  // Dodane sprawdzenie nullptr
                        auto& przeciwnicy = aktualnyPokoj->pobierzPrzeciwnikow();
                        if (!aktualnyPokoj->czyPokonany() && 
                            any_of(przeciwnicy.begin(), przeciwnicy.end(), 
                                [](const auto& p) { return p && p->czyZyje(); })) 
                        {
                            vector<unique_ptr<Przeciwnik>> przeciwnicyKopia;
                            for (auto& p : przeciwnicy) {
                                if (p) {
                                    przeciwnicyKopia.push_back(make_unique<Przeciwnik>(*p));
                                }
                            }
                            walka(*gracz, przeciwnicyKopia, wszystkiePokoje);
                            // Aktualizacja oryginalnej listy przeciwników
                            przeciwnicy == move(przeciwnicyKopia);
                            
                            if (gracz->pobierzHp() <= 0) {
                                graTrwa = false;
                            } 
                            else if (none_of(przeciwnicy.begin(), przeciwnicy.end(),
                                        [](const auto& p) { return p && p->czyZyje(); })) 
                            {
                                aktualnyPokoj->ustawPokonany(true);
                                int zdobyteZloto = 20 + rand() % 30;
                                gracz->ustawZloto(gracz->pobierzZloto() + zdobyteZloto);
                                cout << "\nZdobyto " << zdobyteZloto << " zlota!\n";
                                czekajNaKlawisz();
                            }
                            wybranoAkcje = true;
                        }
                    }
                        // Obsługa statystyk
                        else if (wybor == kierunki.size() + 1) {
                            // Statystyki
                            clearScreen();
                            gracz->wyswietlStatystyki(false); // false = nie czyść ekranu ponownie
                            
                            // Dodatkowe informacje o przeciwnikach
                            if (aktualnyPokoj->czySaPrzeciwnicy()) {
                                cout << "\n=== PRZECIWNICY W POKOJU ===\n";
                                for (const auto& przeciwnik : aktualnyPokoj->pobierzPrzeciwnikow()) {
                                    if (przeciwnik && przeciwnik->czyZyje()) {
                                        cout << "- " << przeciwnik->pobierzNazwe() 
                                             << " (HP: " << przeciwnik->pobierzHp() 
                                             << "/" << przeciwnik->pobierzMaxHp() << ")\n";
                                    }
                                }
                            }
                            czekajNaKlawisz();
                            
                            // Przywrócenie oryginalnego widoku
                            wyswietlInterfejsWalki(*gracz, aktualnyPokoj->pobierzPrzeciwnikow());
                        }
                        
                        // Obsługa mikstur
                        else if (wybor == kierunki.size() + 2 && gracz->mikstury > 0) {
                            clearScreen();
                            cout << "================================\n";
                            cout << "       UZYCIE MIKSTURY\n";
                            cout << "================================\n";
    
                            // 1. Sprawdzenie czy zdrowie jest już pełne (NOWE)
                            if (gracz->pobierzHp() >= gracz->pobierzMaxHp()) {
                                cout << "Masz juz pelne zdrowie! (" << gracz->pobierzHp() << "/" << gracz->pobierzMaxHp() << ")\n";
                                cout << "Nie uzyto mikstury.\n";
                                czekajNaKlawisz();
                                continue;  // Powrót do menu bez zużycia mikstury
                            }

                            int hpPrzed = gracz->pobierzHp();
                            gracz->uzyjMikstury();
                            int hpPo = gracz->pobierzHp();
    
                            // 2. Naprawione pobierzMaxHp() (NOWE)
                            cout << "Uzyles mikstury! (+" << (hpPo - hpPrzed) << " HP)\n";
                            cout << "Aktualne HP: " << hpPo << "/" << gracz->maxHp << "\n";  // Bezpośredni dostęp do maxHp
                            cout << "Pozostalo mikstur: " << gracz->mikstury << "\n";
    
                            czekajNaKlawisz();
                            wybranoAkcje = true;
                            wyswietlInterfejsWalki(*gracz, aktualnyPokoj->pobierzPrzeciwnikow());
                        }
                        
                        // Obsługa wyjścia z gry
                        else if (wybor == kierunki.size() + (gracz->mikstury > 0 ? 3 : 2)) {
                            cout << "\nCzy na pewno chcesz wyjsc? (T/N): ";
                            char potwierdzenie;
                            cin >> potwierdzenie;
                            if (tolower(potwierdzenie) == 't') {
                                graTrwa = false;
                                wybranoAkcje = true;
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                cout << "================================\n";
                cout << (gracz->pobierzHp() <= 0 ? "         GAME OVER" : "        KONIEC GRY") << "\n";
                cout << "================================\n\n";
                cout << "Twoje osiagniecia:\n";

                // Poprawione podsumowanie
                cout << "- Osiagniety poziom: " << gracz->poziom << "\n";
                cout << "- Zdobyte zloto: " << gracz->pobierzZloto() << "\n";

                // Statystyki pokonanych lokacji
                int pokonanePokoje = count_if(wszystkiePokoje.begin(), wszystkiePokoje.end(),
                    [](const unique_ptr<Pokoj>& p) { return p && p->czyPokonany(); });
                cout << "- Pokonane lokacje: " << pokonanePokoje 
                     << "/" << wszystkiePokoje.size() << "\n";

            czekajNaKlawisz();
        }
        catch (const exception& e) {
            cerr << "\n################################\n";
            cerr << "   BŁĄD KRYTYCZNY: " << e.what() << endl;
            cerr << "################################\n";
            #ifdef _WIN32
                system("pause");
            #endif
        }
    }
    int main() 
    {
        // Inicjalizacja generatora liczb losowych
        srand(static_cast<unsigned>(time(nullptr)));
    
        // Inicjalizacja struktur gry
        vector<unique_ptr<Pokoj>> wszystkiePokoje;
        unique_ptr<Pokoj> startowy;
        unique_ptr<Postac> gracz;
    
        try {
            // 1. Wybór postaci przez gracza
            gracz = wybierzPostac();
            if (!gracz) {
                throw runtime_error("Nie udało się stworzyć postaci");
            }
            
            // 2. Inicjalizacja mapy
            inicjalizujMape(startowy, wszystkiePokoje, gracz->pobierzPoziom());
            
            // 3. Sprawdzenie poprawności inicjalizacji
            if (!startowy) {
                throw runtime_error("Błąd inicjalizacji - brak pokoju startowego");
            }
    
            // 4. Przypisanie broni startowej
            if (dynamic_cast<Opancerzony*>(gracz.get())) {
                gracz->aktualnaBron = new Bron("zelazny miecz", 5);
            } 
            else if (dynamic_cast<Bandyta*>(gracz.get())) {
                gracz->aktualnaBron = new Bron("zatrute sztylety", 8, 0.35f, EfektStatusu::ZATRUCIE);
            }
    
            // 5. Rozpoczęcie gry z przekazaniem przez referencję
            rozpocznijGre(gracz, startowy, wszystkiePokoje);
    
        } 
        catch (const bad_alloc& e) {
            cerr << "\n################################\n";
            cerr << "   BRAK PAMIĘCI!: " << e.what() << endl;
            cerr << "################################\n";
            return 3;
        }
        catch (const exception& e) {
            cerr << "\n################################\n";
            cerr << "   BŁĄD KRYTYCZNY: " << e.what() << endl;
            cerr << "################################\n";
            #ifdef _WIN32
                system("pause");
            #endif
            return 1;
        }
        catch (...) {
            cerr << "\n################################\n";
            cerr << "   NIEZNANY BŁĄD KRYTYCZNY!\n";
            cerr << "################################\n";
            #ifdef _WIN32
                system("pause");
            #endif
            return 2;
        }
    
        // 6. Zakończenie programu
        cout << "\nDziękujemy za grę! Do zobaczenia!\n";
        #ifdef _WIN32
            system("pause");
        #endif
        return 0;
    }