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

vector<Przeciwnik*> losujPrzeciwnikow() {
    vector<Przeciwnik*> przeciwnicy;
    int liczba = rand() % 2 + 1; // 1–2 przeciwników

    for (int i = 0; i < liczba; ++i) {
        int typ = rand() % 2; // 0 lub 1
        if (typ == 0) {
            przeciwnicy.push_back(new Przeciwnik("Wilkolak", 60, 12, 2));
        } else {
            przeciwnicy.push_back(new Przeciwnik("Szlam", 40, 6, 1));
        }
    }

    return przeciwnicy;
}

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
        return Postac();
    } else {
        return Bandyta();
    }
}

void walka(Postac &gracz, vector<Przeciwnik*> &przeciwnicy) {
    int exp = 0;
    int poziom = 1;
    int zloto = 0;

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
                if (przeciwnicy[i]->nazwa == "Wilkolak") {
                    zdobyteExp = 30;
                } else if (przeciwnicy[i]->nazwa == "Szlam") {
                    zdobyteExp = 15;
                }

                exp += zdobyteExp;

                // Wzor na zdobyte zloto z przeciwnikow
                int losowyBonus = rand() % 11 + 5; // 5-15%
                int obrazenia = przeciwnicy[i]->obrazenia;
                int maxHp = przeciwnicy[i]->maxHp;
                int poziomPrzeciwnika = przeciwnicy[i]->poziom;

                int bazaZloto = poziomPrzeciwnika * (obrazenia + maxHp / 2);
                int zdobyteZloto = bazaZloto + (bazaZloto * losowyBonus / 100);

                zloto += zdobyteZloto;

                cout << "Zdobyles " << zdobyteExp << " EXP oraz " << zdobyteZloto << " zlota!\n";

                // Usuwanie przeciwnika
                delete przeciwnicy[i];
                przeciwnicy.erase(przeciwnicy.begin() + i);
                --i;

                // Sprawdzenie awansu poziomu
                int wymaganeExp = poziom * 100;
                while (exp >= wymaganeExp && poziom < 10) {
                    exp -= wymaganeExp;
                    poziom++;
                    cout << "Awansowales na poziom " << poziom << "!\n";

                    // Skalowanie statystyk gracza
                    gracz.hp += 10;
                    gracz.obrazenia += 2;

                    cout << "Zyskales 10 HP, 2 obrazenia i 50 zlota!\n";
                    wymaganeExp = poziom * 100;
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
                int obrazeniaPrzeciwnika = przeciwnik->obliczObrazenia(przeciwnik->obrazenia);
                gracz.hp -= obrazeniaPrzeciwnika;
                cout << przeciwnik->nazwa << " zadal " << obrazeniaPrzeciwnika << " obrazen graczowi!\n";
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
    if (gracz.hp > 0 && przeciwnicy.empty()) {
        cout << "\n--- Podsumowanie walki ---\n";
        cout << "Poziom postaci: " << poziom << "\n";
        cout << "Doswiadczenie: " << exp << "/" << (poziom * 100) << "\n";
        cout << "HP: " << gracz.hp << "\n";
        cout << "Obrazenia: " << gracz.obrazenia << "\n";
        cout << "Zloto: " << zloto << "\n";
    }

    czekajNaKlawisz();
    clearScreen();
}

struct Pokoj {
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

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<Przeciwnik*> przeciwnicy = losujPrzeciwnikow(); // do ogólnej walki na koniec
    Postac gracz = wybierzPostac();

    // Stałe pokoje
    Pokoj startowy = {"Startowy"};
    Pokoj pokojLewo = {"Pokoj Lewo"};
    Pokoj pokojPrawo = {"Pokoj Prawo"};
    Pokoj pokojPrzod = {"Pokoj Przod"};

    // Główna pętla eksploracji
    Pokoj* aktualnyPokoj = &startowy;
    bool eksploracjaTrwa = true;

    while (eksploracjaTrwa) {
        clearScreen();
        cout << "--- Znajdujesz sie w: " << aktualnyPokoj->nazwa << " ---\n";
    
        if (!aktualnyPokoj->pokonany && aktualnyPokoj != &startowy) {
            if (!aktualnyPokoj->odwiedzony) {
                aktualnyPokoj->przeciwnicy = losujPrzeciwnikowDoPokoju();
                aktualnyPokoj->odwiedzony = true;
            }
            walka(gracz, aktualnyPokoj->przeciwnicy);
            if (gracz.hp <= 0) break;
            if (aktualnyPokoj->przeciwnicy.empty()) {
                aktualnyPokoj->pokonany = true;
                cout << "Pokoj oczyszczony z przeciwnikow!\n";
                czekajNaKlawisz();
                clearScreen();
            }
        }
    
        clearScreen();  // Dodajemy clearScreen() tuż przed pytaniem o kolejny wybór
    
        cout << "\nGdzie chcesz isc?\n";
        if (aktualnyPokoj == &startowy) {
            cout << "1. Idz w lewo\n";
            cout << "2. Idz w prawo\n";
            cout << "3. Idz do przodu\n";
            cout << "4. Sprawdz statystyki\n";
            cout << "5. Uzyj mikstury\n";
            cout << "Wybor: ";
            int wybor;
            cin >> wybor;
    
            switch (wybor) {
                case 1: aktualnyPokoj = &pokojLewo; break;
                case 2: aktualnyPokoj = &pokojPrawo; break;
                case 3: aktualnyPokoj = &pokojPrzod; break;
                case 4: gracz.wyswietlStatystyki(); czekajNaKlawisz(); break;
                case 5: gracz.uzyjMikstury(); czekajNaKlawisz(); break;
                default: cout << "Nieprawidlowy wybor.\n"; czekajNaKlawisz(); clearScreen(); break;
            }
        } else {
            cout << "1. Wroc do pokoju startowego\n";
            cout << "Wybor: ";
            int wybor;
            cin >> wybor;
            if (wybor == 1) {
                aktualnyPokoj = &startowy;
            } else {
                cout << "Nieprawidlowy wybor.\n";
                czekajNaKlawisz();
                clearScreen();
            }
        }
    }    

    if (gracz.hp <= 0)
        cout << "\nKoniec walki, przeciwnicy zostali pokonani" << endl;

    // Czyszczenie pamięci
    for (size_t i = 0; i < przeciwnicy.size(); ++i)
        delete przeciwnicy[i];

    return 0;
}
