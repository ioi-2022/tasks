#include "aplusb.h"

int add_two_numbers(int A, int B) {
  if (B == 0) return A;
  return add_two_numbers(A ^ B, (A & B) << 1);
}
