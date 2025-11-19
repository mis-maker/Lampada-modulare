/*
  ============================================================================
  CONTROLLO ANELLO NEOPIXEL CON DUE SENSORI TOUCH - VERSIONE MODULARE
  ============================================================================
  
  Questo programma controlla un anello di 16 LED NeoPixel utilizzando due
  sensori touch (TB1 e TB2) collegati ad una scheda Arduino ESP32.
  
  STRUTTURA MODULARE:
  - Lampada_Modulare.ino (questo file principale)
  - config.h (configurazione centralizzata)
  - animation_white.h (animazione bianco fisso)
  - animation_random.h (animazione colori casuali)
  - animation_rainbow.h (animazione arcobaleno)
  - animation_rainbow_wave.h (animazione arcobaleno con onda)
  - animation_wave.h (animazione onda di luci)
  - animation_wave_inverse.h (animazione onda inversa CON ARCOBALENO)
  - animation_wave_gradient.h (animazione onda con colori graduali)
  - animation_wave_variable.h (animazione onda velocità variabile)
  - animation_pattern3.h (animazione pattern 3-4 LED CON ARCOBALENO)
  
  FUNZIONALITÀ:
  
  TB1 (Pin D3) - Pulsante navigazione indietro e accensione/spegnimento:
    - CLICK BREVE (< 700ms): Va all'animazione PRECEDENTE (solo se accesa)
    - PRESSIONE LUNGA (≥ 700ms): Accende o spegne completamente l'anello
    - Alla prima accensione l'anello riprende l'ultima animazione utilizzata
  
  TB2 (Pin D4) - Controllo animazioni avanti e luminosità:
    - CLICK BREVE (< 700ms): Va all'animazione SUCCESSIVA
    - PRESSIONE LUNGA (≥ 700ms): Varia la luminosità in modo continuo
      Sequenza: Bianco fisso → Colori casuali → Rainbow → Rainbow Wave → 
                Onda → Onda inversa → Onda graduale → Onda variabile → 
                Pattern 3 LED → Bianco fisso
    
    - Pressione LUNGA (≥ 500ms): varia la luminosità in modo continuo
      * Se luminosità > 50%: diminuisce
      * Se luminosità ≤ 50%: aumenta
      * Ad ogni nuova pressione lunga: inverte la direzione
  
  MEMORIA PERSISTENTE (usando Preferences):
    - L'ultima animazione utilizzata viene salvata nella memoria flash
    - L'ultima luminosità utilizzata viene salvata nella memoria flash
    - Ad ogni accensione, la lampada riprende dall'ultima configurazione
  
  HARDWARE:
  - ESP32
  - Anello NeoPixel 12 LED collegato al pin GPIO 5
  - Sensore touch TB1 collegato al pin D3
  - Sensore touch TB2 collegato al pin D4
  
  LIBRERIE NECESSARIE:
  - Adafruit_NeoPixel (installabile dal Library Manager di Arduino IDE)
  - Preferences (inclusa nell'ambiente ESP32)
  
  ============================================================================
*/

#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

// Includi il file di configurazione condiviso
#include "config.h"

// Includi tutti i file delle animazioni
#include "animation_white.h"
#include "animation_random.h"
#include "animation_rainbow.h"
#include "animation_rainbow_wave.h"
#include "animation_wave.h"
#include "animation_wave_inverse.h"
#include "animation_wave_gradient.h"
#include "animation_wave_variable.h"
#include "animation_pattern3.h"

// ============================================================================
// OGGETTO PREFERENCES
// ============================================================================

Preferences preferences;

// ============================================================================
// INIZIALIZZAZIONE OGGETTO NEOPIXEL
// ============================================================================

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
// VARIABILI PER LA GESTIONE DEI PULSANTI
// ============================================================================

bool TB1_pressed = false;
bool TB1_aux = false;
unsigned long TB1_pressTime = 0;
bool lampOn = false;

bool TB2_pressed = false;
bool TB2_aux = false;
unsigned long TB2_pressTime = 0;
bool TB2_longPressActive = false;

// Variabili per debouncing TB2 (anti-rimbalzo)
unsigned long TB2_lastPressTime = 0;

