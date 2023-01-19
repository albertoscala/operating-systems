# Operating Systems

## Homework 1 - Basic C programming

Si chiede di realizzare un programma in C che calcoli il diff tra due file di testo. Le specifiche del
programma sono le seguenti:
1. Il programma riceve in input il nome di due file di testo, da ora in poi denominati F1 e F2,
ed una serie di opzioni, elencate al punto 3.
2. Il programma, una volta avviato, carica in memoria i due file di testo in blocchi da 50
righe, che vengono processate prima di leggere le successive righe del file.
3. Il programma calcola il diff tra la riga i-esima del file F1 e la riga i-esima del file F2,
restituendo un output che dipende dalle opzioni passate da riga di comando, e che sono

   - **-g**: se F1 ed F2 sono diversi in almeno una riga restituisce la stringa 
     `F1 ed F2 sono differenti`. Al posto di F1 ed F2 vanno restituiti i nomi dei file elaborati
   - **-s**: se F1 ed F2 sono uguali in tutte le righe restituisce la stringa `F1 ed F2 sono
     uguali`. Al posto di F1 ed F2 vanno restituiti i nomi dei file elaborati
   - **-d**: restituisce I numeri delle righe differenti
   - **-u**: restituisce I numeri delle righe uguali
   - **-v**: combinato con **–d** o **–u** stampa le righe che differiscono o che sono uguali. Il
   formato di output deve essere<br><br>
   ```
   #Riga, FILENAME F1: <riga di F1 che differisce o che e’ uguale>
   #Riga, FILENAME F2: <riga di F2 che differisce o che e’ uguale>
   ```
   - **-o NOME_FILE_OUTPUT**: ridireziona l’output su di un file di testo.

*Le opzioni –g e –s possono essere combinate; le opzioni –d e –u sono mutuamente
esclusive; l’opzione -o può essere sempre utilizzata.*

4. Nel caso in cui i due file abbiano lunghezza diversa, nel caso vengano specificate le
opzioni -d, –u e -v il confronto deve essere effettuato per il numero di righe contenuto nel
file più piccolo (quello con il minor numero di righe).

### Tests

Il programma deve funzionare correttamente nei seguenti casi:

  1. L’input consiste di due file uguali (testare con 5 differenti coppie di file di lunghezza
  maggiore e minore di 50 righe)
  2. L’input consiste di due file differenti (testare con 5 differenti coppie di file di lunghezza
  maggiore e minore di 50 righe)
  3. Entrambi I file in input, o uno dei due, non esiste
  4. Uno dei due file in input e’ vuoto
  5. I due file di input hanno lunghezza diversa (testare con 5 differenti coppie di file)

## Homework 2 - System programming

Si chiede di realizzare, con il linguaggio C, un programma server (da ora in poi server) ed un
programma client (da ora in poi client) che soddisfino le seguenti specifiche:
1. Il server riceve richieste da client remoti mediante un socket TPC/IP
2. Il client richiede che il server svolga un’operazione aritmetica e restituisca il risultato. Le
richieste effettuate dai client contengono quindi un'operazione aritmetica ed i relativi
operandi
3. Il server calcola l’operazione richiesta e restituisce il risultato al client mediante socket
TCP/IP.
Ulteriori specifiche di dettaglio sono:
4. Le operazioni che il server deve essere in grado di calcolare sono: **+**, **-**, <strong>*</strong>, **/**
5. Il messaggio inviato dal client ha la seguente struttura

<p align="center"><code>[operazione, Operando1, Operando2]</code></p>

6. Il messaggio di risposta inviato dal server al client ha la seguente struttura

<p align="center"><code>[timestamp ricezione richiesta, timestamp invio risposta, risultato operazione]</code></p>

7. Il server deve essere in grado di gestire piu’ client concorrentemente, ovvero e’
multithread
8. Il server scrive in un log unico le operazioni effettuate, i risultati, i timestamp, l’id del
client (ad es. Indirizzo IP). Deve essere garantito e gestito l’accesso concorrente al log.
9. Il client deve consentire all’utente di inserire un'operazione mediante una stringa del tipo
`Operando1 operazione Operando2`.
10. Il client deve trasformare la stringa di input nel formato del messaggio specificato
11. Quando il client riceve la risposta ad una richiesta, deve visualizzare il risultato
dell’operazione e il tempo di servizio, ovvero `timestamp invio risposta - timestamp
ricezione richiesta`
12. Il client, una volta attivato, deve consentire di inserire quante operazioni l’utente
desidera, fino ad esplicita terminazione del client stesso. L’utente, non puo’ richiedere
una nuova operazione prima della ricezione della risposta alla richiesta precedente.

***NOTA**: non e’ richiesta l’implementazione di un'interfaccia grafica.*

### Tests

Il programma deve funzionare correttamente nei seguenti casi:
- A + B, A - B, A * B, A/B dove A e B sono numeri reali diversi da 0 (calcolare almeno 5
operazioni per ogni operatore)
- A/B, dove B è’ zero.
- La stringa di input fornita dall’utente non e’ correttamente formattata, ovvero uno degli
operandi manca o non e’ un numero, l’operatore e’ assente o non e’ uno di quelli previsti
- Il processo client non riesce a connettersi al processo server, perche’ ad esempio il
server non è attivo
- Il processo server termina inaspettatamente
