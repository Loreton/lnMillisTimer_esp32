ESP32 context

# lnMillisTimer (ESP32)

`lnMillisTimer` è una libreria leggera e non bloccante per la gestione dei temporizzatori su ESP32 utilizzando il framework Arduino. Progettata per sostituire l'uso di `delay()`, permette di gestire eventi temporizzati lasciando il loop principale libero di eseguire altre operazioni.

## Caratteristiche

- **Non bloccante**: Utilizza `millis()` per il tracciamento del tempo.
- **Callback Support**: Supporta funzioni di callback che ricevono un puntatore all'istanza del timer (utile per gestire più timer con la stessa funzione).
- **Auto-Reload**: Opzione per riavviare automaticamente il timer alla scadenza.
- **Log Integrato**: Utilizza la libreria `lnLogger` per il debug e il monitoraggio degli stati.
- **Flessibile**: Metodi distinti per `start()` (avvio sicuro) e `restart()` (riavvio forzato).
- **Stato in tempo reale**: Metodi per ottenere il tempo rimanente, trascorso e lo stato di esecuzione.

---

## Installazione (PlatformIO)

Aggiungi le dipendenze al tuo file `platformio.ini`:

```ini
lib_deps =
    https://github.com/Loreton/lnLogger_esp32.git#v1.1.4
    # Aggiungi qui il path della tua libreria lnMillisTimer
```

---

## API Reference

### Inizializzazione
- `void init(const char *name, uint32_t duration, TimerCallback callback=nullptr, bool autoReload=false)`
  Inizializza il timer con un nome, la durata in ms, una funzione di callback opzionale e il flag per l'auto-reload.

### Controllo
- `void start(uint32_t msDuration=0)`: Avvia il timer. Se `msDuration` è 0, usa quella impostata in `init`.
- `void restart(uint32_t msDuration=0)`: Forza il riavvio del timer da zero.
- `void stop()`: Ferma il timer e resetta lo stato.
- `void update()`: **Fondamentale.** Deve essere chiamata nel `loop()` per processare la scadenza del tempo.

### Stato
- `bool isRunning()`: Ritorna `true` se il timer sta contando.
- `bool hasExpired()`: Ritorna `true` se il timer ha completato il ciclo (reset dopo la lettura o il riavvio).
- `uint32_t getRemainingTime()`: Millisecondi mancanti alla scadenza.
- `uint32_t getElapsedTime()`: Millisecondi trascorsi dall'avvio.

---

## Esempi d'uso

### 1. Utilizzo con Callback (Consigliato)
La callback viene eseguita automaticamente durante la chiamata a `.update()`.

```cpp
#include "lnMillisTimer.h"

MillisTimer myTimerCB;

void myTimerCallBack(MillisTimer *t) {
    Serial.printf("Timer %s completato!\n", t->name());
    // Posso riavviarlo con un nuovo tempo direttamente qui
    t->restart(2000);
}

void setup() {
    myTimerCB.init("LuceScale", 5000, myTimerCallBack);
    myTimerCB.start();
}

void loop() {
    myTimerCB.update(); // Gestisce la scadenza e chiama la callback
}
```

### 2. Utilizzo tramite Polling (Senza Callback)
Ideale per chi preferisce gestire la logica direttamente nel loop.

```cpp
MillisTimer myTimer;

void setup() {
    myTimer.init("Sensore", 10000);
    myTimer.start();
}

void loop() {
    myTimer.update();

    if (myTimer.hasExpired()) {
        // Logica eseguita alla scadenza
        Serial.println("Tempo scaduto!");
        myTimer.restart(); // Riavvia se necessario
    }
}
```

---

## Struttura Progetto

- `src/lnMillisTimer.h`: Definizione della classe.
- `src/lnMillisTimer.cpp`: Logica di funzionamento.
- `test/lnTimer_test.cpp`: Esempio completo di test con e senza callback.
- `platformio.ini`: Configurazione dell'ambiente di build.

---

## Requisiti

- **Framework**: Arduino ESP32
- **Librerie esterne**: [lnLogger_esp32](https://github.com/Loreton/lnLogger_esp32)

---

## Note per lo Sviluppatore
- Evitare l'uso di `delay()` all'interno delle funzioni di callback per non bloccare l'esecuzione del `loop()` e degli altri timer.
- Assicurarsi di chiamare sempre `.update()` per ogni istanza di timer definita.

**Autore:** Loreto Notarantonio
**Data Ultimo Aggiornamento:** 30-08-2025