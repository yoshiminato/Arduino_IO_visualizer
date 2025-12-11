#ifndef ERROR_H
#define ERROR_H

#include "pin_state.h"

void raiseNoPinError(int pin);
void raiseInvalidFuncError(int pin, int mode);
void raiseInvalidModeError(int pin, int mode);
void raiseInvalidPinNumberError(int pin);
void raiseLoadInputFileError(const char* filename);
void raiseFileOpenError(const char* filename);
void raiseStateOverflowError(int pin);

#endif /* ERROR_H */


