#include<stdio.h> 

/* 
0.  
로테이션(왼쪽) 
*/ 
unsigned int rotation_l(unsigned int num, unsigned int n) 
{ 
	int i = 0x00; 

	return (((num << n) & 0x0fffffff) | (num >> (28 - n))); 

} 



/* 
1. 
초기전치함수 
*/ 
unsigned long long ip(unsigned long long plaintext) 
{ 
	unsigned long long num = 0x0000000000000000; 
	int i; 
	unsigned long long aftpt = 0x00; 
	char ip[64] 
	= { 57,49,41,33,25,17, 9, 1,59,51,43,35,27,19,11, 3,61,53,45,37,29,21,13, 
	5,63,55,47,39,31,23,15, 7,56,48,40,32,24,16, 8, 0,58,50,42,34,26,18,10, 
	2,60,52,44,36,28,20,12, 4,62,54,46,38,30,22,14, 6 }; 

	for (i = 0; i < 64; i++) 
	{ 
		num = plaintext >> (63 - ip[i]); 
		num = num << 63; 
		num = num >> i; 

		aftpt = aftpt | num; 
	} 

	//printf("%llx\n", aftpt); 
	return aftpt; 
} 

/* 

2_1. pc_fun 

*/ 

unsigned long long pc(unsigned long long var, int pc[], int number) { 
	int a; 
	unsigned long long numb = 0x00; 
	unsigned long long aft_ch = 0x00; 

	for (a = 0; a < number; a++) 
	{ 
		if (number == 56) { 
			numb = var >> ((number + 8) - pc[a]); 
            numb = numb << (number + 7); 
            numb = numb >> (a + 8); 
            aft_ch = (aft_ch | numb); 
		} 

        else//number==48 
        { 
            numb = var >> ((number + 8) - pc[a]); 
            numb = numb << (number + 15); 
            numb = numb >> (a + 16); 
            aft_ch = (aft_ch | numb); 
		} 


	} 

	return aft_ch; 
} 


/* 
2. 
key schedule 
*/ 
void key_expansion(unsigned long long key, unsigned long long subkeys[]) 
{ 
  unsigned long long count = 0x00; 
  int pc1[56] = { 
    57, 49, 41, 33, 25, 17,  9, 
     1, 58, 50, 42, 34, 26, 18, 
    10,  2, 59, 51, 43, 35, 27, 
    19, 11,  3, 60, 52, 44, 36, 
    63, 55, 47, 39, 31, 23, 15, 
     7, 62, 54, 46, 38, 30, 22, 
    14,  6, 61, 53, 45, 37, 29, 
    21, 13,  5, 28, 20, 12,  4 
  }; 

  int pc2[48] = { 
    14, 17, 11, 24,  1,  5, 
     3, 28, 15,  6, 21, 10, 
    23, 19, 12,  4, 26,  8, 
    16,  7, 27, 20, 13,  2, 
    41, 52, 31, 37, 47, 55, 
    30, 40, 51, 45, 33, 48, 
    44, 49, 39, 56, 34, 53, 
    46, 42, 50, 36, 29, 32 
  }; 


/* 
unsigned int pc1_1[28] = { 56,48,40,32,24,16,8,0,57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35 }; 
unsigned int pc1_2[28] = { 62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,60,52,44,36,28,20,12,4,27,19,11,3 }; 
*/ 
//printf("%llx", key); 

  unsigned int rotation[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 }; 
  key = pc(key, pc1, 56); 
  printf("this is real key: %llx\n", key); 

  int i; 
  unsigned int c = (unsigned int)(key >> 28); 
  unsigned int d = (unsigned int)(key & 0x0fffffff); 
  printf("this is c: %x\n this is d: %x\n", c, d); 
  for (i = 0; i < 16; i++) 
  { 
    unsigned long long subkey = 0x00; 
    subkey = subkey & 0x00; 

    c = rotation_l(c, rotation[i]); 
    //printf("[%d] c: %llx =&", i,(unsigned long long)c); 
    d = rotation_l(d, rotation[i]); 
    //printf("[%d] d: %llx => ",i,(unsigned long long)d); 
    //subkey = (((unsigned long long)c) << (28 - i)); 
    //subkey = (((unsigned long long)c) << (28-i)) & ((0x00fffffff0000000-i)); 
    subkey = (((unsigned long long)c) << 28) & (0xffffffffffffff00); 
    //printf(" subkey: %llx\n\n", subkey); 


    subkey = (subkey | d); 
    //printf(" 잘 되었나? subkey2: %llx\n\n", subkey); 
    subkeys[i] = pc(subkey, pc2, 48); 
    //printf("[%d] final     %llx\n", i, subkeys[i]); 
	} 


} 

