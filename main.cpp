#include "Keypad.h"
#include "TextLCD.h"
#include "iostream"
#include "mbed.h"
#include "string"


using namespace std;

TextLCD lcd(D2, D3, D4, D5, D6, D7, TextLCD::LCD16x2);
Keypad teclado(D8, D9, D10, D11, D12, D13, D14, D15);

// PUNTO 1: Raices de un polinomio

// int raices() {
//   int contador = 0;
//   float numeros[3];
//   string numero;

//   while (true) {
//     for (int i = 0; i < numRows; i++) {
//       rowPins[i] = 0;

//       for (int j = 0; j < numCols; j++) {
//         if (colPins[j] == 0) {
//           numero += keyMap[i][j];
//           ThisThread::sleep_for(500ms);
//         }
//       }
//       rowPins[i] = 1;
//     }

//     if (numero.back() == '#') {
//       numero.pop_back();
//       numeros[contador] = stof(numero);
//       cout << "Numero ingresado: " << numeros[contador] << endl;
//       contador++;
//       numero = "";

//       if (contador == 3)
//         break;
//     }
//   }

//   float a = (numeros[0]);
//   float b = (numeros[1]);
//   float c = (numeros[2]);

//   float determinante = pow(b, 2) - 4 * a * c;

//   if (determinante < 0) {
//     cout << "No existe una solución en los números reales" << endl;
//     return 0;
//   }

//   float x1, x2;

//   x1 = (-b + sqrt(determinante)) / (2 * a);
//   x2 = (-b - sqrt(determinante)) / (2 * a);

//   cout << "La solucion de la ecuación es: x1 = " << x1 << endl;
//   cout << "La solucion de la ecuación es: x2 = " << x2 << endl;

//   return 0;
// }

int main() {
  char key;
  int released = 1;
//   string pressedKey = "putos sean los costeños";
//   lcd.printf(pressedKey);
  while (true) {
    key = teclado.ReadKey();

    if (key == '\0')
      released = 1;

    if ((key != '\0') && (released == 1)) {

      lcd.printf("%c", key);
      wait_us(100);
      released = 0;
    }
  }
}
