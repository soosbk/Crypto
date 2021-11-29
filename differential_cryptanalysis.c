#include<stdio.h>
#include<stdlib.h>
#include<time.h>




unsigned int re_ip(unsigned int ciphertext)
{
	unsigned char Perm[32] =
	{
	   6,9,12,3,2,5,8,31,30,1,4,27,26,29,0,23,
	   22,25,28,19,18,21,24,15,14,17,20,11,10,13,16,7
	};
	unsigned int num = 0x00;
	for (int i = 0; i < 32; i++)
	{
		num = (num | ((ciphertext >> i) & 0x1) << (Perm[31 - i]));
	}

	return num;

}








unsigned int s(unsigned int cipher)  //2459
{
	unsigned int s1[16] =
	{
		0xd, 0x0, 0x8, 0x6, 0x2, 0xc, 0x4, 0xb, 0xe, 0x7, 0x1, 0xa, 0x3, 0x9, 0xf, 0x5
	};
	unsigned int keep[4] = { 0x00, };
	unsigned int num = 0x00;
	for (int i = 0; i < 4; i++)
	{
		keep[i] = cipher >> (4 * i);  //2459 
		keep[i] = keep[i] & 0x0000000f;//9
		keep[i] = s1[(keep[i])];  //7
		keep[i] = keep[i] << (4 * i);
		num = num | (keep[i]);
	}
	
	//\printf("%d %d\n", cipher, s1[cipher]);
	return num;
}

void print_count(unsigned int count[][16]) {

	for (int s = 0; s <4; s++)
	{
		printf("%d box\t", s);
	}
	printf("\n");
	for (int s = 0; s < 16; s++)
	{
		for (int t = 0; t <4; t++)
		{
			printf("[%d] %d\t", s, count[t][s]);
		}
		printf("\n");
	}

}

void selection(unsigned int count[], unsigned int num) {
	int key = 0x00;
	int k = 0x00;

	for (unsigned int j = 1; j < num; j++)
	{
		key = count[j];
		for (k = j - 1; k >= 0 && count[k] > key; k--)
		{
			count[k + 1] = count[k];
		}
		count[k + 1] = key;
	}


	printf("%d %d %d\n", count[num - 1], count[num - 2], count[num - 3]);

}

int key_sum(unsigned int key[], unsigned int count[][16])
{
	unsigned int n1 = 0x00;
	unsigned int n2 = 0x00;
	unsigned int n3 = 0x00;
	unsigned int n4 = 0x00;
	unsigned int c = 0x00;
	for (unsigned int s = 0; s < 65536; s++)
	{
		n1 = (s >> 12) & 0x0f;
		n2 = (s >> 8) & 0x0f;
		n3 = (s >> 4) & 0x0f;
		n4 = s & 0x0f;
		key[s] = count[0][n1] + count[1][n2] + count[2][n3] + count[3][n4];
		//printf("[%04x] %d\n", s, key[s]);

	}
	int max = key[0];
	for (unsigned int a = 0; a < 65536; a++)
	{
		if (max < key[a])
			max = key[a];

	}
	for (unsigned int b = 0; b < 65536; b++)
	{
		if (max == key[b])
		{
			c += 1;
			printf("[%04x]%d\n", b, key[b]);
		}
	}


	selection(key, 65536);

}

void sum(unsigned int key[]) {
	unsigned int c = 0x00;

	unsigned int max = key[0];
	for (unsigned int a = 0; a < 65536; a++)
	{
		if (max < key[a])
			max = key[a];

	}
	for (unsigned int b = 0; b < 65536; b++)
	{
		if (max == key[b])
		{
			c += 1;
			printf("[%04x]%d\n", b, key[b]);
		}
	}

	printf("num: %d\n", c);
	//selection(key, 65536);
}


int main()
{
	unsigned int* text1;
	unsigned int* text2;
	text1 = (unsigned int*)malloc(sizeof(int) * 2048);  //2048
	text2 = (unsigned int*)malloc(sizeof(int) * 2048);  //2048
	//pt_made(plaintext);
	srand((unsigned int)time(NULL));
	unsigned int t = 0x01000000;

	for (int i = 0; i < 2048; i++)  //2048
	{
		text1[i] = ((((unsigned int)rand() << 16) | rand())) % 4294967296;
		//text1[i] = (((unsigned int)rand() << 16) | rand());
		text2[i] = (text1[i]) ^ t;
	}
	FILE* pt = fopen("plainttext.txt", "w");

	for (int j = 0; j < 2048; j++)  //2048
	{
		fprintf(pt, "%08x\n", text1[j]);
		//printf("%08x\n", text1[j]);
		fprintf(pt, "%08x\n", text2[j]);
		//printf("[%d]%08x\n",j, text2[j]);
	}

	FILE * ct = fopen("ciphertext.txt", "r");
	for (int t = 0; t <4096; t++) //4096
	{
		if (t % 2 == 0)
		{
			fscanf(ct, "%08x", &text1[t / 2]);
			//printf("%08x\n", text1[t/2]);
		}
		else if (t % 2 == 1)
		{
			fscanf(ct, "%08x", &text2[t / 2]);
			//printf("[%d]%08x\n",t, text2[t/2]);
		}
	}
	//ct_made(ciphertext);
	unsigned int y1 = 0x00;
	unsigned int y2 = 0x00;
	unsigned int v1 = 0x00;
	unsigned int v2 = 0x00;
	unsigned int u1 = 0x00;
	unsigned int u2 = 0x00;
	unsigned int xor = 0x00;
	unsigned int num = 0x00001842;
	//unsigned int num = 0x00008124;
	//unsigned int num = 6210;
	//unsigned int num = 0x00002481;
	unsigned int countup = 0;
	//unsigned int keep = 0x00;
	int c = 0x00;
	unsigned int key[65536] = { 0x00, };
	
	for (int i = 0; i < 2048; i++)  //2048
	{
		//printf("%08x\n", text1[i]);
		text1[i] = re_ip(text1[i]);
		//printf("%08x\n", text1[i]);
		text2[i] = re_ip(text2[i]);
		if (((text1[i]) & 0xffff0000) != ((text2[i]) & 0xffff0000))
			continue;
		else
			countup += 1;
		int u = 2;
		y1 = text1[i] & 0x0000ffff;
		
		//printf("%04x\n", y1);
		y2 = text2[i] & 0x0000ffff;
		
		for (int j = 0; j < 65536; j++)
		{
			v1 = y1 ^ j;
			v2 = y2 ^ j;
			//printf("%04x\n", v1);
			
			u1 = s(v1);
			//printf("%04x\n", u1);
			u2 = s(v2);
			
			xor = u1 ^ u2;
			xor = xor &(0x0000ffff);
			//printf("%04x\n", xor);
			if (xor == num)
			{
				//printf("%08x", xor);
				//printf("\n%08x\n\n\n", j);
				key[j]+=1;
			}
		}

		
	}
	printf("countup: %d\n", countup);
	sum(key); 
	free(text1);
	free(text2);
}
