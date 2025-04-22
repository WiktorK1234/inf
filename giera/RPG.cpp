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

class Przeciwnik;
class Postac;

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
    
        // Konstruktor główny
        Przeciwnik(string n, int h, int dmg, float p, float pen, float unik, int lvl)
            : nazwa(move(n)), hp(h), maxHp(h), obrazenia(dmg), 
              pancerz(clamp(p, 0.0f, 0.8f)),
              penetracjaPancerza(clamp(pen, 0.0f, 0.5f)),
              szansaNaUnik(clamp(unik, 0.0f, 0.5f)), poziom(lvl) {}
    
        virtual ~Przeciwnik() = default;
    
        // Funkcje pomocnicze do skalowania
    private:
        void resetujBazoweStatystyki() {
            if (nazwa == "Wilk") {
                maxHp = 35; obrazenia = 5; pancerz = 0.0f;
                penetracjaPancerza = 0.0f; szansaNaUnik = 0.05f;
            }
            else if (nazwa == "Grzybol") {
                maxHp = 35; obrazenia = 5; pancerz = 0.05f;
                penetracjaPancerza = 0.0f; szansaNaUnik = 0.05f;
            }
            else if (nazwa == "Szlam") {
                maxHp = 30; obrazenia = 5; pancerz = 0.2f;
                penetracjaPancerza = 0.0f; szansaNaUnik = 0.0f;
            }
            else if (nazwa == "Wilkolak") {
                maxHp = 50; obrazenia = 10; pancerz = 0.05f;
                penetracjaPancerza = 0.0f; szansaNaUnik = 0.05f;
            }
            else if (nazwa == "Lupiezca") {
                maxHp = 85; obrazenia = 8; pancerz = 0.0f;
                penetracjaPancerza = 0.0f; szansaNaUnik = 0.2f;
            }
            else if (nazwa == "Golem") {
                maxHp = 100; obrazenia = 12; pancerz = 0.15f;
                penetracjaPancerza = 0.0f; szansaNaUnik = 0.0f;
            }
            else if (nazwa == "Straznik korzeni") {
                maxHp = 200; obrazenia = 15; pancerz = 0.15f;
                penetracjaPancerza = 0.0f; szansaNaUnik = 0.0f;
            }
            
            hp = maxHp;
        }
    
        virtual void resetujSpecjalneStatystyki() {} // Domyślnie puste
    
    public:
    void skalujDoPoziomu(int poziomGracza) {
        resetujBazoweStatystyki();
        resetujSpecjalneStatystyki();
        
        int roznica = poziom - poziomGracza;
        
        // Przeciwnik tego samego poziomu - podstawowe statystyki
        if (roznica == 0) {
            return;
        }
        // Przeciwnik niższy poziomem - zwiększ statystyki
        else if (roznica < 0) {
            float mnoznik = pow(1.1f, abs(roznica));
            maxHp = static_cast<int>(maxHp * mnoznik);
            obrazenia = static_cast<int>(obrazenia * mnoznik);
            pancerz = clamp(pancerz * (mnoznik * 0.8f), 0.0f, 0.8f);
        }
        // Przeciwnik wyższy poziomem - zmniejsz statystyki
        else if (roznica > 0) {
            float mnoznik = pow(1.25f, roznica);
            maxHp = max(1, static_cast<int>(maxHp / mnoznik));
            obrazenia = max(1, static_cast<int>(obrazenia / mnoznik));
            pancerz = clamp(pancerz / (mnoznik * 0.8f), 0.0f, 0.8f);
        }
        
        hp = maxHp;
        penetracjaPancerza = clamp(penetracjaPancerza, 0.0f, 0.5f);
        szansaNaUnik = clamp(szansaNaUnik, 0.0f, 0.5f);
    }
    
        // Funkcje wspólne dla wszystkich przeciwników
        void wyswietlStatystyki(int poziomGracza) const {
            cout << "=== " << nazwa << " (lvl " << poziom;
            if (poziom != poziomGracza) {
                cout << " [" << (poziom > poziomGracza ? "+" : "-") 
                     << abs(poziom - poziomGracza) << "]";
            }
            cout << ") ===\n";
            cout << "HP: " << hp << "/" << maxHp << "\n";
            cout << "Obrażenia: " << obrazenia << "\n";
            cout << "Pancerz: " << static_cast<int>(pancerz * 100) << "%\n";
            cout << "Penetracja: " << static_cast<int>(penetracjaPancerza * 100) << "%\n";
            cout << "Unik: " << static_cast<int>(szansaNaUnik * 100) << "%\n\n";
        }
    
        bool czyUnik() const {
            return (static_cast<float>(rand()) / RAND_MAX) < szansaNaUnik;
        }
    
        virtual int obliczObrazenia(int obrazeniaAtakujacego) {
            float redukcja = max(0.0f, pancerz - min(penetracjaPancerza, pancerz));
            return max(1, static_cast<int>(obrazeniaAtakujacego * (1.0f - redukcja)));
        }
    
        virtual int atak() {
            return obrazenia;
        }
    };
    
    // Klasy pochodne
    class Wilk : public Przeciwnik {
    public:
        Wilk() : Przeciwnik("Wilk", 35, 5, 0.0f, 0.0f, 0.05f, 1) {}
    };
    
    class Grzybol : public Przeciwnik {
    public:
        Grzybol() : Przeciwnik("Grzybol", 35, 5, 0.05f, 0.0f, 0.05f, 1) {}
    };
    
    class Szlam : public Przeciwnik {
    public:
        Szlam() : Przeciwnik("Szlam", 30, 5, 0.2f, 0.0f, 0.0f, 1) {}
    };
    
    class Wilkolak : public Przeciwnik {
    public:
        Wilkolak() : Przeciwnik("Wilkolak", 50, 10, 0.05f, 0.0f, 0.05f, 2) {}
    };
    
    class Lupiezca : public Przeciwnik {
    public:
        Lupiezca() : Przeciwnik("Lupiezca", 85, 8, 0.0f, 0.0f, 0.2f, 2) {}
    };
    
    class Golem : public Przeciwnik {
    public:
        Golem() : Przeciwnik("Golem", 100, 12, 0.15f, 0.0f, 0.0f, 2) {}
    };
    
    class StraznikKorzeni : public Przeciwnik {
        int tura = 0;
    public:
        StraznikKorzeni() : Przeciwnik("Straznik korzeni", 200, 15, 0.15f, 0.0f, 0.0f, 3) {}
        
        void resetujSpecjalneStatystyki() override {
            tura = 0;
        }
        
        int obliczObrazenia(int obrazeniaAtakujacego) override {
            tura++;
            if (tura % 4 == 0) {
                int leczenie = static_cast<int>(0.15f * maxHp);
                hp = min(hp + leczenie, maxHp);
                cout << nazwa << " leczy się o " << leczenie << " HP!\n";
            }
            return Przeciwnik::obliczObrazenia(obrazeniaAtakujacego);
        }
    
        int atak() override {
            if (rand() % 2 == 0) {
                cout << nazwa << " używa Ciosu Młotem!\n";
                return obrazenia + 3;
            }
            cout << nazwa << " używa Gwałtownych Korzeni!\n";
            return obrazenia;
        }
    };
    
    Przeciwnik* stworzPrzeciwnika(int typ, int poziomGracza) {
        if (typ < 0 || typ > 6) return nullptr;

        Przeciwnik* nowy = nullptr;
        
        switch(typ) {
            case 0: nowy = new Szlam(); break;
            case 1: nowy = new Wilk(); break;
            case 2: nowy = new Wilkolak(); break;
            case 3: nowy = new Grzybol(); break;
            case 4: nowy = new Golem(); break;
            case 5: nowy = new Lupiezca(); break;
            case 6: nowy = new StraznikKorzeni(); break;
        }
    
        if (!nowy) return nullptr;
    
        // Losowe wzmocnienie tylko jeśli poziom przeciwnika jest <= poziomowi gracza
    // i gracz ma przynajmniej poziom 2 (żeby nie wzmacniać na samym początku)
    if (nowy->poziom <= poziomGracza && poziomGracza >= 2 && (rand() % 100) < 30) {
        nowy->poziom = min(poziomGracza + 1, 10);
    }
    
    nowy->skalujDoPoziomu(poziomGracza);
    return nowy;
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
        
        void aktualizujPoziomPrzeciwnikow(vector<Pokoj*>& wszystkiePokoje, int poziomGracza) {
            for (auto pokoj : wszystkiePokoje) {
                for (auto przeciwnik : pokoj->przeciwnicy) {
                    if (przeciwnik) {
                        przeciwnik->skalujDoPoziomu(poziomGracza);
                    }
                }
            }
        }
        
        void zdobadzDoswiadczenie(int ilosc, vector<Pokoj*>& wszystkiePokoje) {
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
                czekajNaKlawisz();
            }
        
            if (poziom > staryPoziom) {
                aktualizujPoziomPrzeciwnikow(wszystkiePokoje, poziom);
            }
        }

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
        cout << "Pancerz: " << static_cast<int>(pancerz * 100) << "%\n";
        cout << "Penetracja pancerza: " << static_cast<int>(penetracjaPancerza * 100) << "%\n";
        
        // Statystyki specjalne
        cout << "\n=== SPECJALNE ===\n";
        cout << "Szansa na unik bierny: " << static_cast<int>(szansaNaUnikBierny * 100) << "%\n";
        cout << "Unik manualny: " << static_cast<int>(unikManualny * 100) << "%\n";
        cout << "Szansa na ucieczke: " << static_cast<int>(szansaNaUcieczke * 100) << "%\n";
        
        // Ekwipunek
        cout << "\n=== EKWIPUNEK ===\n";
        cout << "Zloto: " << zloto << "\n";
        cout << "Mikstury: " << mikstury << "\n";
        cout << "================================\n";
    }

    bool czyUnikBierny() {
        // Dodaj zabezpieczenie przed zbyt wysokim sumarycznym unikiem
        if (szansaNaUnikBierny + unikManualny > 0.8f) {
            szansaNaUnikBierny = 0.8f - unikManualny;
        }
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
    clearScreen();
    cout << "\n=== " << gracz.nazwa << " (lvl " << gracz.poziom << ", HP: " << gracz.hp << "/" << gracz.maxHp << ") ===";
    cout << "\nMikstury: " << gracz.mikstury << "\n";
    cout << "Przeciwnicy:\n";
    
    int licznik = 1;
    for (size_t i = 0; i < przeciwnicy.size(); ++i) {
        if (przeciwnicy[i] && przeciwnicy[i]->hp > 0) {
            int roznica = przeciwnicy[i]->poziom - gracz.poziom;
            cout << (i+1) << ". " << przeciwnicy[i]->nazwa << " (lvl " << przeciwnicy[i]->poziom;
            
            // Poprawne wyświetlanie różnicy poziomów
            if (roznica != 0) {
                cout << " [" << (roznica > 0 ? "+" : "") << roznica << "]";
            }
            
            cout << ", HP: " << przeciwnicy[i]->hp << "/" << przeciwnicy[i]->maxHp << ")\n";
        }
    }
}

