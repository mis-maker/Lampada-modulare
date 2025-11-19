# 🌈 Lampada Modulare con ESP32 e NeoPixel

Lampada intelligente con 16 LED NeoPixel controllata da ESP32 con 9 animazioni luminose e controlli intuitivi.

---

## 🎮 Come Si Usa

### Controlli Semplici e Intuitivi

```
┌─────────────────────────────────────────────────┐
│  TB1 (Sinistra)        TB2 (Destra)             │
│  ◀️ INDIETRO            ▶️ AVANTI                │
│  🔴 ON/OFF              ☀️ LUMINOSITÀ            │
└─────────────────────────────────────────────────┘
```

#### Pulsante TB1 (Pin D3) - Sinistra
- **Click breve** (< 0.7 sec): Va all'**animazione PRECEDENTE** ⬅️
- **Pressione lunga** (≥ 0.7 sec): **Accende/Spegne** la lampada 🔴

#### Pulsante TB2 (Pin D4) - Destra
- **Click breve** (< 0.7 sec): Va all'**animazione SUCCESSIVA** ➡️
- **Pressione lunga** (≥ 0.7 sec): **Varia la luminosità** in modo continuo ☀️

---

## 🎨 Le 9 Animazioni

0. **Bianco fisso** - Luce bianca costante per illuminazione
1. **Colori casuali** - LED che si accendono con colori random
2. **Rainbow** - Arcobaleno rotante classico
3. **Rainbow Wave** - Arcobaleno che pulsa con effetto onda
4. **Onda di luci** - Effetto onda con cambio colore ciclico
5. **Onda inversa Rainbow** - Onda invertita con arcobaleno rotante
6. **Onda graduale** - Onda con colori che cambiano fluidamente
7. **Onda velocità variabile** - Onda con effetto "respiro"
8. **Pattern Rainbow** - 4 gruppi di LED con arcobaleno rotante

---

## 🚀 Esempio d'Uso

### Prima Accensione
1. **Tieni premuto TB1** per 1 secondo → Lampada si accende
2. **Click su TB2** → Passa all'animazione successiva
3. **Click su TB1** → Torna all'animazione precedente
4. **Tieni premuto TB2** → Regola la luminosità
5. **Tieni premuto TB1** → Spegni la lampada

### Navigazione Veloce
```
Click TB2 → 0 → 1 → 2 → 3 → 4 → 5 → 6 → 7 → 8 → 0 (loop)
Click TB1 → 8 → 7 → 6 → 5 → 4 → 3 → 2 → 1 → 0 → 8 (loop)
```

---

## ⚙️ Personalizzazione

Tutte le impostazioni sono nel file **config.h**:

### Numero di LED
```cpp
#define NUM_LEDS 16  // Cambia con il tuo numero di LED
```

### Colore Bianco
```cpp
#define WHITE_R 150  // Rosso (0-255)
#define WHITE_G 230  // Verde (0-255)
#define WHITE_B 255  // Blu (0-255)
```

Presets consigliati:
- **Bianco freddo**: 150, 230, 255 (default)
- **Bianco neutro**: 200, 255, 255
- **Bianco caldo**: 220, 255, 240

### Velocità Animazioni
```cpp
#define RAINBOW_DELAY 20           // Velocità rainbow (più basso = più veloce)
#define WAVE_UPDATE_DELAY 15       // Velocità onde
#define PATTERN3_BRIGHTNESS_STEP 5 // Velocità pattern
```

### Tempi di Controllo
```cpp
#define LONG_PRESS_TIME 700  // Tempo per pressione lunga (millisecondi)
#define DEBOUNCE_TIME 50     // Filtro anti-rimbalzo pulsanti
```

---

## 🔧 Hardware Necessario

- **ESP32** (Arduino Nano ESP32 o compatibile)
- **Anello NeoPixel** (16 LED, ma configurabile per qualsiasi numero)
- **2 Sensori Touch** (o pulsanti normali)
- **Alimentazione** adeguata per i LED (5V, minimo 2A per 16 LED)

### Collegamenti

```
ESP32          NeoPixel
─────          ────────
Pin 5 (GPIO5)  → DIN (Data In)
5V             → VCC
GND            → GND

ESP32          Pulsanti
─────          ────────
Pin D3         → TB1 (Sinistra)
Pin D4         → TB2 (Destra)
GND            → GND comune
```

