#include "Keypad.h"
#include "TextLCD.h"
#include "iostream"
#include "mbed.h"
#include "string"
#include <cmath>
#include <cstdio>

using namespace std;

TextLCD lcd(D2, D3, D4, D5, D6, D7, TextLCD::LCD16x2);


PwmOut ledR(LED1);
PwmOut ledG(LED2);
PwmOut ledB(LED3);

void setRGBColor(float red, float green, float blue) {
    ledR = red;
    ledG = green;
    ledB = blue;
}

const int numRows = 4;
const int numCols = 4;

char keyMap[numRows][numCols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

DigitalOut rowPins[numRows] = {DigitalOut(D8), DigitalOut(D9), DigitalOut(D10), DigitalOut(D11)};
DigitalIn colPins[numCols] = {DigitalIn(D12), DigitalIn(D14), DigitalIn(D15), DigitalIn(PTB8)};

// Función para calcular las raíces de un polinomio de grado 2

char inputBufferRaiz[50];
int bufferIndexRaiz = 0;
bool recordingRaiz = false;

int coefficients[3]; // Array para almacenar los coeficientes a, b y c
int coefficientIndex = 0; // Índice para el coeficiente actual

void calculateRoots(int a, int b, int c) {
    int discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {
        // Dos raíces enteras
        int root1 = (-b + sqrt(discriminant)) / (2 * a);
        int root2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("Raices enteras: %d y %d\n", root1, root2);   
        lcd.cls();
        lcd.printf("Raices: %d & %d", root1, root2); // Utilizamos una cadena de formato
        wait_us(3500000);
    } else if (discriminant == 0) {
        // Una raíz real
        int root = -b / (2 * a);
        printf("Raiz entera unica: %d\n", root);
        lcd.cls();
        lcd.printf("Raiz unica: %d", root); // Utilizamos una cadena de formato      
        wait_us(3500000);
    } else {
        // Raíces complejas (no aplicable en este caso)
        printf("El polinomio no tiene raices enteras.\n");
        lcd.cls();
        lcd.printf("No tiene raices"); // Utilizamos una cadena de formato
        lcd.locate(0,1);
        lcd.printf("enteras");
        wait_us(3500000);
    }
}

void processKeyRaiz(char key) {
    if (recordingRaiz) {
        if (key == '*') {
            inputBufferRaiz[bufferIndexRaiz] = '\0';  // Null-terminate the string
            printf("Coeficiente %c: %s\n", 'a' + coefficientIndex, inputBufferRaiz);
            lcd.cls();
            lcd.printf("Coeficiente %c: %s\n", 'a' + coefficientIndex, inputBufferRaiz);
            wait_us(200000);

            // Interpretar '#' como signo negativo al inicio
            int coefficientValue;
            if (inputBufferRaiz[0] == '#') {
                if (sscanf(inputBufferRaiz + 1, "%d", &coefficientValue) == 1) {
                    coefficientValue = -coefficientValue;
                }
            } else {
                if (sscanf(inputBufferRaiz, "%d", &coefficientValue) != 1) {
                    printf("Entrada invalida. Por favor, ingrese un numero valido.\n");
                    lcd.cls();
                    lcd.printf("Entrada invalida");
                    wait_us(500000);
                    bufferIndexRaiz = 0;
                    return;
                }
            }

            coefficients[coefficientIndex] = coefficientValue;
            coefficientIndex++;
            if (coefficientIndex < 3) {
                //lcd.cls();
                //lcd.printf("Ingrese coeficiente: "); 
                printf("Por favor, ingrese el coeficiente %c: ", 'a' + coefficientIndex);
                lcd.cls();
                lcd.printf("Ingrese el coeficiente %c: ", 'a' + coefficientIndex);
                wait_us(500000);
                bufferIndexRaiz = 0;
            } else {
                printf("Coeficientes: a=%d, b=%d, c=%d\n", coefficients[0], coefficients[1], coefficients[2]);
                lcd.cls();
                lcd.printf("Coeficientes:");
                wait_us(200000);
                lcd.locate(0,1);
                lcd.printf("a=%d, b=%d, c=%d\n", coefficients[0], coefficients[1], coefficients[2]);
                wait_us(3500000);
                // Calcula las raíces
                calculateRoots(coefficients[0], coefficients[1], coefficients[2]);
                coefficientIndex = 0;
                recordingRaiz = false;
            }
        } else {
            inputBufferRaiz[bufferIndexRaiz++] = key;
        }
    } else {
        if (key == '*') {
            recordingRaiz = true;
            printf("Por favor, ingrese el coeficiente %c: ", 'a' + coefficientIndex);
            lcd.cls();
            lcd.printf("Ingrese el coeficiente %c: ", 'a' + coefficientIndex);
            wait_us(3500000);
            bufferIndexRaiz = 0;
        }
    }
}


void llamarCalculateRoots(){
    printf("Ingrese los coeficientes del polinomio de grado 2.\n");
    lcd.cls();
    lcd.printf("Ingrese coefts del polinomio");
    wait_us(3500000);
    while (true) {
        for (int i = 0; i < numRows; i++) {
            rowPins[i] = 0;

            for (int j = 0; j < numCols; j++) {
                if (!colPins[j]) {
                    processKeyRaiz(keyMap[i][j]);
                    ThisThread::sleep_for(500ms);  // Evita lecturas múltiples mientras la tecla está presionada
                }
            }

            rowPins[i] = 1;
        }
    }
}


char inputBuffer[50];
int bufferIndex = 0;
bool recording = false;

// Notas ****************************************************************
char convertToGrade(float N) {
    char nota;
    
    if (N >= 0 && N <= 3) {
        nota = 'A';
    } else if (N > 3 && N <= 4) {
        nota = 'B';
    } else if (N > 4 && N <= 5) {
        nota = 'C';
    } else if (N > 5 && N <= 7) {
        nota = 'D';
    } else if (N > 7 && N < 9) {
        nota = 'E';
    } else if (N >= 9 && N <= 10) {
        nota = 'F';
    } else {
        nota = '?';  // Valor fuera de rango
    }
    
    return nota;
}

void processKey(char key) {
    if (key == '*' && !recording) {
        recording = true; // Iniciar la grabación del número
    } else if (key == '*' && recording) {
        if (bufferIndex > 0) {
            inputBuffer[bufferIndex] = '\0';  // Null-terminate the string
            printf("Numero ingresado: %s\n", inputBuffer);
            lcd.cls();
            lcd.printf("Numero ingresado: %s\n", inputBuffer);
            wait_us(3500000);
            
            // Convertir la entrada a un número
            float N = atof(inputBuffer);
            
            if (N >= 0 && N <= 10) {
                char nota = convertToGrade(N);
                printf("Nota alfanumerica: %c\n", nota);
                lcd.cls();
                lcd.printf("Nota alfanumerica: %c\n", nota);
                wait_us(3500000);
            } else {
                printf("Valor de nota no valido: %s\n", inputBuffer);
                lcd.cls();
                lcd.printf("Valor de nota no valido: %s\n", inputBuffer);
                wait_us(3500000);
            }
            
            bufferIndex = 0;
        }
        recording = false; // Detener la grabación después de la conversión
    } else if (recording) {
        inputBuffer[bufferIndex++] = key;
    }
}

void mostrarNotas(){
    while (true) {
            for (int i = 0; i < numRows; i++) {
                rowPins[i] = 0;

                for (int j = 0; j < numCols; j++) {
                    if (!colPins[j]) {
                        processKey(keyMap[i][j]);
                        ThisThread::sleep_for(500ms);  // Evita lecturas múltiples mientras la tecla está presionada
                    }
                }

                rowPins[i] = 1;
            }
        }
}

// Función para mostrar colores
void showColors() {
    char inputBuffer[32];
    int bufferIndex = 0;
    unsigned int numerosDecimales[3];
    int numeroActual = 0;

    while (numeroActual < 3) {
        for (int i = 0; i < numRows; i++) {
            rowPins[i] = 0;

            for (int j = 0; j < numCols; j++) {
                if (!colPins[j]) {
                    char keyPressed = keyMap[i][j];

                    if (keyPressed == '*') {
                        inputBuffer[bufferIndex] = '\0';

                        sscanf(inputBuffer, "%u", &numerosDecimales[numeroActual]);
                        printf("Numero %d ingresado: %u\n", numeroActual + 1, numerosDecimales[numeroActual]);
                        lcd.cls();
                        lcd.printf("Numero %d ingresado: %u\n", numeroActual + 1, numerosDecimales[numeroActual]);
                        wait_us(500000);

                        memset(inputBuffer, 0, sizeof(inputBuffer));
                        bufferIndex = 0;

                        numeroActual++;

                        if (numeroActual >= 3) {
                            break;
                        }
                    } else {
                        inputBuffer[bufferIndex] = keyPressed;
                        bufferIndex++;
                    }

                    ThisThread::sleep_for(500ms);
                }
            }

            if (numeroActual >= 3) {
                break;
            }

            rowPins[i] = 1;
        }
    }

    float red = 1 - (float)(numerosDecimales[0] / 255.0f);
    float green = 1 - (float)(numerosDecimales[1] / 255.0f);
    float blue = 1 - (float)(numerosDecimales[2] / 255.0f);

    setRGBColor(red, green, blue);
}


int darEleccion(){
    int eleccion = -1;

    // Leer la elección del usuario desde el teclado de membrana
    while (eleccion < 0 || eleccion > 3) {
        for (int i = 0; i < numRows; i++) {
            rowPins[i] = 0;

            for (int j = 0; j < numCols; j++) {
                if (!colPins[j]) {
                    char keyPressed = keyMap[i][j];

                    if (keyPressed >= '0' && keyPressed <= '3') {
                        eleccion = keyPressed - '0';
                    } else if (keyPressed == '*') {
                        break;
                    }

                    ThisThread::sleep_for(500ms);
                }
            }

            if (eleccion >= 0 && eleccion <= 3) {
                break;
            }

            rowPins[i] = 1;
        }        
    }
    return eleccion;
}

void darMensaje(){    
    lcd.printf("Bienvenido profe Juan, Arq Hard");
    wait_us(2000000);
    lcd.cls();
    lcd.printf("Elija una opcion y presione *");
    wait_us(2000000);
    lcd.cls();    
    lcd.printf("(1) Raices de un polinomio");
    wait_us(2000000);
    lcd.cls();
    lcd.printf("(2) Mostrar colores");
    wait_us(2000000);
    lcd.cls();
    lcd.printf("(3) Mostrar notas");    
}


int main() {
    darMensaje();
    
    // printf("Elija una opcion: ");

    int eleccion = darEleccion();

    switch (eleccion) {
    case 1:
        llamarCalculateRoots();
        break;
    case 2:
        showColors();
        break;
    case 3:
        mostrarNotas();
        break;
    default:
        printf("Programa terminado.\n");
        lcd.cls();
        lcd.printf("Programa terminado.");
        wait_us(3500000);
        break;
    }
    
    printf("Programa terminado.\n");
    lcd.cls();
    lcd.printf("Programa terminado.");
    wait_us(3500000);
    
    return 0;
}