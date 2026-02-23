#include "override.h"
#include "pin_state.h"
#include "error.h"
#include "draw.h"
#include "queue.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

/* Thread identifiers.  The structure of the attribute type is not exposed on purpose.  */
typedef unsigned long int pthread_t;

pthread_t draw_thread;
pthread_t publish_thread;
pthread_t input_thread;
double scale = 1.0 / 100000;
void setup();
void loop();

int main(int argc, char *argv[]) {
    end_simulation_time_us = 10 * pow(10, 7);
    init();
    setup();
    if (argc > 1)
        loadInputPinsStateFromFile(argv[1]);
    if (argc == 3)
        end_simulation_time_us = atoll(argv[2]) * pow(10, 6);
        
    InitQueue(dataBuffer);
    pthread_create(&draw_thread, NULL, draw, NULL);
    pthread_create(&publish_thread, NULL, publishThreadFunction, NULL);
    pthread_create(&input_thread, NULL, makeInputThread, NULL);
    while(!end_sim_flag){
        loop();
        if(getSimulationTimeus() >= end_simulation_time_us){ 
            end_sim_flag = 1;
            break;
        }
    }
    cleanupSimulation();
    pthread_join(draw_thread, NULL);
    pthread_join(publish_thread, NULL);
    pthread_join(input_thread, NULL);
    return 0;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int led = 10;
  int sw = digitalRead(2);
  static int dir = 1;

  int currentDir = (sw == LOW) ? -dir : dir;
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);

  digitalWrite(led, HIGH);
  delay(500);

  led += currentDir;

  if (led > 13) led = 10;
  if (led < 10) led = 13;
}
