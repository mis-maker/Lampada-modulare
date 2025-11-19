# 🎨 Lampada NeoPixel Modulare - Guida Completa

## 📦 Contenuto del Progetto

Il progetto è composto da **9 file** organizzati in modo modulare:

### 1️⃣ File Principale
- **Lampada_Modulare.ino** (15 KB)
  - Gestisce hardware, pulsanti, memoria e orchestrazione animazioni

### 2️⃣ File Animazioni (8 totali)

| File | Dimensione | Descrizione |
|------|-----------|-------------|
| **animation_white.h** | 739 B | Luce bianca fissa |
| **animation_random.h** | 1.1 KB | Colori casuali sequenziali |
| **animation_rainbow.h** | 1.2 KB | Arcobaleno rotante |
| **animation_wave.h** | 2.4 KB | Onda alternata pari/dispari |
| **animation_wave_inverse.h** | 2.4 KB | Onda invertita |
| **animation_wave_gradient.h** | 2.4 KB | Onda con colori fluidi |
| **animation_wave_variable.h** | 3.1 KB | Onda con velocità variabile |
| **animation_pattern3.h** | 2.7 KB | Pattern ogni 3 LED |

### 3️⃣ Documentazione
- **README.md** (4.8 KB)
  - Guida completa all'uso e personalizzazione

---

## 🎭 Descrizione Dettagliata delle Animazioni

### 🤍 **1. Bianco Fisso**
- **Effetto**: Luce bianca costante e stabile
- **Uso ideale**: Illuminazione d'ambiente, lettura
- **Personalizzabile**: Temperatura colore (caldo/freddo)

### 🎲 **2. Colori Casuali**
- **Effetto**: I LED si accendono uno alla volta con colori random
- **Velocità**: 100ms per LED (configurabile)
- **Uso ideale**: Atmosfera festosa, party

### 🌈 **3. Rainbow (Arcobaleno)**
- **Effetto**: Arcobaleno che ruota continuamente
- **Velocità**: 20ms per frame
- **Ciclo**: 5 giri completi poi riparte
- **Uso ideale**: Decorazione, atmosfera rilassante

### 🌊 **4. Onda di Luci**
- **Effetto**: LED pari e dispari si alternano creando un'onda
- **Comportamento**:
  - LED pari: 10 → 255 → 10 (luminosità)
  - LED dispari: 255 → 10 → 255 (luminosità)
- **Colori**: Cambiano ad ogni ciclo completo (rosso → arancione → giallo → verde → blu → viola → rosso)
- **Uso ideale**: Effetto dinamico e ritmico

### 🔄 **5. Onda Inversa**
- **Effetto**: Come l'onda normale ma invertita
- **Comportamento**:
  - LED pari: 255 → 10 → 255
  - LED dispari: 10 → 255 → 10
- **Differenza**: Parte con i LED pari al massimo
- **Uso ideale**: Variante dell'onda, alterna con l'onda normale

### 🎨 **6. Onda Graduale**
- **Effetto**: Onda con colori che cambiano fluidamente
- **Caratteristica speciale**: Il colore cambia ad ogni frame (non a scatti)
- **Risultato**: Transizione molto fluida e ipnotica
- **Velocità colore**: 1 step per frame attraverso tutto lo spettro
- **Uso ideale**: Meditazione, rilassamento, atmosfera zen

### 💨 **7. Onda Velocità Variabile**
- **Effetto**: Onda con velocità che varia dinamicamente
- **Comportamento**:
  - Inizia lenta (velocità 1)
  - Accelera gradualmente (fino a velocità 6)
  - Rallenta di nuovo (torna a velocità 1)
  - Ciclo continuo
- **Effetto "respiro"**: Sembra che la lampada respiri
- **Cambio velocità**: Ogni 30 frame
- **Uso ideale**: Effetto molto dinamico e coinvolgente

### 🎯 **8. Pattern 3 LED**
- **Effetto**: I 12 LED sono divisi in 4 gruppi da 3
- **Comportamento**:
  - Gruppo 1 (LED 0,1,2): si accende
  - Gruppo 2 (LED 3,4,5): si accende
  - Gruppo 3 (LED 6,7,8): si accende
  - Gruppo 4 (LED 9,10,11): si accende
  - Ripete il ciclo
