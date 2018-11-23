/*
 * Sistema cronometrico per line follower
 * Il sistema deve fornire due informazioni:
 * Velocità Istantanea
 * Tempo di percorrenza del circuito
 * Le misure sono riferite ad un solo mezzo per cui, durante le misurazioni altri mezzi non dovranno percorrere il circuito.
 * La misura della "velocità istantanea" avviene in un tratto noto e sufficientemente corto del circuito
 * identificato dalla base del cronometro. Questo permette la stima della velocità su rettifilo del mezzo.
 * 
 * Il tempo di percorrenza è dato dal tempo trascorso fra la prima e la seconda entrata del mezzo nella
 * porta di misurazione.
 * 
 * La rilevazione del mezzo è effettuata tramite taglio di un fascio "Laser" ottenuto da un puntatore LED
 * a stretta focalizzazione posto in posizione semi radente la pista così da poter rilevare la maggior 
 * parte dei mezzi da line follower.
 * 
 * Principio di funzionamento e limiti
 * Le criticità per il funzionamento del sistema cronometrico sono individuabili:
 * -nella assenza a bordo dei mezzi da cronometrare di dispositivi specifici che partecipino alla misurazione.
 * - nella lunghezza del tratto noto e dunque nella capacità di stimare con precisione tempi brevi.
 * - nella notorità della lunghezza della pista e dunque nella difficoltà di calcolare la velocità
 * media di percorrenza con affidabilità.
 * 
 * Il sistema è pensato per poter misurare velocità istantanee dell'ordinne di quelle realizzate 
 * attualmente dai Lagorino ovvero, velocità prossime ai 3 m/s ovvero inferiori ai 12 Km/h con almeno 
 * un decimale sulla grandezza espressa in m/s.
 * Il tratto noto, sia per corenza con il principio di indeterminazione, sia per questioni pratiche,
 * non potrà essere di lunghezza rilevante menntre, non potrà essere di lunghezza irrisoria a causa
 * della velocità di elaborazionen del sistema di rilevazione ma anche delle tolleranzre meccaniche
 * imposte dalle tecniche di costruzione ed assemblaggio del rilevatore il quale dovrà permettere una 
 * facile ed economica realizzazione e una smontabilità e riassemblaggio affidabile se non inninfluente
 * per quanto riguarda le distanze e facile e pratico per quanto riguarda gli aspetti meccanici veri
 * e propri.
 * Partendo proprio dal sistema di rilevazione, si assume come unità centrale un dispositivo basato su
 * processore 328 della Atmel funzionante a 16 Mhz con una velocità di ciclo pari a 4 periodi di clock
 * e un numero di istruzioni minime per la gestione della rilevazione non inferiore a 50. Ne deriva un 
 * tempo di insensibilità del sistema pari a
 *                       1/16000000 * 4 * 50 = 0,0125 ms
 * Assumento arbitrariamente una distanza fra i sensori pari a 15 cm e una velocità di percorrenza di
 * 15 Km/h pari a m/s
 *                       15 * 1000 / 3600 = 4,16666 m/s
 * si ha un tempo di percorrenza pari a
 *                       0,15 / 4,16666 = 0,036 s = 36 ms
 * questo significa che, se consideriamo il caso più sfavorevole ovvero quello in cui il tempo di 
 * insensibilità dei due eventi di rilevazione (start e stop) si somma, la velocità istantanea rilevata 
 * dallo strumento sarà
 *                       36 - 2 * 0,0125 = 35,975 ms
 * pari ad uan velocità stimata di 
 *                       0,15 / 0,035975 = 4,16956 m/s = 15,01 Km/h
 * ne consegue che l'errore assoluto massimo atteso sul fondo scala (15Km/h) sarà intorno alla seconda
 * cifra decimale ovvero uguale o inferiore a 1/1000 della grandezza misurata.
 * L'errore calcolato con questo procedimento assume come irrilevante la copoonente dovuta ai 
 * dispositivi optoelettronici e la componente meccanica del sistema di rilevazione stesso.
 * Continuando ad assumere come trascurabile ogni errore introdotto dall'elettronica, rimangono le
 * componenti meccaniche. Gli elementi rilevanti in tal senso sono:
 * Le tolleranze di posizionamento
 * Le tolleranze di allineamento.
 * Per rispondere all'assunto iniziale secondo il quale i mezzi cronometrati non devono avere a bordo
 * dotazioni specifiche per la rilevazione se non i loro ingombri. Il sistema di rilevazione utilizza
 * una barrieta che rileva il passaggio dei mezzi.
 * L'errore di posizionamento dei sensori è assunto come ininfluente in quanto la superficie di
 * rilevamento del sensore può essere considerata rilevante nei confronti del diametro del fascio incidente.
 * 
 * La distanza delle sorgenti 
 * Considerando il porta sorgenti non disassemblabile e realizzato con tecniche non di alta precisione ovvero
 * non in grado di garantire pricisioni inferiori agli 0.5 mm su 15 cm (stampa 3d, taglio laser, assemblati
 * ibridi, ...), la stima dell'errore di pisizionamento introdotto è di 1 mm.
 * L'errore percentuale introtto è pari a
 *                          1 / 150 * 100 = 0.666 %
 * L'incidenza massima della variazione dimensionale sulla stima della velicità di riferimento (15 km/h), è
 *                     (0,15 - 0.001) / 4,16666 = 0,036 s = 35,760 ms
 * ovvero
 *                     (0,15 + 0.001) / 4,16666 = 0,036 s = 36,2400 ms
 * ne deriva che
 *                        0,15 / 0,035760 = 4,19463 m/s = 15,1 Km/h
 * ovvero
 *                         0,15 / 0,036240 = 4,1390 m/s = 14,9 Km/h
 *
 * In conclusione, si può affermare che il sistema garantisce nel range 0-15 km/h una precisione uguale o
 * migliore allo 0,7 %.
 * 
 * Realizzazione
 * 
 * Il dispositivo è composto da due unità indipendenti:
 * 1) unità illuminatore
 * 2) unità presentazione
 *
 * Unità illuminatore
 * Unità indipendente dotata di batteria LiPo, sistema di accensione e ricarica e di due illuminatori. L'unità è indipendente
 * dal punto di vista meccanico ed elettrico, va posizionata davanti all'unità presentazione allineandola in maniera che gli
 * illuminatori coincidano con i sensori. 
 * Unità presentazione
 * Unità indipendente dotata di batteria LiPo, sistema di accensione e ricarica, due sensori, unità di elaboraione e display 
 * con tastiera. Indipendente da un punto di vista meccanico ed elettrico, va posizionata di fornte all'illuminatore in
 * maniera tale che i due fasci generati dagli illuminitari colpiscano i sensori. Sarà dotata di un dispositivo per facilitare
 * l'allineamento.
 * Il display deve fornire indicazioni sulla velocità istantanea e sul tempo di percorrenza del circuito. Le velocità
 * saranno fornite in metri al secondo e in km/h mentre il tempo di percorrenza in secondi.
 * L'alimentazione è fornita da una cella LiPo con sistema di ricarica e sistema di accensione.
 * A portata di mano dovrà esserci il Reset e, dovrà essere valutata la possibilità di dotarlo di unità display separabile
 * o separata così da poterla posizionare in maniera comoda per la lettura.
 * 
 * In questa versione NON è previsto ne un sistema di memorizzazione dei risultati ne un sistema di trasmissione dei dati
 * verso risorse esterne, l'unico media di uscita è il display.
 * 
 */

