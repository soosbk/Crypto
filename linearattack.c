#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>


unsigned int sbox(unsigned int cipher)  //2459
{
	uint8_t static IN_S_box[16] = {
   0x5, 0xe, 0xf, 0x8,
   0xc, 0x1, 0x2, 0xd,
   0xb, 0x4, 0x6, 0x3,
   0x0, 0x7, 0x9, 0xa
	};
	unsigned int keep[4] = { 0x00, };
	unsigned int num = 0x00;
	for (int i = 0; i < 4; i++)
	{
		keep[i] = cipher >> (4 * i);  //2459 
		keep[i] = keep[i] & 0x0000000f;//9
		keep[i] = IN_S_box[(keep[i])];  //7
		keep[i] = keep[i] << (4 * i);
		num = num | (keep[i]);
	}

	
	return num;
}

void sum(int key[]) {
	
	int max = key[0]- 2048;
	if (max < 0)
		max = 0 - max;
	int num = 0x00;
	int keep = 0x00;
	for (unsigned int a = 1; a < 65536; a++)
	{
		keep =key[a]- 2048;
		if (keep < 0)
			keep = 0 - keep;

		if (max <= keep) {
			max = keep;
			num = a;
			printf("%04x, %d,%d\n",a, key[a],keep);
		}
		
	}
	
	
	printf("%04x", num);
	//selection(key, 65536);
}

int main()
{
	unsigned int* text1;
	unsigned int* text2;
	text1 = (unsigned int*)malloc(sizeof(int) * 4096);  //2048
	text2 = (unsigned int*)malloc(sizeof(int) * 4096);
	

	FILE* pt = fopen("PlainText1.txt", "r");
	for (int t = 0; t < 4096; t++) //4096
	{
		fscanf(pt, "%08x", &text1[t]);
		

	}

	FILE* ct = fopen("CipherText1.txt", "r");
	for (int t = 0; t < 4096; t++) //4096
	{
		fscanf(ct, "%08x", &text2[t]);
		

	}
	
	unsigned int y = 0x00;
	
	unsigned int v = 0x00;

	unsigned int u = 0x00;
	
	unsigned int z = 0x00;
	
	unsigned int input_save = 0;
	int key[65536] = { 0x00, };
	unsigned int t = 0x00;
	unsigned int x28 = 0x00;
	unsigned int x29 = 0x00;
	unsigned int x31 = 0x00;
	unsigned int u19 = 0x00;
	unsigned int u20 = 0x00;
	unsigned int u25 = 0x00;
	unsigned int u30 = 0x00;
	for (int i = 0; i < 4096; i++)
	{
		x28 = (text1[i]>>3)&0x01;
		x29 = (text1[i] >> 2) & 0x01;
		x31 = (text1[i]) & 0x01;
		y = text2[i];
		y = y & 0x0000ffff;
		
		for (int j = 0; j < 65536; j++)
		{
			v = y ^ j;
			u = sbox(v);
			u19 = (u >> 12) & 0x01;
			u20 = (u >> 11) & 0x01;
			u25 = (u >> 6) & 0x01;
			u30 = (u >> 1) & 0x01;
			z = x28 ^ x29 ^ x31 ^ u19 ^ u20 ^ u25 ^ u30;
			if (z == 0)
				key[j] = key[j] + 1;

			z = 0;
			v = 0;
			u = 0;
		}
	}
	
	sum(key);
	free(text1);
	free(text2);

}
