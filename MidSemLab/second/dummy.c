#include<stdio.h>
int main(int argc, char const *argv[]) {
  printf("Do you want to receive 50 more bytes (y=1 or n=0) :");
  int c=0;
  scanf("%d",&c);
  printf("%d\n",c);
  return 0;
}
