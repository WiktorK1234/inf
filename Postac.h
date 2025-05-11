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
    virtual ~Postac() = default;
    virtual string pobierzNazwe() const = 0;
    virtual void dodajEfektStatusu(const string& efekt, int tury) = 0;
    Postac(const Postac& other);
    Postac& operator=(const Postac& other);

    const unordered_map<string, int>& pobierzEfektyStatusowe() const { return efektyStatusow; }
    
    void dodajEfektStatusu(const string& efekt, int tury);
    int pobierzZloto() const { return zloto; }
    void ustawZloto(int noweZloto) { zloto = noweZloto; }
    void otrzymajObrazenia(int ile);

    bool czyPowalony() const;
    void ustawPowalenie(int tury = 1);
    void aktualizujPowalenie();
    void aktualizujEfektyStatusowe();
    void aktualizujPoziomPrzeciwnikow(const vector<unique_ptr<Pokoj>>& wszystkiePokoje, int poziomGracza);
    void zdobadzDoswiadczenie(int ilosc, const vector<unique_ptr<Pokoj>>& wszystkiePokoje);
    void uzyjMikstury();
    void wyswietlStatystyki(bool czyscEkran = true);
    
    bool czyUnikBierny();
    bool czyUnikManualny();
    bool czyUcieczka();
    int obliczObrazenia(int obrazeniaAtakujacego);
    int obliczCalkowiteObrazenia();
    int& pobierzHp() { return hp; }
    int pobierzMaxHp() const { return maxHp; }
    void ustawHp(int noweHp) { hp = noweHp; }
    int pobierzPoziom() const { return poziom; }

    virtual ~Postac() { delete aktualnaBron; }

    virtual bool maEfekt(const string& efekt) const {
        return efektyStatusow.count(efekt) > 0;
    }

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