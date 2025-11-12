#include <Servo.h>

//Sensor Ultrasonik
#define TRIG_PIN D1
#define ECHO_PIN D2

//Motor DC
#define MOTOR_KIRI_MAJU D3
#define MOTOR_KIRI_MUNDUR D4
#define MOTOR_KANAN_MAJU D5
#define MOTOR_KANAN_MUNDUR D6

#define PWM_KIRI D7
#define PWM_KANAN D8

//Servo
#define SERVO_PIN D0

Servo servoMotor;

//SETUP
void setup() {
  Serial.begin(115200);

  // Motor
  pinMode(MOTOR_KIRI_MAJU, OUTPUT);
  pinMode(MOTOR_KIRI_MUNDUR, OUTPUT);
  pinMode(MOTOR_KANAN_MAJU, OUTPUT);
  pinMode(MOTOR_KANAN_MUNDUR, OUTPUT);
  pinMode(PWM_KIRI, OUTPUT);
  pinMode(PWM_KANAN, OUTPUT);

  // Sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(90);

  Serial.println("Sistem Siap 🚗💨");
}

// LOOP
void loop() {
  long jarak = bacaJarak();
  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");

  analogWrite(PWM_KIRI, 700);   // 0–1023 untuk ESP8266
  analogWrite(PWM_KANAN, 700);

  if (jarak <= 25 && jarak > 0) {
    mundur();
    delay(500);
    berhentiMotor();
    delay(500);

    int arah = pindaiArah();

    if (arah == 45) belokKiri();
    else if (arah == 135) belokKanan();
    else maju();
  } 
  else if (jarak > 25 && jarak < 400) {
    maju();
  } 
  else {
    berhentiMotor();
  }

  delay(200);
}

// Fungsi Sensor
long mikrodetikKeCm(long mikrodetik) {
  return mikrodetik / 29 / 2;
}

long bacaJarak() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long durasi = pulseIn(ECHO_PIN, HIGH, 30000);
  if (durasi == 0) return 500;
  return mikrodetikKeCm(durasi);
}

//Fungsi Motor
void berhentiMotor() {
  digitalWrite(MOTOR_KIRI_MAJU, LOW);
  digitalWrite(MOTOR_KIRI_MUNDUR, LOW);
  digitalWrite(MOTOR_KANAN_MAJU, LOW);
  digitalWrite(MOTOR_KANAN_MUNDUR, LOW);
}

void maju() {
  digitalWrite(MOTOR_KIRI_MAJU, HIGH);
  digitalWrite(MOTOR_KIRI_MUNDUR, LOW);
  digitalWrite(MOTOR_KANAN_MAJU, HIGH);
  digitalWrite(MOTOR_KANAN_MUNDUR, LOW);
}

void mundur() {
  digitalWrite(MOTOR_KIRI_MAJU, LOW);
  digitalWrite(MOTOR_KIRI_MUNDUR, HIGH);
  digitalWrite(MOTOR_KANAN_MAJU, LOW);
  digitalWrite(MOTOR_KANAN_MUNDUR, HIGH);
}

void belokKiri() {
  digitalWrite(MOTOR_KIRI_MAJU, LOW);
  digitalWrite(MOTOR_KIRI_MUNDUR, LOW);
  digitalWrite(MOTOR_KANAN_MAJU, HIGH);
  digitalWrite(MOTOR_KANAN_MUNDUR, LOW);
  delay(500);
}

void belokKanan() {
  digitalWrite(MOTOR_KIRI_MAJU, HIGH);
  digitalWrite(MOTOR_KIRI_MUNDUR, LOW);
  digitalWrite(MOTOR_KANAN_MAJU, LOW);
  digitalWrite(MOTOR_KANAN_MUNDUR, LOW);
  delay(500);
}

//Fungsi Servo Scan
int pindaiArah() {
  int jarakKiri, jarakTengah, jarakKanan;

  servoMotor.write(30);
  delay(300);
  jarakKiri = bacaJarak();

  servoMotor.write(90);
  delay(300);
  jarakTengah = bacaJarak();

  servoMotor.write(150);
  delay(300);
  jarakKanan = bacaJarak();

  servoMotor.write(90);

  if (jarakKiri > jarakTengah && jarakKiri > jarakKanan) return 45;
  if (jarakKanan > jarakTengah && jarakKanan > jarakKiri) return 135;
  return 90;
}
