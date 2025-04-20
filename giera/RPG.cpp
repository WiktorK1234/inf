#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>
#include <random>
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

class Przeciwnik {
    public:
        string nazwa;
        int hp;
        int maxHp;
        int obrazenia;
        float pancerz;           // Redukcja otrzymywanych obrażeń (np. 0.15 = 15%)
        float penetracjaPancerza; // Redukcja pancerza przeciwnika (np. 0.1 = 10%)
        float szansaNaUnik;
        int poziom;
    
        // Konstruktor główny
        Przeciwnik(string n, int h, int dmg, float p, float pen, float unik, int lvl)
            : nazwa(n), hp(h), maxHp(h), obrazenia(dmg), pancerz(p),
              penetracjaPancerza(pen), szansaNaUnik(unik), poziom(lvl) {}
    
        virtual ~Przeciwnik() = default;  // WIRTUALNY DESTRUKTOR
    
        void wyswietlStatystyki() {
            std::cout << "Statystyki przeciwnika (" << nazwa << "):\n";
            std::cout << "HP: " << hp << "/" << maxHp << "\n";
            std::cout << "Pancerz: " << static_cast<int>(pancerz * 100) << "%\n";
            std::cout << "Obrazenia: " << obrazenia << "\n";
            std::cout << "Szansa na unik: " << static_cast<int>(szansaNaUnik * 100) << "%\n";
            std::cout << "Poziom: " << poziom << "\n\n";  // Dodane dodatkowe \n dla czytelności
        }
    
        bool czyUnik() {
            return (rand() % 100) < (szansaNaUnik * 100);
        }
    
