#define WET 400
#define DRY 900
#define volumePin A0
#define MOS 5
#define PERIOD 259200
#define WORK 60


uint32_t mainTimer, myTimer;
boolean state = false;
void setup() {
  pinMode(volumePin, INPUT);
  Serial.begin(9600);
  pinMode(MOS, OUTPUT);
  digitalWrite(MOS, LOW);
}


void loop() { 
  Serial.println(analogRead(volumePin));
  delay(100);
  mainTimer++;
    if (!state) {                           // если помпа не включена
    if ((long)mainTimer - myTimer > PERIOD) {   // таймер периода
      myTimer = mainTimer;                // сброс таймера
      state = true;                       // флаг на запуск
      pinMode(MOS, OUTPUT);               // пин как выход
      digitalWrite(MOS, HIGH);            // врубить
    }
  } else {                                // если помпа включена
    if (((long)mainTimer - myTimer > WORK) || (analogRead(volumePin) < DRY)) {
      myTimer = mainTimer;                // сброс
      state = false;                      // флаг на выкл
      digitalWrite(MOS, LOW);             // вырубить
      pinMode(MOS, INPUT);                // пин как вход (экономия энергии)
    }
  }
}
