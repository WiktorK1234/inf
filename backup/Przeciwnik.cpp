#include "Przeciwnik.h"
#include "Postac.h"
#include "EfektyStatusowe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <new>  // Dla std::nothrow

using namespace std;

// Implementacja klasy bazowej Przeciwnik
Przeciwnik::Przeciwnik(string n, int h, int dmg, float p, float pen, float unik, int lvl)
    : nazwa(move(n)), hp(h), maxHp(h), obrazenia(dmg), 
      pancerz(clamp(p, 0.0f, 0.8f)),
      penetracjaPancerza(clamp(pen, 0.0f, 0.5f)),
      szansaNaUnik(clamp(unik, 0.0f, 0.5f)), 
      poziom(lvl), czyBoss(false), czyMiniBoss(false) {}

void Przeciwnik::zmniejszObrazenia(float mnoznik) {
    obrazenia = static_cast<int>(obrazenia * mnoznik);
}

void Przeciwnik::zmniejszPancerz(float mnoznik) {
    pancerz = clamp(pancerz * mnoznik, 0.0f, 0.8f);
}

bool Przeciwnik::czyUnik() const {
    return (static_cast<float>(rand())) / RAND_MAX < szansaNaUnik;
}

int Przeciwnik::obliczObrazenia(int bazowe) const {
    return static_cast<int>(bazowe * (1.0f - pancerz * (1.0f - penetracjaPancerza)));
}

void Przeciwnik::resetujBazoweStatystyki() {
    // Implementacja domyślna - nic nie robi
    // Klasy pochodne powinny nadpisać tę metodę
}

