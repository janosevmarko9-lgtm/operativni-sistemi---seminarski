#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>

using namespace std;

struct ZahtevKontrolera {
    int redniBroj;
    string tipKontrolera;
    string operacija;
    string podatak;
    int velicinaBloka;
    string stanje;
};

struct CvorZahteva {
    ZahtevKontrolera zahtev;
    CvorZahteva* sledeci;
};

CvorZahteva* glava = nullptr;
CvorZahteva* rep = nullptr;
int sledeciRedniBroj = 1;

void sacekaj(int milisekunde) {
    this_thread::sleep_for(chrono::milliseconds(milisekunde));
}

string odrediNazivKontrolera(int izbor) {
    switch (izbor) {
        case 1: return "NVMe";
        case 2: return "SATA";
        case 3: return "Mrezni";
        case 4: return "USB";
        default: return "Nepoznat";
    }
}

void dodajNaKrajListe(const ZahtevKontrolera& noviZahtev) {
    CvorZahteva* noviCvor = new CvorZahteva;
    noviCvor->zahtev = noviZahtev;
    noviCvor->sledeci = nullptr;

    if (glava == nullptr) {
        glava = noviCvor;
        rep = noviCvor;
    } else {
        rep->sledeci = noviCvor;
        rep = noviCvor;
    }
}

void unesiZahtev() {
    int izborKontrolera;
    ZahtevKontrolera noviZahtev;

    cout << "\n1. NVMe kontroler\n";
    cout << "2. SATA kontroler\n";
    cout << "3. Mrezni kontroler\n";
    cout << "4. USB kontroler\n";
    cout << "Izaberite kontroler: ";
    cin >> izborKontrolera;

    noviZahtev.tipKontrolera = odrediNazivKontrolera(izborKontrolera);
    if (noviZahtev.tipKontrolera == "Nepoznat") {
        cout << "Izbor kontrolera nije ispravan.\n";
        return;
    }

    cout << "Unesite operaciju (CITANJE ili UPIS): ";
    cin >> noviZahtev.operacija;
    cout << "Unesite podatak bez razmaka: ";
    cin >> noviZahtev.podatak;
    cout << "Unesite velicinu bloka: ";
    cin >> noviZahtev.velicinaBloka;

    if (noviZahtev.velicinaBloka <= 0) {
        cout << "Velicina bloka mora biti veca od nule.\n";
        return;
    }

    noviZahtev.redniBroj = sledeciRedniBroj++;
    noviZahtev.stanje = "SPREMAN";
    dodajNaKrajListe(noviZahtev);
    cout << "Zahtev je dodat u jednostruko spregnutu listu.\n";
}

void prikaziListu() {
    if (glava == nullptr) {
        cout << "Lista zahteva je prazna.\n";
        return;
    }

    CvorZahteva* tekuci = glava;
    cout << "\nRB | KONTROLER | OPERACIJA | PODATAK | BLOK | STANJE\n";
    while (tekuci != nullptr) {
        cout << tekuci->zahtev.redniBroj << " | "
             << tekuci->zahtev.tipKontrolera << " | "
             << tekuci->zahtev.operacija << " | "
             << tekuci->zahtev.podatak << " | "
             << tekuci->zahtev.velicinaBloka << " | "
             << tekuci->zahtev.stanje << '\n';
        tekuci = tekuci->sledeci;
    }
}

void obradiProzivanjem() {
    if (glava == nullptr) {
        cout << "Nema zahteva za obradu.\n";
        return;
    }

    CvorZahteva* tekuci = glava;
    while (tekuci != nullptr) {
        tekuci->zahtev.stanje = "ZAUZET";
        cout << "\nProcesor proverava registar stanja za zahtev "
             << tekuci->zahtev.redniBroj;
        for (int provera = 1; provera <= 3; provera++) {
            cout << ".";
            sacekaj(150);
        }
        tekuci->zahtev.stanje = "OBRADJEN";
        cout << " zahtev je obradjen.\n";
        tekuci = tekuci->sledeci;
    }
}

void obradiPrekidima() {
    if (glava == nullptr) {
        cout << "Nema zahteva za obradu.\n";
        return;
    }

    CvorZahteva* tekuci = glava;
    while (tekuci != nullptr) {
        tekuci->zahtev.stanje = "ZAUZET";
        cout << "\nKontroler obradjuje zahtev "
             << tekuci->zahtev.redniBroj
             << ", a procesor obavlja drugi posao";
        for (int korak = 1; korak <= 3; korak++) {
            cout << ".";
            sacekaj(150);
        }
        tekuci->zahtev.stanje = "OBRADJEN";
        cout << "\nPrekid: kontroler je obavestio procesor o zavrsetku.\n";
        tekuci = tekuci->sledeci;
    }
}

void obradiDirektnimPristupomMemoriji() {
    if (glava == nullptr) {
        cout << "Nema zahteva za obradu.\n";
        return;
    }

    CvorZahteva* tekuci = glava;
    while (tekuci != nullptr) {
        tekuci->zahtev.stanje = "DMA_PRENOS";
        int brojKoraka = tekuci->zahtev.velicinaBloka;
        if (brojKoraka > 10) {
            brojKoraka = 10;
        }

        cout << "\nDirektan prenos zahteva "
             << tekuci->zahtev.redniBroj << ": ";
        for (int korak = 1; korak <= brojKoraka; korak++) {
            cout << "#";
            sacekaj(100);
        }
        tekuci->zahtev.stanje = "OBRADJEN";
        cout << " zavrsen, prekid je poslat procesoru.\n";
        tekuci = tekuci->sledeci;
    }
}