        virtual int obliczObrazenia(int obrazeniaAtakujacego) {
            float redukcja = std::max(0.0f, pancerz - penetracjaPancerza);
            int obrazenia = static_cast<int>(obrazeniaAtakujacego * (1.0f - redukcja));
            return std::max(1, obrazenia); // Min. 1 obrażenie
        }
    };
    
    class Wilkolak : public Przeciwnik {
    public:
        Wilkolak() : Przeciwnik(
            "Wilkolak",  // nazwa
            50,          // hp
            10,          // obrażenia
            0.05f,       // pancerz (5%)
            0.0f,        // penetracja pancerza
            0.05f,       // szansa na unik (5%)
            2            // poziom
        ) {}
        ~Wilkolak() override = default;
    };
    
    class Szlam : public Przeciwnik {
    public:
        Szlam() : Przeciwnik(
            "Szlam",
            30,
            5,
            0.2f,    // 20% pancerza
            0.0f,
            0.0f,    // brak uniku
            1
        ) {}
        ~Szlam() override = default;
    };

    class Wilk : public Przeciwnik {
        public:
            Wilk() : Przeciwnik(
                "Wilk",       // nazwa
                35,           // hp
                5,            // obrażenia
                0.0f,         // pancerz (0%)
                0.0f,         // penetracja pancerza
                0.05f,        // szansa na unik (5%)
                1            // poziom
            ) {}
            ~Wilk() override = default;
        };
        
        class Golem : public Przeciwnik {
        public:
            Golem() : Przeciwnik(
                "Golem",
                100,
                12,
                0.1f,      // 10% pancerza
                0.0f,
                0.0f,      // brak uniku
                3
            ) {}
            ~Golem() override = default;
        };
        
        class Lupiezca : public Przeciwnik {
        public:
            Lupiezca() : Przeciwnik(
                "Lupiezca",
                85,
                8,
                0.0f,      // brak pancerza
                0.0f,
                0.2f,      // 20% szansy na unik
                3
            ) {}
            ~Lupiezca() override = default;
        };
        
        class Grzybol : public Przeciwnik {
        public:
            Grzybol() : Przeciwnik(
                "Grzybol",
                35,
                5,
                0.05f,     // 5% pancerza
                0.0f,
                0.05f,     // 5% szansy na unik
                2
            ) {}
            ~Grzybol() override = default;
        };
    
    class StraznikKorzeni : public Przeciwnik {
    private:
        int tura = 0;
        
    public:
        StraznikKorzeni() : Przeciwnik(
            "Straznik korzeni",
            200,
            15,
            0.15f,   // 15% pancerza
            0.0f,
            0.0f,
            3
        ) {
            maxHp = 200; // Nadpisanie, bo konstruktor bazowy ustawia maxHp = hp
        }
        ~StraznikKorzeni() override = default;
    
        int obliczObrazenia(int obrazeniaAtakujacego) override {
            tura++;
            if (tura % 4 == 0) {
                int leczenie = static_cast<int>(0.15f * maxHp);
                hp = std::min(hp + leczenie, maxHp);
                std::cout << nazwa << " uzywa Fotosyntezy i leczy sie o " << leczenie << " HP!\n";
            }
            
            // Standardowe obliczenie obrażeń z pancerza
            float redukcja = std::max(0.0f, pancerz - penetracjaPancerza);
            int obrazenia = static_cast<int>(obrazeniaAtakujacego * (1.0f - redukcja));
            return std::max(1, obrazenia);
        }
    
        int atak() {
            int wybor = rand() % 2;
            if (wybor == 0) {
                std::cout << nazwa << " uzywa Ciosu Mlotem!\n";
                return 15;
            } else {
                std::cout << nazwa << " uzywa Gwaltownych Korzeni!\n";
                return 10;
            }
        }
    };

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

    void wyswietlStatystyki(bool czyscEkran = true) {
        if (czyscEkran) clearScreen();
        cout << "Statystyki postaci (" << nazwa << "):\n";
        cout << "HP: " << hp << "/" << maxHp << "\n";
        cout << "Pancerz: " << static_cast<int>(pancerz * 100) << "%\n";
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
        // Oblicz redukcję obrażeń przez pancerz (np. 15% pancerza = 15% mniej obrażeń)
        float redukcja = pancerz; // pancerz jest wartością 0-1 (np. 0.15 dla 15%)
        int obrazenia = obrazeniaAtakujacego * (1.0f - redukcja);
        return max(1, obrazenia); // Zawsze przynajmniej 1 obrażenie
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
        obrazenia = 200;
        penetracjaPancerza = 0.15f;
        szansaNaUnikBierny = 0.35f;
        unikManualny = 0.75f;
        szansaNaUcieczke = 0.50f;
    }
};

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

void wyswietlInterfejsWalki(const Postac& gracz, const vector<Przeciwnik*>& przeciwnicy) {
    clearScreen(); // Zawsze czyść ekran przed wyświetleniem
    cout << "\n=== " << gracz.nazwa << " (HP: " << gracz.hp << "/" << gracz.maxHp << ") ===";
    cout << "\nMikstury: " << gracz.mikstury << "\n";
    cout << "Przeciwnicy:\n";
    for (size_t i = 0; i < przeciwnicy.size(); ++i) {
        if (przeciwnicy[i]) {
            cout << i+1 << ". " << przeciwnicy[i]->nazwa 
                 << " (HP: " << przeciwnicy[i]->hp << "/" 
                 << przeciwnicy[i]->maxHp << ")\n";
        }
    }
}

