#include <Arduino.h>
#include <LiquidCrystal.h>

// Crie um objeto LCD e associe os pinos da interface LCD (reset, enable, d4, d5, d6, d7) com os pinos do arduino.
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);
// Esta variável irá indicar qual rede está operando. 0 para inversor e 1 para RGE.
int invRGE = 0;

// put function declarations here:
void tensaoBateria(uint8_t pino);
float leitura(uint8_t pino);
void alerta();

void setup() {
  // put your setup code here, to run once:
  // Inicialize um display de 16 colunas e 2 linhas.
  lcd.begin(16, 2);
  lcd.print("Monitorador de tensão");
  lcd.clear();
}

void loop() {
  // 0123456789ABCDEF
  // B1:XX.X  B2:XX.X
  // B1:XX.X REDE RGE
  //         INVERSOR

  lcd.setCursor(0, 0);
  lcd.print("B1:");
  lcd.setCursor(3, 0);
  tensaoBateria(A0);

  lcd.setCursor(9, 0);
  lcd.print("B2:");
  lcd.setCursor(12, 0);
  tensaoBateria(A1);

  lcd.setCursor(0, 1);
  lcd.print("B3:");
  lcd.setCursor(3, 1);
  tensaoBateria(A2);

  lcd.setCursor(8, 1);
  switch (invRGE) {
    case 0:
      lcd.print("INVERSOR");
      break;
    
    case 1:
      lcd.print("REDE RGE");
      break;
    }
}

// Definição de funções:
float leitura(uint8_t pino) {
  int valorSensor = analogRead(pino);
  float tensao = valorSensor * (5.0 / 1023.0);
  return tensao;
}

void tensaoBateria(uint8_t pino) {
  float tensao_1 = leitura(pino);
  delay(50);
  float tensao_2 = leitura(pino);
  delay(50);
  float tensao_3 = leitura(pino);

  while (true) {
    if (tensao_1 - tensao_2 >= 4.0 || tensao_1 - tensao_2 <= -4.0) {
      tensao_1 = leitura(pino);
      delay(50);
      tensao_2 = leitura(pino);
    } else if (tensao_2 - tensao_3 >= 4.0 || tensao_2 - tensao_3 <= -4.0) {
      tensao_2 = leitura(pino);
      delay(50);
      tensao_3 = leitura(pino);
    } else {
      break;
    }
  }

  float tensaoMed = (tensao_1 + tensao_2 + tensao_3) / 3;
  lcd.print(tensaoMed);
  if (tensaoMed <= 12.0) {
    invRGE = 1;
    alerta();
    digitalWrite(3, HIGH);
    digitalWrite(9, HIGH);
  } else {
    invRGE = 0;
    digitalWrite(3, LOW);
    digitalWrite(9, LOW);
  }
}

void alerta() {
  // TODO: Criar um beep alerta ao trocar para a RGE
}