// ============================================================================
// VARIABILI PER LA GESTIONE DELLE ANIMAZIONI
// ============================================================================

// Identificativo dell'animazione corrente
int currentAnimation = 0;

// ============================================================================
// VARIABILI PER LA GESTIONE DELLA LUMINOSITÀ
// ============================================================================

int brightness = 255;
bool brightnessIncreasing = false;

unsigned long lastBrightnessChange = 0;

// ============================================================================
// VARIABILI GLOBALI PER LE ANIMAZIONI
// ============================================================================

unsigned long lastAnimationUpdate = 0;

// ============================================================================
// DICHIARAZIONI FORWARD DELLE FUNZIONI
// ============================================================================

void handleTB1();
void handleTB2();
void runCurrentAnimation();

// ============================================================================
// FUNZIONI PER LA GESTIONE DELLA MEMORIA PREFERENCES
// ============================================================================

void saveAnimation() {
  preferences.putInt("anim", currentAnimation);
  Serial.print(">>> Animazione salvata: ");
  Serial.println(currentAnimation);
}

int loadAnimation() {
  int savedAnimation = preferences.getInt("anim", 0);
  
  if (savedAnimation >= 0 && savedAnimation < NUM_ANIMATIONS) {
    Serial.print(">>> Animazione caricata: ");
    Serial.println(savedAnimation);
    return savedAnimation;
  } else {
    Serial.println(">>> Valore non valido, uso animazione predefinita (0)");
    return 0;
  }
}

void saveBrightness() {
  preferences.putInt("bright", brightness);
  Serial.print(">>> Luminosità salvata: ");
  Serial.print(brightness);
  Serial.print(" (");
  Serial.print((brightness * 100) / 255);
  Serial.println("%)");
}

int loadBrightness() {
  int savedBrightness = preferences.getInt("bright", 255);
  
  if (savedBrightness >= 10 && savedBrightness <= 255) {
    Serial.print(">>> Luminosità caricata: ");
    Serial.print(savedBrightness);
    Serial.print(" (");
    Serial.print((savedBrightness * 100) / 255);
    Serial.println("%)");
    return savedBrightness;
  } else {
    Serial.println(">>> Valore non valido, uso luminosità predefinita (255)");
    return 255;
  }
}

// ============================================================================
// FUNZIONE SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n\nLampada_Modulare");
  
  preferences.begin("lampada", false);
  Serial.println("Preferences inizializzate");
  
  currentAnimation = loadAnimation();
  brightness = loadBrightness();
  
  Serial.print(">>> Stato iniziale: currentAnimation=");
  Serial.println(currentAnimation);
  
  pixels.begin();
  pixels.setBrightness(brightness);
  pixels.clear();
  pixels.show();
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TOUCH1_PIN, INPUT);
  pinMode(TOUCH2_PIN, INPUT);
  
  Serial.println("\n\n=== SISTEMA AVVIATO ===");
  Serial.println("Test sensori touch:");
  Serial.print("TB1 (D3): ");
  Serial.println(digitalRead(TOUCH1_PIN));
  Serial.print("TB2 (D4): ");
  Serial.println(digitalRead(TOUCH2_PIN));
  
  Serial.println("\n>>> PREMI TB1 PER ACCENDERE <<<");
  Serial.println("Monitoraggio stato ogni 500ms...\n");
}

// ============================================================================
// FUNZIONE LOOP
// ============================================================================

void loop() {
  handleTB1();
  
  if (lampOn) {
    handleTB2();
    runCurrentAnimation();
  } else {
    pixels.clear();
    pixels.show();
  }
  
  delay(10);
}

// ============================================================================
// GESTIONE PULSANTE TB1
// ============================================================================

// ============================================================================
// GESTIONE PULSANTE TB1
// ============================================================================