- **Animazione gruppo**: Ogni gruppo pulsa da luminosità 10 a 255
- **Colori**: Cambiano ad ogni giro completo dei 4 gruppi
- **Uso ideale**: Effetto segmentato, stile "Knight Rider"

---

## ⚙️ Parametri Configurabili

### Velocità Animazioni

Ogni animazione ha le sue costanti modificabili:

```cpp
// Onda normale
#define WAVE_BRIGHTNESS_STEP 3    // Velocità (1-10)
#define WAVE_UPDATE_DELAY 15      // Millisecondi per frame

// Onda graduale
#define WAVE_GRAD_COLOR_STEP 1    // Velocità cambio colore

// Onda variabile
#define WAVE_VAR_MIN_SPEED 1      // Velocità minima
#define WAVE_VAR_MAX_SPEED 6      // Velocità massima

// Pattern 3 LED
#define PATTERN3_BRIGHTNESS_STEP 5  // Velocità pulsazione
```

### Colore Bianco (nel file principale)

```cpp
#define WHITE_R 150   // Componente rossa (0-255)
#define WHITE_G 230   // Componente verde (0-255)
#define WHITE_B 255   // Componente blu (0-255)

// Presets:
// Bianco freddo:      150, 230, 255 (default)
// Bianco neutro:      200, 255, 255
// Bianco caldo:       220, 255, 240
```

---

## 🎮 Come Funzionano i Controlli

### Pulsante TB1 (Accensione/Spegnimento)
1. **Premi**: Lampada si accende/spegne
2. **All'accensione**: 
   - Carica l'ultima animazione usata
   - Ripristina l'ultima luminosità
3. **Allo spegnimento**:
   - Salva la luminosità corrente
   - LED si spengono ma ESP32 resta alimentato

### Pulsante TB2 (Animazioni e Luminosità)

**Pressione BREVE** (< 500ms):
- Cambia animazione in sequenza
- Salva immediatamente la scelta

**Pressione LUNGA** (≥ 500ms):
- Varia la luminosità continuamente
- Direzione automatica:
  - Se > 50% → diminuisce
  - Se ≤ 50% → aumenta
- Ad ogni nuova pressione lunga → inverte direzione

---

## 🔧 Installazione

### Passo 1: Preparazione
1. Scarica il file `Lampada_Modulare.zip`
2. Estrai la cartella `Lampada_Modulare`
3. Verifica che contenga tutti e 9 i file

### Passo 2: Arduino IDE
1. Apri Arduino IDE
2. Installa la libreria **Adafruit_NeoPixel**:
   - Menu → Sketch → Include Library → Manage Libraries
   - Cerca "Adafruit NeoPixel"
   - Installa l'ultima versione

3. Seleziona la scheda ESP32:
   - Tools → Board → ESP32 Arduino → ESP32 Dev Module

### Passo 3: Caricamento
1. Apri `Lampada_Modulare.ino`
2. Collega l'ESP32 al computer
3. Seleziona la porta COM corretta
4. Carica lo sketch

### Passo 4: Verifica
1. Apri Serial Monitor (115200 baud)
2. Dovresti vedere:
   ```
   Lampada_Modulare
   Preferences inizializzate
   >>> Animazione caricata: 0
   >>> Luminosità caricata: 255
   === SISTEMA AVVIATO ===
   ```

---

## 🛠️ Hardware Necessario

### Componenti
- **ESP32** (qualsiasi variante con supporto touch)
- **Anello NeoPixel** 12 LED (WS2812B o compatibili)
- **2x Sensori Touch** capacitivi (TTP223 o simili)
- **Alimentazione** 5V 2A minimo

### Collegamenti

```
ESP32          →  NeoPixel Ring
GPIO 5 (DATA)  →  DIN (Data In)
5V             →  5V
GND            →  GND

ESP32          →  Touch Buttons
D3             →  TB1 Signal
D4             →  TB2 Signal
3.3V           →  VCC (entrambi)
GND            →  GND (entrambi)
```

---

## 💾 Gestione Memoria

### Cosa Viene Salvato
- **Animazione corrente** (0-7)
- **Luminosità corrente** (10-255)

