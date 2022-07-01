#ifndef SUMOINFO_H
#define SUMOINFO_H

/*================|| DEFINIÇÕES ||====================*/
#define ECHO 13
#define TRIG 12
#define BOT 2
#define NumberFotoSensor 4
#define NumberUltraSonic 1


//Struct de dados para os sensores fotoelétricos
struct sensorFE{
  bool state[NumberFotoSensor];
};

/*================|| CLASSES ||====================*/
class motorDC {
  private:
    //Atributos
    uint16_t Speed = 255, pin1, pint2;
  public:
    //Configurando pinos do motor
    void pinout(uint16_t in1, uint16_t in2);

    //velocidade
    void SpeedConfig(uint16_t spd);

    //girar no sentido direto
    void Forward();

    //girar no sentido inverso
    void Backward();

    //Parar o motor
    void Stop();

    //Girar o motor tantos graus.
    void turn(uint16_t spd, uint16_t angle);
};

//classe controlando o robô
class Robot: private motorDC {
  private:
    //Velocidade total de ambos os motores.
    uint16_t speedtot = 255;
    motorDC rightMotor,leftMotor;
  public:
    //Cadastrar motores para controlar o carro
    void setupCar(motorDC m1, motorDC m2);

    //Andar para frente
    void ForwardCar(uint16_t t);

    //Dar ré
    void BackwardCar(uint16_t t);

    //Parar o carro
    void StopCar(uint16_t t);

    //Andar para a direita
    void RightCar(uint16_t t);

    //Andar para a esquerda
    void LeftCar(uint16_t t);

    //Girar theta graus
    void turn(uint16_t spd, uint16_t angle);
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

//Configura as portas para serem utilizadas
void initPins();

#endif
