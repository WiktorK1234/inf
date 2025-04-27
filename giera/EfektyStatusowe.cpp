#include "EfektyStatusowe.h"
#include "Bron.h"
#include <iostream>
#include <vector>

using namespace std;

void spowolnienie(Postac* gracz) {
    cout << gracz->pobierzNazwe() << " jest spowolniony! (Zmniejszona szansa na unik przez 2 tury)\n";
    gracz->dodajEfektStatusu("spowolnienie", 2);
}

void krwawienie(Postac* gracz) {
    cout << gracz->pobierzNazwe() << " krwawi! (6 obrazen przez 2 tury)\n";
    gracz->dodajEfektStatusu("krwawienie", 2);
}

void zatrucie(Postac* gracz) {
    cout << gracz->pobierzNazwe() << " zostal zatruty! (3 obrazenia przez 4 tury)\n";
    gracz->dodajEfektStatusu("zatrucie", 4);
}

void ogluszenie(Postac* gracz) {
    cout << gracz->pobierzNazwe() << " zostal ogluszony! (Tracisz nastepna ture)\n";
    gracz->dodajEfektStatusu("ogluszenie", 1);
}

void kradziez(Postac* gracz, int ilosc) {
    if (gracz->pobierzZloto() <= 0) {
        cout << "Lupiezca probowal cie okrasc, ale jestes zbyt biedny!\n";
    } else {
        int faktycznieSkradzione = min(ilosc, gracz->pobierzZloto());
        gracz->ustawZloto(gracz->pobierzZloto() - faktycznieSkradzione);
        cout << "Lupiezca ukradl ci " << faktycznieSkradzione << " zlota!\n";
    }
}

void zatrucie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji zatrucie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal zatruty! (3 obrazenia przez 4 tury)\n";
    przeciwnik->otrzymajObrazenia(3);
    przeciwnik->dodajEfektStatusu("zatrucie", 4);
}

void zamrozenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji zamrozenie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal zamrozony! (Traci nastepna ture)\n";
    przeciwnik->dodajEfektStatusu("zamrozenie", 1);
}

void podpalenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji podpalenie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal podpalony! (4 obrazenia przez 3 tury)\n";
    przeciwnik->otrzymajObrazenia(4);
    przeciwnik->dodajEfektStatusu("podpalenie", 3);
}

void krwawienie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji krwawienie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " krwawi! (6 obrazen przez 2 tury)\n";
    przeciwnik->otrzymajObrazenia(6);
    przeciwnik->dodajEfektStatusu("krwawienie", 2);
}

void powalenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji powalenie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal powalony! (Ma zmniejszone obrazenia i pancerz na 2 tury)\n";
    przeciwnik->zmniejszObrazenia(0.75f);
    przeciwnik->zmniejszPancerz(0.75f);
    przeciwnik->dodajEfektStatusu("powalenie", 2);
}

void ogluszenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji ogluszenie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal ogluszony! (Traci nastepna ture)\n";
    przeciwnik->dodajEfektStatusu("ogluszenie", 1);
}

void kradziez(Przeciwnik* przeciwnik, Postac* gracz) {
    if (!przeciwnik || !gracz) return;
    
    int skradzioneZloto = rand() % 10 + 5;
    gracz->zloto += skradzioneZloto;
    
    cout << "Okradles przeciwnika z " << skradzioneZloto 
         << " zlota! (Twoje zloto: " << gracz->zloto << ")\n";
    
    przeciwnik->dodajEfektStatusu("kradziez", 1);
}

void oslepienie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji oslepienie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal oslepiony! (Twoja szansa na unik znacznie wzrosla na 3 tury)\n";
    przeciwnik->dodajEfektStatusu("oslepienie", 3);
}

void smierc(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji smierc()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal natychmiastowo zabity!\n";
    przeciwnik->otrzymajObrazenia(przeciwnik->pobierzHp());
    przeciwnik->dodajEfektStatusu("smierc", 1);
}

void strach(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji strach()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal wystraszony! (Traci nastepna ture)\n";
    przeciwnik->dodajEfektStatusu("strach", 1);
}

void porazenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji porazenie()\n";
        return;
    }
    cout << przeciwnik->pobierzNazwe() << " zostal porazony! (Ma zmniejszone obrazenia i pancerz na 2 tury)\n";
    przeciwnik->zmniejszObrazenia(0.75f);
    przeciwnik->zmniejszPancerz(0.75f);
    przeciwnik->dodajEfektStatusu("porazenie", 2);
}

void chaos(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Blad: Przeciwnik jest nullptr w funkcji chaos()\n";
        return;
    }
    
    vector<EfektStatusu> efekty = {
        EfektStatusu::ZATRUCIE,
        EfektStatusu::ZAMROZENIE,
        EfektStatusu::PODPALENIE,
        EfektStatusu::KRWAWIENIE,
        EfektStatusu::POWALENIE,
        EfektStatusu::OGLUSZENIE,
        EfektStatusu::OSLEPIENIE,
        EfektStatusu::STRACH,
        EfektStatusu::PORAZENIE,
        EfektStatusu::KRADZIEZ
    };
    
    EfektStatusu losowyEfekt = efekty[rand() % efekty.size()];
    cout << "Chaos! " << przeciwnik->pobierzNazwe() << " otrzymal efekt.\n";
    
    switch(losowyEfekt) {
        case EfektStatusu::ZATRUCIE: zatrucie(przeciwnik); break;
        case EfektStatusu::ZAMROZENIE: zamrozenie(przeciwnik); break;
        case EfektStatusu::PODPALENIE: podpalenie(przeciwnik); break;
        case EfektStatusu::KRWAWIENIE: krwawienie(przeciwnik); break;
        case EfektStatusu::POWALENIE: powalenie(przeciwnik); break;
        case EfektStatusu::OGLUSZENIE: ogluszenie(przeciwnik); break;
        case EfektStatusu::OSLEPIENIE: oslepienie(przeciwnik); break;
        case EfektStatusu::STRACH: strach(przeciwnik); break;
        case EfektStatusu::PORAZENIE: porazenie(przeciwnik); break;
        case EfektStatusu::KRADZIEZ: kradziez(przeciwnik); break;
    }
}