void walka(Postac &gracz, vector<Przeciwnik*> &przeciwnicy) {
    int expSuma = 0;
    int zlotoSuma = 0;
    bool ucieczka = false;

    try {
        if (przeciwnicy.empty()) {
            cout << "Brak przeciwnikow do walki!\n";
            czekajNaKlawisz();
            return;
        }

        while (gracz.hp > 0 && !przeciwnicy.empty() && !ucieczka) {
            // --- TURA GRACZA ---
            clearScreen();
            cout << "\n=== " << gracz.nazwa << " (HP: " << gracz.hp << "/" << gracz.maxHp << ") ===";
            cout << "\nMikstury: " << gracz.mikstury << "\n";
            cout << "Przeciwnicy:\n";
            
            for (size_t i = 0; i < przeciwnicy.size(); ++i) {
                if (przeciwnicy[i] != nullptr) {
                    cout << i+1 << ". " << przeciwnicy[i]->nazwa 
                         << " (HP: " << przeciwnicy[i]->hp << "/" 
                         << przeciwnicy[i]->maxHp << ")\n";
                }
            }

            // Menu akcji
            int wybor = 0;
            bool akcjaWykonana = false;
            
            while (!akcjaWykonana) {
                cout << "\nWybierz akcje:\n";
                cout << "1. Atakuj\n";
                cout << "2. Unik (szansa: " << gracz.unikManualny*100 << "%)\n";
                cout << "3. Ucieczka (szansa: " << gracz.szansaNaUcieczke*100 << "%)\n";
                cout << "4. Statystyki\n";
                cout << "5. Uzyj mikstury\n";
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
                            cout << cel->nazwa << " unika ataku!\n";
                        } else {
                            int obrazenia = gracz.obliczObrazenia(gracz.obrazenia);
                            cel->hp -= obrazenia;
                            cout << "Zadales " << obrazenia << " obrazen " << cel->nazwa << "!\n";
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
                        
                        // Wyświetl statystyki z czyszczeniem ekranu
                        clearScreen(); // Dodane czyszczenie ekranu przed wyświetleniem statystyk
                        gracz.wyswietlStatystyki(false); // false = nie czyść ekranu ponownie
                        
                        cout << "\n=== STATYSTYKI PRZECIWNIKOW ===\n";
                        for (auto p : przeciwnicy) {
                            if (p) {
                                cout << "Przeciwnik: " << p->nazwa << "\n";
                                cout << "HP: " << p->hp << "/" << p->maxHp << "\n";
                                cout << "Pancerz: " << static_cast<int>(p->pancerz * 100) << "%\n";
                                cout << "Obrazenia: " << p->obrazenia << "\n\n";
                            }
                        }
                        
                        czekajNaKlawisz();
                        
                        // Pełne odtworzenie interfejsu walki
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

            if (gracz.hp <= 0) break;

            czekajNaKlawisz();
        }
    }
    catch (const exception& e) {
        cerr << "Krytyczny błąd walki: " << e.what() << endl;
        for (auto p : przeciwnicy) {
            if (p != nullptr) delete p;
        }
        przeciwnicy.clear();
    }

    // Podsumowanie walki
    clearScreen();
    cout << "\n=== PODSUMOWANIE WALKI ===\n";
    gracz.wyswietlStatystyki();
    czekajNaKlawisz();
}

// Funkcja do losowania jednego przeciwnika
vector<Przeciwnik*> losujPrzeciwnikow() {
    vector<Przeciwnik*> przeciwnicy;
    try {
        int liczba = 2 + rand() % 2; // Mniej przeciwników: 2-3
        for (int i = 0; i < liczba; ++i) {
            Przeciwnik* nowy = nullptr;
            int typ = rand() % 4; // Ogranicz typy
            switch(typ) {
                case 0: nowy = new Szlam(); break;
                case 1: nowy = new Wilk(); break;
                case 2: nowy = new Wilkolak(); break;
                case 3: nowy = new Grzybol(); break;
            }
            if (nowy) przeciwnicy.push_back(nowy);
        }
    }
    catch (...) {
        for (auto p : przeciwnicy) delete p;
        throw;
    }
    return przeciwnicy;
}

struct Pokoj {
    string nazwa;
    bool odwiedzony = false;
    bool pokonany = false;
    vector<Przeciwnik*> przeciwnicy;
    map<string, Pokoj*> polaczenia;
    function<bool(Postac&)> specjalnaAkcja = nullptr;
    
    Pokoj(const string& nazwa) : nazwa(nazwa) {}
    
    ~Pokoj() {
        for (auto p : przeciwnicy) {
            delete p;
        }
    }
    
    void dodajPolaczenie(const string& kierunek, Pokoj* cel) {
        polaczenia[kierunek] = cel;
    }
};

Pokoj* aktualnyPokoj = nullptr;

void rozszerzMape(Pokoj* skrzyzowanie2, vector<Pokoj*>& wszystkiePokoje) {
    try {
        // Tworzenie pokoi
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

        // Dodawanie przeciwników z zabezpieczeniem
        auto dodajPrzeciwnikow = [](Pokoj* pokoj, bool miniboss = false) {
            if (miniboss) {
                pokoj->przeciwnicy = {new StraznikKorzeni()};
            } else {
                pokoj->przeciwnicy = losujPrzeciwnikow();
                if (pokoj->przeciwnicy.empty()) {
                    pokoj->przeciwnicy.push_back(new Szlam());
                }
            }
        };

        dodajPrzeciwnikow(komnata1);
        dodajPrzeciwnikow(komnata2);
        dodajPrzeciwnikow(komnata3);
        dodajPrzeciwnikow(czaszka1, true); // Miniboss
        boss->przeciwnicy.clear(); // Pokój bossa początkowo pusty

        // Dodanie do listy pokoi
        wszystkiePokoje.reserve(wszystkiePokoje.size() + 6);
        wszystkiePokoje.push_back(fontanna);
        wszystkiePokoje.push_back(komnata1);
        wszystkiePokoje.push_back(komnata2);
        wszystkiePokoje.push_back(czaszka1);
        wszystkiePokoje.push_back(komnata3);
        wszystkiePokoje.push_back(boss);

    } catch (const exception& e) {
        cerr << "Blad podczas rozszerzania mapy: " << e.what() << endl;
    }
}

void rozszerzMapeDalej(Pokoj* skrzyzowanie1, vector<Pokoj*>& wszystkiePokoje) {
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
        auto dodajPrzeciwnikow = [](Pokoj* pokoj) {
            pokoj->przeciwnicy = losujPrzeciwnikow();
            if (pokoj->przeciwnicy.empty()) {
                pokoj->przeciwnicy.push_back(new Szlam());
            }
        };

        dodajPrzeciwnikow(komnata4);
        dodajPrzeciwnikow(komnata5);
        dodajPrzeciwnikow(komnata6);
        dodajPrzeciwnikow(komnata7);
        czaszka2->przeciwnicy.clear(); // Początkowo pusty

        // Dodanie do listy pokoi
        wszystkiePokoje.reserve(wszystkiePokoje.size() + 6);
        wszystkiePokoje.push_back(komnata4);
        wszystkiePokoje.push_back(komnata5);
        wszystkiePokoje.push_back(komnata6);
        wszystkiePokoje.push_back(sklep);
        wszystkiePokoje.push_back(komnata7);
        wszystkiePokoje.push_back(czaszka2);

    } catch (const exception& e) {
        cerr << "Blad podczas rozszerzania mapy: " << e.what() << endl;
    }
}

