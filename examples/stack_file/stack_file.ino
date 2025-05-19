#include <Arduino.h>
#include <LittleFS.h>
#include <stack/stack.h>

void setup() {
    Serial.begin(115200);
    LittleFS.begin();
    Serial.println();

    // запись в файл
    gtl::stack_uniq<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    // st.pushList(2, 3, 4);

    for (int i = 0; i < st.length(); i++) {
        Serial.print(st[i]);
        Serial.print(',');
    }
    Serial.println();

    st.writeToFile(LittleFS, "/stack.st");

    // чтение из файла в другой стек
    gtl::stack_uniq<int> st2;

    st2.readFromFile(LittleFS, "/stack.st");
    for (int i = 0; i < st2.length(); i++) {
        Serial.print(st2[i]);
        Serial.print(',');
    }
    Serial.println();
}

void loop() {
}