void sacuvajUDatoteku(const string& nazivDatoteke) {
    ofstream izlaznaDatoteka(nazivDatoteke);
    if (!izlaznaDatoteka) {
        cout << "Datoteka ne moze da se otvori za upis.\n";
        return;
    }

    CvorZahteva* tekuci = glava;
    while (tekuci != nullptr) {
        izlaznaDatoteka << tekuci->zahtev.redniBroj << '|'
                        << tekuci->zahtev.tipKontrolera << '|'
                        << tekuci->zahtev.operacija << '|'
                        << tekuci->zahtev.podatak << '|'
                        << tekuci->zahtev.velicinaBloka << '|'
                        << tekuci->zahtev.stanje << '\n';
        tekuci = tekuci->sledeci;
    }
    cout << "Lista je sacuvana u datoteku " << nazivDatoteke << ".\n";
}

void obrisiListu() {
    while (glava != nullptr) {
        CvorZahteva* zaBrisanje = glava;
        glava = glava->sledeci;
        delete zaBrisanje;
    }
    rep = nullptr;
}

void ucitajIzDatoteke(const string& nazivDatoteke) {
    ifstream ulaznaDatoteka(nazivDatoteke);
    if (!ulaznaDatoteka) {
        cout << "Datoteka ne postoji ili ne moze da se otvori.\n";
        return;
    }

    obrisiListu();
    string red;
    int najveciRedniBroj = 0;

    while (getline(ulaznaDatoteka, red)) {
        size_t prvo = red.find('|');
        size_t drugo = red.find('|', prvo + 1);
        size_t trece = red.find('|', drugo + 1);
        size_t cetvrto = red.find('|', trece + 1);
        size_t peto = red.find('|', cetvrto + 1);

        if (prvo == string::npos || drugo == string::npos ||
            trece == string::npos || cetvrto == string::npos ||
            peto == string::npos) {
            continue;
        }

        ZahtevKontrolera ucitaniZahtev;
        ucitaniZahtev.redniBroj = stoi(red.substr(0, prvo));
        ucitaniZahtev.tipKontrolera = red.substr(prvo + 1, drugo - prvo - 1);
        ucitaniZahtev.operacija = red.substr(drugo + 1, trece - drugo - 1);
        ucitaniZahtev.podatak = red.substr(trece + 1, cetvrto - trece - 1);
        ucitaniZahtev.velicinaBloka = stoi(red.substr(cetvrto + 1, peto - cetvrto - 1));
        ucitaniZahtev.stanje = red.substr(peto + 1);
        dodajNaKrajListe(ucitaniZahtev);

        if (ucitaniZahtev.redniBroj > najveciRedniBroj) {
            najveciRedniBroj = ucitaniZahtev.redniBroj;
        }
    }

    sledeciRedniBroj = najveciRedniBroj + 1;
    cout << "Zahtevi su ucitani iz datoteke.\n";
}

void obrisiObradjeneZahteve() {
    CvorZahteva* tekuci = glava;
    CvorZahteva* prethodni = nullptr;

    while (tekuci != nullptr) {
        if (tekuci->zahtev.stanje == "OBRADJEN") {
            CvorZahteva* zaBrisanje = tekuci;
            if (prethodni == nullptr) {
                glava = tekuci->sledeci;
            } else {
                prethodni->sledeci = tekuci->sledeci;
            }
            tekuci = tekuci->sledeci;
            if (zaBrisanje == rep) {
                rep = prethodni;
            }
            delete zaBrisanje;
        } else {
            prethodni = tekuci;
            tekuci = tekuci->sledeci;
        }
    }
    cout << "Obradjeni zahtevi su uklonjeni iz liste.\n";
}

void prikaziMeni() {
    cout << "\nSIMULACIJA KONTROLERA UREDJAJA\n";
    cout << "1. Dodaj zahtev\n";
    cout << "2. Prikazi listu zahteva\n";
    cout << "3. Obradi prozivanjem\n";
    cout << "4. Obradi prekidima\n";
    cout << "5. Obradi direktnim pristupom memoriji\n";
    cout << "6. Sacuvaj listu u datoteku\n";
    cout << "7. Ucitaj listu iz datoteke\n";
    cout << "8. Obrisi obradjene zahteve\n";
    cout << "0. Izlaz\n";
    cout << "Izbor: ";
}

int main() {
    int izbor;
    const string nazivDatoteke = "zahtevi_kontrolera.txt";

    do {
        prikaziMeni();
        cin >> izbor;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            izbor = -1;
        }

        switch (izbor) {
            case 1: unesiZahtev(); break;
            case 2: prikaziListu(); break;
            case 3: obradiProzivanjem(); break;
            case 4: obradiPrekidima(); break;
            case 5: obradiDirektnimPristupomMemoriji(); break;
            case 6: sacuvajUDatoteku(nazivDatoteke); break;
            case 7: ucitajIzDatoteke(nazivDatoteke); break;
            case 8: obrisiObradjeneZahteve(); break;
            case 0: cout << "Program je zavrsen.\n"; break;
            default: cout << "Izbor nije ispravan.\n";
        }
    } while (izbor != 0);

    obrisiListu();
    return 0;
}