void handleTB1() {
  TB1_pressed = digitalRead(TOUCH1_PIN);
  
  // Rileva inizio pressione
  if (TB1_pressed && !TB1_aux) {
    TB1_pressTime = millis();
    Serial.println("TB1 premuto");
  }
  
  // Rileva rilascio pressione
  if (!TB1_pressed && TB1_aux) {
    unsigned long pressDuration = millis() - TB1_pressTime;
    
    if (pressDuration >= LONG_PRESS_TIME) {
      // -----------------------------------------------------------------------
      // PRESSIONE LUNGA: Accende/Spegne la lampada
      // -----------------------------------------------------------------------
      lampOn = !lampOn;
      
      if (lampOn) {
        Serial.print("DEBUG: Luminosità ripristinata: ");
        Serial.println(brightness);
        
        pixels.setBrightness(brightness);
        
        Serial.print(">>> Lampada ACCESA - Animazione: ");
        
        // Inizializza l'animazione corrente
        switch(currentAnimation) {
          case 0: 
            Serial.println("Bianco fisso");
            initWhiteAnimation();
            break;
          case 1: 
            Serial.println("Colori casuali");
            initRandomAnimation();
            break;
          case 2: 
            Serial.println("Rainbow");
            initRainbowAnimation();
            break;
          case 3:
            Serial.println("Rainbow Wave");
            initRainbowWaveAnimation();
            break;
          case 4:
            Serial.println("Onda di luci");
            initWaveAnimation();
            break;
          case 5:
            Serial.println("Onda inversa Rainbow");
            initWaveInverseAnimation();
            break;
          case 6:
            Serial.println("Onda graduale");
            initWaveGradientAnimation();
            break;
          case 7:
            Serial.println("Onda velocità variabile");
            initWaveVariableAnimation();
            break;
          case 8:
            Serial.println("Pattern Rainbow");
            initPattern3Animation();
            break;
        }
        
      } else {
        saveBrightness();
        Serial.println(">>> Lampada SPENTA");
      }
      
    } else if (lampOn) {
      // -----------------------------------------------------------------------
      // CLICK BREVE: Va all'animazione precedente (solo se lampada accesa)
      // -----------------------------------------------------------------------
      currentAnimation--;
      if (currentAnimation < 0) {
        currentAnimation = NUM_ANIMATIONS - 1;  // Wraparound: da 0 vai a 8
      }
      
      saveAnimation();
      
      // Reset di tutte le animazioni
      initWhiteAnimation();
      initRandomAnimation();
      initRainbowAnimation();
      initRainbowWaveAnimation();
      initWaveAnimation();
      initWaveInverseAnimation();
      initWaveGradientAnimation();
      initWaveVariableAnimation();
      initPattern3Animation();
      
      Serial.print(">>> Animazione PRECEDENTE: ");
      switch(currentAnimation) {
        case 0: Serial.println("Bianco fisso"); break;
        case 1: Serial.println("Colori casuali"); break;
        case 2: Serial.println("Rainbow"); break;
        case 3: Serial.println("Rainbow Wave"); break;
        case 4: Serial.println("Onda di luci"); break;
        case 5: Serial.println("Onda inversa Rainbow"); break;
        case 6: Serial.println("Onda graduale"); break;
        case 7: Serial.println("Onda velocità variabile"); break;
        case 8: Serial.println("Pattern Rainbow"); break;
      }
    }
  }
  
  TB1_aux = TB1_pressed;
}



// ============================================================================
// GESTIONE PULSANTE TB2
// ============================================================================

// ============================================================================
// GESTIONE PULSANTE TB2
// ============================================================================

