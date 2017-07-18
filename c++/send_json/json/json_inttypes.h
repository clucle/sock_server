
#ifndef _json_inttypes_h_
#define _json_inttypes_h_

#ifdef __WIN32__
# include "json_config_win32.h"
#else
# include "json_config.h"
#endif



#ifdef JSON_C_HAVE_INTTYPES_H
/* inttypes.h includes stdint.h */
#include <inttypes.h>

#else
#include <stdint.h>

#define PRId64 "I64d"
#define SCNd64 "I64d"

#endif

#endif