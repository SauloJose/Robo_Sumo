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
#define ECHO 12
#define TRIG 13

//Definindo pinos dos sensores infravermelho
#define SD1 9
#define SD2 8

//velocidade
#define _VATK 250
#define _VRST 180

//Distância para ataque
#define _DATK 50

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
void frente();
void parado();
void para();
void esquerda();
void direita();
void rastreamento();
void re();

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
  float cm=0;
  cm=cmDet();
  Serial.print("Dist: ");
  Serial.print(cm);
  Serial.println(" cm");
  //Rastreia até encontrar algo, e então ele vai em direção.
  if (cm > _DATK) {
    //Rotina de rastreamento
    rastreamento();
  }
  else{
    //Começa a atacar
    frente();
  }
  /*//Aqui vai estar o código para testar os sensores. 
  */
}
/*============================================================================*/
//Definindo funções utilizadas pelo robô.

void frente()
{
  // Comando para o motor ir para frente
  Serial.println("EM FRENTE!");
  velocidade(_VATK);//Velocidae máxima para atacar
  Motor1.Forward();
  Motor2.Forward();
}

void re()
{
  // Comando para o motor ir para trás
  Motor1.Backward();
  Motor2.Backward();
}

void esquerda()
{
  //Comando para o motor parar
  Motor1.Stop(); //Comando para o motor para
  Motor2.Forward();

}

void direita()
{
  Motor1.Forward();
  Motor2.Stop();

}

void para()
{
  Motor1.Stop(); // Comando para o motor parar
  Motor2.Stop();

}

void velocidade(int num)
{
  //Transforma o número no intervalo de 0 a 255
  constrain(num, 0, 255);
  //Define a velocidade do carrinho
  Motor1.Speed(num);
  Motor2.Speed(num);
  Serial.print("Velocidade dos motores configurada para:");
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

  if (newMillis - lastMillis < 50){}
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

void rastreamento() {
  //Gira até detectar alguém
  Serial.println("RASTREAMENTO");
  velocidade(_VRST);
  Motor1.Forward();
  Motor2.Backward();
  
}


void rotina_motor(int t)
{
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

void initRobot(){

  //Controle de pinos do sensor de distância
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  //Pinos dos sensores infravermelhos.
  pinMode(SD1,INPUT);
  pinMode(SD2,INPUT);
  
  //Iniciando a Serial.
  Serial.begin(9600);
  Serial.print("Distancia de ataque configurada: ");
  Serial.print(_DATK);
  Serial.println(" cm");
}