void handleTB2() {
  TB2_pressed = digitalRead(TOUCH2_PIN);
  
  // ============================================================================
  // DEBOUNCING - Filtro anti-rimbalzo
  // ============================================================================
  // Ignora le letture troppo ravvicinate (entro DEBOUNCE_TIME ms)
  
  if (TB2_pressed && !TB2_aux) {
    // Rileva pressione del pulsante
    unsigned long currentTime = millis();
    
    // Controlla se è passato abbastanza tempo dall'ultima pressione valida
    if (currentTime - TB2_lastPressTime < DEBOUNCE_TIME) {
      // Troppo presto! È un rimbalzo, ignora questa pressione
      return;  // Esce dalla funzione senza fare nulla
    }
    
    // Pressione valida!
    TB2_lastPressTime = currentTime;
    TB2_pressTime = currentTime;
    TB2_longPressActive = false;
    Serial.println("TB2 premuto");
  }
  
  if (TB2_pressed && TB2_aux) {
    unsigned long pressDuration = millis() - TB2_pressTime;
    
    if (pressDuration >= LONG_PRESS_TIME && !TB2_longPressActive && lampOn) {
      TB2_longPressActive = true;
      
      if (brightness > 127) {
        brightnessIncreasing = false;
      } else {
        brightnessIncreasing = true;
      }
      
      Serial.print("Pressione lunga - Luminosità ");
      Serial.println(brightnessIncreasing ? "AUMENTA" : "DIMINUISCE");
    }
    
    if (TB2_longPressActive && lampOn) {
      if (millis() - lastBrightnessChange >= BRIGHTNESS_DELAY) {
        
        if (brightnessIncreasing) {
          brightness += BRIGHTNESS_STEP;
          if (brightness > 255) brightness = 255;
        } else {
          brightness -= BRIGHTNESS_STEP;
          if (brightness < 10) brightness = 10;
        }
        
        pixels.setBrightness(brightness);
        lastBrightnessChange = millis();
        
        Serial.print("Luminosità: ");
        Serial.print(brightness);
        Serial.print(" (");
        Serial.print((brightness * 100) / 255);
        Serial.println("%)");
      }
    }
  }
  
  if (!TB2_pressed && TB2_aux) {
    unsigned long pressDuration = millis() - TB2_pressTime;
    
    if (pressDuration < LONG_PRESS_TIME && lampOn) {
      // -----------------------------------------------------------------------
      // CLICK BREVE: Avanza all'animazione successiva
      // -----------------------------------------------------------------------
      currentAnimation = (currentAnimation + 1) % NUM_ANIMATIONS;
      
      saveAnimation();
      
      // Reset di tutte le animazioni
      initWhiteAnimation();
      initRandomAnimation();
      initRainbowAnimation();
      initRainbowWaveAnimation();
      initWaveAnimation();
      initWaveInverseAnimation();
      initWaveGradientAnimation();
      initWaveVariableAnimation();
      initPattern3Animation();
      
      Serial.print(">>> Animazione SUCCESSIVA: ");
      switch(currentAnimation) {
        case 0: Serial.println("Bianco fisso"); break;
        case 1: Serial.println("Colori casuali"); break;
        case 2: Serial.println("Rainbow"); break;
        case 3: Serial.println("Rainbow Wave"); break;
        case 4: Serial.println("Onda di luci"); break;
        case 5: Serial.println("Onda inversa Rainbow"); break;
        case 6: Serial.println("Onda graduale"); break;
        case 7: Serial.println("Onda velocità variabile"); break;
        case 8: Serial.println("Pattern Rainbow"); break;
      }
    }
    
    if (TB2_longPressActive) {
      brightnessIncreasing = !brightnessIncreasing;
      Serial.print("Rilascio - direzione cambiata: ");
      Serial.println(brightnessIncreasing ? "AUMENTA" : "DIMINUISCE");
      TB2_longPressActive = false;
    }
  }
  
  TB2_aux = TB2_pressed;
}



// ============================================================================
// ESECUZIONE DELL'ANIMAZIONE CORRENTE
// ============================================================================

void runCurrentAnimation() {
  switch (currentAnimation) {
    case 0:
      whiteAnimation();
      break;
    case 1:
      randomAnimation();
      break;
    case 2:
      rainbowAnimation();
      break;
    case 3:
      rainbowWaveAnimation();
      break;
    case 4:
      waveAnimation();
      break;
    case 5:
      waveInverseAnimation();
      break;
    case 6:
      waveGradientAnimation();
      break;
    case 7:
      waveVariableAnimation();
      break;
    case 8:
      pattern3Animation();
      break;
  }
}

// ============================================================================
// FUNZIONE AUSILIARIA: WHEEL
// ============================================================================

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// ============================================================================
// FUNZIONE AUSILIARIA: DIMCOLOR
// ============================================================================

uint32_t dimColor(uint32_t color, byte brightness) {
  byte r = (color >> 16) & 0xFF;
  byte g = (color >> 8) & 0xFF;
  byte b = color & 0xFF;
  
  r = (r * brightness) / 255;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;
  
  return pixels.Color(r, g, b);
}