### Quando Viene Salvato
- Animazione: subito quando cambi con TB2
- Luminosità: quando spegni la lampada con TB1

### Tecnologia Usata
- **Preferences** (NVS Flash)
- Namespace: "lampada"
- Chiavi: "anim", "bright"
- Vantaggi: ottimizzazione automatica, wear leveling

---

## 🎨 Aggiungere Nuove Animazioni

### Template File Animazione

```cpp
#ifndef ANIMATION_NOME_H
#define ANIMATION_NOME_H

// Variabili di stato (sempre static!)
static int miaVariabile = 0;
static byte mioColore = 0;

// Costanti configurabili
#define MIA_VELOCITA 15
#define MIO_STEP 3

// Funzione di inizializzazione
void initNomeAnimation() {
  miaVariabile = 0;
  mioColore = 0;
}

// Funzione dell'animazione
void nomeAnimation() {
  if (millis() - lastAnimationUpdate >= MIA_VELOCITA) {
    
    // Il tuo codice qui
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, ...);
    }
    
    pixels.show();
    
    // Aggiorna variabili
    miaVariabile++;
    
    lastAnimationUpdate = millis();
  }
}

#endif
```

### Modifiche al File Principale

1. **Aggiungi include**:
```cpp
#include "animation_nome.h"
```

2. **Incrementa numero**:
```cpp
#define NUM_ANIMATIONS 9  // Era 8, ora 9
```

3. **Aggiungi nei case**:
```cpp
// In handleTB1():
case 8:
  Serial.println("Nome Animazione");
  initNomeAnimation();
  break;

// In handleTB2():
case 8: Serial.println("Nome Animazione"); break;

// In runCurrentAnimation():
case 8:
  nomeAnimation();
  break;
```

---

## 🐛 Risoluzione Problemi

### LED non si accendono
- Verifica collegamento DATA_PIN (GPIO 5)
- Controlla alimentazione 5V sufficiente
- Verifica tipo LED corretto nel codice

### Sensori touch non rispondono
- Controlla che siano collegati a D3 e D4
- Verifica alimentazione 3.3V
- Testa con Serial Monitor per vedere lo stato

### Animazioni vanno troppo veloci/lente
- Modifica le costanti `_UPDATE_DELAY` e `_STEP`
- Valori più alti = più lento
- Valori più bassi = più veloce

### Impostazioni non vengono salvate
- Verifica messaggio "Preferences inizializzate" nel Serial Monitor
- Controlla che l'ESP32 abbia partizione NVS
- Prova a cancellare e ricaricare lo sketch

---

## 📊 Statistiche Progetto

- **Dimensione totale**: ~38 KB
- **File totali**: 10 (9 codice + 1 documentazione)
- **Animazioni**: 8 diverse
- **Memoria Flash usata**: < 1 MB
- **RAM usata**: < 10 KB
- **Compatibilità**: Tutti gli ESP32

---

## 🎓 Concetti Tecnici

### Modulare = Facile da Mantenere
- Ogni animazione è isolata
- Modifiche non impattano altre animazioni
- Facile aggiungere/rimuovere animazioni

### Variabili Static
- Mantengono lo stato tra chiamate
- Non condividono stato con altre animazioni
- Essenziali per animazioni fluide

### Timing Non-Bloccante
- Usa `millis()` invece di `delay()`
- Permette controlli reattivi
- Animazioni fluide senza freeze

### Preferences vs EEPROM
- Più sicuro e moderno
- Ottimizzazione automatica scritture
- Wear leveling integrato

---

## 🚀 Prossimi Passi Consigliati

1. **Prova tutte le animazioni** per capire quali preferisci
2. **Sperimenta con i parametri** per personalizzare velocità e colori
3. **Crea la tua animazione** usando il template fornito
4. **Combina effetti** creando animazioni ibride

---

## 📧 Note Finali

Questo progetto è progettato per essere:
- ✅ **Facile da usare**: Plug and play
- ✅ **Facile da personalizzare**: Parametri chiari e documentati
- ✅ **Facile da espandere**: Struttura modulare
- ✅ **Educativo**: Codice ben commentato

Buon divertimento con la tua lampada! 🎉
