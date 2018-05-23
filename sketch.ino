#define START_RGB_PIN   9
#define GC_PIN      3
#define BUFFER_SIZE 128

char* buffer;

void setup() {
  buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
  int i;
  for(i = START_RGB_PIN; i < START_RGB_PIN+3; i++) {
    analogWrite(i, 0);
  }
  analogWrite(GC_PIN, LOW);
  Serial.begin(9600);
}

int next_ws(char* s) {
  char* p = s;
  for(; *p != 0 && *p != ' ' && *p != '\n' && *p != '\t'; p++);
  if(*p) return p - s;
  return -1;
}

void loop() {
  if(Serial.available() > 0) {
    int n = Serial.readBytesUntil('\n', buffer, BUFFER_SIZE-1);
    if(n <= 0) return;
    buffer[n] = 0;
    Serial.print(buffer);

    // static color, currently all that is supported
    if(buffer[0] == '0') {
      buffer += 2;
      int i;
      unsigned char vals[3];
      sscanf(buffer, "%hhu %hhu %hhu", &vals[0], &vals[1], &vals[2]);
      for(i = START_RGB_PIN; i < START_RGB_PIN + 3; i++) {
        analogWrite(i, vals[i-START_RGB_PIN]);
      }
    } else if(strcmp(buffer, "on") == 0) {
        analogWrite(GC_PIN, HIGH);
    } else if(strcmp(buffer, "off") == 0) {
        analogWrite(GC_PIN, LOW);
    }
  }
}