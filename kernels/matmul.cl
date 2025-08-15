__kernel void matmul(__global const uint *A, __global const uint *B, __global int *C, const int M, const int N, const int K, const int chunks, const uint lastMask)
{
  const int row = get_global_id(0);
  const int col = get_global_id(1);
  if (row >= M || col >= N) return;

  int matches = 0;
  for (int c = 0; c < chunks; ++c)
  {
    uint a = A[row * chunks + c];
    uint b = B[col * chunks + c];
    uint x = ~(a ^ b);
    if (c == chunks - 1) { x &= lastMask; }
    matches += popcount(x);
  }
  int dot          = matches * 2 - K; // bipolar mapping
  C[row * N + col] = dot;
}