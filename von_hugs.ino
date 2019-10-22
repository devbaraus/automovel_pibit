#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include "DHT.h"
#include <Ultrasonic.h>

// BLUETOOTH
// Bluetooth TX -> Arduino D8
#define BLUETOOTH_TX 8
// Bluetooth RX -> Arduino D7
#define BLUETOOTH_RX 7

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth);

//MOTORES
#define pef 10 // motor esquerdo para frente
#define peb 11 // motor esquerdo pra tras
#define pdf 12 // motor direito para frente
#define pdb 13 // motor direito para tras


//BUZZER
#define buzzer 9
int freqBuzzer = 0;


//DHT - TEMPERATURA E HUMIDADE
#define DHTPIN 3    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

// ULTRASOM
//Define os pinos para o trigger e echo
#define pino_trigger 5
#define pino_echo 6

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);


float AC = 0;
bool E = false;
bool L = false;
bool G = false;
int PC = 0;

void move_frente(int tempo) {
  //MF9900
  // tempo em segundos
  Serial.print("Indo pra frente por ");
  Serial.print(tempo);
  Serial.println(" segundos");
  digitalWrite(pdb, LOW);
  digitalWrite(pdf, HIGH);
  digitalWrite(pef, HIGH);
  digitalWrite(peb, LOW);
  delay(tempo * 1000);
  digitalWrite(pdb, LOW);
  digitalWrite(pdf, LOW);
  digitalWrite(pef, LOW);
  digitalWrite(peb, LOW);
}

void move_tras(int tempo) {
  //MT9900
  Serial.print("Indo pra tras por ");
  Serial.print(tempo);
  Serial.println(" segundos");
  digitalWrite(pdb, HIGH);
  digitalWrite(pdf, LOW);
  digitalWrite(pef, LOW);
  digitalWrite(peb, HIGH);
  delay(tempo * 1000);
  digitalWrite(pdb, LOW);
  digitalWrite(pdf, LOW);
  digitalWrite(pef, LOW);
  digitalWrite(peb, LOW);
  Serial.println("Carro parado!");
}

void vira_esquerda(int tempo) {
  //VE9900
  //  Serial.println("Girando pra esquerda por" , tempo,  " segundos" );
  digitalWrite(pdb, HIGH);
  digitalWrite(pdf, LOW);
  digitalWrite(pef, HIGH);
  digitalWrite(peb, LOW);
  delay(tempo * 1000);
  digitalWrite(pdb, LOW);
  digitalWrite(pdf, LOW);
  digitalWrite(pef, LOW);
  digitalWrite(peb, LOW);
  Serial.println("Carro parado!");
}

void vira_direita(int tempo) {
  //VD9900
  //   Serial.println("Girando pra direita" , tempo,  " segundos" );
  digitalWrite(pdb, LOW);
  digitalWrite(pdf, HIGH);
  digitalWrite(pef, LOW);
  digitalWrite(peb, HIGH);
  delay(tempo * 1000);
  digitalWrite(pdb, LOW);
  digitalWrite(pdf, LOW);
  digitalWrite(pef, LOW);
  digitalWrite(peb, LOW);
  Serial.println("Carro parado!");
}

void faz_nada(int tempo) {
  //FN9900
  // Serial.println("Fazendo nada por " , tempo,  " segundos" );
  delay(tempo * 1000);
}

void buzzer_freq(int freq) {
  //BF1024
  //  Serial.println("Configurando frequencia do buzzer para frequencia de ", freq);
  freqBuzzer = freq;
}

void buzzer_toca(int tempo) {
  //TB9900
  //  Serial.println("Tocando buzzer por" , tempo,  " segundos" );
  tone(buzzer, freqBuzzer);
  delay(tempo*1000);
  noTone(buzzer);
}

void get_luminosidade() {
  //GL0000
  //  Serial.println("Pegando luminosidade" );
  return 2.5f;
}

void get_umidade() {
  //GU0000
  float hum = dht.readHumidity();
  Serial.print("Umidade: ");
  Serial.println(hum);
  AC = hum;
}

void get_temperatura() {
  //GT0000
  float temp = dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.println(temp);
  AC = temp;
}

void get_distancia() {
  // GD0000
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);

  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.println(cmMsec);
  AC = cmMsec;
}

void led_liga(int pos) {
  //LL9900
  //  Serial.println("Ligando LED na posição ", pos );
}

