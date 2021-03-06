// cl_I_to_UQ().

// General includes.
#include "base/cl_sysdep.h"

// Specification.
#include "integer/cl_I.h"


// Implementation.

#ifdef intQsize

#include "cln/number.h"
#include "base/digitseq/cl_DS.h"
#include "cln/io.h"
#include "cln/integer_io.h"
#include "cln/exception.h"
#include <sstream>

namespace cln {

uint64 cl_I_to_UQ (const cl_I& obj)
{
	if (fixnump(obj)) {
		// Fixnum
		var sintV wert = FN_to_V(obj);
		if (wert >= 0)
			return (uint64)(uintV)wert;
		goto bad;
	} else { // Bignum
		var cl_heap_bignum* bn = TheBignum(obj);
		var uintC len = bn->length;
		if ((sintD)mspref(arrayMSDptr(bn->data,len),0) < 0)
			goto bad;
		#define IF_LENGTH(i)  \
		  if (bn_minlength <= i) /* genau i Digits überhaupt möglich? */\
		    if (len == i) /* genau i Digits? */				\
		      /* 2^((i-1)*intDsize-1) <= obj < 2^(i*intDsize-1) */	\
		      if ( (i*intDsize-1 > 64)					\
		           && ( ((i-1)*intDsize-1 >= 64)			\
		                || (mspref(arrayMSDptr(bn->data,len),0) >= (uintD)bitc(64-(i-1)*intDsize)) \
		         )    )							\
		        goto bad;						\
			else
		#if (intDsize==64)
		IF_LENGTH(1)
			return (uint64)arrayLSref(bn->data,1,0);
		IF_LENGTH(2)
			return (uint64)arrayLSref(bn->data,2,0);
		#endif
		#if (intDsize==32)
		IF_LENGTH(1)
			return (uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,1));
		IF_LENGTH(2)
			return ((uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,2) lspop 1) << 32) | (uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,2));
		IF_LENGTH(3)
			return ((uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,3) lspop 1) << 32) | (uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,3));
		#endif
		#if (intDsize==16)
		IF_LENGTH(1)
			return (uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,1));
		IF_LENGTH(2)
			return (uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,2));
		IF_LENGTH(3)
			return ((uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,3) lspop 2) << 32) | (uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,3));
		IF_LENGTH(4)
			return ((uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,4) lspop 2) << 32) | (uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,4));
		IF_LENGTH(5)
			return ((uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,5) lspop 2) << 32) | (uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,5));
		#endif
		#if (intDsize==8)
		IF_LENGTH(1)
			return (uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,1));
		IF_LENGTH(2)
			return (uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,2));
		IF_LENGTH(3)
			return (uint64)get_uint3D_Dptr(arrayLSDptr(bn->data,3));
		IF_LENGTH(4)
			return (uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,4));
		IF_LENGTH(5)
			return ((uint64)get_uint1D_Dptr(arrayLSDptr(bn->data,5) lspop 4) << 32) | (uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,5));
		IF_LENGTH(6)
			return ((uint64)get_uint2D_Dptr(arrayLSDptr(bn->data,6) lspop 4) << 32) | (uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,6));
		IF_LENGTH(7)
			return ((uint64)get_uint3D_Dptr(arrayLSDptr(bn->data,7) lspop 4) << 32) | (uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,7));
		IF_LENGTH(8)
			return ((uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,8) lspop 4) << 32) | (uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,8));
		IF_LENGTH(9)
			return ((uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,9) lspop 4) << 32) | (uint64)get_uint4D_Dptr(arrayLSDptr(bn->data,9));
		#endif
		#undef IF_LENGTH
	}
	bad: // unpassendes Objekt
	std::ostringstream buf;
	fprint(buf, "Not a 64-bit integer: ");
	fprint(buf, obj);
	throw runtime_exception(buf.str());
}

}  // namespace cln

#endif
