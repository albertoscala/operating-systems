# Operating Systems

## Homework 1 - Basic C programming

You are asked to make a program in C that calculates the differences between two text files. The specifications of the
program are as follows:
1. The program receives as input the name of two text files, henceforth referred to as F1 and F2,
and a set of options, listed in Step 3.
2. The program, once started, loads the two text files into memory in blocks of 50
lines, which are processed before reading the next lines of the file.
3. The program calculates the diff between the i-th line of file F1 and the i-th line of file F2,
returning an output that depends on the options passed from the command line, which are

   - **-g**: if F1 and F2 are different in at least one line returns the string 
     `F1 and F2 are different`. The names of the processed files should be returned instead of F1 and F2.
   - **-s**: if F1 and F2 are equal in all rows returns the string `F1 and F2 are
     equal`. The names of the processed files should be returned instead of F1 and F2.
   - **-d**: returns the numbers of different rows
   - **-u**: returns the numbers of the equal rows
   - **-v**: combined with **-d** or **-u** prints lines that differ or are the same. The
   output format must be<br><br>
   ```
   #Line, FILENAME F1: <F1 line that differs or is the same>
   #Line, FILENAME F2: <F2 line that differs or is the same>
   ```
   - **-o FILENAME_OUTPUT**: redirects the output to a text file.

*The -g and -s options can be combined; the -d and -u options are mutually
exclusive; the -o option can always be used.*

4. In case the two files have different lengths, in case the
-d, -u and -v options, the comparison must be made for the number of rows contained in the
smaller file (the one with the smallest number of rows).

### Tests

The program should work properly in the following cases:

  1. The input consists of two equal rows (test with 5 different pairs of rows of lengths
  greater and less than 50 rows)
  2. The input consists of two different files (test with 5 different pairs of files of lengths
  greater and less than 50 lines)
  3. Both input files, or either one, does not exist
  4. One of the two input files is empty
  5. The two input files have different lengths (test with 5 different pairs of files)

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
