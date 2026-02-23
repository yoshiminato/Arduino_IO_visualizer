#include "input.h"

int input_buttons[] = {
    KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
};

void* makeInputThread(void* arg) {

    while(end_sim_flag == 0) {

        int input_count = 0;

        for (int i = 0; i < pin_count; i++) {

            PinState* pin_state = &pin_states[i];
            int mode = pin_state->mode;

            if (mode == OUTPUT) continue;

            if(IsKeyDown(input_buttons[input_count])) {
                if(mode == INPUT_PULLUP)
                    updatePinState(pin_state->number, LOW);
                else
                    updatePinState(pin_state->number, HIGH);
            } else {
                if(mode == INPUT_PULLUP)
                    updatePinState(pin_state->number, HIGH);
                else
                    updatePinState(pin_state->number, LOW);
            }
            input_count++;
        }
    }
    return NULL;
}