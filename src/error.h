#ifndef _NODE_ERROR_H_
#define _NODE_ERROR_H_

#if defined(__cplusplus)
extern "C" {
#endif

enum {
	NODE_E_ERROR			= -1,
	NODE_E_INVALID		= -2,
	NODE_E_INUSE       	= -3,
	NODE_E_NOEXIST     	= -4,
	NODE_E_SHORTBUF    	= -5,
	NODE_E_NOMEMORY    	= -6,
	NODE_E_NOTSUPP     	= -7
};

#if defined(__cplusplus)
}
#endif

#endif // _NODE_ERROR_H_
