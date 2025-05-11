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
    const vector<unique_ptr<Przeciwnik>>& pobierzPrzeciwnikow() const;

    // Zarządzanie połączeniami
    void dodajPolaczenie(const string& kierunek, Pokoj* cel);
    const map<string, Pokoj*>& pobierzPolaczenia() const;

    // Metody dostępowe
    string pobierzNazwe() const { return nazwa; }
    bool czyOdwiedzony() const { return odwiedzony; }
    void ustawOdwiedzony(bool wartosc) { odwiedzony = wartosc; }
    bool czyPokonany() const { return pokonany; }
    void ustawPokonany(bool wartosc) { pokonany = wartosc; }

    void wyczyscPrzeciwnikow();
    bool czySaPrzeciwnicy() const;
    size_t liczbaZywychPrzeciwnikow() const;
    string serializuj() const;
    void deserializuj(const vector<string>& dane);
    int znajdzTypPrzeciwnika(const string& nazwa) const;

    // Wyłącz kopiowanie i przypisanie
    Pokoj(const Pokoj&) = delete;
    Pokoj& operator=(const Pokoj&) = delete;

    void ustawSpecjalnaAkcje(function<bool(Postac&)> akcja) {
        specjalnaAkcja = move(akcja);
    }

    void wyczyscPrzeciwnikow() {
        przeciwnicy.clear();
    }

    vector<unique_ptr<Przeciwnik>>& pobierzPrzeciwnikow() { return przeciwnicy; }
    const vector<unique_ptr<Przeciwnik>>& pobierzPrzeciwnikow() const { return przeciwnicy; }
    
    function<bool(Postac&)>& pobierzSpecjalnaAkcje() { return specjalnaAkcja; }
    bool maSpecjalnaAkcje() const { return static_cast<bool>(specjalnaAkcja); }
};

#endif // POKOJ_H