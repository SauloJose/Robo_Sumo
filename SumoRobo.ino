/***************************************************************************************************
   PROGRAMA PRINCIPAL DO PROJETO DE ROBÔ SUMÔ - IFAL PALMEIRA - GRUPO DE ROBÓTICA - VERSÃO 1.0
   Instituto Federal de Alagoas (IFAL) - Campus Palmeira dos Índios.
   Equipe: Alan, Italo, Saulo, Wellington.
   Objetivo: O robô em questão irá batalhar em um ringue de batalha de robô sumo (classificação 500g)
   no qual deverá agir e batalhar de forma autônoma, sem ajuda de terceiros.
   Ultima modificação: 
   placa: UNO.
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
****************************************************************************************************/

 //Incluindo bibliotecas
#include "sumoInfo.h"
#include <Arduino_FreeRTOS.h>
#include <queue.h>

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
volatile bool onRobot = HIGH;


//Iniciando dados importantes do robô
void setup() {
  //Inicializando comunicação Serial
  Serial.begin(9600);
  Serial.println("[BMO]: Iniciando configurações....");
  initPins();//Configura as portas que serão utilizadas.
  initFreeRTOS();//Inicializando o sistema operacional
  //A partir daqui as tarefas já estão rodando.
}

//Deletando função looping
void loop() {
  vTaskDelete(NULL); //Deletando a taskLoop, já que não será necessário.
}


/*============================|| DEFINIÇÃO DAS FUNÇÕES DE INICIALIZAÇÃO||==============================*/
//Procedimento para iniciar o FreeRTOS com as tarefas, depois disso, o trabalho é só das tarefas
//Sequência de procedimentos.
//Cria filas -> Cria tarefa de sensores -> Cria tarefa cérebro -> Cria tarefa motor.
void initFreeRTOS() {
  Serial.println("[BMO]: Iniciando FreeRTOS.");
  BaseType_t returnSensor, returnBrain, returnMotor, returnOnOFF;

  //Gerando filas necessárias.
  //Fila para movimentação do robÔ
  xFilaMov = xQueueCreate(
               2,//Quantidade de comandos enviados;
               sizeof(uint8_t));//Tamanho do comando enviado;

  //Fila para receber dados do sensor
  xFilaPosition = xQueueCreate(
                    2,//Quantidade de dados enviados
                    sizeof(sensorFE)); //tamanho do dado enviado

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
