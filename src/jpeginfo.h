#ifndef _NODE_JPEGINFO_H_
#define _NODE_JPEGINFO_H_

#if defined(__cplusplus)
extern "C" {
#endif


int getWidth(const char* inputfilename);
int getHeight(const char* infputfilename);
int getColorDepth(const char* inputfilename);
int getJpegHeaderInfo(const char *inputfilename, struct jpeg_decompress_struct *dinfo) ;

#if defined(__cplusplus)
}
#endif

#endif //_NODE_JPEGINFO_H_