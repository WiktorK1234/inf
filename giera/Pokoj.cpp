#include "Pokoj.h"
#include <iostream>

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