void led_desliga(int pos) {
  //LD9900
  //  Serial.println("Desligando LED na posição " , pos );
}

void cmp(int val) {
  //CM9999
  //  Serial.println("Comparando AC com o valor " , val );
  E = AC == val;
  L = AC < val;
  G = AC > val; 
}

void jmp(int end) {
  //JM9999
  //  Serial.println("Pulando para o endereço " , end );
  PC = end;
}

void jne(int end) {
  //NE9999
  // Serial.println("Pulando caso seja diferente para o endereço " , end );
  if(!E){
    PC = end;
  }
}

void je(int end) {
  //JE9999
  // Serial.println("Pulando caso seja igual para o endereço " , end );
  if(E){
    PC = end;
  }
}

void jl(int end) {
  //JL9999
  // Serial.println("Pulando caso seja menor para o endereço " , end );
  if(L){
    PC = end;
  }
}

void jg(int end) {
  //JG9999
  //   Serial.println("Pulando caso seja maior para o endereço " , end );
  if(G){
    PC = end;
  }
}

void jge(int end) {
  //GE9999
  // Serial.println("Pulando caso seja maior ou igual para o endereço " , end );
  if(G && E){
    PC = end;
  }
}

void jle(int end) {
  //LE9999
  // Serial.println("Pulando caso seja menor ou igual para o endereço " , end );
  if(L && E){
    PC = end;
  }
}

void load(int end) {
  //LO9999
  // Serial.println("Carregando dado do endereço " , end );
  AC = end;
}

void store(int end) {
  //ST9999
  // Serial.println("Guardando dado no endereço " , end );
  end = AC;
}

void add(int val) {
  //AD9999
  //  Serial.println("Adicionando ao AC o valor " , val );
  AC = AC + val;
}

void sub(int val) {
  //SU9999
  // Serial.println("Subtraindo do AC o valor " , val );
  AC = AC - val;
}

void divi(int val) {
  //DI9999
  //  Serial.println("Dividindo o AC pelo valor " , val );
  AC = AC / val;
}

void mul(int val) {
  // MU9999
  // Serial.println("Multiplicando o AC pelo valor " , val );
  AC = AC * val;
}

void execcommands(String op, int operando) {
  if (op == "MF") {
    move_frente(operando);
  } else if (op == "MT") {
    move_tras(operando);
  } else if (op == "VE") {
    vira_esquerda(operando);
  } else if (op == "VD") {
    vira_direita(operando);
  } else if (op == "FN") {
    faz_nada(operando);
  } else if (op == "BF") {
    buzzer_freq(operando);
  } else if (op == "BT") {
    buzzer_toca(operando);
  } else if (op == "GL") {
    get_luminosidade();
  } else if (op == "GU") {
    get_umidade();
  } else if (op == "GT") {
    get_temperatura();
  } else if (op == "GD") {
    get_distancia();
  } else if (op == "LL") {
    led_liga(operando);
  } else if (op == "LD") {
    led_desliga(operando);
  } else if (op == "CM") {
    cmp(operando);
  } else if (op == "JM") {
    jmp(operando);
  } else if (op == "NE") {
    jne(operando);
  } else if (op == "JE") {
    je(operando);
  } else if (op == "JL") {
    jl(operando);
  } else if (op == "JG") {
    jg(operando);
  } else if (op == "GE") {
    jge(operando);
  } else if (op == "LE") {
    jle(operando);
  } else if (op == "LO") {
    load(operando);
  } else if (op == "ST") {
    store(operando);
  } else if (op == "AD") {
    add(operando);
  } else if (op == "SU") {
    sub(operando);
  } else if (op == "DI") {
    divi(operando);
  } else if (op == "MU") {
    mul(operando);
  } else {
    return;
  }
}


//char buffercomandos[] = {"MF0045", "FN9900", "GT0000", "KM0000"};

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  bluetooth.begin(9600);
  dht.begin();
  
  delay(100);

  pinMode(pef, OUTPUT);
  pinMode(peb, OUTPUT);
  pinMode(pdf, OUTPUT);
  pinMode(pdb, OUTPUT);

  pinMode(buzzer, OUTPUT);

  Serial.println("OK!");
  return 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  String str = phone.getText();
  //  String str = "MF0045";
  String opcode = str.substring(0, 2);
  int operando = str.substring(2).toInt();
  execcommands(opcode, operando);
}
