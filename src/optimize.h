#ifndef _NODE_OPTIMIZE_H_
#define _NODE_OPTIMIZE_H_

#if defined(__cplusplus)
extern "C" {
#endif


int optimize(const char* inputfilename, const char* outpufilename, int quality, int verbose);
int optimize_loss_less(const char* inputfilename, const char* outputfilename);

#if defined(__cplusplus)
}
#endif

#endif //_NODE_OPTIMIZE_H_