/*
This is a simple sketch to demonstrate the use of analogComp, a
library to manage the analog comparator included in a wide 
variety of Atmel microcontrollers
This sketch enables an interrupt to be raised when on the analog input 0
(pin AIN0) there is a voltage greater than the voltage on the
analog input 1 (pin AIN1). To test the sketch, build the following
circuit:
- connect pin AIN1 to pin 3V3
- connect pin AIN0 to GND using a pull-down resistor (10Kohm or greater)
- connect pin AIN0 to pin 5V to activate the interrupt
More info on the analog comparator can be found reading the datasheet.
Please read the README file to know how to use this library.
Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>
  
This code and the analogComp library are free software; you can redistribute 
and/or modify them under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3.0 of the License,
or (at your option) any later version.
This work is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

//include the library
#include "analogComp.h"

//global variables
const byte LED13 = 13; //set the output LED
volatile boolean enableLed = false; //used to check if the interrupt has raised

//let's set up the hardware
void setup() {
    pinMode(LED13, OUTPUT); //LED pin as output
    analogComparator.setOn(AIN0, AIN1); //we instruct the lib to use voltages on the pins
    analogComparator.enableInterrupt(changeStatus, CHANGE); //we set the interrupt and when it has to be raised
}

//main loop
void loop() {
    if (enableLed) { //let's check if the analog comparator has raised the interrupt
        //yes, so we do a little blink of the LED
        digitalWrite(LED13, HIGH);
        delay(200);
        digitalWrite(LED13, LOW);
        enableLed = false;
    }
}

//interrupt to be raised by the analog comparator
void changeStatus() {
    enableLed = true; //let's inform the main loop that the condition has been reached by the analog comparator
}
