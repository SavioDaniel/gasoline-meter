// Define os pinos dos LEDs
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int numLeds = 9;

// Definição das cores
const int VERMELHO_INICIO = 0;  // Índice do primeiro vermelho (pino 2)
const int VERMELHO_FIM = 2;     // Índice do último vermelho (pino 4)
const int AMARELO_INICIO = 3;   // Índice do primeiro amarelo (pino 5)
const int AMARELO_FIM = 5;      // Índice do último amarelo (pino 7)
const int VERDE_INICIO = 6;     // Índice do primeiro verde (pino 8)
const int VERDE_FIM = 8;        // Índice do último verde (pino 10)

const int potPin = A1;  // Pino do potenciômetro

// Variáveis para controle do piscar
unsigned long previousMillis = 0;
int blinkState = HIGH;
int blinkSpeed = 0;  // 0 = não pisca, 1 = rápido, 2 = médio, 3 = lento

void setup() {
  // Inicializa os pinos dos LEDs como saída
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  // Inicializa a comunicação serial
  Serial.begin(9600);
}

void loop() {
  // Lê o valor do potenciômetro (0-1023)
  int potValue = analogRead(potPin);
  
  // Mapeia o valor do potenciômetro para o número de LEDs (0-9)
  int ledsToLight = map(potValue, 0, 1023, 0, numLeds);
  
  // Verifica se algum LED amarelo está aceso
  bool amareloAtivo = (ledsToLight > AMARELO_INICIO);
  
  // Define a velocidade do piscar baseado em quantos LEDs vermelhos estão ativos
  // MAS só pisca se NENHUM amarelo estiver aceso
  int vermelhosAtivos = constrain(ledsToLight - VERMELHO_INICIO, 0, 3);
  
  if (amareloAtivo) {
    blinkSpeed = 0;  // Para de piscar quando amarelo acender
  } else {
    if (vermelhosAtivos == 1) {
      blinkSpeed = 1;  // Rápido
    } else if (vermelhosAtivos == 2) {
      blinkSpeed = 2;  // Médio
    } else if (vermelhosAtivos >= 3) {
      blinkSpeed = 3;  // Lento
    } else {
      blinkSpeed = 0;  // Não pisca
    }
  }
  
  // Controla o piscar dos LEDs vermelhos (só se não tiver amarelo aceso)
  if (!amareloAtivo) {
    controlarPiscar();
  } else {
    blinkState = HIGH;  // Mantém aceso quando amarelo estiver ativo
  }
  
  // Atualiza todos os LEDs
  for (int i = 0; i < numLeds; i++) {
    if (i < ledsToLight) {
      // Para LEDs vermelhos, usa blinkState se estiver piscando, ou HIGH se não
      if (i <= VERMELHO_FIM && !amareloAtivo && blinkSpeed > 0) {
        digitalWrite(ledPins[i], blinkState);
      } else {
        digitalWrite(ledPins[i], HIGH);
      }
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
  
  // Debug no monitor serial
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
  
  // Define o intervalo baseado na velocidade
  switch (blinkSpeed) {
    case 1: // Rápido
      interval = 200;
      break;
    case 2: // Médio
      interval = 500;
      break;
    case 3: // Lento
      interval = 1000;
      break;
    default: // Não pisca
      blinkState = HIGH;
      return;
  }
  
  // Atualiza o estado do piscar
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    blinkState = !blinkState;
  }
}