void walka(Postac &gracz, vector<Przeciwnik*> &przeciwnicy, vector<Pokoj*>& wszystkiePokoje) {
    int expSuma = 0;
    int zlotoSuma = 0;
    bool ucieczka = false;

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
            przeciwnicy.erase(
                remove_if(przeciwnicy.begin(), przeciwnicy.end(),
                    [](Przeciwnik* p) {
                        if (p && p->hp <= 0) {
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
                            
                            if (cel->hp <= 0) {
                                cout << cel->nazwa << " pokonany!\n";
                                
                                // Obliczanie EXP - podobny wzór jak dla złota
                                int losowyBonusExp = rand() % 11 + 5; // 5-15%
                                int bazaExp = cel->poziom * (cel->obrazenia * 2 + cel->maxHp / 3);
                                expSuma += bazaExp + (bazaExp * losowyBonusExp / 100);
                                
                                // Twoja oryginalna formuła na złoto z poprawkami
                                int losowyBonusZloto = rand() % 11 + 5; // 5-15%
                                int bazaZloto = cel->poziom * (cel->obrazenia + cel->maxHp / 2);
                                zlotoSuma += bazaZloto + (bazaZloto * losowyBonusZloto / 100);
                                
                                // Specjalne bonusy dla unikalnych przeciwników
                                if (cel->nazwa == "Straznik korzeni") {
                                    expSuma += 100; // dodatkowy bonus
                                    zlotoSuma += 50;
                                }
                            }
                        }
                        akcjaWykonana = true;
                        czekajNaKlawisz();
                        break;
                    }
                    // ... (reszta przypadków switch pozostaje bez zmian)
                }
            }

            if (gracz.hp <= 0 || przeciwnicy.empty() || ucieczka) break;

            // --- TURA PRZECIWNIKA ---
            for (size_t i = 0; i < przeciwnicy.size(); ++i) {
                if (przeciwnicy[i] && przeciwnicy[i]->hp > 0) {
                    if (gracz.czyUnikBierny()) {
                        cout << "Unikles ataku " << przeciwnicy[i]->nazwa << "!\n";
                    } else {
                        int obrazenia = przeciwnicy[i]->obliczObrazenia(przeciwnicy[i]->obrazenia);
                        gracz.hp -= obrazenia;
                        cout << przeciwnicy[i]->nazwa << " zadal ci " << obrazenia << " obrazen!\n";
                        
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
                        if (p && p->hp <= 0) {
                            delete p;
                            return true;
                        }
                        return false;
                    }),
                przeciwnicy.end()
            );
            
            czekajNaKlawisz();
        }

        // Podsumowanie walki
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
            if (p != nullptr) delete p;
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
        
        Przeciwnik* p = stworzPrzeciwnika(typ, poziomGracza);
        if (p) {
            przeciwnicy.push_back(p);
        }
    }
    
    for (auto p : przeciwnicy) {
        if (p) {
            p->skalujDoPoziomu(poziomGracza);
        }
    }
    
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
                pokoj->przeciwnicy[0]->poziom = poziomGracza + 2;
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
                pokoj->przeciwnicy[0]->poziom = poziomGracza + 2;
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
