#ifndef EFEKTY_STATUSOWE_H
#define EFEKTY_STATUSOWE_H

#include "Postac.h"
#include "Przeciwnik.h"
#include <string>
#include <unordered_map>

using namespace std;

void spowolnienie(Postac* gracz);
void krwawienie(Postac* gracz);
void zatrucie(Postac* gracz);
void ogluszenie(Postac* gracz);
void kradziez(Postac* gracz, int ilosc);
void zatrucie(Przeciwnik* przeciwnik);
void zamrozenie(Przeciwnik* przeciwnik);
void podpalenie(Przeciwnik* przeciwnik);
void krwawienie(Przeciwnik* przeciwnik);
void powalenie(Przeciwnik* przeciwnik);
void ogluszenie(Przeciwnik* przeciwnik);
void kradziez(Przeciwnik* przeciwnik);
void oslepienie(Przeciwnik* przeciwnik);
void smierc(Przeciwnik* przeciwnik);
void strach(Przeciwnik* przeciwnik);
void porazenie(Przeciwnik* przeciwnik);
void chaos(Przeciwnik* przeciwnik);

#endif // EFEKTY_STATUSOWE_H