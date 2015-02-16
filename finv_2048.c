#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 2048
#define LOOPS 2000000
#define SRAND 5  // must check 5 LOOPS 2000000
#define KAI 12

#define INFILE "finv_table2048.txt"

typedef union u2f{
  uint32_t utemp;
  float ftemp;
}u2f;

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
  FILE *fp = fopen(INFILE,"r");
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
  if(downto(i,22,0) == 0){
    exp = 254 - downto(i,30,23);
  }else{
    exp = 253 - downto(i,30,23);
  }
  mant = y + ((d * ((1 << KAI) - (downto(i,KAI-1,0)))) >> KAI);
  ans = make_ans(sign,exp,downto(mant,22,0));
  return ans;
}

char *makebinary(){
  char *ret = (char*)malloc(sizeof(char)*50);
  int i;
  for(i=0;i<32;i++){
    int ra=rand()%2;
    if(ra == 1)
      ret[i] = '1';
    else if(ra == 0)
      ret[i] = '0';
  }
  ret[32] = '\n';
  for(i=33;i<50;i++){
    ret[i] = 0;
  }
  char temp[9];
  for(i=0;i<8;i++){
    temp[i] = ret[1+i];
  }
  temp[8] = 0;
  if(strcmp(temp,"00000000") == 0 || strcmp(temp,"11111111") == 0 || strcmp(temp,"11111110") == 0 || strcmp(temp,"11111101") == 0){
    free(ret);
    ret = makebinary();
  }
  return ret;
}

int main(){
  init_tlb();
  u2f temp,temp2;
  if(SRAND == 1){
    srand(time(NULL));
  }
  else if(SRAND != 0){
    srand(SRAND);
  }
  int i;
  int count[17] = {0};
  int distance;
  int max=0,min=0;
  char my_maxstr[50]={0},my_minstr[50]={0};
  char ans_max[50]={0},ans_min[50]={0};
  char quest_max[50]={0},quest_min[50]={0};
  for(i=0;i<LOOPS;i++){
    char *random = makebinary();
    uint32_t ux = binarytouint(random);
    temp.utemp = ux;
    uint32_t finv_answer = temp2.utemp = finv(ux);
     //  printf("finv     : %s : %f\n",uinttobinary(finv_answer),temp2.ftemp);
    float fx = temp.ftemp;
    temp.ftemp = 1.0 / fx;
    distance = finv_answer - temp.utemp;
    if(distance <= -8){
      count[0]++;
    }
    else if(distance >= 8){
      count[16]++;
    }
    else{
      count[distance+8]++;
    }
    //if(distance > 500 || distance < -500)
    // continue; // exp が0x00または0xffのときおかしくなるので
    if(max < distance){
      max = distance;
      strcpy(my_maxstr,uinttobinary(finv_answer));
      strcpy(ans_max,uinttobinary(temp.utemp));
      strcpy(quest_max,random);
    }
    else if(min > distance){
      min = distance;
      strcpy(my_minstr,uinttobinary(finv_answer));
      strcpy(ans_min,uinttobinary(temp.utemp));
      strcpy(quest_min,random);
    }
    //printf("answer   : %s : %f\n",uinttobinary(temp.utemp),temp.ftemp);
    free(random);
  }
  for(i=0;i<17;i++){
    printf("count[%d] = %d\n",i-8,count[i]);
  }
  printf("指数部が0x00または0xffのとき除外している。\n");
  printf("max = %d\n",max);
  printf("finv : %s\n",my_maxstr);
  printf("ansx : %s\n",ans_max);
  printf("qmax : %s\n\n",quest_max);

  printf("min = %d\n",min);
  printf("finv : %s\n",my_minstr);
  printf("ansn : %s\n",ans_min);
  printf("qmin : %s\n",quest_min);
  return 0;
}
