/*================|| DEFINIÇÕES ||====================*/
#define ECHO 13
#define TRIG 12
#define BOT 2
/*================|| CLASSES ||====================*/
class motorDC {
  private:
    //Atributos
    uint16_t Speed = 255, pin1, pint2;
  public:
    //Métodos
    //Configurando pinos do motor
    void pinout(uint16_t in1, in2);

    //velocidade
    void SpeedConfig(uint16_t spd);

    //girar no sentido direto
    void Forward();

    //girar no sentido inverso
    void Backward();

    //Parar o motor
    void Stop();
};

//classe controlando o robô
class motorDC: public robo {
    private
    //Velocidade total de ambos os motores.
    uint16_t speedtot = 255;

    //Criando objetos para ambos os motores.
    motorDC motorA, motorB;

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

//Fazendo as configurações
