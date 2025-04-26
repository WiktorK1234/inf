import prompt from "prompt-sync";
const promptSync = prompt();

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

  oblicz(): number | string {
    if (this.dzialanie === "dodawanie" || this.dzialanie === "+") {
      return this.nr1 + this.nr2;
    } else if (this.dzialanie === "odejmowanie" || this.dzialanie === "-") {
      if (this.liczbap == 1) {
        return this.nr1 - this.nr2;
      } else if (this.liczbap == 2) {
        return this.nr2 - this.nr1;
      }
    } else if (this.dzialanie === "mnozenie" || this.dzialanie === "*") {
      return this.nr1 * this.nr2;
    } else if (this.dzialanie === "dzielenie" || this.dzialanie === "/") {
      if (this.liczbap == 1) {
        if (this.nr2 == 0) {
          return "nie mozna dzielic przez zero";
        } else {
          return this.nr1 / this.nr2;
        }
      } else if (this.liczbap == 2) {
        if (this.nr1 == 0) {
          return "nie mozna dzielic przez zero";
        } else {
          return this.nr2 / this.nr1;
        }
      }
    }
    return 0;
  }
}

const nr1 = parseFloat(promptSync("Podaj pierwszą liczbę: ")) || 0;
const nr2 = parseFloat(promptSync("Podaj drugą liczbę: ")) || 0;
const dzialanie =
  promptSync(
    "Wybierz działanie (dodawanie +/odejmowanie -/mnozenie */dzielenie /): "
  ) || "dodawanie";

let liczbap = 1;

if (
  dzialanie === "odejmowanie" ||
  dzialanie === "dzielenie" ||
  dzialanie === "-" ||
  dzialanie === "/"
) {
  while (true) {
    const liczbapInput = promptSync(
      "Wybierz kolejność (1 - normalna, 2 - odwrotna): "
    );
    liczbap = parseInt(liczbapInput);

    if (liczbap === 1 || liczbap === 2) break;
    console.log("Nieprawidłowy wybór. Wybierz 1 lub 2.");
  }
}

const rownanieUzytkownika = new kalkulator(nr1, nr2, dzialanie, liczbap);
const wynik = rownanieUzytkownika.oblicz();

console.log("\n--- Podsumowanie ---");
console.log(`Liczba 1: ${nr1}`);
console.log(`Liczba 2: ${nr2}`);
console.log(`Działanie: ${dzialanie}`);
if (
  dzialanie === "odejmowanie" ||
  dzialanie === "dzielenie" ||
  dzialanie === "-" ||
  dzialanie === "/"
) {
  console.log(`Kolejność: ${liczbap === 1 ? "normalna" : "odwrotna"}`);
}
console.log(`Wynik: ${wynik}`);
