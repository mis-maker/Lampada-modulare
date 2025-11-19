# 📘 Guida Completa - Lampada Modulare ESP32

Documentazione tecnica completa del progetto Lampada Modulare con NeoPixel.

---

## 📑 Indice

1. [Panoramica del Progetto](#panoramica)
2. [Hardware e Collegamenti](#hardware)
3. [Controlli Dettagliati](#controlli)
4. [Le Animazioni](#animazioni)
5. [Configurazione](#configurazione)
6. [Struttura del Codice](#codice)
7. [Memoria Persistente](#memoria)
8. [Aggiungere Nuove Animazioni](#nuove-animazioni)

---

## 🎯 Panoramica del Progetto {#panoramica}

### Cos'è

Una lampada decorativa controllata da ESP32 che gestisce un anello di 16 LED NeoPixel con 9 animazioni diverse, controlli intuitivi tramite due pulsanti e memoria che ricorda le impostazioni.

### Caratteristiche Principali

- **9 animazioni** precaricate (da bianco fisso a pattern arcobaleno)
- **Navigazione bidirezionale**: avanti e indietro tra le animazioni
- **Controllo luminosità** continuo e intuitivo
- **Memoria NVS**: salva animazione e luminosità anche a ESP32 spento
- **Sistema modulare**: ogni animazione in un file separato
- **Configurazione centralizzata**: tutti i parametri in config.h
- **Debouncing hardware**: elimina i rimbalzi dei pulsanti

---

## 🔌 Hardware e Collegamenti {#hardware}

### Componenti Necessari

| Componente | Specifiche | Note |
|------------|------------|------|
| **Microcontrollore** | ESP32 (Arduino Nano ESP32) | Qualsiasi ESP32 compatibile |
| **LED** | Anello NeoPixel 16 LED | Configurabile per qualsiasi numero |
| **Pulsanti** | 2x Sensori touch o pulsanti | TB1 e TB2 |
| **Alimentazione** | 5V, 2-3A | Dipende dal numero di LED |
| **Cavi** | Jumper wire | Per collegamenti |

### Schema Elettrico

```
┌──────────────┐
│   ESP32      │
│ Nano ESP32   │
│              │
│  D3 ●────────┼─── TB1 (Pulsante Sinistra)
│  D4 ●────────┼─── TB2 (Pulsante Destra)
│              │
│  GPIO5 ●─────┼─── DIN (Data) → NeoPixel
│  5V ●────────┼─── VCC → NeoPixel
│  GND ●───────┼─── GND → NeoPixel + Pulsanti
└──────────────┘
```

### Collegamenti Dettagliati

#### NeoPixel
```
ESP32 Pin 5 (GPIO5)  →  DIN (Data Input)
ESP32 5V             →  VCC/5V
ESP32 GND            →  GND
```

#### Pulsante TB1 (Sinistra)
```
ESP32 Pin D3  →  Un lato del pulsante
GND           →  Altro lato del pulsante
```

#### Pulsante TB2 (Destra)
```
ESP32 Pin D4  →  Un lato del pulsante
GND           →  Altro lato del pulsante
```

### Note sull'Alimentazione

- **LED a piena luminosità**: ~60mA per LED
- **16 LED al massimo**: ~1A
- **ESP32**: ~500mA
- **Totale consigliato**: Alimentatore da 2-3A

⚠️ **Importante**: Se usi più di 8 LED, alimenta il NeoPixel direttamente (non attraverso ESP32).

---

## 🎮 Controlli Dettagliati {#controlli}

### Pulsante TB1 - Navigazione Indietro e ON/OFF

#### Click Breve (< 700ms)
**Funzione**: Animazione PRECEDENTE

**Come funziona**:
1. Premi e rilascia velocemente TB1
2. L'animazione corrente decrementa di 1
3. Se sei sull'animazione 0, vai all'animazione 8 (wraparound)

**Esempio**:
```
Sei su animazione 5 → Click TB1 → Vai a animazione 4
Sei su animazione 0 → Click TB1 → Vai a animazione 8
```

**Nel codice**:
```cpp
currentAnimation--;
if (currentAnimation < 0) {
  currentAnimation = NUM_ANIMATIONS - 1;  // Wraparound
}
```

#### Pressione Lunga (≥ 700ms)
**Funzione**: ACCENDE/SPEGNE la lampada

**Come funziona**:
1. Tieni premuto TB1 per almeno 0.7 secondi
2. La variabile `lampOn` si inverte (true ↔ false)
3. Se accendi: ripristina l'ultima animazione e luminosità salvate
4. Se spegni: salva la luminosità corrente

**Esempio**:
```
Lampada spenta → Tieni TB1 → Lampada ACCESA (animazione 3, luminosità 180)
Lampada accesa → Tieni TB1 → Lampada SPENTA (salva stato)
```

### Pulsante TB2 - Navigazione Avanti e Luminosità

#### Click Breve (< 700ms)
**Funzione**: Animazione SUCCESSIVA

**Come funziona**:
1. Premi e rilascia velocemente TB2
2. L'animazione corrente incrementa di 1
3. Se sei sull'animazione 8, vai all'animazione 0 (wraparound)

**Esempio**:
```
Sei su animazione 3 → Click TB2 → Vai a animazione 4
Sei su animazione 8 → Click TB2 → Vai a animazione 0
```

**Nel codice**:
```cpp
currentAnimation = (currentAnimation + 1) % NUM_ANIMATIONS;
```

#### Pressione Lunga (≥ 700ms)
**Funzione**: VARIA LUMINOSITÀ

**Come funziona**:
1. Tieni premuto TB2
2. Sistema determina direzione automaticamente:
   - Se luminosità > 50% → Diminuisce
   - Se luminosità < 50% → Aumenta
3. La luminosità cambia continuamente fino al rilascio
4. Range: 10 (minimo) - 255 (massimo)

**Esempio**:
```
Luminosità 200 → Tieni TB2 → Diminuisce: 195, 190, 185...
Luminosità 30 → Tieni TB2 → Aumenta: 35, 40, 45...
```

**Velocità cambio**: 5 step ogni 50ms (configurabile in config.h)

### Debouncing - Anti-Rimbalzo

**Problema**: I pulsanti meccanici "rimbalzano" quando premuti, creando multipli segnali.

**Soluzione**: Il codice ignora qualsiasi pressione entro 50ms dalla precedente.

```cpp
if (currentTime - lastPressTime < DEBOUNCE_TIME) {
  return;  // Ignora rimbalzo
}
```

---

## 🎨 Le Animazioni {#animazioni}

### 0. Bianco Fisso
**File**: `animation_white.h`

**Descrizione**: Tutti i LED accesi con luce bianca costante.

**Uso**: Illuminazione pratica, lettura, luce ambiente.

**Personalizzazione**:
```cpp
// In config.h
#define WHITE_R 150  // Più rosso = bianco più caldo
#define WHITE_G 230
#define WHITE_B 255  // Più blu = bianco più freddo
```

---

### 1. Colori Casuali
**File**: `animation_random.h`

**Descrizione**: I LED si accendono uno alla volta con colori casuali.

**Velocità**: Configurabile con `RANDOM_DELAY`

**Effetto visivo**:
```
t=0ms:    LED 0 = Rosso
t=100ms:  LED 1 = Verde
t=200ms:  LED 2 = Blu
t=300ms:  LED 3 = Giallo
...
```

---

### 2. Rainbow (Arcobaleno)
**File**: `animation_rainbow.h`

**Descrizione**: Arcobaleno completo che ruota attorno all'anello.

**Come funziona**: Ogni LED ha un colore diverso dell'arcobaleno, e tutti i colori ruotano insieme.

**Matematica**:
```cpp
color = Wheel(((ledPosition * 256 / NUM_LEDS) + rotation) & 255)
```

**Velocità**: `RAINBOW_DELAY` (default 20ms per frame)

---

### 3. Rainbow Wave
**File**: `animation_rainbow_wave.h`

**Descrizione**: Arcobaleno rotante + effetto onda pulsante.

**Come funziona**:
- Colori: Come Rainbow (ruotano)
- Luminosità: LED pari e dispari alternano luminosità (effetto onda)

**Effetto visivo**:
```
Frame 1: Pari=Scuro, Dispari=Brillante, Colori=Rosso→Verde
Frame 2: Pari=Medio, Dispari=Medio, Colori=Arancione→Giallo
Frame 3: Pari=Brillante, Dispari=Scuro, Colori=Giallo→Verde
```

**Velocità**: `RAINBOW_WAVE_UPDATE_DELAY` e `RAINBOW_WAVE_BRIGHTNESS_STEP`

---

### 4. Onda di Luci
**File**: `animation_wave.h`

**Descrizione**: Onda che pulsa tra LED pari e dispari. Il colore cambia a ogni ciclo completo.

**Come funziona**:
- LED pari: luminosità da 10 → 255 → 10
- LED dispari: luminosità da 255 → 10 → 255
- Cambio colore ogni ciclo

**Pattern**:
```
Ciclo 1: Rosso (pari bassi, dispari alti)
Ciclo 2: Verde (pari alti, dispari bassi)
Ciclo 3: Blu (pari bassi, dispari alti)
```

---

### 5. Onda Inversa Rainbow
**File**: `animation_wave_inverse.h`

**Descrizione**: Come Onda di Luci, ma con arcobaleno rotante invece di colori fissi.

**Differenza con Rainbow Wave**:
- **Rainbow Wave**: Tutti i LED hanno luminosità che pulsa uguale
- **Onda Inversa Rainbow**: LED pari/dispari pulsano opposti + arcobaleno

**Effetto**: Onda dinamica con tutti i colori dell'arcobaleno.

---

### 6. Onda Graduale
**File**: `animation_wave_gradient.h`

**Descrizione**: Onda con cambio colore graduale e fluido (non a step).

**Come funziona**: Mentre l'onda pulsa, il colore cambia continuamente attraverso l'arcobaleno.

**Effetto**: Transizioni di colore molto morbide e ipnotiche.

---

### 7. Onda Velocità Variabile
**File**: `animation_wave_variable.h`

**Descrizione**: Onda che cambia velocità ciclicamente (effetto "respiro").

**Come funziona**:
```
Fase 1: Onda lenta (speed = 1)
Fase 2: Onda accelera gradualmente
Fase 3: Onda veloce (speed = 6)
Fase 4: Onda rallenta gradualmente
Ripeti
```

**Effetto**: Come se la lampada "respirasse" cambiando ritmo.

---

### 8. Pattern Rainbow
**File**: `animation_pattern3.h`

**Descrizione**: 4 gruppi di LED, ognuno con colore arcobaleno diverso. I gruppi si accendono in sequenza.

**Struttura gruppi (16 LED)**:
```
Gruppo 0: LED 0-3   → Rosso
Gruppo 1: LED 4-7   → Verde
Gruppo 2: LED 8-11  → Blu
Gruppo 3: LED 12-15 → Giallo
```

**Animazione**: Un gruppo alla volta pulsa da scuro a brillante, mentre i colori ruotano.

**Adattivo**: Funziona con qualsiasi numero di LED (divide sempre in 4 gruppi).

---

## ⚙️ Configurazione {#configurazione}

### File config.h - Centro di Controllo

Tutti i parametri configurabili sono in **config.h**. Questo file viene incluso da tutti gli altri.

### Sezione Hardware

```cpp
// Numero di LED nell'anello
#define NUM_LEDS 16

// Pin di collegamento
#define TOUCH1_PIN D3       // TB1 (Sinistra)
#define TOUCH2_PIN D4       // TB2 (Destra)
#define DATA_PIN 5          // GPIO 5 per NeoPixel
```

**Per cambiare numero LED**: Modifica solo `NUM_LEDS`. Tutto si adatta automaticamente!

### Sezione Colore Bianco

```cpp
#define WHITE_R 150   // Rosso (0-255)
#define WHITE_G 230   // Verde (0-255)
#define WHITE_B 255   // Blu (0-255)
```

**Esempi**:
- Bianco freddo: `150, 230, 255`
- Bianco neutro: `200, 255, 255`
- Bianco caldo: `220, 255, 240`

### Sezione Velocità Animazioni

```cpp
// Colori casuali
#define RANDOM_DELAY 100        // ms tra un LED e l'altro

// Rainbow
#define RAINBOW_DELAY 20        // ms per frame

// Onde
#define WAVE_BRIGHTNESS_STEP 3  // Step luminosità (1-10)
#define WAVE_UPDATE_DELAY 15    // ms per frame
```

**Regola generale**:
- Valori `DELAY` più bassi = Più veloce
- Valori `STEP` più alti = Cambio più brusco

### Sezione Controlli

```cpp
// Tempo per pressione lunga
#define LONG_PRESS_TIME 700     // ms

// Debouncing
#define DEBOUNCE_TIME 50        // ms

// Luminosità
#define BRIGHTNESS_STEP 5       // Quanto cambia
#define BRIGHTNESS_DELAY 50     // ms tra cambi
```

### Variabili Globali Esportate

```cpp
extern unsigned long lastAnimationUpdate;
extern Adafruit_NeoPixel pixels;
extern uint32_t Wheel(byte WheelPos);
extern uint32_t dimColor(uint32_t color, byte brightness);
```

Queste permettono ai file delle animazioni di accedere alle funzioni comuni.

---

## 💻 Struttura del Codice {#codice}

### File Principale: Lampada_Modulare.ino

#### Setup()
```cpp
void setup() {
  // 1. Inizializza Serial
  Serial.begin(115200);
  
  // 2. Inizializza Preferences (memoria NVS)
  preferences.begin("lampada", false);
  
  // 3. Carica impostazioni salvate
  currentAnimation = loadAnimation();
  brightness = loadBrightness();
  
  // 4. Inizializza NeoPixel
  pixels.begin();
  pixels.setBrightness(brightness);
  
  // 5. Configura pin pulsanti
  pinMode(TOUCH1_PIN, INPUT);
  pinMode(TOUCH2_PIN, INPUT);
}
```

#### Loop()
```cpp
void loop() {
  // 1. Gestisce TB1 (indietro + ON/OFF)
  handleTB1();
  
  // 2. Se lampada accesa
  if (lampOn) {
    handleTB2();           // Gestisce TB2 (avanti + luminosità)
    runCurrentAnimation(); // Esegue animazione corrente
  } else {
    pixels.clear();        // Spegne LED
    pixels.show();
  }
  
  delay(10);  // Piccolo delay per stabilità
}
```

### Funzioni Principali

#### handleTB1()
Gestisce tutti gli input di TB1:
- Debouncing
- Distinzione click/pressione lunga
- Cambio animazione precedente
- ON/OFF

#### handleTB2()
Gestisce tutti gli input di TB2:
- Debouncing
- Distinzione click/pressione lunga
- Cambio animazione successiva
- Variazione luminosità continua

#### runCurrentAnimation()
```cpp
void runCurrentAnimation() {
  switch (currentAnimation) {
    case 0: whiteAnimation(); break;
    case 1: randomAnimation(); break;
    case 2: rainbowAnimation(); break;
    // ... tutte le 9 animazioni
  }
}
```

### Funzioni Utility

#### Wheel()
Converte un valore 0-255 in un colore dell'arcobaleno:
```cpp
0   → Rosso
64  → Giallo
128 → Verde
192 → Blu
255 → Viola
```

#### dimColor()
Riduce la luminosità di un colore senza cambiare la tonalità:
```cpp
uint32_t dimmedColor = dimColor(originalColor, brightness);
```

---

## 💾 Memoria Persistente {#memoria}

### Sistema NVS (Non-Volatile Storage)

ESP32 usa la libreria `Preferences` che salva dati nella memoria flash.

### Cosa Viene Salvato

1. **Animazione corrente** (`currentAnimation`)
   - Chiave: `"anim"`
   - Tipo: int (0-8)
   - Quando: Ogni cambio animazione

2. **Luminosità** (`brightness`)
   - Chiave: `"bright"`
   - Tipo: int (10-255)
   - Quando: Ogni spegnimento

### Funzioni di Gestione

```cpp
void saveAnimation() {
  preferences.putInt("anim", currentAnimation);
}

int loadAnimation() {
  int saved = preferences.getInt("anim", 0);  // 0 = default
  if (saved >= 0 && saved < NUM_ANIMATIONS) {
    return saved;
  }
  return 0;  // Fallback
}
```

### Vantaggi

✅ Sopravvive allo spegnimento  
✅ Sopravvive al reset  
✅ Non richiede EEPROM.commit()  
✅ Gestione errori automatica  
✅ Scritture "atomiche" (più sicure)

---

## ➕ Aggiungere Nuove Animazioni {#nuove-animazioni}

### Procedura Completa

#### 1. Crea il File dell'Animazione

Crea `animation_nuova.h`:

```cpp
#ifndef ANIMATION_NUOVA_H
#define ANIMATION_NUOVA_H

#include "config.h"

// Variabili di stato (se servono)
static int nuovaStep = 0;

void initNuovaAnimation() {
  nuovaStep = 0;
}

void nuovaAnimation() {
  if (millis() - lastAnimationUpdate >= 50) {
    
    // Il tuo codice dell'animazione qui
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    }
    pixels.show();
    
    nuovaStep++;
    lastAnimationUpdate = millis();
  }
}

#endif
```

#### 2. Includi nel File Principale

In `Lampada_Modulare.ino`:
```cpp
#include "animation_nuova.h"
```

#### 3. Aggiorna NUM_ANIMATIONS

In `config.h`:
```cpp
#define NUM_ANIMATIONS 10  // Era 9, ora 10
```

#### 4. Aggiungi agli Switch

In `handleTB1()` e negli altri switch:
```cpp
case 9:
  Serial.println("Nuova Animazione");
  initNuovaAnimation();
  break;
```

In `runCurrentAnimation()`:
```cpp
case 9:
  nuovaAnimation();
  break;
```

#### 5. Ricompila e Carica

Fatto! La nuova animazione è integrata.

---

## 🎓 Consigli Finali

### Best Practices

✅ Modifica solo `config.h` per personalizzazioni  
✅ Usa `Serial.println()` per debug  
✅ Testa un'animazione alla volta  
✅ Mantieni `lastAnimationUpdate` per timing corretto  
✅ Usa `dimColor()` per effetti luminosità

### Performance

- Evita `delay()` nelle animazioni
- Usa `millis()` per timing non bloccante
- `pixels.show()` solo quando necessario (è lento)
- Mantieni i loop corti

### Debug

```cpp
// In loop(), aggiungi:
Serial.print("Animazione: ");
Serial.print(currentAnimation);
Serial.print(" | Luminosità: ");
Serial.println(brightness);
```

---

## 📞 Supporto

Per domande o problemi:
1. Controlla questa guida
2. Verifica `config.h`
3. Usa Serial Monitor per debug
4. Verifica collegamenti hardware

---

**Buon coding e buon divertimento con la tua lampada! 🌈✨**
