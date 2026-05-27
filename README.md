# Cifru Electronic (Smart Safe)

| | |
|-|-|
|`Author` | [Trasca Stefan-Gabriel]

## Description
Acest proiect reprezintă un **sistem de securitate electronic cu acces pe bază de parolă**, realizat cu ajutorul unui microcontroler. Scopul său principal este de a simula panoul de control al unui seif, validând input-ul utilizatorului și oferind feedback vizual și textual în timp real. Un element cheie al proiectului este persistența datelor: parola setată nu se pierde la oprirea curentului.

Procesul tehnologic din spatele sistemului urmează un flux logic bine definit:

* **Preluarea datelor:** Utilizatorul introduce codul PIN folosind o tastatură matricială (4x4). Fiecare apăsare de tastă este scanată și înregistrată de microcontroler.
* **Procesarea și validarea:** Placa Arduino compară secvența de caractere introdusă cu parola stocată în memoria nevolatilă (EEPROM). 
* **Gestionarea stărilor (Feedback-ul):** În funcție de rezultatul verificării, sistemul reacționează instant. Dacă parola este corectă, accesul este permis (se aprinde LED-ul Verde). Dacă parola este greșită, se înregistrează o încercare eșuată (se aprinde LED-ul Roșu). La trei încercări greșite consecutive, sistemul intră într-o stare de blocare de securitate. 

Rezultatul este un mecanism de control al accesului robust. În plus, sistemul dispune de un mod "Admin" care permite utilizatorului autentificat să schimbe parola curentă, noua valoare fiind scrisă direct în memoria fizică a cipului pentru a fi păstrată chiar și după deconectarea de la sursa de alimentare.

## Motivation
Am ales să realizez acest proiect din dorința de a explora concepte mai avansate de programare și electronică, trecând dincolo de simpla aprindere a unor componente. Mi-am dorit să construiesc un sistem care are o utilitate practică reală și care pune accent pe logica software.

Acest proiect a fost oportunitatea perfectă pentru a înțelege cum funcționează salvarea permanentă a datelor (memoria EEPROM) și cum se gestionează stările complexe ale unui sistem (Mașina de Stări - sistem blocat, sistem deblocat, mod de editare parolă). Dincolo de partea tehnică, satisfacția vine din crearea unui mecanism interactiv complet, similar cu sistemele de securitate pe care le întâlnim în viața de zi cu zi la interfoane sau seifuri hoteliere.

## Architecture
Arhitectura proiectului definește structura bloc-în-bloc a sistemului și modul în care informația și energia circulă între componente. Aceasta este împărțită în trei niveluri principale: **Hardware**, **Software** și **Alimentare/Energie**.

---

### 1. Arhitectura Hardware (Fluxul de Date)

Sistemul este configurat să comunice bidirecțional, având atât componente de input, cât și de output:

* **Blocul de Achiziție (Intrare):** Tastatura matricială 4x4 transmite semnale digitale către pinii Arduino (2-9) folosind o metodă de scanare pe rânduri și coloane pentru a detecta tasta apăsată.
* **Blocul de Procesare (Central):** Placa **Arduino Uno** rulează logica principală, verifică parolele și comunică prin Serial Monitor cu PC-ul (pe portul COM la 9600 baud rate).
* **Blocul de Execuție (Ieșire):** Prin pinii digitali `10` și `11`, Arduino controlează starea celor două LED-uri (Roșu și Verde) pentru a oferi feedback vizual instantaneu.

---

### 2. Arhitectura de Alimentare (Managementul Energiei)

Alimentarea este simplă și eficientă, bazându-se pe un circuit de tensiune mică:

* **Sursa de Putere:** Întregul sistem este alimentat direct prin cablul USB (5V) conectat la PC sau la un adaptor de priză.
* **Magistrala de Logică / Semnal:** Placa Arduino distribuie cei 5V interni către tastatură și pinii digitali. 
* **Protecția componentelor:** LED-urile nu sunt conectate direct la pinii Arduino, ci prin intermediul unor rezistoare de limitare a curentului, prevenind astfel arderea acestora.
* **Masa Comună (GND):** LED-urile împart aceeași conexiune de masă (GND) cu placa Arduino, închizând astfel circuitul electric în siguranță.

---

### 3. Arhitectura Software (Logica Codului)

Codul rulat pe Arduino este structurat pe baza unei **Mașini de Stări (State Machine)** care reacționează la evenimente externe:

[ Așteptare Input ] ➡️ [ Verificare vs EEPROM ] ➡️ [ Update Stare (Acces / Eroare / Blocare) ]

1. **Inițializarea (Setup):** La pornire, microcontrolerul citește adresa `0` din memoria EEPROM. Dacă găsește un cod salvat, îl încarcă în memorie; altfel, folosește codul implicit "1234".
2. **Procesarea Input-ului:** În bucla principală (`loop`), sistemul așteaptă apăsarea unei taste. Cifrele sunt adăugate într-un buffer, tasta `*` resetează input-ul, iar tasta `#` declanșează funcția de verificare.
3. **Logica de Securitate:** Dacă parola introdusă se potrivește, contoarele de eroare se resetează. Dacă este greșită, se incrementează o variabilă `incercariGresite`. Când aceasta atinge pragul de 3, variabila booleană `blocat` devine `true`, refuzând orice acces până la introducerea codului corect.


### Block diagram



### Schematic



### Components

| Device | Usage | Price |
|--------|--------|-------|
| Arduino Uno | Proceseaza logica și stochează datele | 55 RON |
| Tastatură 4x4 | Introducerea parolei de acces | 15 RON |
| LED Verde (3mm) | Semnalizează accesul permis | 1 RON |
| LED Roșu (3mm) | Semnalizează erorile/blocarea | 1 RON |
| 2x Rezistor 220Ω| Protejeaza LED-urile | 1 RON |
| Breadboard | Baza de conectare a componentelor | 10 RON |
| Fire Jumper | Conectează componentele la placă | 12 RON |

### Libraries

| Library | Description | Usage |
| :--- | :--- | :--- |
| [Arduino.h](https://www.arduino.cc/reference/en/) | Core-ul sistemului | Funcțiile de bază (I/O, Serial) |
| [Keypad.h](https://playground.arduino.cc/Code/Keypad/) | Gestionarea tastaturilor matriciale | Citirea rapidă a tastelor apăsate fără debounce manual |
| [EEPROM.h](https://docs.arduino.cc/learn/built-in-libraries/eeprom/) | Interacțiunea cu memoria nevolatilă | Salvarea și citirea parolei setate de utilizator |

## Log

### Week 6 - 12 May

### Week 7 - 19 May

### Week 20 - 26 May
