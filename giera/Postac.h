#pragma once
#ifndef POSTAC_H
#define POSTAC_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "Bron.h"
#include "Pokoj.h"
#include <memory>

using namespace std;

class Przeciwnik;
class Pokoj;

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
    int zloto = 0;
    int mikstury = 5;
    Bron* aktualnaBron = nullptr;
    unordered_map<string, int> efektyStatusow;
    static vector<Bron> dostepneBronie;

    Postac() : 
        nazwa(""), hp(100), maxHp(100), pancerz(0.0f), obrazenia(10), 
        penetracjaPancerza(0.0f), szansaNaUnikBierny(0.0f), 
        unikManualny(0.0f), szansaNaUcieczke(0.0f), 
        poziom(1), exp(0), zloto(0), mikstury(5), 
        aktualnaBron(nullptr), powalony(false), pozostaleTuryPowalenia(0) 
    {}
    virtual ~Postac();
    Postac(const Postac& other);
    Postac& operator=(const Postac& other);
    
    void dodajEfektStatusu(const string& efekt, int tury);
    const string& pobierzNazwe() const { return nazwa; }
    int pobierzZloto() const { return zloto; }
    void ustawZloto(int noweZloto) { zloto = noweZloto; }
    void otrzymajObrazenia(int ile);

    bool czyPowalony() const;
    void ustawPowalenie(int tury = 1);
    void aktualizujPowalenie();
    void aktualizujEfektyStatusowe();
    void aktualizujPoziomPrzeciwnikow(vector<Pokoj*>& wszystkiePokoje, int poziomGracza);
    void zdobadzDoswiadczenie(int ilosc, vector<Pokoj*>& wszystkiePokoje);
    void uzyjMikstury();
    void wyswietlStatystyki(bool czyscEkran = true);
    
    bool czyUnikBierny();
    bool czyUnikManualny();
    bool czyUcieczka();
    int obliczObrazenia(int obrazeniaAtakujacego);
    int obliczCalkowiteObrazenia();

    virtual ~Postac() { delete aktualnaBron; }

private:
    bool powalony = false;
    int pozostaleTuryPowalenia = 0;
};

class Opancerzony : public Postac {
    public:
        Opancerzony(Bron* bron);  // Przyjmuje wskaźnik do broni
    };
    
class Bandyta : public Postac {
    public:
        Bandyta(Bron* bron);  // Przyjmuje wskaźnik do broni
    };

#endif // POSTAC_H