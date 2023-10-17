#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int is_happy_number(int num)
{
      for (int i = 0; i < 1000; i++)
      {
            int next = 0;
            while (num > 0)
            {
                  int div = ((int) pow(10, (int) log10(num)));
                  int digit = num / div;
                  num -= digit * div;

                  next += digit * digit;
            }
            if (next == 1)
                  return i + 1;
            num = next;
      }
      return 0;
}

int main(int argc, const char** argv)
{
      int limit = 1000;
      if (argc > 1)
            limit = (int)strtol(argv[1], (char **)NULL, 10);

      for (int i = 0; i < limit; i++)
      {
            int happyNum = is_happy_number(i);
            if (happyNum)
                  printf("%i: %i\n", i, happyNum);
      }

      return 0;
}