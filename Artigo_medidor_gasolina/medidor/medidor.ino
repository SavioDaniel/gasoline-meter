
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int numLeds = 9;

const int VERMELHO_INICIO = 0;  
const int VERMELHO_FIM = 2;    
const int AMARELO_INICIO = 3;   
const int AMARELO_FIM = 5;      
const int VERDE_INICIO = 6;     
const int VERDE_FIM = 8;        

const int potPin = A1;  

unsigned long previousMillis = 0;
int blinkState = HIGH;
int blinkSpeed = 0; 

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);
  
  int ledsToLight = map(potValue, 0, 1023, 0, numLeds);
  
  bool amareloAtivo = (ledsToLight > AMARELO_INICIO);
  
  int vermelhosAtivos = constrain(ledsToLight - VERMELHO_INICIO, 0, 3);
  
  if (amareloAtivo) {
    blinkSpeed = 0;  
  } else {
    if (vermelhosAtivos == 1) {
      blinkSpeed = 1; 
    } else if (vermelhosAtivos == 2) {
      blinkSpeed = 2;  
    } else if (vermelhosAtivos >= 3) {
      blinkSpeed = 3; 
    } else {
      blinkSpeed = 0;  
    }
  }

  if (!amareloAtivo) {
    controlarPiscar();
  } else {
    blinkState = HIGH;  
  }
  
  for (int i = 0; i < numLeds; i++) {
    if (i < ledsToLight) {
      
      if (i <= VERMELHO_FIM && !amareloAtivo && blinkSpeed > 0) {
        digitalWrite(ledPins[i], blinkState);
      } else {
        digitalWrite(ledPins[i], HIGH);
      }
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
  
  Serial.print("Potenciômetro: ");
  Serial.print(potValue);
  Serial.print(" | LEDs acesos: ");
  Serial.print(ledsToLight);
  Serial.print(" | Amarelo ativo: ");
  Serial.print(amareloAtivo ? "SIM" : "NÃO");
  Serial.print(" | Velocidade piscar: ");
  Serial.println(blinkSpeed);
  
  delay(50);
}

void controlarPiscar() {
  unsigned long currentMillis = millis();
  unsigned long interval = 0;
  
  switch (blinkSpeed) {
    case 1:
      interval = 200;
      break;
    case 2: 
      interval = 500;
      break;
    case 3:
      interval = 1000;
      break;
    default: 
      blinkState = HIGH;
      return;
  }
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    blinkState = !blinkState;
  }
}

