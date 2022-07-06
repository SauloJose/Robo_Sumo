/***************************************************************************************************
  PROGRAMA PRINCIPAL DO PROJETO DE ROBÔ SUMÔ - IFAL PALMEIRA - GRUPO DE ROBÓTICA - VERSÃO 1.0
  Instituto Federal de Alagoas (IFAL) - Campus Palmeira dos Índios.
  Equipe: Alan, Italo, Saulo, Wellington.
  Objetivo: O robô em questão irá batalhar em um ringue de batalha de robô sumo (classificação 500g)
  no qual deverá agir e batalhar de forma autônoma, sem ajuda de terceiros.
  Ultima modificação: 05/07/2022 - 15:47
  placa: UNO.
****************************************************************************************************/

/*============================================================================*/
//Defines do projeto.
//Defines do sensor ultrassônico
#define ECHO 12
#define TRIG 13

//Definindo pinos dos sensores infravermelho
#define SD1 9
#define SD2 8
#define SD3 4
#define SD4 3

//Sensores infravermelhos retornan LOW quando acionados.

//velocidades de ataque (VATK) e rastreamento (VRST)
#define _VATK 255
#define _VRST 180

//Distâncias para  atacar
#define _DATK 50
#define _DATKMIN 10

//definir pino de interrupção, no nosso caso, será remoto.
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
void frente();
void frente(uint8_t t);
void parado();
void parado(uint8_t t);
void para();
void para(uint8_t t);
void esquerda();
void esquerda(uint8_t t);
void direita();
void direita(uint8_t t);
void rastreamento(bool dir);
void re();
void re(uint8_t t);

//Função para configurar pinos e etc
void initRobot();

//Criando função de medição
long detectar();

//Variável transforma em dist.
float cmDet();

//função callback
void chaveOnoff();

//Função para testar motores.
void rotina_motor(int t);

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
  //Função para configurar pinos do robô
  initRobot();
  delay(1000);
}

/*============================================================================*/

//Iniciando o loop infinito.
void loop() {
  bool op;
  //Pega valor em cm da distância.
  float cm = 0;
  cm = cmDet();

  //Valores dos sensores infravermelhos
  //V1,V2,V3 e V4 ficam em true caso sejam acionadas
  bool v1 = (LOW == digitalRead(SD1));
  bool v2 = (LOW == digitalRead(SD2));
  bool v3 = (LOW == digitalRead(SD3));
  bool v4 = (LOW == digitalRead(SD4));

  //Valor de localização do sensor ultrassônico
  bool us = false;//Inicializa em false
  us = (cm < _DATK); //Verifica se está ou não dentro da distância


  //Pega os valores da distância.
  Serial.print("Dist: ");
  Serial.print(cm);
  Serial.println(" cm");
  //Rastreia até encontrar algo, e então ele vai em direção.
  if (us) {//Caso a distância entre objeto seja menor que a distância mínima de ataque.
    //Maneiras de ataque
    if (((~v1)&&(~V2)) || ((~v2) && v4) || ((~v1) && v4)) { //Situação A1 ->ATAQUE
      frente();
    }
    else if ((~v1) && v2 && (~v2)) { //-> Situação E1 -> Combinação de ataque.
      //Analisa distância entre os objetos.
      if (cm < _DATKMIN) {//Distância mínima para ataque!
        frente();
      }
      else {//Reposicionar, estratégia!
        esquerda(100);//Reposicionando
        rastreamento(true);//Rastrear para a direita
      }
    }
    else if ((v1) && (~v2) && (~v4)) { //-> SITUAÇÃO E2-> COMBINAÇÃO DE ATAQUE
      //Analisa distÂncia entre os objetos
      if (cm < _DATKMIN) {//Distância mínima para ataque!
        frente();
      }
      else {//Reposicionar, estratégia!
        direita(100);
        rastreamento(false);//Rastrear para a esquerda
      }
    }
    else if ((v1 && v2 && (~v3) && (v4)) || (v1 && v2 && (v3) && (~v4)))  { //=> SITUAÇÃO E3 -> COMBINAÇÃO DE ATAQUE.
      re(200);//Dá ré
      rastreamento(true);//Rastreia para a direita
    }
    else if (v1 && v2 && (~v3) && (~v4)) {//Caso de vitória
      //Nesse caso, provavelmente ele ganhou. Então só comemora
      rastreamento(true);
      velocidade(255);
      //Colocar uns leds para piscar e um buzzer para fzr zuada (tocando a música de starwars)

    }
    else {//Situação de erro, ou seja, impossível de acontecer.
      //Sinalisar um led é uma boa opção...
      rastreamento(true);
    }
  }
  else {//Se a distância detectada for maior que a distância de ataque
    //Rastrear
    if (((~v1) && (~v3) && (v4)) || ((~v1) && (v2) && (~v3))) { //Caso R1

    }
    else if (((~v1) && (~v2) && v3) || ((~v2) && (~v4))) { //Caso R2

    }
    else if ((v1 && v2 && (~v3)) || (v1 && v2 && (~v4))) { //Caso R3

    }
    else { //Caso de erro ou não planejado
      rastreamento(true);

    }


  }
  /*//Aqui vai estar o código para testar os sensores.
    //Código base de apenas rastrear e seguir.
    if (cm > _DATK) {
    //Enquanto não encontrar um alvo dentro do alcance, ele rastreina os inimigos.
    //Rotina de rastreamento
    rastreamento();
    }
    else {
    frente();
    }

  */
}
/*============================================================================*/
//Definindo funções utilizadas pelo robô.

