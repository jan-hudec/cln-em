// m > 0, m = 2^m1 - 1 (m1 > 1)

namespace cln {

class cl_heap_modint_ring_pow2m1 : public cl_heap_modint_ring {
	SUBCLASS_cl_heap_modint_ring()
public:
	// Constructor.
	cl_heap_modint_ring_pow2m1 (const cl_I& m, uintC m1); // m = 2^m1 - 1
	// Destructor.
	~cl_heap_modint_ring_pow2m1 () {}
	// Additional information.
	uintC m1;
};

static inline const cl_I pow2m1_reduce_modulo (cl_heap_modint_ring* _R, const cl_I& x)
{
	var cl_heap_modint_ring_pow2m1* R = (cl_heap_modint_ring_pow2m1*)_R;
	// Method:
	// If x>=0, split x into pieces of m1 bits and sum them up.
	//   x = x0 + 2^m1*x1 + 2^(2*m1)*x2 + ... ==>
	//   mod(x,m) = mod(x0+x1+x2+...,m).
	// If x<0, apply this to -1-x, and use mod(x,m) = m-1-mod(-1-x,m).
 {	Mutable(cl_I,x);
	var bool sign = minusp(x);
	if (sign) { x = lognot(x); }
	var const uintC m1 = R->m1;
	if (x >= R->modulus) {
		x = plus1(x); // avoid staying at x = m
		do {
			var uintC xlen = integer_length(x);
			var cl_I y = ldb(x,cl_byte(m1,0));
			for (var uintC i = m1; i < xlen; i += m1)
				y = y + ldb(x,cl_byte(m1,i));
			x = y;
		} while (x > R->modulus);
		x = minus1(x);
	}
	// Now 0 <= x < m.
	if (sign) { x = R->modulus - 1 - x; }
	return x;
}}

static const _cl_MI pow2m1_canonhom (cl_heap_modint_ring* R, const cl_I& x)
{
	return _cl_MI(R, pow2m1_reduce_modulo(R,x));
}

static const _cl_MI pow2m1_mul (cl_heap_modint_ring* _R, const _cl_MI& x, const _cl_MI& y)
{
	var cl_heap_modint_ring_pow2m1* R = (cl_heap_modint_ring_pow2m1*)_R;
	var const uintC m1 = R->m1;
	var cl_I zr = x.rep * y.rep;
	zr = ldb(zr,cl_byte(m1,m1)) + ldb(zr,cl_byte(m1,0));
	return _cl_MI(R, zr >= R->modulus ? zr - R->modulus : zr);
}

static const _cl_MI pow2m1_square (cl_heap_modint_ring* _R, const _cl_MI& x)
{
	var cl_heap_modint_ring_pow2m1* R = (cl_heap_modint_ring_pow2m1*)_R;
	var const uintC m1 = R->m1;
	var cl_I zr = square(x.rep);
	zr = ldb(zr,cl_byte(m1,m1)) + ldb(zr,cl_byte(m1,0));
	return _cl_MI(R, zr >= R->modulus ? zr - R->modulus : zr);
}

#define pow2m1_addops std_addops
static cl_modint_mulops pow2m1_mulops = {
	std_one,
	pow2m1_canonhom,
	pow2m1_mul,
	pow2m1_square,
	std_expt_pos,
	std_recip,
	std_div,
	std_expt,
	pow2m1_reduce_modulo,
	std_retract
};

static void cl_modint_ring_pow2m1_destructor (cl_heap* pointer)
{
	(*(cl_heap_modint_ring_pow2m1*)pointer).~cl_heap_modint_ring_pow2m1();
}

cl_class cl_class_modint_ring_pow2m1 = {
	cl_modint_ring_pow2m1_destructor,
	cl_class_flags_modint_ring
};

// Constructor.
inline cl_heap_modint_ring_pow2m1::cl_heap_modint_ring_pow2m1 (const cl_I& m, uintC _m1)
	: cl_heap_modint_ring (m, &std_setops, &pow2m1_addops, &pow2m1_mulops), m1 (_m1)
{
	type = &cl_class_modint_ring_pow2m1;
}

}  // namespace cln