---

## 💾 Memoria Persistente

La lampada ricorda automaticamente:
- ✅ Ultima animazione usata
- ✅ Livello di luminosità impostato

Alla riaccensione riprende esattamente da dove avevi lasciato!

---

## 📁 Struttura del Progetto

```
Lampada_Modulare/
├── Lampada_Modulare.ino          # Sketch principale
├── config.h                       # ⭐ Configurazione centralizzata
├── animation_white.h              # Animazione 0
├── animation_random.h             # Animazione 1
├── animation_rainbow.h            # Animazione 2
├── animation_rainbow_wave.h       # Animazione 3
├── animation_wave.h               # Animazione 4
├── animation_wave_inverse.h       # Animazione 5
├── animation_wave_gradient.h      # Animazione 6
├── animation_wave_variable.h      # Animazione 7
├── animation_pattern3.h           # Animazione 8
├── README.md                      # Questa guida
└── GUIDA_COMPLETA.md             # Documentazione dettagliata
```

---

## 🛠️ Installazione

1. **Scarica** tutti i file del progetto
2. **Apri** `Lampada_Modulare.ino` in Arduino IDE
3. **Seleziona** la scheda corretta: `Tools → Board → ESP32 Arduino → Arduino Nano ESP32`
4. **Seleziona** la porta corretta: `Tools → Port → COM...`
5. **Carica** lo sketch sulla scheda
6. **Testa** i pulsanti e le animazioni!

---

## 🎓 Tips & Tricks

### Trovare l'Animazione Perfetta
```
1. Click veloce su TB2 per scorrere tutte le animazioni
2. Quando ne vedi una interessante, fermati
3. Se passi quella giusta, click su TB1 per tornare indietro
```

### Regolare la Luminosità
```
1. Tieni premuto TB2
2. La luminosità aumenta o diminuisce automaticamente
3. Rilascia quando è perfetta
4. Se va nella direzione sbagliata, tieni premuto di nuovo
```

### Cambio Veloce Tra Due Animazioni
```
Click TB2 → Vai avanti
Click TB1 → Torna indietro
Click TB2 → Vai avanti di nuovo
```

---

## 🐛 Risoluzione Problemi

### La lampada non si accende
- Verifica l'alimentazione (5V adeguati)
- Controlla i collegamenti
- Assicurati di **tenere premuto** TB1 per almeno 0.7 secondi

### I click non funzionano bene
- Aumenta `DEBOUNCE_TIME` in config.h se hai rimbalzi
- Aumenta `LONG_PRESS_TIME` se attivi per sbaglio la pressione lunga

### Le animazioni sono troppo veloci/lente
- Modifica i valori `*_DELAY` e `*_STEP` in config.h
- Valori più alti = più lento
- Valori più bassi = più veloce

### Alcuni LED non si accendono
- Verifica che `NUM_LEDS` in config.h corrisponda al tuo anello
- Controlla l'alimentazione (LED numerosi richiedono più corrente)

---

## 📖 Documentazione Aggiuntiva

- **GUIDA_COMPLETA.md** - Spiegazione dettagliata di ogni funzione
- **CONFIGURAZIONE.md** - Guida alla personalizzazione
- **CONTROLLI_SEMPLIFICATI.md** - Tutorial controlli
- **DEBOUNCING.md** - Spiegazione tecnica filtro anti-rimbalzo

---

## 🌟 Caratteristiche Principali

✅ **9 animazioni** colorate e dinamiche  
✅ **Controlli intuitivi** - Sinistra/Destra come un telecomando  
✅ **Memoria persistente** - Ricorda le tue preferenze  
✅ **Completamente configurabile** - Tutto in config.h  
✅ **Sistema modulare** - Facile aggiungere nuove animazioni  
✅ **Debouncing hardware** - Nessun problema di rimbalzi  
✅ **16 LED** (configurabile per qualsiasi numero)  

---

## 🎉 Buon Divertimento!

La tua lampada è pronta per illuminare e decorare! 🌈✨

Per domande o nuove idee, consulta la GUIDA_COMPLETA.md o modifica config.h per personalizzare ogni aspetto.

**Goditi le tue animazioni! 💡**
