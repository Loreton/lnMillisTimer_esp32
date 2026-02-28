//
// updated by ...: Loreto Notarantonio
// Date .........: 30-08-2025 10.58.13
//


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  lnLOG_MODULE_LEVEL lnLOG_LEVEL_DEBUG
#include <lnLogger_Class.h> // Assicurati che questo sia disponibile nel tuo ambiente
#include "lnMillisTimer.h" // Includi il tuo nuovo file header della classe

// Costruttore vuoto....
MillisTimer::MillisTimer(void) {} ;
// MillisTimer(const char* name = nullptr, uint32_t duration = 0, TimerCallback callback = nullptr);
// Inizializza il timer
void MillisTimer::init(const char *name, uint32_t duration, TimerCallback callback, bool autoReload) {
    m_name = name;
    m_duration = duration;
    m_onCompleteCallback = callback;
    m_autoReload = autoReload;
    if (m_onCompleteCallback) {
        lnLOG_INFO("[TIMER: %s] created for %lu ms. (with callback)", m_name, m_duration);
    } else {
        lnLOG_INFO("[TIMER: %s] created for %lu ms. (without callback)", m_name, m_duration);
    }
}

// Avvia o riavvia il timer
/* ann
void MillisTimer::start(uint32_t duration) {
    if (m_isRunning) {
        lnLOG_WARN("[TIMER: %s] already running, new start command was rejected", m_name);
    } else {
        m_duration = (duration==0) ? m_duration : duration;
        m_isRunning = true;
        m_hasExpired = false;
        m_startTime = millis();
    }
    lnLOG_NOTIFY("[TIMER: %s] - start....%lu", m_name, m_duration);
}
*/
void MillisTimer::start(uint32_t duration) {
    m_duration = (duration==0) ? m_duration : duration;
    m_isRunning = true;
    m_hasExpired = false;
    m_startTime = millis();
    lnLOG_NOTIFY("[TIMER: %s] - started....%lu", m_name, m_duration);
}

// riavvia il timer (indipendete da start....)
void MillisTimer::restart(uint32_t duration) {
    m_duration = (duration==0) ? m_duration : duration;
    m_isRunning = true;
    m_hasExpired = false;
    m_startTime = millis();
    lnLOG_NOTIFY("[TIMER: %s] - restarted....%lu", m_name, m_duration);
}

// Ferma il timer
void MillisTimer::stop() {
    m_isRunning = false;
    m_hasExpired = false; // Resetta lo stato di completamento quando fermato
    lnLOG_NOTIFY("[TIMER: %s] stopped!", m_name);
}

// Controlla se il timer è scaduto e gestisce la callback
// All'interno di `update()`, setti `m_isRunning = false` **prima** di eseguire il callback.
//      Questo è ottimo perché permette al callback stesso di chiamare `restart()`
//      senza generare avvisi di "timer già in esecuzione".
void MillisTimer::update() {
    if (!m_isRunning) return; // Esci subito se non attivo

    unsigned long currentMillis = millis();
    m_elapsed = currentMillis - m_startTime;

    if (m_elapsed >= m_duration) {
        m_remaining = 0;

        // Gestione stato prima del callback
        if (m_autoReload) {
            m_startTime = currentMillis;
            m_hasExpired = false; // In auto-reload "scade" ma ricomincia subito
        } else {
            m_isRunning = false;
            m_hasExpired = true;
        }

        lnLOG_NOTIFY("[TIMER: %s] completed!", m_name);

        if (m_onCompleteCallback) {
            m_onCompleteCallback(this);
        }
    } else {
        m_remaining = m_duration - m_elapsed;
    }
}



// #############################################################
// # Se uso questo ovviamente posso fare a meno di fare l'update()
// #############################################################
bool MillisTimer::hasExpired(void) {
    // update(); NNNOOOO E lascia che sia l’utente a decidere quando chiamare
    return m_hasExpired;
}

// #############################################################
// # Se uso questo ovviamente posso fare a meno di fare l'update()
// #############################################################
bool MillisTimer::isRunning(void) {
    // update(); NNNOOOO E lascia che sia l’utente a decidere quando chiamare
    return m_isRunning;
}


// #############################################################
// # Se uso questo ovviamente posso fare a meno di fare l'update()
// #############################################################
void MillisTimer::showStatus(void) {
    lnLOG_NOTIFY("[TIMER: %s] - duration: %lu - remaining: %lu - hasExpired: %d - isRunning: %d", m_name, m_duration, m_remaining, m_hasExpired, m_isRunning);
}