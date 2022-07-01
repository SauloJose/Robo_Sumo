/***************************************************************************************************
   PROGRAMA PRINCIPAL DO PROJETO DE ROBÔ SUMÔ - IFAL PALMEIRA - GRUPO DE ROBÓTICA - VERSÃO 0.9
   Instituto Federal de Alagoas (IFAL) - Campus Palmeira dos Índios.
   Equipe: Alan, Italo, Saulo, Wellington.
   Objetivo: O robô em questão irá batalhar em um ringue de batalha de robô sumo (classificação 500g)
   no qual deverá agir e batalhar de forma autônoma, sem ajuda de terceiros.
   Ultima modificação: 
   placa: UNO.
****************************************************************************************************/

/*============================================================================*/
//Defines do projeto.
//Defines do sensor ultrassônico
#define ECHO 13
#define TRIG 12

//velocidade
#define _VATK 250
#define _VRST 100
//definir pino de interrupção
#define BOT 2 //Interrupção 0.

/*============================================================================*/

//Classe para controlar o motor.
class DCmotor
{
    //Definindo variáveis internas da classe
    int spd = 255, pin1, pin2;

    //Métodos e variáveis que são utilizados
  public:
    void Pinout(int in1, int in2) {//Método para inicializar as entradas para o motor.
      pin1 = in1;
      pin2 = in2;
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
    }
    void Speed(int in1) //Método para configurar a velocidade
    {
      spd = in1;
    }
    void Forward() //Método para girar no sentido direto
    {
      analogWrite(pin1, spd);
      digitalWrite(pin2, LOW);
    }
    void Backward() //Método para girar no sentido inverso
    {
      digitalWrite(pin1, LOW);
      analogWrite(pin2, spd);
    }
    void Stop() //Método para parar.
    {
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
    }
};
//Criando os objetos motor1 e motor2
DCmotor Motor1, Motor2;

/*============================================================================*/
//Criando as funções
void frente(int t);
void parado(int t);
void para(int t);
void esquerda(int t);
void direita(int t);
void rastreamento(int t);

//Criando função de medição
long detectar();

//Variável transforma em dist.
float cmDet();

//função callback
void chaveOnoff();

void rotina_motor();

/*============================================================================*/

//Definindo variável para interrupção.
volatile int estado = HIGH;
//OBS: INICIA NO ESTADO LOW(Desligado).

/*============================================================================*/
//Iniciando o Setup
void setup() {
  //Configurando os pinos do motor
  Motor1.Pinout(11, 10);
  Motor2.Pinout(6, 5);

  //Gerando interrupção no pino 2.

  attachInterrupt(0, chaveOnOff, RISING);

  /**************************************************
     0 -> Interrupção no pino 2;
     ChaveOnOff: Função de callback da interrupção
     Rising: Altera de low para high => Interrupção.
   ***************************************************/

  //Controle de pinos do sensor de distância
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  //Iniciando a Serial.
  Serial.begin(9600);

  //Deixar velocidade fixa
  velocidade(150);
}

/*============================================================================*/

//Iniciando o loop infinito.
void loop() {
  bool op;
  //Pega valor em cm da distância.
  float cm;
  //Rastreia até encontrar algo, e então ele vai em direção.
  if (cm > 20) {
    velocidade(_VRST);//Velocidade para rastrear
    rastreamento(3000);
    cm=cmDet();
  }
  else{
    velocidade(_VATK);//Velocidae máxima para atacar
    frente(3000);//Vai em frente por 3 segundos
    cm=cmDet();
  }
}
/*============================================================================*/
//Definindo funções utilizadas pelo robô.

void frente(int t)
{
  // Comando para o motor ir para frente
  Motor1.Forward();
  Motor2.Forward();
  delay(t);
}

void re(int t)
{
  // Comando para o motor ir para trás
  Motor1.Backward();
  Motor2.Backward();

  delay(t);
}

void esquerda(int t)
{
  //Comando para o motor parar
  Motor1.Stop(); //Comando para o motor para
  Motor2.Forward();
  delay(t);
}

void direita(int t)
{
  Motor1.Forward();
  Motor2.Stop();
  delay(t);
}

void para(int t)
{
  Motor1.Stop(); // Comando para o motor parar
  Motor2.Stop();
  delay(t);
}

void velocidade(int num)
{
  //Transforma o número no intervalo de 0 a 255
  constrain(num, 0, 255);
  //Define a velocidade do carrinho
  Motor1.Speed(num);
  Motor2.Speed(num);
  Serial.println("Velocidade dos motores configurada para:");
  Serial.println(num);
}

//Função de detectar do sensor.
long detectar()
{
  digitalWrite(TRIG, LOW);
  delay(3);
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH);
}


//Função de conversão de valor.
float  cmDet()
{
  long tempo = detectar();
  tempo = tempo / 2;
  float distancia = tempo * 0.034; // cm

  return distancia;
}

//Função de callback do carro.
//Aqui é uma função para ligar e desligar, por enquanto.
void chaveOnOff()
{
  static unsigned long lastMillis = 0;
  unsigned long newMillis = millis();

  if (newMillis - lastMillis < 50)
  {

  }
  else
  {
    estado = !estado;
    Serial.println("Estado mudou.");
    if (estado)
    {
      Serial.println("Ligado!");
    }
    else
    {
      Serial.println("Desligado!");
    }
    lastMillis = newMillis;
  }

}

void rastreamento(int t) {
  //Gira até detectar alguém
  Motor1.Forward();
  Motor2.Backward();
  delay(t);
}


void rotina_motor()
{
  frente(3000);
  Serial.println("Frente");
  esquerda(2000);
  Serial.println("Esquerad");
  direita(2000);
  Serial.println("Direita");
  re(2000);
  Serial.println("Re");
  para(4000);
}
