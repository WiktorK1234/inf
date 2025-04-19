#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void czekajNaKlawisz() {
    cout << "\nNacisnij Enter, aby kontynuowac...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

class Postac {
public:
    string nazwa;
    int hp;
    int maxHp;
    float pancerz;
    int obrazenia;
    float penetracjaPancerza;
    float szansaNaUnikBierny;
    float unikManualny;
    float szansaNaUcieczke;
    int poziom = 1;
    int exp = 0;
    int zloto=0;
    int mikstury=5;

    void uzyjMikstury() {
        if (mikstury > 0) {
            int ileLeczenia = maxHp * 0.2;
            hp = min(hp + ileLeczenia, maxHp);
            mikstury--;
            cout << "Uzyles mikstury. Odzyskales " << ileLeczenia << " HP. Pozostalo mikstur: " << mikstury << "\n";
        } else {
            cout << "Nie masz mikstur!\n";
        }
    }

    Postac() = default;

    virtual void wyswietlStatystyki() {
        cout << "Statystyki postaci (" << nazwa << "):\n";
        cout << "HP: " << hp << "\n";
        cout << "Pancerz: " << pancerz * 100 << "%\n";
        cout << "Obrazenia: " << obrazenia << "\n";
        cout << "Penetracja pancerza: " << penetracjaPancerza * 100 << "%\n";
        cout << "Szansa na unik bierny: " << szansaNaUnikBierny * 100 << "%\n";
        cout << "Unik manualny: " << unikManualny * 100 << "%\n";
        cout << "Szansa na ucieczke: " << szansaNaUcieczke * 100 << "%\n";
        cout << "Poziom: " << poziom << "\n";
        cout << "Doswiadczenie: " << exp << " / " << poziom * 100 << "\n";
        cout << "Zloto: " << zloto << "\n";
        cout << "Mikstury: " << mikstury << "\n";
        cout << "------------------\n";
    }
    
    void zdobadzDoswiadczenie(int ilosc) {
        exp += ilosc;
        while (exp >= poziom * 100 && poziom < 10) {
            exp -= poziom * 100;
            poziom++;
            hp += 10;
            obrazenia += 2;
            cout << "Awans! Osiagnieto poziom " << poziom << "! (+10 HP, +2 obrazenia, +50 zlota)\n";
        }
    }

    bool czyUnikBierny() {
        return static_cast<float>(rand()) / RAND_MAX < szansaNaUnikBierny;
    }

    bool czyUnikManualny() {
        return static_cast<float>(rand()) / RAND_MAX < unikManualny;
    }

    bool czyUcieczka() {
        return static_cast<float>(rand()) / RAND_MAX < szansaNaUcieczke;
    }

    int obliczObrazenia(int obrazeniaAtakujacego) {
        float efektywneObrazenia = obrazeniaAtakujacego * (1 - pancerz + penetracjaPancerza);
        return static_cast<int>(efektywneObrazenia);
    }
};

class Opancerzony : public Postac {
public:
    Opancerzony() {
        nazwa = "Opancerzony";
        hp = 140;
        maxHp=140;
        pancerz = 0.08f;
        obrazenia = 10;
        penetracjaPancerza = 0.08f;
        szansaNaUnikBierny = 0.15f;
        unikManualny = 0.50f;
        szansaNaUcieczke = 0.05f;
    }
};

class Bandyta : public Postac {
public:
    Bandyta() {
        nazwa = "Bandyta";
        hp = 85;
        maxHp=85;
        pancerz = 0.0f;
        obrazenia = 14;
        penetracjaPancerza = 0.15f;
        szansaNaUnikBierny = 0.35f;
        unikManualny = 0.75f;
        szansaNaUcieczke = 0.50f;
    }
};

class Przeciwnik {
public:
    string nazwa;
    int hp;
    int maxHp;
    int obrazenia;
    float pancerz;
    float penetracjaPancerza;
    float szansaNaUnik;
    int poziom;

    Przeciwnik(string n, int h, int dmg, int lvl)
        : nazwa(n), hp(h), maxHp(h), obrazenia(dmg), poziom(lvl) {}
        
