/* ピン設定 */
void  pinMode(int pin, int mode);

/* ピン制御 */
void  digitalWrite(int pin, int value);
int   digitalRead(int pin);
void  analogWrite(int pin, int value);
float analogRead(int pin);

/* 時間制御 */
float millis();
void  delay(int ms);
void  delayMicroseconds(int us);

