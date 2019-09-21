// https://godbolt.org/z/vrRUe9

// Compiled with x86-64 GCC 9.2; optimizations turned off.
// Follows System V AMD64 ABI calling convention; default in all Unix-like OSs.
// https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI

// compile-time evaluted; no assembly generated
static_assert(sizeof(int) == 4);

// 1. store caller’s stack bottom (rbp) to hardware stack
// 2. copy caller’s stack top (rsp) to this stack’s bottom
// 3. store arguments from edi and esi to stack
int add(int x, int y) {
// 4. load values from stack to edx and eax
// 5. add edx to eax
  return x + y;
// 6. restore caller’s stack bottom to frame pointer (rbp)
// 7. jump to caller
}

// 1. store caller’s stack bottom (rbp) to hardware stack
// 2. copy caller’s stack top (rsp) to this stack’s bottom
// 3. offset stack top away from bottom to allocate 16 bytes of stack space to
//    accomodate 2 ints; define 3 more ints to see a 32 byte stack allocation
int main() {
// 4. store constants to stack
  int a = 1, b = -4;
// 5. load constants from stack to registers edx and eax
// 6. copy values from edx and eax to esi and edi for argument passing
// 7. jump to callee
  return add(a, b);
// 8. leave: reverses 1 and 2 (basically restores caller’s stack top and bottom)
//    1. copy this stack’s bottom to caller’s stack top
//    2. restore caller’s stack bottom to rbp
// 9. jump to caller
}
