#include "override.h"
#include "pin_state.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double scale = 1.0 / 100000;
void setup();
void loop();

int main(int argc, char *argv[]) {
    end_simulation_time_us = 10 * pow(10, 6);
    init();
    setup();
    if (argc > 1)
        loadInputPinsStateFromFile(argv[1]);
    if (argc == 3)
        end_simulation_time_us = atoll(argv[2]) * pow(10, 6);
        
    while(!end_sim_flag){
        loop();
        if(getSimulationTimeus() >= end_simulation_time_us) 
            break;
    }
    cleanupSimulation();
    for(int i = 0; i < pin_count; i++) {
        PinState* state = &pin_states[i];
        int pin = state->number;
        printf("%-3d : ", pin);
        long long total_duration = 0;
        for(int j = 0; j <= state->state_count; j++) {
            State* s = &state->log[j];
            long long duration = s->duration;
            total_duration += duration;
            double value = s->value;
            if(total_duration > end_simulation_time_us)
                duration -= (total_duration - end_simulation_time_us);
            if(s->value == 1)
                for(long long k=0; k<(duration*scale - 0.5); k++)
                    printf("*");
            else 
                for(long long k=0; k<(duration*scale - 0.5); k++)
                    printf(" ");
        }
    printf("\n");
    }
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
