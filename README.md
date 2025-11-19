# Lampada NeoPixel Modulare

## 📁 Struttura del Progetto

Questo progetto è organizzato in modo modulare per facilitare l'aggiunta e la modifica delle animazioni.

### File Principali

- **Lampada_Modulare.ino** - Sketch principale che gestisce:
  - Inizializzazione hardware (NeoPixel, sensori touch)
  - Gestione pulsanti TB1 e TB2
  - Salvataggio/caricamento impostazioni con Preferences
  - Cambio tra animazioni
  - Controllo luminosità

- **config.h** - File di configurazione condiviso:
  - Numero di LED (NUM_LEDS)
  - Pin di collegamento
  - Colore bianco personalizzato
  - Velocità di tutte le animazioni
  - **MODIFICA SOLO QUESTO FILE per personalizzare la tua lampada!**

### File delle Animazioni (8 totali)

Ogni animazione è in un file `.h` separato con due funzioni:

1. **animation_white.h** - Bianco fisso
   - Luce bianca costante

2. **animation_random.h** - Colori casuali
   - LED si accendono uno alla volta con colori random

3. **animation_rainbow.h** - Arcobaleno
   - Arcobaleno rotante intorno all'anello

4. **animation_wave.h** - Onda di luci
   - LED pari/dispari alternati in luminosità
   - Colore cambia dopo ogni ciclo

5. **animation_wave_inverse.h** - Onda inversa
   - Come l'onda normale ma invertita

6. **animation_wave_gradient.h** - Onda graduale
   - Onda con colori che cambiano gradualmente

7. **animation_wave_variable.h** - Onda velocità variabile
   - Onda con velocità che aumenta e diminuisce (effetto respiro)

8. **animation_pattern3.h** - Pattern ogni 3 LED
   - 4 gruppi di 3 LED che si accendono in sequenza

## 🔧 Come Usare

### Installazione

1. Crea una cartella chiamata `Lampada_Modulare`
2. Copia tutti i file nella cartella:
   - Lampada_Modulare.ino
   - animation_white.h
   - animation_random.h
   - animation_rainbow.h
   - animation_wave.h
   - animation_wave_inverse.h
   - animation_wave_gradient.h
   - animation_wave_variable.h
   - animation_pattern3.h

3. Apri `Lampada_Modulare.ino` con Arduino IDE
4. Carica lo sketch sull'ESP32

### Hardware Necessario

- ESP32
- Anello NeoPixel 12 LED (pin GPIO 5)
- Sensore touch TB1 (pin D3) - Accensione/Spegnimento
- Sensore touch TB2 (pin D4) - Cambio animazioni e luminosità

### Controlli

**TB1 (Touch Button 1)**:
- Premi per accendere/spegnere la lampada
- All'accensione riprende l'ultima animazione e luminosità

**TB2 (Touch Button 2)**:
- **Pressione BREVE** (< 500ms): Cambia animazione
- **Pressione LUNGA** (≥ 500ms): Varia la luminosità
  - Se luminosità > 50% → diminuisce
  - Se luminosità ≤ 50% → aumenta

## 🎨 Aggiungere Nuove Animazioni

Per aggiungere una nuova animazione:

1. **Crea un nuovo file** `animation_nome.h`

2. **Struttura del file**:
```cpp
#ifndef ANIMATION_NOME_H
#define ANIMATION_NOME_H

// Variabili di stato (con static)
static int miaVariabile = 0;

// Funzione di inizializzazione
void initNomeAnimation() {
  miaVariabile = 0;
  // Reset altre variabili
}

// Funzione dell'animazione
void nomeAnimation() {
  // Il tuo codice qui
  // Usa pixels.setPixelColor() e pixels.show()
  // Usa lastAnimationUpdate per il timing
}

#endif
```

3. **Modifica Lampada_Modulare.ino**:
   - Aggiungi `#include "animation_nome.h"`
   - Incrementa `NUM_ANIMATIONS`
   - Aggiungi case nello switch di `runCurrentAnimation()`
   - Aggiungi case nello switch di `handleTB1()`
   - Aggiungi case nello switch di `handleTB2()` (stampa nome)
   - Aggiungi chiamata a `initNomeAnimation()` nel reset

## 📝 Personalizzazione

### ⚡ Cambio Numero LED

**IMPORTANTE:** Per cambiare il numero di LED, modifica SOLO il file `config.h`:

```cpp
// In config.h
#define NUM_LEDS 16   // Cambia questo valore (12, 16, 24, etc.)
```

Tutti i file delle animazioni useranno automaticamente il nuovo valore!

### Velocità Animazioni

Tutte le velocità si configurano in `config.h`:

```cpp
// Animazione Colori Casuali
#define RANDOM_DELAY 100

// Animazione Rainbow  
#define RAINBOW_DELAY 20

// Animazioni Onda
#define WAVE_BRIGHTNESS_STEP 3
#define WAVE_UPDATE_DELAY 15
```

### Colore Bianco

Nel file `config.h` modifica:
```cpp
#define WHITE_R 150   // Rosso (0-255)
#define WHITE_G 230   // Verde (0-255)
#define WHITE_B 255   // Blu (0-255)
```

### Numero LED

Se hai un anello diverso, modifica:
```cpp
#define NUM_LEDS 12   // Cambia con il tuo numero di LED
```

## 🛠️ Funzioni Utility

Lo sketch principale fornisce funzioni utili per le animazioni:

- `Wheel(byte WheelPos)` - Converte 0-255 in colore arcobaleno
- `dimColor(uint32_t color, byte brightness)` - Riduce luminosità di un colore
- `lastAnimationUpdate` - Timestamp globale per timing

## 💾 Memoria Persistente

Il progetto usa Preferences (NVS) per salvare:
- Ultima animazione selezionata
- Ultima luminosità impostata

Le impostazioni vengono caricate automaticamente all'accensione.

## 📊 Sequenza Animazioni

1. Bianco fisso
2. Colori casuali
3. Rainbow
4. Onda di luci
5. Onda inversa
6. Onda graduale
7. Onda velocità variabile
8. Pattern 3 LED
(poi torna a 1)

## 🐛 Debug

Apri il Serial Monitor a 115200 baud per vedere:
- Stato sensori touch
- Animazione corrente
- Luminosità
- Salvataggio/caricamento impostazioni

## 📖 Librerie Richieste

- **Adafruit_NeoPixel** - Gestione LED NeoPixel
- **Preferences** - Salvataggio impostazioni (inclusa in ESP32)

Installa Adafruit_NeoPixel dal Library Manager di Arduino IDE.