     Przeciwnik() : nazwa("Nieznany"), hp(0), maxHp(0), obrazenia(0), poziom(1) {}

    virtual void wyswietlStatystyki() {
        cout << "Statystyki przeciwnika (" << nazwa << "):\n";
        cout << "HP: " << hp << "\n";
        cout << "Pancerz: " << pancerz * 100 << "%\n";
        cout << "Obrazenia: " << obrazenia << "\n";
        cout << "Szansa na unik: " << szansaNaUnik * 100 << "%\n";
    }

    bool czyUnik() {
        return static_cast<float>(rand()) / RAND_MAX < szansaNaUnik;
    }

    virtual int obliczObrazenia(int obrazeniaAtakujacego) {
        return obrazeniaAtakujacego;
    }

    virtual ~Przeciwnik() = default;
};

class Wilkolak : public Przeciwnik {
public:
    Wilkolak() {
        nazwa = "Wilkolak";
        hp = 50;
        pancerz = 0.0f;
        obrazenia = 10;
        szansaNaUnik = 0.05f;
    }
};

class Szlam : public Przeciwnik {
public:
    Szlam() {
        nazwa = "Szlam";
        hp = 30;
        pancerz = 0.2f;
        obrazenia = 5;
        szansaNaUnik = 0.0f;
    }
};

class StraznikKorzeni : public Przeciwnik {
    private:
        int tura = 0;
    
    public:
        StraznikKorzeni() {
            nazwa            = "Straznik korzeni";
            hp               = maxHp = 200;
            pancerz          = 0.05f;   // 5%
            obrazenia        = 15;      // bazowe obrazenia młotem
            penetracjaPancerza = 0.0f;  // brak penetracji
            szansaNaUnik     = 0.0f;    // 0% unik bierny
            poziom           = 3;
        }
    
        // Nadpisujemy obliczanie obrazen, żeby wprowadzic dwa typy ataku + fotosynteza
        int obliczObrazenia(int /*unused*/) override {
            tura++;
            if (tura % 4 == 0) {
                int leczenie = static_cast<int>(0.15f * maxHp);
                hp = min(hp + leczenie, maxHp);
                cout << nazwa << " uzywa Fotosyntezy i leczy sie o " << leczenie << " HP!\n";
            }
            // co 4 tury tylko fotosynteza, ale i tak atakuje dalej:
            int wybor = rand() % 2;
            if (wybor == 0) {
                cout << nazwa << " uzywa Ciosu Mlotem i zadaje 15 obrazen!\n";
                return 15;
            } else {
                cout << nazwa << " uzywa Gwaltownych Korzeni i zadaje 10 obrazen!\n";
                return 10;
            }
        }
    };

    struct Pokoj {
        string nazwa;
        bool odwiedzony = false;
        bool pokonany = false;
        vector<Przeciwnik*> przeciwnicy;
        
        Pokoj(const string& nazwa) : nazwa(nazwa) {}
        
        // Dodaj destruktor do zwalniania pamięci
        ~Pokoj() {
            for (auto p : przeciwnicy) {
                delete p;
            }
        }
    };

    vector<Przeciwnik*> losujPrzeciwnikow() {
        vector<Przeciwnik*> przeciwnicy;
        srand(time(0));  // Inicjalizacja generatora liczb losowych
    
        // Losowanie liczby szlamów (3-5) i wilkołaków (1-2)
        int liczbaSzlamow = rand() % 3 + 3;  // 3, 4 lub 5
        int liczbaWilkolakow = rand() % 2 + 1;  // 1 lub 2
    
        // Sprawdzamy, czy któryś typ jest maksymalny
        bool maksSzlamy = (liczbaSzlamow == 5);
        bool maksWilkolaki = (liczbaWilkolakow == 2);
    
        // Jeśli któryś typ jest maksymalny, dodajemy tylko ten typ
        if (maksSzlamy || maksWilkolaki) {
            if (maksSzlamy) {
                // Tylko szlamy (bo jest ich max)
                for (int i = 0; i < liczbaSzlamow; ++i) {
                    przeciwnicy.push_back(new Szlam());
                }
            } else {
                // Tylko wilkołaki (bo jest ich max)
                for (int i = 0; i < liczbaWilkolakow; ++i) {
                    przeciwnicy.push_back(new Wilkolak());
                }
            }
        } 
        else {
            // Jeśli żaden typ nie jest maksymalny, losujemy czy mają się pojawić razem (50% szans)
            bool razem = (rand() % 2 == 0);
    
            if (razem) {
                // Dodajemy oba typy
                for (int i = 0; i < liczbaSzlamow; ++i) {
                    przeciwnicy.push_back(new Szlam());
                }
                for (int i = 0; i < liczbaWilkolakow; ++i) {
                    przeciwnicy.push_back(new Wilkolak());
                }
            } else {
                // Losujemy tylko jeden typ
                if (rand() % 2 == 0) {
                    // Tylko szlamy
                    for (int i = 0; i < liczbaSzlamow; ++i) {
                        przeciwnicy.push_back(new Szlam());
                    }
                } else {
                    // Tylko wilkołaki
                    for (int i = 0; i < liczbaWilkolakow; ++i) {
                        przeciwnicy.push_back(new Wilkolak());
                    }
                }
            }
        }
    
        return przeciwnicy;
    }

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
    
    cout << "\n2. Bandyta\n";
    cout << "   - HP: 85\n";
    cout << "   - Pancerz: 0%\n";
    cout << "   - Obrazenia: 14\n";
    cout << "   - Penetracja pancerza: 15%\n";
    cout << "   - Szansa na unik bierny: 35%\n";
    cout << "   - Unik manualny: 75%\n";
    cout << "   - Szansa na ucieczke: 50%\n";
    
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

