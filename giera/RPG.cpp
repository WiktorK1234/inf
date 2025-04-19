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
    float pancerz;
    int obrazenia;
    float penetracjaPancerza;
    float szansaNaUnikBierny;
    float unikManualny;
    float szansaNaUcieczke;

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
    float pancerz;
    int obrazenia;
    float szansaNaUnik;

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

vector<Przeciwnik*> losujGrupePrzeciwnikow() {
    vector<Przeciwnik*> grupa;
    int liczbaPrzeciwnikow;
    
    // Losowanie typu przeciwnika
    int typPrzeciwnika = rand() % 2;
    
    if (typPrzeciwnika == 0) { // Szlamy
        liczbaPrzeciwnikow = 3 + rand() % 3; // 3-5 szlamów
        for (int i = 0; i < liczbaPrzeciwnikow; i++) {
            grupa.push_back(new Szlam());
        }
    } else { // Wilkołaki
        liczbaPrzeciwnikow = 1 + rand() % 2; // 1-2 wilkołaki
        for (int i = 0; i < liczbaPrzeciwnikow; i++) {
            grupa.push_back(new Wilkolak());
        }
    }
    
    return grupa;
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
    while (gracz.hp > 0 && !przeciwnicy.empty()) {
        clearScreen();
        cout << "\n--- Tura gracza (" << gracz.nazwa << ") ---\n";
        cout << "Wybierz akcje:\n";
        cout << "1. Atakuj\n";
        cout << "2. Unikaj manualnie\n";
        cout << "3. Sprobuj uciec\n";
        cout << "4. Sprawdz statystyki wszystkich postaci\n";
        cout << "5. Sprawdz statystyki wszystkich przeciwnikow\n";
        cout << "Wybor: ";
        int wybor;
        cin >> wybor;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool akcjaWykonana = false;
        if (wybor == 1) {
            cout << gracz.nazwa << " atakuje!\n";
            for (size_t i = 0; i < przeciwnicy.size(); ++i) {
                Przeciwnik* przeciwnik = przeciwnicy[i];
                if (przeciwnik->czyUnik()) {
                    cout << "Przeciwnik " << przeciwnik->nazwa << " udalo sie uniknac ataku!\n";
                } else {
                    int obrazeniaGracza = gracz.obliczObrazenia(gracz.obrazenia);
                    przeciwnik->hp -= obrazeniaGracza;
                    cout << gracz.nazwa << " zadal " << obrazeniaGracza << " obrazen przeciwnikowi " << przeciwnik->nazwa << "!\n";
                }
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
            continue;
        } else if (wybor == 5) {
            clearScreen();
            for (size_t i = 0; i < przeciwnicy.size(); ++i) {
                przeciwnicy[i]->wyswietlStatystyki();
                cout<<endl;
            }
            czekajNaKlawisz();
            continue;
        } else {
            cout << "Nieprawidlowy wybor! Sprobuj ponownie.\n";
            czekajNaKlawisz();
            continue;
        }

        if (!akcjaWykonana) continue;

        // Sprawdzanie, czy przeciwnik zostal pokonany
        for (size_t i = 0; i < przeciwnicy.size(); ++i) {
            if (przeciwnicy[i]->hp <= 0) {
                cout << "Przeciwnik " << przeciwnicy[i]->nazwa << " zostal pokonany!\n";
                delete przeciwnicy[i];
                przeciwnicy.erase(przeciwnicy.begin() + i);
                --i; // Zmniejszamy indeks, aby nie pominąć następnego przeciwnika po usunięciu
            }
        }

        if (przeciwnicy.empty()) {
            cout << "Wszyscy przeciwnicy zostali pokonani!\n";
            break;
        }

        czekajNaKlawisz();

        clearScreen();
        cout << "\n--- Tura przeciwników ---\n";
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
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<Przeciwnik*> przeciwnicy = losujGrupePrzeciwnikow();
Postac gracz = wybierzPostac();
walka(gracz, przeciwnicy);

    if (gracz.hp <= 0) {
        cout << "\nKoniec gry – postać została pokonana!" << endl;
   }

// Czyszczenie pamięci (jeśli nie używasz smart pointerów)
for (size_t i = 0; i < przeciwnicy.size(); ++i) {
    delete przeciwnicy[i];
}

    return 0;
}
