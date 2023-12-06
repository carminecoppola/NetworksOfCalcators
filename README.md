# NetworksOfCalcators

Questo repository contiene una serie di esercizi del corso di Reti dei Calcolatori implementati in linguaggio C. Ogni esercizio è organizzato in una cartella separata, contenente sia il codice del client che del server, oppure una connessione peer-to-peer.

## Comandi per l'esecuzione del server e del client

Prima di eseguire il server e il client, assicurati di compilare i file sorgenti utilizzando il compilatore GCC. Fornisci loro nomi significativi:

  ```
  gcc nomeFileServer.c -o server
  gcc nomeFileClient.c -o client
  ```

Questi comandi compileranno i file sorgenti in eseguibili con i nomi "server" e "client".

Per avviare il server, esegui il seguente comando che avvierà il server in background:

  ```
  ./server &
  ```

Per avviare il client e connetterlo al server, esegui il seguente comando, specificando l'indirizzo IP del server (nel caso dell'esempio, è "127.0.0.1"):

  ```
  ./client 127.0.0.1
  ```

## Comandi per l'uccisione dei processi

Per elencare tutti i processi in esecuzione, puoi utilizzare uno dei seguenti comandi:

  ```
  ps -a
  ps -ax
  ```
Per identificare il processo del server, puoi filtrare l'output utilizzando il comando "grep" in questo modo:

  ```
  ps -ax | grep server
  ```
Questo comando ti fornirà il nome del processo associato al server.

Per uccidere un processo, usa il comando "kill -9" seguito dal nome del processo. Ad esempio:

  ```
  kill -9 nomeProcesso
  ```
## Esecuzione di comandi multipli con il loop "for"

Per eseguire il client ripetutamente da terminale, puoi utilizzare un ciclo "for". Ecco un esempio:

  ```
  for i in {1..5}
  do
    ./client 127.0.0.1
  done
  ```

Questo eseguirà il client cinque volte, connettendolo al server con l'indirizzo IP "127.0.0.1".
