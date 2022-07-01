#include "sumoinfo.h"
//Implementação das funções
/*==================================|| DEFINIÇÃO DAS FUNÇÕES ||==============================*/
//Ativar pulso do sensor ultrasônico
float Ultrassonic() {
  digitalWrite(TRIG,LOW);
  vTaskDelay(3/15);
  digitalWrite(TRIG,HIGH);
  vTaskDelay(10/15);
  digitalWrite(TRIG,LOW)

  return pulseIn(ECHO,HIGH);
}

//Converter leitura para distância
float convertCm() {
  long tempo = Ultrassonic();
  float dist;
  tempo=tempo/2;
  dist=tempo*0.034; //Convertendo em cm

  return dist;
}

//Rotina do sensor.
void routineMotor(uint16_t t) {

}

/*====================================|| DEFINIÇÃO DOS TAREFAS ||==============================*/
//Tarefa para tomar decisões
void vTaskBrain(void* pvParamaters) {
  (void*) pvParamaters;
  while (1) {

  }
}

//Tarefa para coletar dados dos sensores
void vTaskSensor(void* pvParamaters) {
  (void*) pvParamaters;

  //Variáveis para enviar ao cérebro.
  float distance;
  bool sensors[4];
  
  while (1) {
    //Analisando dados do sensor ultrassÔnico
  }
}


//Tarefa de ligar e desligar o robô
void vTaskOnOFF(void* pvParamaters) {
  (void*) pvParamaters;
  while (1) {
    //Só faz algo quando um semáforo for liberado por uma interrupção.
  }
}


//Tarefa que irá monitorar o comportamento desejado.
void vTaskMotor(void* pvParamaters) {
  (void*) pvParamaters;
  while (1) {

  }
}


/*=====================================|| DEFINIÇÃO DOS MÉTODOS ||==============================*/

//Métodos da classe motor




//Método da classe robÔ