void walka(Postac &gracz, vector<Przeciwnik*> &przeciwnicy) {
    while (gracz.hp > 0 && !przeciwnicy.empty()) {
        clearScreen();
        cout << "\n--- Tura gracza (" << gracz.nazwa << ") ---\n";
        cout << "Wybierz akcje:\n";
        cout << "1. Atakuj\n";
        cout << "2. Unikaj manualnie\n";
        cout << "3. Sprobuj uciec\n";
        cout << "4. Sprawdz statystyki wszystkich postaci\n";
        cout << "5. Sprawdz statystyki wszystkich przeciwnikow\n";
        cout << "6. Uzyj mikstury (pozostalo: " << gracz.mikstury << ")\n";
        cout << "Wybor: ";
        int wybor;
        cin >> wybor;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool akcjaWykonana = false;
        if (wybor == 1) {
            cout << gracz.nazwa << " atakuje!\n";
            cout << "Wybierz przeciwnika do ataku:\n";
            for (size_t i = 0; i < przeciwnicy.size(); ++i) {
                cout << i + 1 << ". " << przeciwnicy[i]->nazwa << " (HP: " << przeciwnicy[i]->hp << ")\n";
            }
            int wyborPrzeciwnika;
            cin >> wyborPrzeciwnika;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            while (wyborPrzeciwnika < 1 || wyborPrzeciwnika > static_cast<int>(przeciwnicy.size())) {
                cout << "Nieprawidlowy wybor. Sprobuj ponownie: ";
                cin >> wyborPrzeciwnika;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            Przeciwnik* cel = przeciwnicy[wyborPrzeciwnika - 1];
            if (cel->czyUnik()) {
                cout << "Przeciwnik " << cel->nazwa << " udalo sie uniknac ataku!\n";
            } else {
                int obrazeniaGracza = gracz.obliczObrazenia(gracz.obrazenia);
                cel->hp -= obrazeniaGracza;
                cout << gracz.nazwa << " zadal " << obrazeniaGracza << " obrazen przeciwnikowi " << cel->nazwa << "!\n";
            }

            akcjaWykonana = true;
        } else if (wybor == 2) {
            if (gracz.czyUnikManualny()) {
                cout << gracz.nazwa << " udalo sie uniknac ataku przeciwnika!\n";
            } else {
                cout << gracz.nazwa << " nie udalo sie uniknac ataku!\n";
            }
            akcjaWykonana = true;
        } else if (wybor == 3) {
            if (gracz.czyUcieczka()) {
                cout << gracz.nazwa << " udalo sie uciec z walki!\n";
                break;
            } else {
                cout << gracz.nazwa << " nie udalo sie uciec!\n";
            }
            akcjaWykonana = true;
        } else if (wybor == 4) {
            clearScreen();
            gracz.wyswietlStatystyki();
            czekajNaKlawisz();
            clearScreen();
            continue;
        } else if (wybor == 5) {
            clearScreen();
            for (size_t i = 0; i < przeciwnicy.size(); ++i) {
                przeciwnicy[i]->wyswietlStatystyki();
                cout << endl;
            }
            czekajNaKlawisz();
            clearScreen();
            continue;
        } else if (wybor == 6) {
            if (gracz.mikstury > 0) {
                int ileLeczenia = gracz.maxHp * 0.2;
                gracz.hp = min(gracz.hp + ileLeczenia, gracz.maxHp);
                gracz.mikstury--;
                cout << gracz.nazwa << " uzywa mikstury! Odzyskuje " << ileLeczenia << " HP.\n";
                cout << "Pozostale mikstury: " << gracz.mikstury << "\n";
                akcjaWykonana = true;
            }
        } else {
            cout << "Nie masz mikstur!\n";
            czekajNaKlawisz();
            clearScreen();
            continue;
        }
        
        if (!akcjaWykonana) continue;

        // Sprawdzanie, czy przeciwnik zostal pokonany
        for (size_t i = 0; i < przeciwnicy.size(); ++i) {
            if (przeciwnicy[i]->hp <= 0) {
                cout << "Przeciwnik " << przeciwnicy[i]->nazwa << " zostal pokonany!\n";

                int zdobyteExp = 0;
                if (przeciwnicy[i]->nazwa == "Straznik korzeni") {
                    zdobyteExp = 250;             // 250 EXP za minibossa
                } else if (przeciwnicy[i]->nazwa == "Wilkolak") {
                    zdobyteExp = 30;
                } else if (przeciwnicy[i]->nazwa == "Szlam") {
                    zdobyteExp = 15;
                }
                gracz.zdobadzDoswiadczenie(zdobyteExp);


                // Wzor na zdobyte złoto
                int losowyBonus = rand() % 11 + 5; // 5-15%
                int bazaZloto = przeciwnicy[i]->poziom * (przeciwnicy[i]->obrazenia + przeciwnicy[i]->maxHp / 2);
                int zdobyteZloto = bazaZloto + (bazaZloto * losowyBonus / 100);

                // Bonus za minibossa
                if (przeciwnicy[i]->nazwa == "Strażnik korzeni") {
                    zdobyteZloto = static_cast<int>(zdobyteZloto * 1.5f);
                }

                gracz.zloto += zdobyteZloto;
                cout << "Zdobyles " << zdobyteExp
                     << " EXP oraz " << zdobyteZloto << " zlota!\n";

                // Usuwanie przeciwnika
                delete przeciwnicy[i];
                przeciwnicy.erase(przeciwnicy.begin() + i);
                --i;

                // Sprawdzenie awansu poziomu
                int wymaganeExp = gracz.poziom * 100;
                while (gracz.exp >= wymaganeExp && gracz.poziom < 10) {
                    gracz.exp -= wymaganeExp;
                    gracz.poziom++;
                    cout << "Awansowales na poziom " << gracz.poziom << "!\n";

                    // Skalowanie statystyk gracza
                    gracz.hp += 10;
                    gracz.obrazenia += 2;

                    cout << "Zyskales 10 HP, 2 obrazenia i 50 zlota!\n";
                    wymaganeExp = gracz.poziom * 100;
                }
            }
        }

        if (przeciwnicy.empty()) {
            cout << "Wszyscy przeciwnicy zostali pokonani!\n";
            break;
        }

        czekajNaKlawisz();
        clearScreen();
        cout << "\n--- Tura przeciwnikow ---\n";
        for (size_t i = 0; i < przeciwnicy.size(); ++i) {
            Przeciwnik* przeciwnik = przeciwnicy[i];
            if (gracz.czyUnikBierny()) {
                cout << gracz.nazwa << " udalo sie uniknac ataku przeciwnika " << przeciwnik->nazwa << "!\n";
            } else {
                int obrażeniaPrzeciwnika = przeciwnik->obliczObrazenia(przeciwnik->obrazenia);
                gracz.hp -= obrażeniaPrzeciwnika;
                cout << przeciwnik->nazwa << " zadal " << obrażeniaPrzeciwnika << " obrazen graczowi!\n";
            }
        }

        if (gracz.hp <= 0) {
            cout << gracz.nazwa << " zostal pokonany...\n";
            break;
        }

        cout << "\nAktualne statystyki:\n";
        cout << "HP Gracza (" << gracz.nazwa << "): " << gracz.hp << "\n";
        for (size_t i = 0; i < przeciwnicy.size(); ++i) {
            cout << "HP Przeciwnika (" << przeciwnicy[i]->nazwa << "): " << przeciwnicy[i]->hp << "\n";
        }

        czekajNaKlawisz();
        clearScreen();
    }

    // === PODSUMOWANIE PO WALCE ===
    cout << "\n--- Podsumowanie walki ---\n";
    cout << "Poziom postaci: "       << gracz.poziom << "\n";
    cout << "Doswiadczenie: "        << gracz.exp << "/" << (gracz.poziom*100) << "\n";
    cout << "HP: "                   << gracz.hp << "/" << gracz.maxHp << "\n";
    cout << "Obrazenia: "            << gracz.obrazenia << "\n";
    cout << "Zloto: "                << gracz.zloto << "\n";
    cout << "Mikstury: "             << gracz.mikstury << "\n";

    czekajNaKlawisz();
    clearScreen();
}

struct Pokoje {
    string nazwa;
    bool odwiedzony = false;
    bool pokonany = false;
    vector<Przeciwnik*> przeciwnicy;
};

// Funkcja do losowania jednego przeciwnika
vector<Przeciwnik*> losujPrzeciwnikowDoPokoju() {
    vector<Przeciwnik*> przeciwnicy;

    int typ = rand() % 2; // 0 = Szlam, 1 = Wilkołak
    int ile = 0;

    if (typ == 0) {
        ile = 3 + rand() % 3; // 3–5 Szlamów
        for (int i = 0; i < ile; ++i) {
            przeciwnicy.push_back(new Przeciwnik{"Szlam", 30, 5, 1});
        }
    } else {
        ile = 1 + rand() % 2; // 1–2 Wilkołaki
        for (int i = 0; i < ile; ++i) {
            przeciwnicy.push_back(new Przeciwnik{"Wilkolak", 50, 10, 2});
        }
    }

    return przeciwnicy;
}

void rozpocznijGre() {
    srand(time(nullptr));

    Postac gracz = wybierzPostac();

    // Inicjalizacja pokoi
    Pokoj startowy("Start");
    Pokoj lewy("Lewy");
    Pokoj prawy("Prawy");
    Pokoj przod("Przod");

    // Ustawienie przeciwników w pokojach
    lewy.przeciwnicy = losujPrzeciwnikow();
    prawy.przeciwnicy = losujPrzeciwnikow();
    przod.przeciwnicy.push_back(new StraznikKorzeni());

    vector<Pokoj*> wszystkiePokoje = {&startowy, &lewy, &prawy, &przod};
    Pokoj* obecny = &startowy;
    bool pokonanyMiniboss = false;

    while (gracz.hp > 0) {
        clearScreen();
        cout << "\n== Obecny pokoj: " << obecny->nazwa << " ==" << endl;
        
        if (obecny->nazwa == "Start") {
            cout << "\nDostepne opcje:" << endl;
            cout << "1. Idz w lewo" << endl;
            cout << "2. Idz w prawo" << endl;
            
            // Pokój z przodu dostępny tylko jeśli pokonano oba boczne pokoje
            if (lewy.pokonany && prawy.pokonany) {
                cout << "3. Idz do przodu (do Straznika Korzeni)" << endl;
            } else {
                cout << "3. <Drzwi do przodu sa zamkniete, musisz najpierw oczycic boczne pokoje>" << endl;
            }
            
            cout << "4. Sprawdz swoje statystyki" << endl;
            cout << "5. Uzyj mikstury (pozostalo: " << gracz.mikstury << ")" << endl;
            cout << "6. Zakoncz gre" << endl;
            cout << "Wybor: ";

            int wybor;
            while (!(cin >> wybor)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nieprawidlowy wybor. Wprowadz liczbe 1-6: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (wybor == 1) {
                obecny = &lewy;
            } else if (wybor == 2) {
                obecny = &prawy;
            } else if (wybor == 3 && lewy.pokonany && prawy.pokonany) {
                obecny = &przod;
            } else if (wybor == 4) {
                clearScreen();
                gracz.wyswietlStatystyki();
                czekajNaKlawisz();
                continue;
            } else if (wybor == 5) {
                gracz.uzyjMikstury();
                czekajNaKlawisz();
                continue;
            } else if (wybor == 6) {
                cout << "Dzieki za gre!" << endl;
                break;
            } else {
                cout << "Nieprawidlowy wybor lub drzwi sa zamkniete!" << endl;
                czekajNaKlawisz();
                continue;
            }
        } else {
            if (!obecny->pokonany && !obecny->przeciwnicy.empty()) {
                cout << "W pokoju znajduje sie przeciwnik!" << endl;
                czekajNaKlawisz();
                
                walka(gracz, obecny->przeciwnicy);
                
                if (gracz.hp <= 0) {
                    cout << "Koniec gry! Twoja postac zgineła..." << endl;
                    break;
                }

                if (obecny->przeciwnicy.empty()) {
                    obecny->pokonany = true;
                    
                    // Specjalna nagroda za Strażnika Korzeni
                    if (obecny->nazwa == "Przod" && !pokonanyMiniboss) {
                        pokonanyMiniboss = true;
                        
                        // Usunięto zdobywanie EXP tutaj, bo już jest w funkcji walka()
                        // Dodajemy tylko złoto
                        int losowyBonus = rand() % 11 + 5; // 5-15%
                        int bazaZloto = 200; // Bazowa wartość dla minibossa
                        int zdobyteZloto = bazaZloto + (bazaZloto * losowyBonus / 100);
                        
                        gracz.zloto += zdobyteZloto;
                        clearScreen();
                        cout << "Gratulacje! Pokonales Straznika Korzeni!\n";
                        cout << "Otrzymales " << zdobyteZloto << " zlota!\n";
                        czekajNaKlawisz();
                    }
                }
            } else {
                cout << "Pokoj jest pusty." << endl;
            }

            cout << "\nDostepne opcje:" << endl;
            cout << "1. Wroc do pokoju startowego" << endl;
            cout << "2. Sprawdz swoje statystyki" << endl;
            cout << "3. Uzyj mikstury (pozostalo: " << gracz.mikstury << ")" << endl;
            cout << "Wybor: ";

            int wybor;
            while (!(cin >> wybor)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nieprawidlowy wybor. Wprowadz liczbe 1-3: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (wybor == 1) {
                obecny = &startowy;
            } else if (wybor == 2) {
                clearScreen();
                gracz.wyswietlStatystyki();
                czekajNaKlawisz();
                continue;
            } else if (wybor == 3) {
                gracz.uzyjMikstury();
                czekajNaKlawisz();
                continue;
            } else {
                cout << "Nieprawidlowy wybor!" << endl;
                czekajNaKlawisz();
                continue;
            }
        }
    }

    // Zwolnienie pamięci
    for (auto pokoj : wszystkiePokoje) {
        for (auto p : pokoj->przeciwnicy) {
            delete p;
        }
        pokoj->przeciwnicy.clear();
    }
}

int main() {
    srand(time(nullptr)); // Inicjalizacja generatora liczb losowych
    rozpocznijGre();
    return 0;
}
