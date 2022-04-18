#include <cassert>
#include <csignal>
#include <cstdio>

#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  assert(argc >= 3);

  {
    // Keep alive on broken pipes
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);
  }

  // Must be in this order
  FILE *fout = fopen(argv[2], "a");
  FILE *fin = fopen(argv[1], "r");

  {
    std::string in_secret = "385045b28fbc1903eca997d513040235e0229e01";
    std::string out_secret = "4faa78c22ee35083e49b65601698a1bd98a87a0f";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != in_secret) {
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }

  int N, M, Q;
  assert(3 == scanf("%d %d %d", &N, &M, &Q));

  std::vector<int> P(N + M), A(M);
  for (int i = 0; i < N + M; ++i) {
    assert(1 == scanf("%d", &P[i]));
  }
  for (int j = 0; j < M; ++j) {
    assert(1 == scanf("%d", &A[j]));
  }

  fprintf(fout, "%d %d\n", N, M);
  for (int i = 0; i < N + M; ++i) {
    if (i > 0) {
      fprintf(fout, " ");
    }
    fprintf(fout, "%d", P[i]);
  }
  fprintf(fout, "\n");
  for (int j = 0; j < M; ++j) {
    if (j > 0) {
      fprintf(fout, " ");
    }
    fprintf(fout, "%d", A[j]);
  }
  fprintf(fout, "\n");
  fflush(fout);

  std::vector<int> counts(Q);
  for (int k = 0; k < Q; ++k) {
    int L, R;
    assert(2 == scanf("%d %d", &L, &R));
    fprintf(fout, "%d %d\n", L, R);
    fflush(fout);

    {
      std::string in_secret = "Z";
      std::string out_secret = "4faa78c22ee35083e49b65601698a1bd98a87a0f";
      char secret[100];
      if (fscanf(fin, "%5s", secret) != 1 || std::string(secret) != in_secret) {
        printf("%s\n", out_secret.c_str());
        printf("SV\n");
        printf("Possible tampering with the output or unexpected termination of"
               " the program\n");
        fclose(stdout);
        return 0;
      }
    }

    if (fscanf(fin, "%d", &counts[k]) != 1) {
      std::string out_secret = "4faa78c22ee35083e49b65601698a1bd98a87a0f";
      printf("%s\n", out_secret.c_str());
      printf("FAIL\n");
      printf("Could not read the query result\n");
      fclose(stdout);
      return 0;
    }
  }
  fprintf(fout, "%d %d\n", -1, -1);
  fflush(fout);

  fclose(fin);
  fclose(fout);

  {
    std::string out_secret = "4faa78c22ee35083e49b65601698a1bd98a87a0f";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  for (int k = 0; k < Q; ++k) {
    printf("%d\n", counts[k]);
  }
  return 0;
}
