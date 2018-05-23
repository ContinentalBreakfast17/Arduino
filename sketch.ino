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
    int n = 0;
    while((buffer[n++] = Serial.read()) != '\n');
    if(n >= 128) return;
    buffer[n] = 0;
    Serial.print(buffer);

    // static color, currently all that is supported
    if(buffer[0] == '0') {
      buffer += 2;
      int pin = START_RGB_PIN;
      int i;
      while((i = next_ws(buffer)) > 0 && pin < START_RGB_PIN + 3) {
        int j = i;
        char val_str[i+1];
        while(j--) val_str[j] = buffer[j];
        val_str[i] = 0;
        analogWrite(pin, atoi(val_str));

        buffer += i;
      }
    } else if(strcmp(buffer, "on") == 0) {
        analogWrite(GC_PIN, HIGH);
    } else if(strcmp(buffer, "off") == 0) {
        analogWrite(GC_PIN, LOW);
    }
  }
}