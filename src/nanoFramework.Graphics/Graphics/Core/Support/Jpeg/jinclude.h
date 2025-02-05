//
// Copyright (c) .NET Foundation and Contributors
// Original work Copyright (c) 1994-1998, Thomas G. Lane
// See LICENSE file in the project root for full license information.
//

/*
 * jinclude.h
 *
 * Copyright (C) 1991-1994, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file exists to provide a single place to fix any problems with
 * including the wrong system include files.  (Common problems are taken
 * care of by the standard jconfig symbols, but on really weird systems
 * you may have to edit this file.)
 *
 * NOTE: this file is NOT intended to be included by applications using the
 * JPEG library.  Most applications need only include jpeglib.h.
 */


 /* Include auto-config file to find out which system include files we need. */

#include "jconfig.h"      /* auto configuration options */
#define JCONFIG_INCLUDED   /* so that jpeglib.h doesn't do it again */

/*
 * We need the NULL macro and CLR_INT32 typedef.
 * On an ANSI-conforming system it is sufficient to include <stddef.h>.
 * Otherwise, we get them from <stdlib.h> or <stdio.h>; we may have to
 * pull in <sys/types.h> as well.
 * Note that the core JPEG library does not require <stdio.h>;
 * only the default error handler and data source/destination modules do.
 * But we must pull it in because of the references to FILE in jpeglib.h.
 * You can remove those references if you want to compile without <stdio.h>.
 */

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef NEED_SYS_TYPES_H
#include <sys/types.h>
#endif

 //#include <stdio.h> // nanoCLR - we don't need stdio.h

 /*
  * We need memory copying and zeroing functions, plus strncpy().
  * ANSI and System V implementations declare these in <string.h>.
  * BSD doesn't have the mem() functions, but it does have bcopy()/bzero().
  * Some systems may declare memset and memcpy in <memory.h>.
  *
  * NOTE: we assume the size parameters to these functions are of type CLR_INT32.
  * Change the casts in these macros if not!
  */

#ifdef NEED_BSD_STRINGS

#include <strings.h>
#define MEMZERO(target,size)   bzero((void *)(target), (CLR_INT32)(size))
#define MEMCOPY(dest,src,size)   bcopy((const void *)(src), (void *)(dest), (CLR_INT32)(size))

#else /* not BSD, assume ANSI/SysV string lib */

#include <string.h>
#define MEMZERO(target,size)   memset((void *)(target), 0, (CLR_INT32)(size))
#define MEMCOPY(dest,src,size)   memcpy((void *)(dest), (const void *)(src), (CLR_INT32)(size))

#endif

  /*
   * In ANSI C, and indeed any rational implementation, CLR_INT32 is also the
   * type returned by sizeof().  However, it seems there are some irrational
   * implementations out there, in which sizeof() returns an int even though
   * CLR_INT32 is defined as long or unsigned long.  To ensure consistent results
   * we always use this SIZEOF() macro in place of using sizeof() directly.
   */

#define SIZEOF(object)   ((CLR_INT32) sizeof(object))

   /*
    * The modules that use fread() and fwrite() always invoke them through
    * these macros.  On some systems you may need to twiddle the argument casts.
    * CAUTION: argument order is different from underlying functions!
    */

#define JFREAD(file,buf,sizeofbuf)  \
  ((CLR_INT32) fread((void *) (buf), (CLR_INT32) 1, (CLR_INT32) (sizeofbuf), (file)))
#define JFWRITE(file,buf,sizeofbuf)  \
  ((CLR_INT32) fwrite((const void *) (buf), (CLR_INT32) 1, (CLR_INT32) (sizeofbuf), (file)))

