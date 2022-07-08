#ifndef SUMOINFO_H
#define SUMOINFO_H

/*================|| DEFINIÇÕES ||====================*/
//Definições do sensor ultrasônico
#define ECHO 13
#define TRIG 12

//Definições gerais
#define BOT 2
#define NumberIRsensor 4
#define NumberUltraSonic 1
#define _VATK 250
#define _VRST 180
#define _DATK 50

//Definição dos sensores infravermelhos
#define SD1 8
#define SD2 7
#define SD3 5
#define SD4 4

//Lado de giro
#define RST 896 //Para a direita
#define LST 897 //Para a esquerda


//Definições para os motores
//motor esquerdo:
#define _MESQA 11
#define _MESQB 10

//Motor direito:
#define _MDIRA 7
#define _MDIRB 6


//Struct responsável por pegar os dados dos sensores infravermelhos
struct sensorIR {
  bool State[NumberIRsensor];
};

/*================|| CLASSES ||====================*/
//Classe para controlar os motores.
class motorDC {
  private:
    //Atributos
    uint16_t Speed = 255, pin1, pin2;
  public:
    //Constructor do motor
    motorDC(
      uint16_t _pin1,//Pino positivo do motor DC
      uint16_t _pin2,//Pino negativo do motor DC
      uint16_t _Speed = 150); //Velocidade inicial do motor.
    //Por default, esse valor é 150, caso não seja enviado.

    motorDC();

    //velocidade
    void SpeedConfig(uint16_t _spd);

    //girar no sentido direto
    void Forward();

    //girar no sentido inverso
    void Backward();

    //Parar o motor
    void Stop();

    //Girar o motor tantos graus.
    void turnM(uint16_t spd, uint16_t angle);
};

//classe que controla o sistema motor do robô.
class MotorSystem: private motorDC {
  private:
    //Velocidade total de ambos os motores.
    uint8_t spd = 255;//Velocidade máxima por default
    float  distDetector;

    //objetos MOTORDC que serão criados para controlar eles, com o objeto robô
    motorDC ME, MD;
    //Motor da esquer (ME) da, motor da direita(MD).
  

  public:
    //Construtóres da classe
    MotorSystem(
      motorDC _ME, //Endereço para controlar o motor do lado esquerdo. Por default, o valor é NULL (Sem endereço).
      motorDC _MD, //Endereço para controlar o motor do lado direito. Por default, o valor é NULL (Sem endereço).
      uint8_t _spd = 255); //Velocidade padrão inicial do robô. Por default, o valor é 255

    MotorSystem(); //Esse construtor é o que se utiliza sem nenhum valor enviado. Aconselhado não utilizar.

    //Andar para frente
    void ForwardCar(uint16_t t);//Andar para frente por t segundos
    void ForwardCar();

    //Dar ré
    void BackwardCar(uint16_t t);//Ré por t segundos
    void BackwardCar();

    //Parar o robô
    void StopCar(uint16_t t);//Parar por t segundos
    void StopCar();

    //Andar para a direita
    void RightCar(uint16_t t);//Andar para a direitar por t segundos
    void RightCar();

    //Andar para a esquerda
    void LeftCar(uint16_t t); // Andar para a esquerda por t segundos
    void LeftCar();

    //Girar o robô x graus
    void turnR(uint16_t _spd, uint16_t _angle);

    //Método que inicia rotina de TrackBack (Rastrear) um carro robô inimigo.
    void TrackBack(uint8_t _sideOfturn);

    //Método para realizar rotina de teste dos motores
    void TestRoutine(uint16_t t);

    //Método para variar a velocidade do sistema de movimento.
    void SpeedConfig(uint16_t _spd);
};

//Classe do sensor ultrassÔnico
class ultrassonic {
  private:
    uint8_t _pinTrig;//Pino do TRIG do sensor.
    uint8_t _pinEcho;//Pino do ECHO do sensor.
    float distance;//Distância atual capturada pelo sensor
    long _timeD; //Variável para manipulação interna do objeto

  public:
    //Construtor
    ultrassonic(
      uint8_t pinTRIG,//Qual pino está conectado o  TRIG
      uint8_t pinECHO);//Qual pino está conectado o  ECHO

    //métodos
    float captureDistance();//Método para retornar a distância que o sensor capturou
};

//Classe do sensor infravermelho;
class sensorIV {
  private:
    //uint8_t _pinAn;
    uint8_t _pinDg;//Pino digital associado ao módulo
    bool state;//Estado do pino . True (Tem algo captado), False (Não tem nada captado)

  public:
    //Construtor
    sensorIV(uint8_t pinDG);//Apenas coloco o pino que ele está conectado.

    //Métodos
    bool ReadSensor();//Retorna se o sensor detectou ou nada algo.
};

/*================|| Funçõoes ||====================*/
//Procedimentos
void initFreeRTOS();

//Procedimento para inicializar pinos
void initSystem();

#endif
