/***************************************************************************************************
   PROGRAMA PRINCIPAL DO PROJETO DE ROBÔ SUMÔ - IFAL PALMEIRA - GRUPO DE ROBÓTICA - VERSÃO 1.0
   Instituto Federal de Alagoas (IFAL) - Campus Palmeira dos Índios.
   Equipe: Alan, Italo, Saulo, Wellington.
   Objetivo: O robô em questão irá batalhar em um ringue de batalha de robô sumo (classificação 500g)
   no qual deverá agir e batalhar de forma autônoma, sem ajuda de terceiros.
   Ultima modificação: 06/07/2022
****************************************************************************************************/

/****************************************************************************************************
  |     NOME       |     CORE       |    PRIORIDADE     |               DESCRIÇÃO                   |
  |----------------|----------------|-------------------|-------------------------------------------|
  |-vTaskBrain     |      00        |         03        |    Tem como objetivo analisar os dados    |
  |                |                |                   |  Colhidos pela tarefa sensor e então tomar|
  |                |                |                   |  decisões de ataque e defesa.             |
  |----------------|----------------|-------------------|-------------------------------------------|
  |-vTaskSensor    |      00        |         02        |  Tarefa responsável por analisar o ambi-  |
  |                |                |                   |ente e enviar as informações para o cerebro|
  |----------------|----------------|-------------------|-------------------------------------------|
  |-vTaskOnOFF     |      00        |         02        |  Responsável por ligar ou desligar o robô |
  |----------------|----------------|-------------------|-------------------------------------------|
  |-vTaskMotor     |      00        |         04        |  Responsável por realizar os movimentos   |
  |                |                |                   |                                           |
****************************************************************************************************/

//Incluindo bibliotecas
#include "sumoInfo.h"
#include <Arduino_FreeRTOS.h>
#include "queue.h"

//Gerando filas
QueueHandle_t xFilaMov;
QueueHandle_t xFilaPosition;
QueueHandle_t xFilaDistance;

//Gerando os Handles (Endereços) das tarefas
TaskHandle_t vTaskBrainHandle;
TaskHandle_t vTaskSensorHandle;
TaskHandle_t vTaskOnOFFhandle;
TaskHandle_t vTaskMotorHandle;

//Gerando protótipo das tarefas;
void vTaskBrain(void* pvParamaters);
void vTaskSensor(void* pvParamaters);
void vTaskOnOFF(void* pvParamaters);
void vTaskMotor(void* pvParamaters);

//Variável responsável por ligar ou desligar o robô
volatile bool onMotorSystem = HIGH;

/*======================================================================================================*/
//Função setup
void setup() {
  //Inicializando configurações iniciais do robô
  initSystem();//Inicializa as configurações dos pinos do robô
  initFreeRTOS();//Inicializando o sistema operacional (tarefas e filas necessárias)
}

//A função loop é inútil, mas pode ser utilizada se você quiser...
void loop() {
  vTaskDelete(NULL); //Deletando a taskLoop, já que não será necessário.
}

/*=======================================|| Tarefas ||===================================================*/
//Tarefa cérebro . Responsável por computador os dados dos sensores
void vTaskBrain(void* pvParamaters) {
  (void*) pvParamaters;

  //Variáveis para capturar os valores e tomar decisões.
  struct sensorIR sensorValues;
  float distUS=150;

  //Variável de tomada de decisão.
  int movDecision;
  
  //A tarefa do cérebro deve receber os dados dos sensores e decidir o que fazer.
  for (;;) {
    if (xQueueReceive(xFilaPosition,&sensorValues,portMAX_DELAY)==pdTRUE && xQueueReceive(xFilaDistance,&distUS,portMAX_DELAY)==pdTRUE){
      //Se ele recebe informações dos sensores, ele computa o que deve fazer, caso não receba, ele apenas espera chegar algo.


      //Toma a decisão e envia para a tarefa do sistema motor do robô.
      xQueueSend(xFilaMov,&movDecision,portMAX_DELAY);
    }
  }
}

//Tarefa sensor . Responsável por pegar os dados dos sensores e enviar para o cérebro
void vTaskSensor(void* pvParamaters) {
  (void*) pvParamaters;//Parâmetro de inicialização da task

  //Criando objetos para manipulação dos dados
  //Sensores infravermelhos
  sensorIV SIV1(SD1),//Frontal-Esquerdo
           SIV2(SD2),//Frontal-Direito
           SIV3(SD3),//Traseiro-esquerdo
           SIV4(SD4);//Traseiro-Direito

  //sensor ultrassônico
  ultrassonic US(TRIG, ECHO);

  //Estrutura de dados para capturar valores
  struct sensorIR sensorValues;//Valores dos sensores infravermelhos
  float distance = 0; //Sensor de distância.

  for (;;) {
    //Capturando valores
    sensorValues.State[0] = SIV1.ReadSensor();
    sensorValues.State[1] = SIV2.ReadSensor();
    sensorValues.State[2] = SIV3.ReadSensor();
    sensorValues.State[3] = SIV4.ReadSensor();

    //Capturando dado do sensor infravermelho frontal (em cm);
    distance = US.captureDistance();

    //Enviando dado para as respectivas filas.
    xQueueSend(xFilaPosition, &sensorValues, portMAX_DELAY); //Fila dos sensores de posição
    xQueueSend(xFilaDistance, &distance, portMAX_DELAY); //Fila para o valor da distância capturada pelo sensor.

    //Delay para releitura dos dados.
    vTaskDelay(3);
  }
}