void Przeciwnik::skalujDoPoziomu(int poziomGracza) {
    resetujBazoweStatystyki();
    resetujSpecjalneStatystyki();
    
    int roznica = poziom - poziomGracza;
    
    if (roznica == 0) {
        return;
    }
    else if (roznica < 0) {
        float mnoznik = pow(1.1f, abs(roznica));
        maxHp = static_cast<int>(maxHp * mnoznik);
        obrazenia = static_cast<int>(obrazenia * mnoznik);
        pancerz = clamp(pancerz * (mnoznik * 0.8f), 0.0f, 0.8f);
    }
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

void Przeciwnik::wyswietlStatystyki(int poziomGracza) const {
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

// Implementacje klas pochodnych
Szlam::Szlam() : Przeciwnik("Szlam", 30, 5, 0.2f, 0.0f, 0.0f, 1) {}

void Szlam::atakuj(Postac& gracz) {
    int obrazenia = obliczObrazenia(this->pobierzObrazenia());
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " opluwa cie lepka mazia, zadajac " << obrazenia << " obrazen!\n";
    
    if (rand() % 100 < 20) {
        spowolnienie(&gracz);
        cout << "Lepka substancja spowalnia twoje ruchy!\n";
    }
}

void Szlam::resetujBazoweStatystyki() {
    ustawMaxHp(30);
    ustawObrazenia(5);
    ustawPancerz(0.2f);
    ustawPenetracjePancerza(0.0f);
    ustawSzanseNaUnik(0.0f);
}

Wilk::Wilk() : Przeciwnik("Wilk", 35, 5, 0.0f, 0.0f, 0.05f, 1) {}

void Wilk::atakuj(Postac& gracz) {
    int obrazenia = obliczObrazenia(pobierzObrazenia());
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " gryzie cie, zadajac " << obrazenia << " obrazen!\n";
    
    if (rand() % 100 < 15) {
        krwawienie(&gracz);
    }
}

void Wilk::resetujBazoweStatystyki() {
    ustawMaxHp(35);
    ustawObrazenia(5);
    ustawPancerz(0.0f);
    ustawPenetracjePancerza(0.0f);
    ustawSzanseNaUnik(0.05f);
}

Wilkolak::Wilkolak() : Przeciwnik("Wilkolak", 50, 10, 0.05f, 0.0f, 0.05f, 2) {}

void Wilkolak::atakuj(Postac& gracz) {
    int obrazenia = obliczObrazenia(this->pobierzObrazenia());
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " szarpie cie pazurami, zadajac " << obrazenia << " obrazen!\n";
    
    if (rand() % 100 < 20) {
        krwawienie(&gracz);
    }
}

void Wilkolak::resetujBazoweStatystyki() {
    ustawMaxHp(50);
    ustawObrazenia(10);
    ustawPancerz(0.05f);
    ustawPenetracjePancerza(0.0f);
    ustawSzanseNaUnik(0.05f);
}

Grzybol::Grzybol() : Przeciwnik("Grzybol", 35, 5, 0.05f, 0.0f, 0.05f, 2) {}

void Grzybol::atakuj(Postac& gracz) {
    int obrazenia = obliczObrazenia(this->pobierzObrazenia());
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " zaraza cie zarodnikami, zadajac " << obrazenia << " obrazen!\n";
    
    if (rand() % 100 < 25) {
        zatrucie(&gracz);
    }
}

void Grzybol::resetujBazoweStatystyki() {
    ustawMaxHp(35);
    ustawObrazenia(5);
    ustawPancerz(0.05f);
    ustawPenetracjePancerza(0.0f);
    ustawSzanseNaUnik(0.05f);
}

Golem::Golem() : Przeciwnik("Golem", 100, 12, 0.15f, 0.0f, 0.0f, 2) {}

void Golem::atakuj(Postac& gracz) {
    int obrazenia = obliczObrazenia(this->pobierzObrazenia());
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " uderza cie kamienna piescia, zadajac " << obrazenia << " obrazen!\n";
    
    if (rand() % 10 == 0) {
        ogluszenie(&gracz);
    }
}

void Golem::resetujBazoweStatystyki() {
    ustawMaxHp(100);
    ustawObrazenia(12);
    ustawPancerz(0.15f);
    ustawPenetracjePancerza(0.0f);
    ustawSzanseNaUnik(0.0f);
}

Lupiezca::Lupiezca() : Przeciwnik("Lupiezca", 85, 8, 0.0f, 0.0f, 0.2f, 2) {}

void Lupiezca::atakuj(Postac& gracz) {
    int obrazenia = obliczObrazenia(this->pobierzObrazenia());
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " dzwiga cie sztyletem, zadajac " << obrazenia << " obrazen!\n";
    
    if (rand() % 100 < 30) {
        kradziez(&gracz, rand() % 10 + 1);
    }
}

void Lupiezca::resetujBazoweStatystyki() {
    ustawMaxHp(85);
    ustawObrazenia(8);
    ustawPancerz(0.0f);
    ustawPenetracjePancerza(0.0f);
    ustawSzanseNaUnik(0.2f);
}

StraznikKorzeni::StraznikKorzeni() 
    : Przeciwnik("Straznik korzeni", 200, 15, 0.15f, 0.0f, 0.0f, 4),
      turyOdOstatniejFotosyntezy(0), fotosyntezaAktywna(false) {}

void StraznikKorzeni::atakuj(Postac& gracz) {
    turyOdOstatniejFotosyntezy++;
    
    if (fotosyntezaAktywna) {
        fotosyntezaAktywna = false;
        cout << pobierzNazwe() << " koncentruje sie na fotosyntezie i traci ture!\n";
        return;
    }

    if (turyOdOstatniejFotosyntezy >= 4 && rand() % 3 == 0) {
        fotosynteza(gracz);
        turyOdOstatniejFotosyntezy = 0;
        return;
    }

    (rand() % 2 == 0) ? atakPodstawowy(gracz) : gwaltowneKorzenie(gracz);
}

bool StraznikKorzeni::czyFotosyntezaAktywna() const { 
    return fotosyntezaAktywna; 
}

void StraznikKorzeni::resetujFotosynteze() { 
    fotosyntezaAktywna = false; 
}

void StraznikKorzeni::resetujBazoweStatystyki() {
    ustawMaxHp(200);
    ustawObrazenia(15);
    ustawPancerz(0.15f);
    ustawPenetracjePancerza(0.0f);
    ustawSzanseNaUnik(0.0f);
}

void StraznikKorzeni::resetujSpecjalneStatystyki() {
    turyOdOstatniejFotosyntezy = 0;
    fotosyntezaAktywna = false;
}

void StraznikKorzeni::atakPodstawowy(Postac& gracz) {
    int obrazenia = obliczObrazenia(this->pobierzObrazenia());
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " uderza korzeniami, zadajac " << obrazenia << " obrazen!\n";
}

void StraznikKorzeni::gwaltowneKorzenie(Postac& gracz) {
    int obrazenia = obliczObrazenia(this->pobierzObrazenia() * 0.75f);
    gracz.otrzymajObrazenia(obrazenia);
    cout << pobierzNazwe() << " atakuje gwaltownymi korzeniami, zadajac " << obrazenia << " obrazen!\n";
    
    if (rand() % 100 < 40) {
        gracz.ustawPowalenie();
    }
}

void StraznikKorzeni::fotosynteza(Postac& gracz) {
    fotosyntezaAktywna = true;
    int leczenie = pobierzMaxHp() * 0.3;
    ustawHp(min(pobierzHp() + leczenie, pobierzMaxHp()));
    cout << pobierzNazwe() << " wykonuje fotosynteze, leczac sie o " << leczenie << " HP!\n";
}

Przeciwnik* stworzPrzeciwnika(int typ, int poziomGracza) {
    if (poziomGracza <= 0) {
        cerr << "Nieprawidłowy poziom gracza: " << poziomGracza << endl;
        return nullptr;
    }
    if (typ < 0 || typ > 6) {
        cerr << "Nieprawidłowy typ przeciwnika: " << typ << endl;
        return nullptr;
    }
            
    Przeciwnik* nowy = nullptr;
    
    switch(typ) {
        case 0: nowy = new (nothrow) Szlam(); break;
        case 1: nowy = new (nothrow) Wilk(); break;
        case 2: nowy = new (nothrow) Wilkolak(); break;
        case 3: nowy = new (nothrow) Grzybol(); break;
        case 4: nowy = new (nothrow) Golem(); break;
        case 5: nowy = new (nothrow) Lupiezca(); break;
        case 6: nowy = new (nothrow) StraznikKorzeni(); break;
    }

    if (!nowy) {
        cerr << "Blad alokacji pamieci dla przeciwnika typu " << typ << endl;
        return nullptr;
    }

    int poziomPrzeciwnika = nowy->pobierzPoziom();

    if (poziomPrzeciwnika <= poziomGracza && poziomGracza >= 2 && (rand() % 100) < 30) {
        nowy->ustawPoziom(min(poziomGracza + 1, 10));
    }
    
    nowy->skalujDoPoziomu(poziomGracza);
    return nowy;
}