/* 
3. sbox 

*/ 
unsigned int s_box(unsigned long long using) 
{ 
  unsigned int sb[8][4][16] = { 
    { 
      14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7, 
      0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8, 
      4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0, 
      15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 
    }, 
    { 
      15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10, 
       3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5, 
       0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 
      13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9 
    }, 
    { 
      10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8, 
      13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1, 
      13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7, 
       1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12 
    }, 
    { 
       7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15, 
      13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9, 
      10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4, 
       3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14 
    }, 
    { 
       2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 
      14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6, 
       4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14, 
      11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 
    }, 
    { 
      12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11, 
      10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8, 
       9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6, 
       4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 
    }, 
    { 
       4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1, 
      13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6, 
       1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2, 
       6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 
    }, 
    { 
      13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7, 
       1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2, 
       7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8, 
       2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 
    } 
 }; 
  //("1:2:13-> %d", sb[1][2][13]); 
  //unsigned long long using = 0x04789FD476E82; 
  unsigned char rows[8] = { 0x00, }; 
  unsigned char cols[8] = { 0x00, }; 
  unsigned char num = 0x00; 
  int i; 

  for (i = 7; i>=0 ; i--) 
  { 
    num = num & 0x0; 
    //printf("[%d] %x\n",i, (unsigned char)using); 

    num = using; 
    //printf("%x\n", num); 
    rows[i] = num & 0b00100001; 
    //printf("%d\n", rows[i]); 
    rows[i] = (rows[i] & 0b00000001) | ((rows[i] >> 5) << 1); 

    cols[i] = num << 3; 
    cols[i] = cols[i] >> 4; 



    //printf("%d %d\n", rows[i], cols[i]); 
    using = using >> 6; 
	} 

  unsigned int aft = 0x00; 
  unsigned int out = 0x00; 
  int row = 0x00; 
  int col = 0x00; 
  aft = aft & 0x00; 
  //printf("잘 됐나? %x\n", aft); 
  for (i = 0; i < 8; i++) 
  { 
    out = out & 0x00; 
    //printf("%d\n", sb[i][rows[i]][cols[i]]); 
    row = rows[i]; 
    col = cols[i]; 
    out = sb[i][row][col]; 
    //printf("%d\n", out); 
    out = out << (28 - 4 * i); 
    aft = aft | out; 

    //printf("\n %d: %x\n\n",i, aft); 
	} 
  //printf("%x\n", aft); 

  return aft; 
} 



/* 
4. P함수 
*/ 


unsigned int P(unsigned int R) 
{ 
  unsigned int num = 0x00000000; 
  unsigned int aft = 0x00000000; 
  int i; 
  int p[32] 
  = { 
    16,  7, 20, 21, 
    29, 12, 28, 17, 
     1, 15, 23, 26, 
     5, 18, 31, 10, 
     2,  8, 24, 14, 
    32, 27,  3,  9, 
    19, 13, 30,  6, 
    22, 11,  4, 25 
  }; 

  //printf("%d\n", aft); 

  for (i = 0; i < 32; i++) 
	{ 
      num = R >> (32 - p[i]); 
      num = num << 31; 
      num = num >> i; 

      aft = aft | num; 

      //printf("[%d]%x\n",i, aft); 
	} 

    //printf("%llx\n", aftpt); 
    return aft; 
} 

