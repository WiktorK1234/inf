#include "EfektyStatusowe.h"
#include "Bron.h"
#include "Postac.h" 
#include "Przeciwnik.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void spowolnienie(Postac* gracz) {
    if (!gracz) {
        cerr << "Błąd: Gracz jest nullptr w funkcji spowolnienie()\n";
        return;
    }
    try {
        cout << gracz->pobierzNazwe() << " jest spowolniony! (Zmniejszona szansa na unik przez 2 tury)\n";
        gracz->dodajEfektStatusu("spowolnienie", 2);
    } catch (const exception& e) {
        cerr << "Błąd w spowolnienie(): " << e.what() << endl;
    }
}

void krwawienie(Postac* gracz) {
    if (!gracz) {
        cerr << "Błąd: Gracz jest nullptr w funkcji krwawienie()\n";
        return;
    }
    try {
        cout << gracz->pobierzNazwe() << " krwawi! (6 obrazen przez 2 tury)\n";
        gracz->dodajEfektStatusu("krwawienie", 2);
    } catch (const exception& e) {
        cerr << "Błąd w krwawienie(): " << e.what() << endl;
    }
}

void zatrucie(Postac* gracz) {
    if (!gracz) {
        cerr << "Błąd: Gracz jest nullptr w funkcji zatrucie()\n";
        return;
    }
    try {
        cout << gracz->pobierzNazwe() << " zostal zatruty! (3 obrazenia przez 4 tury)\n";
        gracz->dodajEfektStatusu("zatrucie", 4);
    } catch (const exception& e) {
        cerr << "Błąd w zatrucie(): " << e.what() << endl;
    }
}

void ogluszenie(Postac* gracz) {
    if (!gracz) {
        cerr << "Błąd: Gracz jest nullptr w funkcji ogluszenie()\n";
        return;
    }
    try {
        cout << gracz->pobierzNazwe() << " zostal ogluszony! (Tracisz nastepna ture)\n";
        gracz->dodajEfektStatusu("ogluszenie", 1);
    } catch (const exception& e) {
        cerr << "Błąd w ogluszenie(): " << e.what() << endl;
    }
}

void kradziez(Postac* gracz, int ilosc) {
    if (!gracz) {
        cerr << "Błąd: Gracz jest nullptr w funkcji kradziez()\n";
        return;
    }
    try {
        if (gracz->pobierzZloto() <= 0) {
            cout << "Lupiezca probowal cie okrasc, ale jestes zbyt biedny!\n";
        } else {
            int faktycznieSkradzione = min(ilosc, gracz->pobierzZloto());
            gracz->ustawZloto(gracz->pobierzZloto() - faktycznieSkradzione);
            cout << "Lupiezca ukradl ci " << faktycznieSkradzione << " zlota!\n";
        }
    } catch (const exception& e) {
        cerr << "Błąd w kradziez(): " << e.what() << endl;
    }
}

void zatrucie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji zatrucie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " zostal zatruty! (3 obrazenia przez 4 tury)\n";
        przeciwnik->otrzymajObrazenia(3);
        przeciwnik->dodajEfektStatusu("zatrucie", 4);
    } catch (const exception& e) {
        cerr << "Błąd w zatrucie(Przeciwnik*): " << e.what() << endl;
    }
}

void zamrozenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji zamrozenie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " zostal zamrozony! (Traci nastepna ture)\n";
        przeciwnik->dodajEfektStatusu("zamrozenie", 1);
    } catch (const exception& e) {
        cerr << "Błąd w zamrozenie(): " << e.what() << endl;
    }
}

void podpalenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji podpalenie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " zostal podpalony! (4 obrazenia przez 3 tury)\n";
        przeciwnik->otrzymajObrazenia(4);
        przeciwnik->dodajEfektStatusu("podpalenie", 3);
    } catch (const exception& e) {
        cerr << "Błąd w podpalenie(): " << e.what() << endl;
    }
}

// Analogicznie kontynuuj dla pozostałych efektów...
void krwawienie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji krwawienie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " krwawi! (6 obrazen przez 2 tury)\n";
        przeciwnik->otrzymajObrazenia(6);
        przeciwnik->dodajEfektStatusu("krwawienie", 2);
    } catch (const exception& e) {
        cerr << "Błąd w krwawienie(Przeciwnik*): " << e.what() << endl;
    }
}

void powalenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji powalenie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " zostal powalony! (Ma zmniejszone obrazenia i pancerz na 2 tury)\n";
        przeciwnik->zmniejszObrazenia(0.75f);
        przeciwnik->zmniejszPancerz(0.75f);
        przeciwnik->dodajEfektStatusu("powalenie", 2);
    } catch (const exception& e) {
        cerr << "Błąd w powalenie(): " << e.what() << endl;
    }
}

void ogluszenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji ogluszenie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " zostal ogluszony! (Traci nastepna ture)\n";
        przeciwnik->dodajEfektStatusu("ogluszenie", 1);
    } catch (const exception& e) {
        cerr << "Błąd w ogluszenie(Przeciwnik*): " << e.what() << endl;
    }
}

void kradziez(Przeciwnik* przeciwnik, Postac* gracz) {
    if (!przeciwnik || !gracz) {
        cerr << "Błąd: Przeciwnik lub gracz jest nullptr w funkcji kradziez()\n";
        return;
    }
    try {
        int skradzione = rand() % 10 + 5;
        gracz->zloto += skradzione;
        cout << "Okradles przeciwnika z " << skradzione << " zlota!\n";
        przeciwnik->dodajEfektStatusu("kradziez", 1);
    } catch (const exception& e) {
        cerr << "Błąd w kradziez(Przeciwnik*): " << e.what() << endl;
    }
}

void oslepienie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji oslepienie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " został oślepiony! (Twoja szansa na unik wzrosła na 3 tury)\n";
        przeciwnik->dodajEfektStatusu("oslepienie", 3);
    } catch (const exception& e) {
        cerr << "Błąd w oslepienie(): " << e.what() << endl;
    }
}

void smierc(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji smierc()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " został natychmiastowo zabity!\n";
        przeciwnik->otrzymajObrazenia(przeciwnik->pobierzHp());
        przeciwnik->dodajEfektStatusu("smierc", 1);
    } catch (const exception& e) {
        cerr << "Błąd w smierc(): " << e.what() << endl;
    }
}

void strach(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji strach()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " został wystraszony! (Traci następną turę)\n";
        przeciwnik->dodajEfektStatusu("strach", 1);
    } catch (const exception& e) {
        cerr << "Błąd w strach(): " << e.what() << endl;
    }
}

void porazenie(Przeciwnik* przeciwnik) {
    if (!przeciwnik) {
        cerr << "Błąd: Przeciwnik jest nullptr w funkcji porazenie()\n";
        return;
    }
    try {
        cout << przeciwnik->pobierzNazwe() << " został porażony! (Zmniejszone obrażenia i pancerz przez 2 tury)\n";
        przeciwnik->zmniejszObrazenia(0.75f);
        przeciwnik->zmniejszPancerz(0.75f);
        przeciwnik->dodajEfektStatusu("porazenie", 2);
    } catch (const exception& e) {
        cerr << "Błąd w porazenie(): " << e.what() << endl;
    }
}

void chaos(Przeciwnik* przeciwnik, Postac* gracz) {
    if (!przeciwnik || !gracz) {
        cerr << "Błąd: Przeciwnik lub gracz jest nullptr w funkcji chaos()\n";
        return;
    }
    try {
        enum Efekt { 
            ZATRUCIE, ZAMROZENIE, PODPALENIE, KRWAWIENIE, POWALENIE, 
            OGLUSZENIE, OSLEPIENIE, STRACH, PORAZENIE, KRADZIEZ 
        };
        
        Efekt losowyEfekt = static_cast<Efekt>(rand() % 10);
        
        cout << "Chaos! " << przeciwnik->pobierzNazwe() << " otrzymał losowy efekt statusu.\n";
        
        switch(losowyEfekt) {
            case ZATRUCIE: zatrucie(przeciwnik); break;
            case ZAMROZENIE: zamrozenie(przeciwnik); break;
            case PODPALENIE: podpalenie(przeciwnik); break;
            case KRWAWIENIE: krwawienie(przeciwnik); break;
            case POWALENIE: powalenie(przeciwnik); break;
            case OGLUSZENIE: ogluszenie(przeciwnik); break;
            case OSLEPIENIE: oslepienie(przeciwnik); break;
            case STRACH: strach(przeciwnik); break;
            case PORAZENIE: porazenie(przeciwnik); break;
            case KRADZIEZ: kradziez(przeciwnik, gracz); break;
        }
    } catch (const exception& e) {
        cerr << "Błąd w chaos(): " << e.what() << endl;
    }
}
