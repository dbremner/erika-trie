#include <iostream>
#include "erika_defs.h"
using namespace std;
using namespace erika;

int main(int argc, char **argv) {
  try {
    int    label;
    ullong degree;
    int    is_tail;
    int    is_value;
    while (cin >> label) {
      if (cin >> degree && cin >> is_tail) {
        if (is_tail) {
          string tail = "";
          int tail_label;
          while (1) {
            cin >> tail_label;
            if (tail_label == 0) { break; }
            tail += uc(tail_label);
          }
          reverse(tail);
          cout << tail << endl;

          cin >> is_value;
          if (is_value) {
            int value_label;
            while (1) {
              cin >> value_label;
              if (value_label == 0) { break; }
            }
          }
        }
      }
    }
  } catch (const char *err) {
    cerr << err << endl;
    return 1;
  }
  return 0;
}

