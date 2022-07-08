#include "sumoInfo.h"
//Implementação das funções

/*==================================|| DEFINIÇÃO DAS FUNÇÕES ||==============================*/
//Função para iniciar robô
void initSystem() {
  //Inicia comunicação Serial 0 e 1
  Serial.begin(9600);
  Serial.println("[BMO]: Iniciando configuração dos pinos...");

  Serial.print("[BMO]: Distância de ataque configurada: ");
  Serial.print(_DATK);
  Serial.println(" cm");
}
/*=====================================|| DEFINIÇÃO DOS MÉTODOS ||==============================*/
//MÉTODOS DA CLASSE MOTOR
//Configura pinos do motor
motorDC::motorDC(uint16_t _pin1, uint16_t _pin2, uint16_t _Speed = 150) {
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
  Speed = 255;
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

//MÉTODOS DA CLASSE MotorSystem
//Cadastrar motores para controlar o carro
MotorSystem::MotorSystem(motorDC _ME, motorDC _MD, uint8_t _spd) {
  //Salvando endereços
  ME = _ME;
  MD = _MD;
  //Configurando velocidade
  constrain(_spd, 0, 255);
  spd = _spd;
}

//Andar para frente
void MotorSystem::ForwardCar(uint16_t t) { //Andar para frente por t segundos
  ME.Forward();
  MD.Forward();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void MotorSystem::ForwardCar() {
  ME.Forward();
  MD.Forward();
}

//Dar ré
void MotorSystem::BackwardCar(uint16_t t) { //Ré por t segundos
  ME.Backward();
  MD.Backward();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void MotorSystem::BackwardCar() {
  ME.Backward();
  MD.Backward();
}

//Parar o robô
void MotorSystem::StopCar(uint16_t t) {
  ME.Stop();
  MD.Stop();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void MotorSystem::StopCar() {
  ME.Stop();
  MD.Stop();
}

//Andar para a direita
void MotorSystem::RightCar(uint16_t t) {
  ME.Stop();
  MD.Forward();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void MotorSystem::RightCar() {
  ME.Stop();
  MD.Forward();
}

//Andar para a esquerda
void MotorSystem::LeftCar(uint16_t t) {
  ME.Forward();
  MD.Stop();
  vTaskDelay(pdMS_TO_TICKS(t));
}
void MotorSystem::LeftCar() {
  ME.Forward();
  MD.Stop();
}

//Girar o robô x graus
void MotorSystem::turnR(uint16_t _spd, uint16_t _angle) {
  //Falta programar
}

//Método que inicia rotina de TrackBack de um carro inimigo.
void MotorSystem::TrackBack(uint8_t _sideOfturn) {
  //Aqui o robô vai estar rodando até encontrar alguém.
  if (_sideOfturn == RST) {//Gira para a direita
    ME.Forward();
    MD.Backward();
  }
  else if (_sideOfturn == LST) {//Gira para a esquerda
    ME.Backward();
    MD.Forward();
  }
  else {//Se for enviado um valor diferente desses dois, considera como rotação para a direita
    ME.Forward();
    MD.Backward();
  }
}

void MotorSystem::SpeedConfig(uint16_t _spd) {
  ME.SpeedConfig(_spd);
  MD.SpeedConfig(_spd);
}

//Método para realizar rotina de teste dos motores
void MotorSystem::TestRoutine(uint16_t t) {
  //Testando sentido de movimento
  ForwardCar(t);//Para frente
  BackwardCar(t);//Para trás
  RightCar(t);//Para a direita
  LeftCar(t);//Para a esquerda.
  //Testanto controle de velocidade
  for (int it = 0; it <= 255; it = it + 5) {
    ForwardCar();
    SpeedConfig(it);
    vTaskDelay(pdMS_TO_TICKS(100));//0.1 s cada variação de velocidade
  }
}

//MÉTODOS DA CLASSE ULTRASSÔNIC
//Construtor
ultrassonic::ultrassonic(uint8_t pinTRIG = 12, uint8_t pinECHO = 13) {
  _pinTrig = pinTRIG;
  _pinEcho = pinECHO;
  pinMode(_pinTrig,OUTPUT);
  pinMode(_pinEcho,INPUT);
  //Por default, TRIG=12 e ECHO=13
}

//Captura distância do sensor.
float ultrassonic::captureDistance() {
  //Capturando tempo de emissão do sinal
  digitalWrite(_pinTrig, LOW);
  vTaskDelay(pdMS_TO_TICKS(3));
  digitalWrite(_pinTrig, HIGH);
  vTaskDelay(pdMS_TO_TICKS(10));
  digitalWrite(_pinTrig, LOW);

  _timeD = pulseIn(_pinEcho, HIGH);

  //Calculando a distância.
  _timeD = _timeD / 2;

  distance = _timeD * 0.034; //Valor da distância em cm.

  //OBS: _timeD e distance são variáveis internas a classe,
  //Portanto, podem ser vistar após a execução desse método.
  //Zerando o tempo para que não ocorra erro na próxima execução
  _timeD = 0;

  return distance;
}


//MÉTODOS DA CLASSE SENSORIV
//Construtor - configura o pino assocaido a esse sensor.
sensorIV::sensorIV(uint8_t pinDG) {
  _pinDg = pinDG;
  pinMode(_pinDG,INPUT); //Configura o pino como entrada de dados.
}

//Método para descobir se o sensor foi ou não acionado
bool sensorIV::ReadSensor() {
  state = (LOW == digitalRead(_pinDg));
  //O módulo utilizado retorna LOW quando encontra um objeto, portanto
  //Caso o objeto esteja em Low,ele irá retornar esse valor.

  return state;
}
