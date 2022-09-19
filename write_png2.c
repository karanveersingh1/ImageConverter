#include<stdio.h>
#include<string.h>
#pragma pack (1)

struct signature{
	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned char d;
	unsigned char e;
	unsigned char f;
	unsigned char g;
	unsigned char h;
}sign;

struct header{
	unsigned char i;
	unsigned char j;
	unsigned char k;
	unsigned char l;
	unsigned char m;
	unsigned char n;
	unsigned char o;
	unsigned char p;
}head;

struct ihdr_info{
	int width;
	int height;
	unsigned char b_d;
	unsigned char color_type;
	unsigned char c_m;
	unsigned char f_m;
	unsigned char i_m;
}ihdr;

int main()
{
	FILE *t = fopen("raw_data","r");
	FILE *fp = fopen("output.png","wb");
	sign.a = 137;
	sign.b = 80;
	sign.c = 78;
	sign.d = 71;
	sign.e = 13;
	sign.f = 10;
	sign.g = 26;
	sign.h = 10;
	head.i = 0;
	head.j = 0;
	head.k = 0;
	head.l = 13;
	head.m = 73;
	head.n = 72;
	head.o = 68;
	head.p = 82;
	ihdr.b_d = 8;
	ihdr.color_type = 0;
	ihdr.c_m = 0;
	ihdr.f_m = 0;
	ihdr.i_m = 0;
	fwrite(&sign,sizeof(struct signature),1,fp);
	fwrite(&head,sizeof(struct header),1,fp);
	fwrite(&ihdr,sizeof(struct ihdr_info),1,fp);
	fclose(fp);
	fclose(t);
	return 0;
}
