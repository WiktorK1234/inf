#include "Pokoj.h"
#include "Przeciwnik.h"
#include "Postac.h"
#include <iostream>
#include <utility>

using namespace std;

Pokoj::Pokoj(const string& nazwa) 
    : nazwa(nazwa.empty() ? "Bez nazwy" : nazwa),
      odwiedzony(false),
      pokonany(false),
      specjalnaAkcja(nullptr) {
    if (nazwa.empty()) {
        cerr << "Ostrzezenie: Stworzono pokoj bez nazwy!\n";
    }
}

void Pokoj::dodajPrzeciwnika(unique_ptr<Przeciwnik> przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Probujesz dodac nullptr jako przeciwnika!\n";
        return;
    }
    przeciwnicy.push_back(move(przeciwnik));
}

const vector<unique_ptr<Przeciwnik>>& Pokoj::pobierzPrzeciwnikow() const {
    return przeciwnicy;
}

void Pokoj::dodajPolaczenie(const string& kierunek, Pokoj* cel) {
    if (!cel) {
        cerr << "Blad: Probujesz dodac polaczenie do nullptr!\n";
        return;
    }
    if (kierunek.empty()) {
        cerr << "Blad: Kierunek polaczenia nie moze byc pusty!\n";
        return;
    }
    polaczenia[kierunek] = cel;
}

const map<string, Pokoj*>& Pokoj::pobierzPolaczenia() const {
    return polaczenia;
}

string Pokoj::pobierzNazwe() const {
    return nazwa;
}

bool Pokoj::czyOdwiedzony() const {
    return odwiedzony;
}

void Pokoj::ustawOdwiedzony(bool wartosc) {
    odwiedzony = wartosc;
}

bool Pokoj::czyPokonany() const {
    return pokonany;
}

void Pokoj::ustawPokonany(bool wartosc) {
    pokonany = wartosc;
}

void Pokoj::wyczyscPrzeciwnikow() {
    przeciwnicy.clear();
}

bool Pokoj::czySaPrzeciwnicy() const {
    return any_of(przeciwnicy.begin(), przeciwnicy.end(),
        [](const auto& p) { return p && p->czyZyje(); });
}

size_t Pokoj::liczbaZywychPrzeciwnikow() const {
    return count_if(przeciwnicy.begin(), przeciwnicy.end(),
        [](const auto& p) { return p && p->czyZyje(); });
}

string Pokoj::serializuj() const {
    string wynik = nazwa + "\n";
    wynik += (odwiedzony ? "1" : "0") + "\n";
    wynik += (pokonany ? "1" : "0") + "\n";
    wynik += to_string(przeciwnicy.size()) + "\n";
    
    for (const auto& p : przeciwnicy) {
        if (p) {
            wynik += p->pobierzNazwe() + "\n";
            wynik += to_string(p->pobierzHp()) + "\n";
            wynik += to_string(p->pobierzPoziom()) + "\n";
        }
    }
    
    return wynik;
}

void Pokoj::deserializuj(const vector<string>& dane) {
    if (dane.size() < 3) return;
    
    odwiedzony = (dane[0] == "1");
    pokonany = (dane[1] == "1");
    
    size_t liczbaPrzeciwnikow;
    try {
        liczbaPrzeciwnikow = stoul(dane[2]);
    } catch (...) {
        return;
    }
    
    przeciwnicy.clear();
    size_t index = 3;
    
    for (size_t i = 0; i < liczbaPrzeciwnikow && index + 2 < dane.size(); ++i) {
        string nazwa = dane[index++];
        int hp, poziom;
        
        try {
            hp = stoi(dane[index++]);
            poziom = stoi(dane[index++]);
        } catch (...) {
            continue;
        }
        
        Przeciwnik* nowy = stworzPrzeciwnika(znajdzTypPrzeciwnika(nazwa), poziom);
        if (nowy) {
            nowy->ustawHp(hp);
            przeciwnicy.emplace_back(nowy);
        }
    }
}

int Pokoj::znajdzTypPrzeciwnika(const string& nazwa) const {
    static const map<string, int> mapa = {
        {"Szlam", 0}, {"Wilk", 1}, {"Wilkolak", 2},
        {"Grzybol", 3}, {"Golem", 4}, {"Lupiezca", 5},
        {"Straznik korzeni", 6}
    };
    auto it = mapa.find(nazwa);
    return it != mapa.end() ? it->second : 0;
}