void frente() {
  // Comando para o motor ir para frente
  Serial.println("EM FRENTE!");
  velocidade(_VATK);//Velocidade máxima para atacar
  Motor1.Forward();
  Motor2.Forward();
}

void frente(uint8_t t) {
  Serial.println("EM FRENTE!");
  velocidade(_VATK);//Velocidade máxima para atacar
  Motor1.Forward();
  Motor2.Forward();
  delay(t);
}
void re() {
  // Comando para o motor ir para trás
  Motor1.Backward();
  Motor2.Backward();
}

void re(uint8_t t) {
  // Comando para o motor ir para trás
  Motor1.Backward();
  Motor2.Backward();
  delay(t);
}

void esquerda() {
  //Comando para o motor parar
  Motor1.Stop(); //Comando para o motor para
  Motor2.Forward();
}

void esquerda(uint8_t t) {
  //Comando para o motor parar
  Motor1.Stop(); //Comando para o motor para
  Motor2.Forward();
  delay(t);
}

void direita() {
  Motor1.Forward();
  Motor2.Stop();
}

void direita(uint8_t t) {
  Motor1.Forward();
  Motor2.Stop();
  delay(t);
}

void para() {
  Motor1.Stop(); // Comando para o motor parar
  Motor2.Stop();
}

void para(uint8_t t) {
  Motor1.Stop(); // Comando para o motor parar
  Motor2.Stop();
  delay(t);
}

void velocidade(int num) {
  //Transforma o número no intervalo de 0 a 255
  constrain(num, 0, 255);
  //Define a velocidade do carrinho
  Motor1.Speed(num);
  Motor2.Speed(num);
  Serial.print("Velocidade dos motores configurada para:");
  Serial.println(num);
}

//Função de detectar do sensor.
long detectar() {
  digitalWrite(TRIG, LOW);
  delay(3);
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH);
}


//Função de conversão de valor.
float  cmDet() {
  long tempo = detectar();
  tempo = tempo / 2;
  float distancia = tempo * 0.034; // cm

  return distancia;
}

//Aqui é uma função para ligar e desligar, por enquanto.
void chaveOnOff() {
  static unsigned long lastMillis = 0;
  unsigned long newMillis = millis();

  if (newMillis - lastMillis < 50) {}
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

void rastreamento(bool dir) {
  //Gira até detectar alguém
  Serial.println("RASTREAMENTO");
  if (dir = true) {
    //Rastreia para o lado direito.
    velocidade(_VRST);
    Motor1.Forward();
    Motor2.Backward();
  }
  else {
    //Rastreia para o lado esquerdo
    velocidade(_VRST);
    Motor1.Backward();
    Motor2.Forward();
  }
}


//Apenas uma rotina para testar se os motores estão funcionando corretamente
void rotina_motor(int t) {
  frente();
  Serial.println("Frente");
  delay(t);
  esquerda();
  Serial.println("Esquerad");
  delay(t);
  direita();
  Serial.println("Direita");
  delay(t);
  re();
  Serial.println("Re");
  delay(t);
  para();
  Serial.println("PARA");
}

//Função para iniciar os GPIO's do Arduino
void initRobot() {
  //Controle de pinos do sensor de distância
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  //Pinos dos sensores infravermelhos.
  pinMode(SD1, INPUT);
  pinMode(SD2, INPUT);
  pinMode(SD3, INPUT);
  pinMode(SD4, INPUT);

  //Iniciando a Serial.
  Serial.begin(9600);
  Serial.print("Distancia de ataque configurada: ");
  Serial.print(_DATK);
  Serial.println(" cm");
}
