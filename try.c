#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)

struct read_ico
{
	short int q;
	short int type;
	short int img_num;
	unsigned char wid;
	unsigned char len;
	unsigned char clrcnt;
	unsigned char resrv;
	short int clrplns;
	short int bitppix;
	int size;
	int ofset;
	unsigned int header_size;
	int width;
	int height;
	unsigned short int colorPlanes;
	unsigned short int bitsperpixel;
	unsigned int compMethod;
	unsigned int sizeofRawBitmapData;
	unsigned int horRes;
	unsigned int vertRes;
	unsigned int colorPaltte;
	unsigned int impColors;
};

struct RGBA
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char A;

};

struct RGB
{
	unsigned char A;
};

struct Mask
{
	unsigned char A0;
};



struct IMAGE
{
	unsigned char B1;
	unsigned char G1;
	unsigned char R1;
	unsigned char A1;
};



int open_ico()
{

	FILE *fp = fopen("Screenshot (21).ico", "rb");
	struct read_ico head;
	fread(&head, sizeof(struct read_ico), 1, fp);

	printf("%d\n", head.q);
	printf("%d\n", head.type);
	printf("%d\n", head.img_num);

	printf("%u\n", head.wid);
	printf("%u\n", head.len);
	printf("%u\n", head.clrcnt);
	printf("%u\n", head.resrv);

	printf("%d\n", head.clrplns);
	printf("%d\n", head.bitppix);
	printf("%d\n", head.size);
	printf("%d\n", head.ofset);

	FILE *fpw=fopen("header.txt", "w");
	fwrite(&head, sizeof(struct read_ico), 1, fpw);

	int len = head.len;
	int wid= head.wid;
	if(head.len==0)
	{
		len=256;
	}
	if(head.wid==0)
	{
		wid=256;
	}

	printf("%d\n", len);
	printf("%d\n", wid);

	FILE *fpw1=fopen("raw_data.txt","w");
	
	char** arr;
	arr = (char**)malloc(len*sizeof(void*));
	for (int i =0;i<len;i++)
		arr[i] = (char*)malloc(wid*sizeof(char));

	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			struct RGBA head1;
			fread(&head1, sizeof(struct RGBA),1,fp);

			struct RGB call;
			call.A=0.1*(head1.B)+0.6*(head1.G)+0.3*(head1.R);

			arr[i][j]=call.A;
			//fwrite(&call, sizeof(struct RGB),1,fpw1);
			//printf("%u", head1.B);
			//printf("%u", head1.G);
			//printf("%u", head1.R);
			//printf("%u", head1.A);
			//printf("%u ", call.A);
		}
			//printf("\n");
	}

	for (int i = len-1; i >=0; i--)
	{
		for (int j = 0; j < wid; j++)
		{
			fwrite(&arr[i][j],1,1,fpw1);
		}
	}



	fclose(fp);
//	freeImage(Image);

	return 0;
}


int main()
{
	open_ico();

	return 0;
}


