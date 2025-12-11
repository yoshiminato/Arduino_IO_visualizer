#include "override.h"
#include "pin_state.h"
#include "error.h"
#include <stdio.h>

double scale = 1.0 / 100;
void setup();
void loop();

int main(int argc, char *argv[]) {
    for(int i=0; i<argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    init();
    setup();
    if (argc > 1)
        loadInputPinsStateFromFile(argv[1]);
    while(!end_sim_flag) {
        loop();
    }

    for(int i = 0; i < pin_count; i++) {
        PinState* state = &pin_states[i];
        int pin = state->number;
        printf("Pin %-3d : ", pin);
        for(int j = 0; j <= state->state_count; j++) {
            State* s = &state->log[j];
            int duration = s->duration;
            double value = s->value;
            // printf("sc=%d duration=%d, value=%.1f \n", j, duration, value);
            if(s->value == 1)
                for(int k=0; k<duration*scale; k++)
                    printf("*");
            else 
                for(int k=0; k<duration*scale; k++)
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
