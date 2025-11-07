const char* tramas[] = {
    "L,H",
    "L,O",
    "L,L",
    "M,2",
    "L,A",
    "L, ",
    "L,W",
    "M,-2",
    "L,O",
    "L,R",
    "L,L",
    "L,D",
    nullptr
};

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    delay(1000);
}

void loop() {
    for (int i = 0; tramas[i] != nullptr; i++) {
        Serial.println(tramas[i]);
        delay(1000);
    }
    delay(5000);
}