//Tarefa liga/desliga . Vai ligar, ou desligar, o robô
void vTaskOnOFF(void* pvParamaters) {
  (void*) pvParamaters;

  for (;;) {
    //Algo...
  }
}

//Tarefa motor . Irá controlar o robô, fazendo ele andar e realizar as manobras necessárias.
void vTaskMotor(void* pvParamaters) {
  (void*) pvParamaters;

  //Cria o objeto do motor para manipular os movimentos dele.
  //Configura inicialmente os motores)->Alocados estáticamente
  motorDC MotorLeft(_MESQA, _MESQB, 180),//Motor da esquerda
          MotorRight(_MDIRA, _MDIRB, 0);//Motor da direita

  //Criando o sistema de controle de movimento BMO.
  MotorSystem BMO(MotorLeft, MotorRight, 180);

  //Variável de controle do movimento.
  uint16_t movState = 0;

  //Configura o robô para que a tarefa trabalhe com ele
  for (;;) {
    if (xQueueReceive(xFilaMov, &movState, portMAX_DELAY) == pdTRUE) { //Caso tenha chegado uma informação do cérebro, ela executa algo.
      switch (movState) {
        case 0: //Se move para frente
          BMO.ForwardCar();
          break;
        case 1://Se move para trás
          BMO.BackwardCar();
          break;
        case 2://Se move para esquerda
          BMO.LeftCar();
          break;
        case 3://Se move para direita
          BMO.RightCar();
          break;
        case 4://Gira em um eixo fixo para a direita (sentido anti-horário);
          BMO.TrackBack(RST);
          break;
        case 5://Gira em um eixo fixo para a Esquerda(Sentido horário);
          BMO.TrackBack(LST);
          break;
        default://Em qualquer caso diferente dos citados, ele apenas fica parado (Código de erro vindo do cérebro).
          BMO.StopCar();
          break;
      }
    }
  }
}

/*============================|| DEFINIÇÃO DA FUNÇÃO DE INICIALIZAÇÃO DO FREERTOS||==============================*/
//Obs: Precisei colocar esses dados aqui, para que as tarefas rodem normalmente.
//Procedimento para iniciar o FreeRTOS com as tarefas
void initFreeRTOS() {
  Serial.println("[BMO]: Iniciando FreeRTOS.");
  BaseType_t returnSensor, returnBrain, returnMotor, returnOnOFF;

  //Gerando filas necessárias.
  //Fila para movimentação do robÔ
  xFilaMov = xQueueCreate(
               1,//Quantidade de comandos enviados;
               sizeof(uint8_t));//Tamanho do comando enviado;

  //Fila para receber dados dos sensores infravermelhos.
  xFilaPosition = xQueueCreate(
                    1,//Quantidade de dados enviados
                    sizeof(sensorIR)); //Tamanho de cada dado

  xFilaDistance = xQueueCreate(
                    5,
                    sizeof(float));

  //Gerando então as tasks;
  if (xFilaMov != NULL && xFilaDistance != NULL && xFilaPosition != NULL) {
    returnSensor = xTaskCreate(
                     vTaskSensor //Tarefa que irá chamar
                     , "TaskSensor" //Nome do identificador para debug
                     , configMINIMAL_STACK_SIZE + 1024 //Memória reservada para essa tarefa
                     , NULL //Parâmetro de inicialização da tarefa;
                     , 2 //Prioridade da tarefa
                     , &vTaskSensorHandle); //Objeto identificador para que o FreeRTOS o reconheça

    if (returnSensor != pdTRUE) {
      Serial.println("[BMO]: Não foi possível gerar a tarefa do sensor.");
      while (1);
    }
    returnBrain = xTaskCreate(
                    vTaskBrain //Tarefa que irá ser executada
                    , "TaskCerebro" //Nome para debug
                    , configMINIMAL_STACK_SIZE + 1024 //Memória reservada
                    , NULL //Parâmetro de inicialização da tarefa
                    , 3 //Prioridade da tarefa
                    , &vTaskBrainHandle); //Objeto identificador (Handle)

    if (returnBrain != pdTRUE) {
      Serial.println("[BMO]: Não foi possível gerar a tarefa BRAIN.");
      while (1);
    }

    returnMotor = xTaskCreate(
                    vTaskMotor //Tarefa que será executada
                    , "TaskMotor" //Nome para debug
                    , configMINIMAL_STACK_SIZE + 1024//Memória reservada
                    , NULL //Parâmetro de inicialização da tarefa
                    , 2 //Prioridad da tarefa
                    , &vTaskMotorHandle); //Objeto para manipular a tarefa

    if (returnMotor != pdTRUE) {
      Serial.println("[BMO]: Não foi possível gerar a tarefa do motor.");
      while (1);
    }
    returnOnOFF = xTaskCreate(
                    vTaskOnOFF //Tarefa que será executada
                    , "TaskOnOFF" //Nome para debug
                    , configMINIMAL_STACK_SIZE + 1024 //Memória reserva
                    , NULL //Parâmetro de inicialização da tarefa
                    , 4 //Priodiade da tarefa
                    , &vTaskOnOFFhandle); //Objeto para manipular a tarefa
    if (returnOnOFF != pdTRUE) {
      Serial.println("[BMO]:Não foi possível gerar a tarefa do motor.");
      while (1);
    }
  }
}
