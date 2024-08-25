#include "LiquidCrystal_I2C.h";

// --- Constantes ---
const float pi = 3.14159265;     //Número de pi
int period = 3000;               //Tempo de medida(miliseconds)
int radius = 147;                //Raio do anemometro(mm)

// --- Variáveis Globais ---
unsigned int Sample  = 0;        //Armazena o número de amostras
unsigned int counter = 0;        //Contador para o sensor
unsigned int RPM = 0;            //Rotações por minuto
float speedwind = 0;             //Velocidade do vento (m/s)
float windspeed = 0;             //Velocidade do vento (km/h)

// --- pinos --- 
int     pin       = A0;  // Declara o pino analógico 0 como "pin"
int     Winddir   = 0;   // Declara o valor inicial em 0
float   valor     = 0;   // declara a variável inicial em 0
#define pin_lm35 A2 //Pino o qual o sensor está conectado

//define os pinos usados para o sensor de luz
#define LUZ A1

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Declara o endereço do Display I2C
 
 
//cria o caractere "°", simbolo de grau celsius
byte grau[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

//cria o caractere "☼", simbolo do sol
byte sol[] = {
  B10101,
  B01110,
  B11011,
  B01110,
  B10101,
  B00000,
  B00000,
  B00000
};

//cria o caractere "☾", simbolo da lua

byte lua[] = {
  B00110,
  B01100,
  B01100,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};


// --- Configurações Inicias ---
void setup() {
  //liga display I2C
  lcd.init();
  lcd.backlight();
  
  pinMode(2, INPUT);        //configura o digital 2 como entrada
  digitalWrite(2, HIGH);    //internall pull-up active
  
  pinMode(pin_lm35, INPUT); // Define o sensor como uma entrada de sinal
  
  // Cria o caractere personalizado no local de memória 0
  lcd.createChar(0, grau);
  // Cria o caractere personalizado no local de memória 1
  lcd.createChar(1, sol);
  // Cria o caractere personalizado no local de memória 2
  lcd.createChar(2, lua);


  Serial.begin(9600);       //inicia serial em 9600 baud rate
  analogReference(DEFAULT);
} //end setup

// --- Loop Infinito ---
void loop() {
calculaVelocidade();
calculaPosicao();
temperatura();
sensorLuz();
//lcd.clear();
}

//função para calcular a velocidade do vento
void calculaVelocidade(){
  Sample++;
  Serial.print(Sample);
  Serial.print(": Inicia Leitura...");
  windvelocity();
  Serial.println("   Finalizado.");
  Serial.print("Contador: ");
  Serial.print(counter);
  Serial.print(";  RPM: ");
  RPMcalc();
  Serial.print(RPM); //exibe o número de rotações por minuto
  //lcd.print(RPM);
  Serial.print(";  Vel. Vento: ");
  

  //*****************************************************************
  //print m/s
  WindSpeed();
  Serial.print(windspeed); //exibe velocidade em m/s
  Serial.print(" [m/s] ");
 //lcd.setCursor(6,1);
 //lcd.print("|V:");
 //lcd.print(windspeed);
 //lcd.setCursor(13, 1);
 //lcd.print("m/s");
  
  //*****************************************************************
  //print km/h
  SpeedWind();
  Serial.print(speedwind); //exibe velocidade em km/h
  Serial.print(" [km/h] ");
  lcd.setCursor(6,1);
  lcd.print("|V:");
  lcd.print(speedwind);
  lcd.setCursor(13, 1);
  lcd.print("k/h");
  Serial.println();
} //end function



//Função para medir velocidade do vento
void windvelocity() {
  speedwind = 0;
  windspeed = 0;

  counter = 0;
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();
  long startTime = millis();
  while (millis() < startTime + period) {}
}

//Função para calcular o RPM
void RPMcalc() {
  RPM = ((counter) * 60) / (period / 1000); // Calculate revolutions per minute (RPM)
}

//Velocidade do vento em m/s
void WindSpeed() {
  windspeed = ((4 * pi * radius * RPM) / 60) / 1000; //Calcula a velocidade do vento em m/s
} //end WindSpeed

//Velocidade do vento em km/h
void SpeedWind() {
  speedwind = (((4 * pi * radius * RPM) / 60) / 1000) * 3.6; //Calcula velocidade do vento em km/h
} //end SpeedWind

//Incrementa contador
void addcount() {
  counter++;
}

//função para calcular posição

void calculaPosicao(){
 valor = analogRead(pin) * (5.0 / 1023.0); // Calcula a tensão
 
 
  // Apresenta os valores da tensão de saída no Monitor Serial
  Serial.print("Leitura do sensor: ");
  Serial.print(valor);
  Serial.println(" volt");


  if (valor <= 1.1) {
    Winddir = 45;
    lcd.setCursor(4, 1);
    lcd.print("NO");
  }

   // Indica a posição oeste
  else if (valor <= 1.23) {
    Winddir = 45;
    lcd.setCursor(4, 1);
    lcd.print(" O");
  }

  // Indica a posição sudoeste
  else if (valor <= 1.35) {
    Winddir = 180;
    lcd.setCursor(4, 1);
    lcd.print("SO");
  }
  
   // Indica a posição sul
  else if (valor <= 1.55) {
    Winddir = 225;
    lcd.setCursor(4, 1);
    lcd.print(" S");
  }
  // Indica a posição sudeste
  else if (valor <= 1.8) {
    Winddir = 270;
    lcd.setCursor(4, 1);
    lcd.print("SE");
  }
  
 // Indica a posição leste
  else if (valor <= 2.2) {
    Winddir = 315;
    lcd.setCursor(4, 1);
    lcd.print(" L");
  }
  
 // Indica a posição nordeste
   else if (valor <= 2.7) {
    Winddir = 0;
    lcd.setCursor(4, 1);
    lcd.print("NE");
  }
  else {
    lcd.setCursor(4, 1);
    lcd.print(" N");
  }
 
 // Imprime a posição do vento na linha 2
  lcd.setCursor(0, 1);
  lcd.print("Dir:");
  //delay(5000);
}

 //função para incorporar sensor de luz

void sensorLuz(){

//lê o sensor de luz
int valorLDR = map(analogRead(LUZ), 0, 1023, 0, 11);
//printa os valores do sensor
Serial.println(valorLDR);
delay(500);

if(valorLDR >= 2){
  Serial.println(valorLDR);
  lcd.setCursor(11,0);
  lcd.print(" | D");
  lcd.write(byte(1)); //Exibe o caractere de sol
  lcd.print(" ");
}
else{
  Serial.println(valorLDR);
  lcd.setCursor(11,0);
  lcd.print(" | N");
  lcd.write(byte(2)); //Exibe o caractere de lua
  lcd.print(" ");
}
  

  
}

void temperatura(){
float valor_analog_lm35 = float(analogRead(pin_lm35)); // Obtém o valor analógico que varia de 0 a 1023
float tensao = (valor_analog_lm35 * 5) / 1023; // Vamos converter esse valor para tensão elétrica
float temperatura = tensao / 0.010; // dividimos a tensão por 0.010 que representa os 10 mV
 
Serial.print("Temperatura: ");
lcd.setCursor(0,0);
lcd.print("tmp:");
Serial.println(temperatura); // Mostra na serial a temperatura do sensor
lcd.print(temperatura-1);
lcd.write(byte(0)); // Exibe o caractere de grau
lcd.print("C");
}