/* 
5. f 함수 

*/ 
unsigned int f(unsigned int right,unsigned long long key) 
{ 
  int e[48] = { 31,0,1,2,3,4, 
  3,4,5,6,7,8, 
  7,8,9,10,11,12 
  ,11,12,13,14,15,16, 
  15,16,17,18,19,20, 
  19,20,21,22,23,24, 
  23,24,25,26,27,28, 
  27,28,29,30,31,0 }; 

  unsigned long long using = 0x00; 
  int i; 
  unsigned long long cheat = 0x00; 
  unsigned int output = 0x00; 
  cheat = cheat & 0x0; 
  //16 
  for (i = 0; i < 48; i++) 
  { 
    using = using & 0x0; 
    using = (unsigned long long)(right >> (31 - e[i])); 
    using = using << 63; 
    using = using >> (i + 16); 

    cheat = (cheat | using); 

	} 

  cheat = (cheat ^ key); 


  output = s_box(cheat); 
  cheat = cheat & 0x00; 
  printf("befor outout: %x\n", output); 
  output = P(output); 
  printf("aft output:%x \n", output); 

  return output; 
} 

/* 
6. 역 전치 함수 
*/ 

unsigned long long ip_re(unsigned long long ciphertext) 
{ 
  unsigned long long num = 0x0000000000000000; 
  int i; 
  unsigned long long aftpt = 0x00; 
  char ip_re[64] 
  = { 39,7,47,15,55,23,63,31 
  ,38,6,46,14,54,22,62,30, 
  37,5,45,13,53,21,61,29 
  ,36,4,44,12,52,20,60,28, 
  35,3,43,11,51,19,59,27, 
  34,2,42,10,50,18,58,26, 
  33,1,41,9,49,17,57,25, 
  32,0,40,8,48,16,56,24 }; 

  for (i = 0; i < 64; i++) 
	{ 
      num = ciphertext >> (63 - ip_re[i]); 
      num = num << 63; 
      num = num >> i; 

      aftpt = aftpt | num; 
	} 

  //printf("%llx\n", aftpt); 
  return aftpt; 
} 


/* 
7. xor 함수 
*/ 
int xor(int left, int rig) { 
	return(left ^ rig); 
} 


/* 
8. main 함수 
*/ 
int main() 
{ 
  unsigned long long plaintext = 0x00; 
  //unsigned long long plaintext = 0x123456abcd132536; 
  unsigned long long cipertext = 0x00; 
  unsigned long long key = 0x00; 
  //unsigned long long key = 0xaabb09182736ccdd; 
  unsigned long long subkey[16] = { 0x00, }; 
  printf("Input key: "); 
  scanf("%llx", &plaintext); 
  printf("this is plaintext: %llx\n", plaintext); 
  plaintext = ip(plaintext); 
  printf("this is at ip_plaintext: %llx\n", plaintext); 
  int r = 0; 

  unsigned int pre_left= 0x00; 
  unsigned int left= 0x00; 
  unsigned int right= 0x00; 

  printf("Input key: "); 
  scanf("%llx", &key); 
  printf("this is input key: %llx\n", key); 

  key_expansion(key,subkey); 
  left = (int)(plaintext >> 32); 
  right = (int)((plaintext << 32) >> 32); 
  printf("left: %x,,  right: %x\n", left, right); 

  for (r = 0; r < 16; r++) 
  { 
    //printf("this is subkey [%d]: %llx\n", r, subkey[r]); 
    pre_left = pre_left & 0x00; 
    pre_left = left; 
    left = left & 0x00; 
    left = right; 
    right = (f(right, subkey[r])); 
    right = xor (pre_left, right); 
    //printf("left: %x,,  right: %x", left, right); 
    printf("[%d]round => key: %llx// left: %x,  right:%x\n\n", r, subkey[r], left, right); 

	} 
  pre_left = left; 
  left = right; 
  right = pre_left; 
  printf("last ciper: %x, %x\n", left, right); 
  //printf("%llx", (unsigned long long)right); 
  cipertext = (unsigned int)left; 
  cipertext = cipertext << 32; 
  cipertext = cipertext | right; 
  printf("com: %llx\n", cipertext); 
  cipertext = ip_re(cipertext); 

  printf("%llx", cipertext); 





}
