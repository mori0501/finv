#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define N 2048

extern uint32_t finv(uint32_t i);

uint32_t binarytouint(char *bin){
  uint32_t ret=0;
  uint32_t temp=1u;
  int i=0;
  for(i=0;i<32;i++){
    if(bin[31-i]=='1'){
      ret += temp << i;
    }
  }
  return ret;
}

char *uinttobinary(uint32_t ui){
  char *ret=(char*)malloc(sizeof(char)*50);
  int i;
  for(i=0;i<32;i++){
    if((ui >> i) & 1u){
      ret[31-i]='1';
    }
    else
      ret[31-i]='0';
  }
  ret[32]='\0';
  return ret;
}

char *lluinttobinary(long long unsigned int ui){
  char *ret=(char*)malloc(sizeof(char)*50);
  int i;
  for(i=0;i<36;i++){
    if((ui >> i) & 1u){
      ret[35-i]='1';
    }
    else
      ret[35-i]='0';
  }
  ret[36]='\0';
  return ret;
}


uint32_t downto(uint32_t i,int high,int low){
  int lsh = 31 - high;
  int rsh = lsh + low;
  i <<= lsh;
  i >>= rsh;
  return i;
}

static char tlb[2048][40];

void init_tlb(){
  int i;
  FILE *fp = fopen("finv_table2048.txt","r");
  char str[40];
  for(i=0;i<N;i++){
    fscanf(fp,"%s",str);
    strcpy(tlb[i],str);
  }
  fclose(fp);
}

long long unsigned int binarytoullint(char *bin){
  long long unsigned int ret=0;
  long long unsigned int temp=1;
  int i=0;
  for(i=0;i<36;i++){
    if(bin[35-i]=='1'){
      ret += temp << i;
    }
  }
  return ret;
}

uint32_t yllui2uint(long long unsigned int i){
  return i>>13;
}

uint32_t lowllui2uint(long long unsigned int i){
  i <<= 32;
  i >>= 32;
  return (uint32_t)i;
}

uint32_t make_ans(uint32_t sign,uint32_t exp,uint32_t mant){
  sign <<= 31;
  exp <<= 23;
  return sign | exp | mant;
}

/*
uint32_t finv(uint32_t i){
  int index = downto(i,22,12);
  long long unsigned int yd = binarytoullint(tlb[index]);
  uint32_t ydtemplow,y,ymant,d;
  ymant = yllui2uint(yd);
  y = (1 << 23) | ymant;
  ydtemplow = lowllui2uint(yd);
  d = downto(ydtemplow,12,0);
  uint32_t ans;
  uint32_t sign,exp,mant;
  sign = downto(i,31,31);
  exp = 253 - downto(i,30,23);
  mant = y + ((d * ((1 << 12) - (downto(i,12-1,0)))) >> 12);
  ans = make_ans(sign,exp,downto(mant,22,0));
  return ans;
}
*/

int main(){
  init_tlb();
  FILE *fpr = fopen("testcase.txt","r");
  FILE *fpw = fopen("finv_ans.txt","w");
  char str[50];
  while(fscanf(fpr,"%s",str) != EOF){
    uint32_t i = binarytouint(str);
    uint32_t ans = finv(i);
    char *ptr = uinttobinary(ans);
    fprintf(fpw,"%s\n",ptr);
    free(ptr);
  }
  fclose(fpr);
  fclose(fpw);
  return 0;
}
