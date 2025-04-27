#ifndef POKOJ_H
#define POKOJ_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include "Przeciwnik.h"

class Postac;

class Pokoj {
private:
    string nazwa;
    bool odwiedzony;
    bool pokonany;
    vector<unique_ptr<Przeciwnik>> przeciwnicy;
    map<string, Pokoj*> polaczenia;  
    function<bool(Postac&)> specjalnaAkcja;

public:
    explicit Pokoj(const string& nazwa);
    ~Pokoj() = default;

    // Zarządzanie przeciwnikami
    void dodajPrzeciwnika(unique_ptr<Przeciwnik> przeciwnik);
    const vector<unique_ptr<Przeciwnik>>& pobierzPrzeciwnikow() const { return przeciwnicy; }

    // Zarządzanie połączeniami
    void dodajPolaczenie(const string& kierunek, Pokoj* cel);
    const map<string, Pokoj*>& pobierzPolaczenia() const;

    // Metody dostępowe
    string pobierzNazwe() const { return nazwa; }
    bool czyOdwiedzony() const { return odwiedzony; }
    void ustawOdwiedzony(bool wartosc) { odwiedzony = wartosc; }
    bool czyPokonany() const { return pokonany; }
    void ustawPokonany(bool wartosc) { pokonany = wartosc; }

    // Wyłącz kopiowanie i przypisanie
    Pokoj(const Pokoj&) = delete;
    Pokoj& operator=(const Pokoj&) = delete;
};

#endif // POKOJ_H