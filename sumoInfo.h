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

//Struct responsável por pegar os dados dos sensores infravermelhos
struct sensorIR {
  bool Value[NumberIRsensor];
};

/*================|| CLASSES ||====================*/
//Classe para controlar os motores.
class motorDC {
  private:
    //Atributos
    uint16_t Speed = 255, pin1, pin2;
  public:
    //Constructor do motor
    motorDC(uint16_t _pin1, uint16_t _pin2, uint16_t _Speed);
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

//classe controlando o robô
class Robot: private motorDC {
  private:
    //Velocidade total de ambos os motores.
    uint8_t spd = 255;//Velocidade máxima por default
    float  distDetector;

    //objetos MOTORDC que serão criados para controlar eles, com o objeto robô
    motorDC* ME, *MD; 
    //Motor da esquer (ME) da, motor da direita(MD).
    //Endereço para poderem ser manipulados por outras classes.
    
  public:
    //Cadastrar motores para controlar o carro
    Robot(motorDC* _ME=NULL, motorDC* _MD=NULL,uint8_t _spd=255);
    Robot();

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
    void turnR(uint16_t spd, uint16_t angle);

    //Método que inicia rotina de rastreamento de um carro inimigo.
    void rastreamento();

    //Método para realizar rotina de teste dos motores
    void rotinaRobo(uint16_t t);
};
/*================|| Funçõoes ||====================*/
//Ativar pulso do sensor ultrasônico
float Ultrassonic();

//Converter leitura para distância
float convertCm();

//Rotina do sensor.
void routineMotor(uint16_t t);

//Procedimentos
void initFreeRTOS();

//Procedimento para inicializar pinos
void initRobot();

#endif
