# 🎯 Sistema di Configurazione Centralizzata

## ✨ Cosa è Cambiato

Ho creato un file **config.h** che contiene TUTTE le impostazioni configurabili in un unico posto!

### 📦 Nuovo File Aggiunto

**config.h** - File di configurazione centralizzato che contiene:
- ✅ Numero di LED (NUM_LEDS)
- ✅ Pin di collegamento (TOUCH1_PIN, TOUCH2_PIN, DATA_PIN)
- ✅ Colore bianco (WHITE_R, WHITE_G, WHITE_B)
- ✅ Numero totale animazioni (NUM_ANIMATIONS)
- ✅ Tempo pressione lunga (LONG_PRESS_TIME)
- ✅ Parametri luminosità (BRIGHTNESS_STEP, BRIGHTNESS_DELAY)
- ✅ Velocità di TUTTE le animazioni

---

## 🎯 Vantaggi

### Prima (❌ Complicato):
Per cambiare il numero di LED dovevi modificare:
1. File principale: `#define NUM_LEDS 12`
2. animation_white.h: sostituire `12` con nuovo numero
3. animation_random.h: sostituire `12` con nuovo numero
4. animation_rainbow.h: sostituire `12` con nuovo numero
5. animation_wave.h: sostituire `12` con nuovo numero
6. animation_wave_inverse.h: sostituire `12` con nuovo numero
7. animation_wave_gradient.h: sostituire `12` con nuovo numero
8. animation_wave_variable.h: sostituire `12` con nuovo numero
9. animation_pattern3.h: sostituire `12` con nuovo numero

**= 9 file da modificare! 😰**

### Ora (✅ Semplice):
Per cambiare il numero di LED modifica SOLO:
1. config.h: `#define NUM_LEDS 16`

**= 1 file da modificare! 🎉**

---

## 🔧 Come Personalizzare la Tua Lampada

Apri **config.h** e modifica i valori che vuoi:

### 🔢 Cambiare Numero LED

```cpp
// Se hai un anello da 16 LED:
#define NUM_LEDS 16

// Se hai un anello da 24 LED:
#define NUM_LEDS 24

// Se hai una striscia da 60 LED:
#define NUM_LEDS 60
```

### 🎨 Cambiare Colore Bianco

```cpp
// Bianco freddo (default):
#define WHITE_R 150
#define WHITE_G 230
#define WHITE_B 255

// Bianco caldo:
#define WHITE_R 220
#define WHITE_G 255
#define WHITE_B 240

// Bianco neutro:
#define WHITE_R 200
#define WHITE_G 255
#define WHITE_B 255
```

### ⚡ Cambiare Velocità Animazioni

```cpp
// Rallentare tutto:
#define RANDOM_DELAY 200        // Era 100
#define RAINBOW_DELAY 40        // Era 20
#define WAVE_UPDATE_DELAY 30    // Era 15

// Velocizzare tutto:
#define RANDOM_DELAY 50         // Era 100
#define RAINBOW_DELAY 10        // Era 20
#define WAVE_UPDATE_DELAY 8     // Era 15
```

### 🎮 Cambiare Controlli

```cpp
// Rendere più difficile la pressione lunga:
#define LONG_PRESS_TIME 1000    // 1 secondo invece di 0.5

// Cambiare velocità variazione luminosità:
#define BRIGHTNESS_STEP 10      // Più veloce (era 5)
#define BRIGHTNESS_DELAY 100    // Più lento (era 50)
```

---

## 📋 Struttura File Aggiornata

```
Lampada_Modulare/
├── config.h                      ⭐ NUOVO! Configura qui
├── Lampada_Modulare.ino         (file principale)
├── animation_white.h            (include config.h)
├── animation_random.h           (include config.h)
├── animation_rainbow.h          (include config.h)
├── animation_wave.h             (include config.h)
├── animation_wave_inverse.h     (include config.h)
├── animation_wave_gradient.h    (include config.h)
├── animation_wave_variable.h    (include config.h)
├── animation_pattern3.h         (include config.h)
├── README.md
└── GUIDA_COMPLETA.md
```

Ogni file `.h` ora include `config.h` e usa le costanti definite lì!

---

## 🎓 Come Funziona Tecnicamente

### Prima:
Ogni file aveva le sue costanti locali:
```cpp
// In animation_wave.h
#define WAVE_UPDATE_DELAY 15  // Duplicato!
for (int i = 0; i < 12; i++)  // Hardcoded!
```

### Ora:
Tutti i file includono config.h:
```cpp
// In animation_wave.h
#include "config.h"

for (int i = 0; i < NUM_LEDS; i++)  // Usa la costante condivisa!
// WAVE_UPDATE_DELAY viene da config.h
```

---

## ✅ Test Consigliati

Dopo aver modificato config.h:

1. **Compila** lo sketch per verificare che non ci siano errori
2. **Carica** sull'ESP32
3. **Testa** ogni animazione per verificare che funzioni correttamente
4. **Regola** i parametri fino a trovare l'effetto desiderato

---

## 🆘 Risoluzione Problemi

### Errore: "NUM_LEDS was not declared"
- Assicurati che **config.h** sia nella stessa cartella degli altri file
- Verifica che ogni file `.h` abbia la riga `#include "config.h"`

### LED non si accendono tutti
- Controlla che `NUM_LEDS` in config.h corrisponda al numero reale di LED
- Verifica l'alimentazione (LED numerosi necessitano più corrente)

### Animazioni troppo veloci/lente
- Modifica i valori `_UPDATE_DELAY` in config.h
- Valori più alti = più lento
- Valori più bassi = più veloce

---

## 🎉 Conclusione

Con questo nuovo sistema:
- ✅ Configurazione centralizzata
- ✅ Modifiche in un solo posto
- ✅ Meno possibilità di errori
- ✅ Più facile da mantenere
- ✅ Più professionale

Goditi la tua lampada personalizzata! 🌈💡