void inicjalizujMape(Pokoj*& startowy, vector<Pokoj*>& wszystkiePokoje) {
    // Podstawowe pokoje
    startowy = new Pokoj("Start");
    Pokoj* lewy = new Pokoj("Lewy");
    Pokoj* prawy = new Pokoj("Prawy");
    Pokoj* przod = new Pokoj("Przod");
    Pokoj* relikwie = new Pokoj("Relikwie");
    Pokoj* skrzyzowanie1 = new Pokoj("Skrzyzowanie1");
    Pokoj* skrzyzowanie2 = new Pokoj("Skrzyzowanie2");
    
    // Podstawowe połączenia
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
    lewy->przeciwnicy = losujPrzeciwnikow();
    prawy->przeciwnicy = losujPrzeciwnikow();
    przod->przeciwnicy = losujPrzeciwnikow();
    skrzyzowanie1->przeciwnicy = losujPrzeciwnikow();
    skrzyzowanie2->przeciwnicy = losujPrzeciwnikow();

    // Inicjalizacja
    wszystkiePokoje = {startowy, lewy, prawy, przod, relikwie, skrzyzowanie1, skrzyzowanie2};
    
    // Rozszerzenia
    rozszerzMape(skrzyzowanie2, wszystkiePokoje);
    rozszerzMapeDalej(skrzyzowanie1, wszystkiePokoje);
}

