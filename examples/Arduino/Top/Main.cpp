#include <examples/Arduino/Top/Components.hpp>
#include <Arduino.h>
int main(int argc, char* argv[]) {
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    constructApp();
    return 0;
}
