#include "Bron.h"
#include <stdexcept>

Bron::Bron(string n, int bonus, float szansa, EfektStatusu ef)
    : nazwa(move(n)),
      bonusObrazen(max(bonus, 0)),
      szansaNaEfekt(clamp(szansa, 0.0f, 1.0f)),
      efekt(ef) {
    
    if (szansaNaEfekt > 0.0f && efekt == EfektStatusu::BRAK) {
        throw invalid_argument("Bron z szansa na efekt musi miec okreslony efekt!");
    }
}

namespace {
    vector<Bron> inicjalizujDostepneBronie() {
        return {
            Bron("zelazny miecz", 5),
            Bron("luk", 5),
            Bron("zlota maczuga", 4),
            Bron("topor", 7),
            Bron("zatrute sztylety", 8, 0.35f, EfektStatusu::ZATRUCIE),
            Bron("tytanowy miecz", 10),
            Bron("cep", 1),
            Bron("kusza lodu", 8, 0.4f, EfektStatusu::ZAMROZENIE),
            Bron("mlot dwureczny", 8, 0.4f, EfektStatusu::POWALENIE),
            Bron("wlocznia magmy", 8, 0.35f, EfektStatusu::PODPALENIE),
            Bron("luk wiatru", 6, 0.25f, EfektStatusu::POWALENIE),
            Bron("kastet chaosu", 5, 0.5f, EfektStatusu::CHAOS),
            Bron("katana swiatla", 8, 0.2f, EfektStatusu::OSLEPIENIE),
            Bron("kosa zniwiarza", 12, 0.4f, EfektStatusu::SMIERC),
            Bron("porazajacy bicz (kabel)", 8, 0.25f, EfektStatusu::PORAZENIE),
            Bron("miecz umarlych", 8, 0.2f, EfektStatusu::STRACH)
        };
    }
}

const vector<Bron>& Bron::pobierzDostepneBronie() {
    static const vector<Bron> bronie = inicjalizujDostepneBronie();
    return bronie;
}