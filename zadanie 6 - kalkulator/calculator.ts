interface znaki {
  nr1: number;
  nr2: number;
  dzialanie: string;
  liczbap: number;
}

class kalkulator {
  nr1: number;
  nr2: number;
  dzialanie: string;
  liczbap: number;
  constructor(nr1: number, nr2: number, dzialanie: string, liczbap: number) {
    this.nr1 = nr1;
    this.nr2 = nr2;
    this.dzialanie = dzialanie;
    this.liczbap = liczbap;
  }
  oblicz(): number {
    if (this.dzialanie === "dodawanie") {
      return this.nr1 + this.nr2;
    } else if (this.dzialanie === "odejmowanie") {
      if (this.liczbap == 1) {
        return this.nr1 - this.nr2;
      } else if (this.liczbap == 2) {
        return this.nr2 - this.nr1;
      }
    } else if (this.dzialanie === "mnozenie") {
      return this.nr1 * this.nr2;
    } else if (this.dzialanie === "dzielenie") {
      if (this.liczbap == 1) {
        return this.nr1 / this.nr2;
      } else if (this.liczbap == 2) {
        return this.nr2 / this.nr1;
      }
    }
    return 0;
  }
}
const azor = new kalkulator(1, 1, "dodawanie", 1);
console.log(azor.oblicz());
