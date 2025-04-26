import prompt from "prompt-sync";
const promptSync = prompt();

interface znaki {
  nr1: number;
  nr2: number;
  nr3: number;
  nr4: number;
  nr5: number;
  dzialanie1: string;
  dzialanie2: string;
  dzialanie3: string;
  dzialanie4: string;
}

class kalkulator {
  nr1: number;
  nr2: number;
  nr3: number;
  nr4: number;
  nr5: number;
  dzialanie1: string;
  dzialanie2: string;
  dzialanie3: string;
  dzialanie4: string;

  constructor(
    nr1: number,
    nr2: number,
    nr3: number,
    nr4: number,
    nr5: number,
    dzialanie1: string,
    dzialanie2: string,
    dzialanie3: string,
    dzialanie4: string
  ) {
    this.nr1 = nr1;
    this.nr2 = nr2;
    this.nr3 = nr3;
    this.nr4 = nr4;
    this.nr5 = nr5;
    this.dzialanie1 = dzialanie1;
    this.dzialanie2 = dzialanie2;
    this.dzialanie3 = dzialanie3;
    this.dzialanie4 = dzialanie4;
  }

  oblicz(): number {
    const elementy: { typ: "liczba" | "operator"; wartosc: number | string }[] =
      [{ typ: "liczba", wartosc: this.nr1 }];

    if (this.nr2 !== 0)
      elementy.push(
        { typ: "operator", wartosc: this.dzialanie1 },
        { typ: "liczba", wartosc: this.nr2 }
      );
    if (this.nr3 !== 0)
      elementy.push(
        { typ: "operator", wartosc: this.dzialanie2 },
        { typ: "liczba", wartosc: this.nr3 }
      );
    if (this.nr4 !== 0)
      elementy.push(
        { typ: "operator", wartosc: this.dzialanie3 },
        { typ: "liczba", wartosc: this.nr4 }
      );
    if (this.nr5 !== 0)
      elementy.push(
        { typ: "operator", wartosc: this.dzialanie4 },
        { typ: "liczba", wartosc: this.nr5 }
      );

    for (let i = 1; i < elementy.length; i += 2) {
      const operator = elementy[i].wartosc;
      if (operator === "mnozenie" || operator === "dzielenie") {
        const lewa = elementy[i - 1].wartosc as number;
        const prawa = elementy[i + 1].wartosc as number;
        let wynikCzastkowy: number;

        if (operator === "mnozenie") {
          wynikCzastkowy = lewa * prawa;
        } else {
          wynikCzastkowy = lewa / prawa;
        }
        elementy.splice(i - 1, 3, { typ: "liczba", wartosc: wynikCzastkowy });
        i -= 2;
      }
    }

    let wynik = elementy[0].wartosc as number;
    for (let i = 1; i < elementy.length; i += 2) {
      const operator = elementy[i].wartosc;
      const liczba = elementy[i + 1].wartosc as number;

      if (operator === "dodawanie") {
        wynik += liczba;
      } else if (operator === "odejmowanie") {
        wynik -= liczba;
      }
    }
    return wynik;
  }
}

const input = promptSync("Wprowadź równanie: ") || "";
const liczby = input.split(/[+\-*/]/).map(Number);
const operator = input.split(/\d+/).filter(Boolean);

const operatory: Record<string, string> = {
  "+": "dodawanie",
  "-": "odejmowanie",
  "*": "mnozenie",
  "/": "dzielenie",
};

if (liczby.length < 2 || operator.length < 1) {
  console.log("Nieprawidłowy format równania!");
  process.exit(1);
}

const nr1 = liczby[0];
const nr2 = liczby[1];
const nr3 = liczby[2];
const nr4 = liczby[3];
const nr5 = liczby[4];
const dzialanie1 = operatory[operator[0]] || "dodawanie";
const dzialanie2 = operatory[operator[1]] || "dodawanie";
const dzialanie3 = operatory[operator[2]] || "dodawanie";
const dzialanie4 = operatory[operator[3]] || "dodawanie";

const rownanieUzytkownika = new kalkulator(
  liczby[0] || 0,
  liczby[1] || 0,
  liczby[2] || 0,
  liczby[3] || 0,
  liczby[4] || 0,
  operatory[operator[0]] || "dodawanie",
  operatory[operator[1]] || "dodawanie",
  operatory[operator[2]] || "dodawanie",
  operatory[operator[3]] || "dodawanie"
);

console.log("Wynik:", rownanieUzytkownika.oblicz());
