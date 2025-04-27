#ifndef BRON_H
#define BRON_H

#include <string>
#include <vector>
#include <algorithm> // dla clamp()
#include <stdexcept> // dla invalid_argument

using namespace std;

enum class EfektStatusu {
    BRAK,
    ZATRUCIE,
    ZAMROZENIE,
    PODPALENIE,
    KRWAWIENIE,
    POWALENIE,
    OGLUSZENIE,
    KRADZIEZ,
    OSLEPIENIE,
    SMIERC,
    STRACH,
    PORAZENIE,
    CHAOS
};

class Bron {
private:
    string nazwa;
    int bonusObrazen;
    float szansaNaEfekt;
    EfektStatusu efekt;

public:
    Bron(string n, int bonus, float szansa = 0.0f, EfektStatusu ef = EfektStatusu::BRAK);
    
    string pobierzNazwe() const { return nazwa; }
    int pobierzBonusObrazen() const { return bonusObrazen; }
    float pobierzSzansaNaEfekt() const { return szansaNaEfekt; }
    EfektStatusu pobierzEfekt() const { return efekt; }

    static const vector<Bron>& pobierzDostepneBronie();
};

#endif // BRON_H