#include <stdio.h>
#include "zlib.h"
#define CHUNK 16384
#include <assert.h>
#pragma pack(1)
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

struct read_header
{
    short int garbage[3];
    unsigned char wid;
    unsigned char hgt;
};

int main()
{
    FILE *fp=fopen("raw_data.txt","rb");
    FILE *fpw=fopen("compressed_raw_data.txt","w");
    FILE *fp1=fopen("header.txt","rb");

    struct read_header head;
    fread(&head,sizeof(struct read_header),1,fp1);

    int hgt=head.hgt;
    int wid=head.wid;
    if(head.hgt==0)
    {
        hgt=256;
    }
    if(head.wid==0) 
    {
        wid=256;
    }

    int pix=(wid)*(hgt);
    unsigned char a[pix];
    printf("%ld\n", sizeof(a)/sizeof(a[0]));


    def(fp,fpw,3);

    fclose(fp);
    fclose(fpw);
    fclose(fp1);

    return 0;
}