void rozpocznijGre() {
    // Inicjalizacja
    srand(time(nullptr));
    Postac gracz = wybierzPostac();
    bool graTrwa = true;

    // Inicjalizacja mapy
    Pokoj* startowy = nullptr;
    vector<Pokoj*> wszystkiePokoje;
    inicjalizujMape(startowy, wszystkiePokoje);
    aktualnyPokoj = startowy;

    // Główna pętla gry
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

        // Specjalna akcja pokoju
        if (aktualnyPokoj->specjalnaAkcja && !aktualnyPokoj->odwiedzony) {
            if (aktualnyPokoj->specjalnaAkcja(gracz)) {
                aktualnyPokoj->odwiedzony = true;
            }
        }

        // Walka z przeciwnikami
        if (!aktualnyPokoj->pokonany && !aktualnyPokoj->przeciwnicy.empty()) {
            walka(gracz, aktualnyPokoj->przeciwnicy);
            
            if (gracz.hp <= 0) {
                graTrwa = false;
                continue;
            }

            if (aktualnyPokoj->przeciwnicy.empty()) {
                aktualnyPokoj->pokonany = true;
                int zdobyteZloto = 20 + rand() % 30;
                gracz.zloto += zdobyteZloto;
                cout << "\nZdobyto " << zdobyteZloto << " zlota za pokonanie przeciwnikow!\n";
                czekajNaKlawisz();
            }
        }

        // Menu nawigacji
        while (graTrwa) {
            clearScreen();
            cout << "================================\n";
            cout << "  " << aktualnyPokoj->nazwa << "\n";
            cout << "================================\n";
            cout << "HP: " << gracz.hp << "/" << gracz.maxHp << " | ";
            cout << "Mikstury: " << gracz.mikstury << "\n\n";

            // Wyświetl dostępne kierunki
            cout << "Dostepne akcje:\n";
            int opcja = 1;
            map<int, string> kierunki;

            for (const auto& polaczenie : aktualnyPokoj->polaczenia) {
                cout << opcja << ". Idz na " << polaczenie.first << " (" << polaczenie.second->nazwa << ")\n";
                kierunki[opcja] = polaczenie.first;
                opcja++;
            }

            // Inne opcje
            cout << opcja++ << ". Statystyki postaci\n";
            if (gracz.mikstury > 0) {
                int leczenie = gracz.maxHp * 0.2; // Poprawione na 20% zgodnie z metodą uzyjMikstury
                cout << opcja++ << ". Uzyj mikstury (+" << leczenie << " HP)\n";
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
                continue;
            }

            // Obsługa wyboru
            if (wybor >= 1 && wybor <= kierunki.size()) {
                // Nawigacja
                string wybranyKierunek = kierunki[wybor];
                aktualnyPokoj = aktualnyPokoj->polaczenia.at(wybranyKierunek);
                break;
            }
            else if (wybor == kierunki.size() + 1) {
                // Statystyki
                clearScreen();
                gracz.wyswietlStatystyki();
                czekajNaKlawisz();
            }
            else if (wybor == kierunki.size() + 2 && gracz.mikstury > 0) {
                // Mikstura
                gracz.uzyjMikstury();
                czekajNaKlawisz();
                break;
            }
            else if (wybor == kierunki.size() + (gracz.mikstury > 0 ? 3 : 2)) {
                // Wyjście z gry
                cout << "\nCzy na pewno chcesz wyjsc? (T/N): ";
                char potwierdzenie;
                cin >> potwierdzenie;
                if (tolower(potwierdzenie) == 't') {
                    graTrwa = false;
                    break;
                }
            }
            else {
                cout << "Nieprawidlowy wybor!\n";
                czekajNaKlawisz();
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
