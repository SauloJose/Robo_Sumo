#include "sumoInfo.h"
//Implementação das funções

/*==================================|| DEFINIÇÃO DAS FUNÇÕES ||==============================*/
//Ativar pulso do sensor ultrasônico
float Ultrassonic() {
  digitalWrite(TRIG, LOW);
  vTaskDelay(pdMS_TO_TICKS(3));
  digitalWrite(TRIG, HIGH);
  vTaskDelay(pdMS_TO_TICKS(10));
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH);
}

//Converter leitura para distância
float convertCm() {
  long tempo = Ultrassonic();
  tempo = tempo / 2;

  float dist;
  dist = tempo * 0.034; //cm

  return dist;
}

//Rotina do sensor.
void routineMotor(uint16_t t) {

}


//Função para iniciar robô
void initRobot() {
  //Inicia comunicação Serial 0 e 1
  Serial.begin(9600);
  Serial.println("[BMO]: Iniciando configuração dos pinos...");
  //Pinos do sensor de distância
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  //Pinos dos sensores infravermelhos
  pinMode(SD1, INPUT);
  pinMode(SD2, INPUT);

  Serial.print("[BMO]: Distância de ataque configurada: ");
  Serial.print(_DATK);
  Serial.println(" cm");
}

/*========================|| DEFINIÇÃO DOS TAREFAS (TASKS) ||==============================*/
//Tarefa cérebro -> Responsável por computador os dados dos sensores
void vTaskBrain(void* pvParamaters) {
  (void*) pvParamaters;

  for (;;) {

  }
}

//Tarefa sensor -> Responsável por pegar os dados dos sensores e enviar para o cérebro
void vTaskSensor(void* pvParamaters) {
  (void*) pvParamaters;

  for (;;) {

  }
}

//Tarefa liga/desliga -> Vai ligar, ou desligar, o robô
void vTaskOnOFF(void* pvParamaters) {
  (void*) pvParamaters;

  for (;;) {

  }
}

//Tarefa motor -> Irá controlar o robô, fazendo ele andar e realizar as manobras necessárias.
void vTaskMotor(void* pvParamaters) {
  (void*) pvParamaters;

  //Configura o robô para que a tarefa trabalhe com ele
  for (;;) {

  }
}





/*=====================================|| DEFINIÇÃO DOS MÉTODOS ||==============================*/
//MÉTODOS DA CLASSE MOTOR
//Configura pinos do motor
motorDC::motorDC(uint16_t _pin1, uint16_t _pin2, uint16_t _Speed = 255) {
  pin1 = _pin1;
  pin2 = _pin2;
  Speed = _Speed;
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

motorDC::motorDC() {
  //Esses são os valores por DEFAULT
  pin1 = 10;
  pin2 = 9;
  Speed=255;
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

//configura velocidade de giro do motor
void motorDC::SpeedConfig(uint16_t _spd) {
  constrain(_spd, 0, 255); //Mantém a velocidade no intervalo de 0 a 255.
  Speed = _spd;
}

//Configura motor para rodar no sentido direto
void motorDC::Forward() {
  analogWrite(pin1, Speed);
  digitalWrite(pin2, LOW);
}

//Configura motor para girar no sentido inverso
void motorDC::Backward() {
  digitalWrite(pin1, LOW);
  analogWrite(pin2, Speed);
}

//Para a rotação do motor
void motorDC::Stop() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

//Gira o motor em tantos graus (servos motores bons!)
void motorDC::turnM(uint16_t spd, uint16_t angle) {
  //Falta programar essa questão do giro.
}

//MÉTODOS DA CLASSE ROBÔ
//Cadastrar motores para controlar o carro
Robot::Robot(motorDC* _ME, motorDC* _MD, uint8_t _spd) {
  //Salvando endereços
  ME = _ME;
  MD = _MD;
  //Configurando velocidade
  constrain(_spd, 0, 255);
  spd = _spd;
}

//Andar para frente
void Robot::ForwardCar(uint16_t t) { //Andar para frente por t segundos
  ME->Forward();
  MD->Forward();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void Robot::ForwardCar() {
  ME->Forward();
  MD->Forward();
}

//Dar ré
void Robot::BackwardCar(uint16_t t) { //Ré por t segundos
  ME->Backward();
  MD->Backward();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void Robot::BackwardCar() {
  ME->Backward();
  MD->Backward();
}

//Parar o robô
void Robot::StopCar(uint16_t t) {
  ME->Stop();
  MD->Stop();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void Robot::StopCar() {
  ME->Stop();
  MD->Stop();
}

//Andar para a direita
void Robot::RightCar(uint16_t t) {
  ME->Stop();
  MD->Forward();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void Robot::RightCar() {
  ME->Stop();
  MD->Forward();
}

//Andar para a esquerda
void Robot::LeftCar(uint16_t t) {
  ME->Forward();
  MD->Stop();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void Robot::LeftCar() {
  ME->Forward();
  MD->Stop();
}

//Girar o robô x graus
void Robot::turnR(uint16_t spd, uint16_t angle) {
  //Falta programar
}

//Método que inicia rotina de rastreamento de um carro inimigo.
void Robot::rastreamento() {
  //Aqui o robô vai estar rodando até encontrar alguém.
  ME->Forward();
  MD->Backward();

  //Quem vai escolher qual hora ele para de rastrear é o cérebro.
}

//Método para realizar rotina de teste dos motores
void Robot::rotinaRobo(uint16_t t) {

}
