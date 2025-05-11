#ifndef PRZECIWNIK_H
#define PRZECIWNIK_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include "Postac.h"
#include "EfektyStatusowe.cpp"

using namespace std;

class Postac;

class Przeciwnik : public Postac {
private:
    string nazwa;
    int hp;
    int maxHp;
    int obrazenia;
    float pancerz;
    float penetracjaPancerza;
    float szansaNaUnik;
    int poziom;
    bool czyBoss;
    bool czyMiniBoss;
    unordered_map<string, int> efektyStatusow;

public:
    Przeciwnik(string n, int h, int dmg, float p, float pen, float unik, int lvl);
    virtual ~Przeciwnik() = default;

    virtual void atakuj(Postac& gracz) = 0;
    virtual bool czyFotosyntezaAktywna() const { return false; }
    virtual void resetujFotosynteze() {}
    virtual void resetujBazoweStatystyki();
    virtual void resetujSpecjalneStatystyki() {}
    
    int pobierzPoziom() const { return poziom; }
    void ustawPoziom(int nowyPoziom) { poziom = nowyPoziom; }
    const string& pobierzNazwe() const { return nazwa; }
    int pobierzHp() const { return hp; }
    int pobierzMaxHp() const { return maxHp; }
    void ustawMaxHp(int hp) { maxHp = hp; }
    void ustawHp(int noweHp) { hp = clamp(noweHp, 0, maxHp); }
    bool czyZyje() const { return hp > 0; }
    void lecz(int ile) { ustawHp(hp + ile); }
    void otrzymajObrazenia(int ile) { ustawHp(hp - ile); }
    void zmniejszObrazenia(float mnoznik);
    void zmniejszPancerz(float mnoznik);
    int pobierzObrazenia() const { return obrazenia; }
    float pobierzPancerz() const { return pancerz; }
    void ustawObrazenia(float noweObrazenia) { obrazenia = noweObrazenia; }
    void ustawPancerz(float nowyPancerz) { pancerz = nowyPancerz; }
    void ustawPenetracjePancerza(float nowaPenetracjaPancerza) { penetracjaPancerza = nowaPenetracjaPancerza; }
    void ustawSzanseNaUnik(float nowaSzansaNaUnik) { szansaNaUnik = nowaSzansaNaUnik; }
    bool czyUnik() const;
    int obliczObrazenia(int bazowe) const;
    void skalujDoPoziomu(int poziomGracza);
    void wyswietlStatystyki(int poziomGracza) const;
    void wyczyscEfektyStatusowe() { efektyStatusow.clear(); }
};

class Szlam : public Przeciwnik {
public:
    Szlam();
    void atakuj(Postac& gracz) override;
protected:
    void resetujBazoweStatystyki() override;
};

class Wilk : public Przeciwnik {
public:
    Wilk();
    void atakuj(Postac& gracz) override;
protected:
    void resetujBazoweStatystyki() override;
};

class Wilkolak : public Przeciwnik {
    public:
        Wilkolak();
        void atakuj(Postac& gracz) override;
    protected:
        void resetujBazoweStatystyki() override;
};

class Golem : public Przeciwnik {
    public:
        Golem();
        void atakuj(Postac& gracz) override;
    protected:
        void resetujBazoweStatystyki() override;
};

class Grzybol : public Przeciwnik {
    public:
        Grzybol();
        void atakuj(Postac& gracz) override;
    protected:
        void resetujBazoweStatystyki() override;
};

class Lupiezca : public Przeciwnik {
    public:
        Lupiezca();
        void atakuj(Postac& gracz) override;
    protected:
        void resetujBazoweStatystyki() override;
};

class StraznikKorzeni : public Przeciwnik {
    public:
        StraznikKorzeni();
        void atakuj(Postac& gracz) override;
        bool czyFotosyntezaAktywna() const override;
        void resetujFotosynteze() override;
        
    protected:
        void resetujBazoweStatystyki() override;
        void resetujSpecjalneStatystyki() override;
    
    private:
        int turyOdOstatniejFotosyntezy;
        bool fotosyntezaAktywna;
        
        void atakPodstawowy(Postac& gracz);
        void gwaltowneKorzenie(Postac& gracz);
        void fotosynteza(Postac& gracz);
};

Przeciwnik* stworzPrzeciwnika(int typ, int poziomGracza);

#endif // PRZECIWNIK_H