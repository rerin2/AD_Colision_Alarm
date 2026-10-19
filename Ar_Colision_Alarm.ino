#define TRIG A4 
#define ECHO A5 

// RGB LED 핀 번호를 배열로 관리 (0: Red, 1: Green, 2: Blue)
const int rgbPins[3] = {11, 5, 3};

#define MIN_DIST 5    // 최소 기준 거리 
#define MAX_DIST 150  // 최대 기준 거리

#define BUZZER 13     // 부저 핀
#define VOLUME_R A1   // 볼륨 조절용 가변저항 핀
#define WARN_DIST 30  // 이 거리(cm) 이하이면 경고음

void setup() {
  pinMode(TRIG, OUTPUT); // 초음파 송신 핀 출력 모드
  pinMode(ECHO, INPUT);  // 초음파 수신 핀 입력 모드
  pinMode(BUZZER, OUTPUT);  // 부저 핀 출력 모드
  pinMode(VOLUME_R, INPUT); // 가변저항 핀 입력 모드

  // 배열과 반복문을 사용하여 RGB LED 핀들을 모두 출력 모드로 설정
  for (int i = 0; i < 3; i++) {
    pinMode(rgbPins[i], OUTPUT);
  }

  Serial.begin(9600); // 시리얼 모니터 통신 속도 설정 (9600bps)
}

void loop() {
  // 초음파 센서 거리 
  int distance = readDistance();

  // RGB 밝기 값 (0: Red, 1: Green, 2: Blue)
  int rgbValues[3] = {0, 0, 0};

  // 거리 값에 따른 RGB 색상 계산
  calculateColor(distance, rgbValues);

  // 계산된 RGB 값으로 LED 출력
  for (int i = 0; i < 3; i++) {
    analogWrite(rgbPins[i], rgbValues[i]);
  }

  // 가변저항 값 읽기 (0 ~ 1023)
  int potValue = analogRead(VOLUME_R);

  // 시리얼 모니터
  printSerialData(distance, rgbValues, potValue);

  delay(100); // 0.1초 대기
}

// 초음파 센서로 거리를 측정하여 cm로 반환
int readDistance() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // 왕복 시간을 cm 거리로 변환 (마이크로초 * 0.017)
  int dist = pulseIn(ECHO, HIGH) * 0.017;
  return dist;
}

// 거리에 따라 RGB 조정
void calculateColor(int distance, int rgb[]) {
  // 거리 값을 5cm ~ 150cm 범위 안으로 제한
  int clampedDist = constrain(distance, MIN_DIST, MAX_DIST);

  if (clampedDist <= 75) {
    // [5cm ~ 75cm] 빨강 -> 주황 -> 노랑 -> 초록
    rgb[0] = map(clampedDist, 5, 75, 255, 0);    // Red 감소
    rgb[1] = map(clampedDist, 5, 75, 0, 255);    // Green 증가
    rgb[2] = 0;                                  // Blue 끎
  }
  else {
    // [76cm ~ 150cm] 초록 -> 청록 -> 은은한 파스텔 하늘/흰색
    rgb[0] = map(clampedDist, 75, 150, 0, 150);   // Red 부드럽게 증가
    rgb[1] = map(clampedDist, 75, 150, 255, 200); // Green 유지하며 약간 조정
    rgb[2] = map(clampedDist, 75, 150, 0, 255);   // Blue 증가
  }
}

// 시리얼 모니터에 거리, RGB 밝기 값, 가변저항 값을 출력하는 함수
void printSerialData(int distance, int rgb[], int potValue) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | R: ");
  Serial.print(rgb[0]); // Red 값 출력
  Serial.print(" G: ");
  Serial.print(rgb[1]); // Green 값 출력
  Serial.print(" B: ");
  Serial.print(rgb[2]);   // Blue 값 출력
  Serial.print(" | Vol: ");
  Serial.println(potValue); // 가변저항 값 출력 후 줄바꿈
}