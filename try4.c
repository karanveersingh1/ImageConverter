#include <stdio.h>
#include "zlib.h"
#pragma pack(1)
#define CHUNK 16384
#include <assert.h>

int def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}


struct sig
{
	unsigned char q;
	char a[3];
	unsigned char CR;
	unsigned char LF1;
	unsigned char ctrlZ;
	unsigned char LF2;
};

struct IHDRlen
{
	unsigned char buffer[4];
	
};

struct IHDR
{
	char chnk_type[4];	
	unsigned char width[4];
	unsigned char height[4];
	unsigned char bitdepth;
	unsigned char colortype;
	unsigned char compressiontype;
	unsigned char filtermethod; 
	unsigned char interlacetype;
	unsigned char q[4];
};

struct read_header
{
	short int garbage[3];
	unsigned char wid;
	unsigned char hgt;
};

struct IDATlen
{
	unsigned char buffer[4];
};

struct IDAT
{
	unsigned char buffer[4];
	char chnk_type1[4];
	unsigned char crcdat[4];
};

struct IENDsize
{
	unsigned char buffer[4];
	
};

struct IEND
{
	char chnk_type2[4];
	unsigned char crcdat[4];
};




unsigned char * SerializeInt(unsigned char *buffer, int value)
{
	/* Write big-endian int value into buffer; assumes 32-bit int and 8-bit char. */
	buffer[0] = value >> 24;
	buffer[1] = value >> 16;
	buffer[2] = value >> 8;
	buffer[3] = value;
	return buffer + 4;
}


int open_ico()
{

	//FILE *fp = fopen("Grayscale_8bits_sample_image.png", "rb");
	//struct read_ico head;
	//fread(&head, sizeof(struct read_ico), 1, fp);

	//int num = (int)head.buffer[0] | (int)head.buffer[1]<<8 | (int)head.buffer[2]<<16 | (int)head.buffer[3]<<24;

	FILE *fpw1=fopen("test_png.png","w");

/*************************************************************************/

	struct sig sign;

	sign.q=137;
	sign.a[0]=80;
	sign.a[1]=78;
	sign.a[2]=71;
	sign.CR=13;
	sign.LF1=10;
	sign.ctrlZ=26;
	sign.LF2=10;


	fwrite(&sign, sizeof(struct sig),1,fpw1);	

	//printf("%c\n", head.q);
	//printf("%c", head.a[0]);
	//printf("%c", head.a[1]);
	//printf("%c\n", head.a[2]);

/*************************************************************************/

	struct IHDRlen size;

	int ihdr_len=13;
	SerializeInt(size.buffer,ihdr_len);
	fwrite(&size, sizeof(struct IHDRlen),1,fpw1);

	struct IHDR len;

	len.chnk_type[0]=73;
	len.chnk_type[1]=72;
	len.chnk_type[2]=68;
	len.chnk_type[3]=82;

	FILE *fp=fopen("header.txt", "rb");
	struct read_header rh;
	fread(&rh,sizeof(struct read_header),1,fp);

	int hgt=rh.hgt;
	int wid=rh.wid;
	if(rh.hgt==0)
	{
		hgt=256;
	}
	if(rh.wid==0)
	{
		wid=256;
	}

//	printf("%d\n", wid);
//	printf("%d\n", hgt);

	SerializeInt(len.width,wid-1);
	SerializeInt(len.height,hgt);

	len.bitdepth=8;
	len.colortype=0;
	len.compressiontype=0;
	len.filtermethod=0;
	len.interlacetype=0;


	SerializeInt(len.q,crc32(0,(char *)&len,17));


//	printf("%d\n", len.width);
//	printf("%d\n", len.height);

	fwrite(&len, sizeof(struct IHDR),1,fpw1);	

	//printf("%u\n", len.buffer[0]);
	//printf("%u\n", len.buffer[1]);
	//printf("%u\n", len.buffer[2]);
	//printf("%u\n", len.buffer[3]);

/*************************************************************************/

	struct IDAT call;

	FILE *fpc=fopen("compressed_raw_data.txt","rb");
	FILE *fpc1=fopen("raw_data.txt","rb");

	fseek(fpc, 0L, SEEK_END);
	long int k = ftell(fpc);

	fseek(fpc, 0L, SEEK_SET);

	unsigned char t[k];
	unsigned char g[k+4];
	g[0]=73;
	g[1]=68;
	g[2]=65;
	g[3]=84;

	fread(t,k,1,fpc);
	for (int i = 0; i < k; i++)
	{
		g[4+i]=t[i];
	}

	SerializeInt(call.crcdat,crc32(0,g,k+4));

	SerializeInt(call.buffer,k);
	call.chnk_type1[0]=73;
	call.chnk_type1[1]=68;
	call.chnk_type1[2]=65;
	call.chnk_type1[3]=84;

	fwrite(&call,8,1,fpw1);

	def(fpc1,fpw1,3);

	fwrite(call.crcdat,4,1,fpw1);
	
/*****************************************************************************************/

	struct IENDsize call1;
	struct IEND call2;

	int u=0;
	SerializeInt(call1.buffer,u);
	fwrite(&call1,sizeof(struct IENDsize),1,fpw1);

	call2.chnk_type2[0]=73;
	call2.chnk_type2[1]=69;
	call2.chnk_type2[2]=78;
	call2.chnk_type2[3]=68;

	fwrite(&call2,4,1,fpw1);


/*****************************************************************************************/

}

int main()
{
	open_ico();
	return 0;
}