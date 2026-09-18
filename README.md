# Seminarski rad – Operativni sistemi

**Tema:** Karakteristike kontrolera uređaja  
**Student:** Marko Janošev  
**Broj indeksa:** 7/23 IT  
**Fakultet:** Tehnički fakultet „Mihajlo Pupin“ Zrenjanin  
**Predmet:** Operativni sistemi  
**Predmetni nastavnik:** Prof. dr Ljubica Kazi  
**Predmetni asistent:** MSc Vuk Amižić  
**Školska godina:** 2025/26.

## Opis projekta

Praktični deo seminarskog rada predstavlja C++ konzolnu aplikaciju koja simulira osnovni rad kontrolera uređaja. Program prikazuje podržane tipove kontrolera i demonstrira I/O operaciju, DMA prenos i slanje prekida (IRQ) nakon završetka operacije.

## Funkcionalnosti

- prikaz NVMe, SATA, Ethernet i GPU kontrolera;
- simulacija upisa podataka i promene stanja kontrolera;
- simulacija DMA prenosa;
- simulacija IRQ signala nakon završetka operacije.

## Korišćene tehnologije

- C++17
- Microsoft Visual Studio 2022
- standardne biblioteke: `iostream`, `string`, `thread`, `chrono`

## Pokretanje

1. Otvoriti projekat ili datoteku `main.cpp` u Visual Studio 2022.
2. Podesiti C++17 standard ako je potrebno.
3. Pokrenuti program pomoću **F5** ili **Ctrl + F5**.
4. U meniju izabrati:
   - **1** – prikaz kontrolera
   - **2** – I/O operacija
   - **3** – DMA prenos
   - **0** – izlaz

## Veza sa seminarskim radom

Program praktično prikazuje registre i mehanizme opisane u seminarskom radu: status i kontrolu operacije, I/O prenos, direktan pristup memoriji (DMA) i prekid (IRQ).