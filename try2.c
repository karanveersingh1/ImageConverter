#include <stdio.h>
#pragma pack(1)

struct read_ico
{
	unsigned char q;
	char a[3];
	unsigned char CR;
	unsigned char LF1;
	unsigned char ctrlZ;
	unsigned char LF2;

//^^^^signature	

//vvvv IHDR
	unsigned char buffer[4];   //---lenght

	char c1; //ihdr sig.(chunk type)  
	char c2; //ihdr sig.(chunk type)
	char c3; //ihdr sig.(chunk type)
	char c4; //ihdr sig.(chunk type)

	//int w;

	unsigned char wid[4];
	unsigned char height[4];
	unsigned char bitdepth;
	unsigned char colortype;
	unsigned char compressiontype;
	unsigned char filtermethod; 
	unsigned char interlacetype;

	unsigned char f1;
	unsigned char f2;
	unsigned char f3;
	unsigned char f4;

	int f;

	char d1; //ihdr sig.(chunk type)  
	char d2; //ihdr sig.(chunk type)
	char d3; //ihdr sig.(chunk type)
	char d4; //ihdr sig.(chunk type)

};

int open_ico()
{

	FILE *fp = fopen("Grayscale_8bits_sample_image.png", "rb");
	struct read_ico head;
	fread(&head, sizeof(struct read_ico), 1, fp);

	//int num = (int)head.buffer[0] | (int)head.buffer[1]<<8 | (int)head.buffer[2]<<16 | (int)head.buffer[3]<<24;

	printf("%c\n", head.q);
	printf("%c", head.a[0]);
	printf("%c", head.a[1]);
	printf("%c\n", head.a[2]);

	printf("%u\n", head.CR);
	printf("%u\n", head.LF1);
	printf("%u\n", head.ctrlZ);
	printf("%u\n", head.LF2);
	printf("\n");

	//printf("%d\n", head.buffer[0]);
	//printf("%d\n", head.buffer[1]);
	//printf("%d\n", head.buffer[2]);
	printf("%d\n", head.buffer[3]);

	printf("\n");

	printf("%c", head.c1);
	printf("%c", head.c2);
	printf("%c", head.c3);
	printf("%c", head.c4);

	printf("\n");

	printf("%d\n", head.wid[3]);
	printf("%d\n", head.height[3]);
	printf("%u\n", head.bitdepth);
	printf("%u\n", head.colortype);
	printf("%u\n", head.compressiontype);
	printf("%u\n", head.filtermethod);
	printf("%u\n", head.interlacetype);

	printf("\n");

	//printf("%d\n", head.w);
	//printf("%u\n", head.d1);
	//printf("%u\n", head.d2);
	//printf("%u\n", head.d3);
	//printf("%u\n", head.d4);
	//printf("%u\n", head.e1);
	//printf("%u\n", head.e2);
	//printf("%u\n", head.e3);
	//printf("%u\n", head.e4);	
	//printf("\n");
	
	printf("%u\n", head.f1);
	printf("%u\n", head.f2);
	printf("%u\n", head.f3);
	printf("%u\n", head.f4);


	printf("%d\n", head.f);
	printf("\n");

	printf("%c\n", head.d1);
	printf("%c\n", head.d2);
	printf("%c\n", head.d3);
	printf("%c\n", head.d4);	

		

//	printf("%d\n", head.size);
//	printf("%d\n", head.ofset);
	fclose(fp);

	return 0;
}

int main()
{
	open_ico();
	return 0;
}


