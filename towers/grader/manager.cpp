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
    std::string in_secret = "557c2e17ba69b458cd8202c239ce25526cdad3b5";
    std::string out_secret = "07cd724d595507c6bd737a863fa1a319a887084b";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != in_secret) {
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }

  int N, Q;
  assert(2 == scanf("%d %d", &N, &Q));

  std::vector<int> H(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &H[i]));
  }

  fprintf(fout, "%d\n", N);
  for (int i = 0; i < N; ++i) {
    if (i > 0) {
      fprintf(fout, " ");
    }
    fprintf(fout, "%d", H[i]);
  }
  fprintf(fout, "\n");
  fflush(fout);

  std::vector<int> towers(Q);
  for (int i = 0; i < Q; ++i) {
    int L, R, D;
    assert(3 == scanf("%d %d %d", &L, &R, &D));
    fprintf(fout, "%d %d %d\n", L, R, D);
    fflush(fout);

    {
      std::string in_secret = "T";
      std::string out_secret = "07cd724d595507c6bd737a863fa1a319a887084b";
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

    if (fscanf(fin, "%d", &towers[i]) != 1) {
      std::string out_secret = "07cd724d595507c6bd737a863fa1a319a887084b";
      printf("%s\n", out_secret.c_str());
      printf("FAIL\n");
      printf("Could not read the query result\n");
      fclose(stdout);
      return 0;
    }
  }
  fprintf(fout, "%d %d %d\n", -1, -1, -1);
  fflush(fout);

  fclose(fin);
  fclose(fout);

  {
    std::string out_secret = "07cd724d595507c6bd737a863fa1a319a887084b";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  for (int i = 0; i < Q; ++i) {
    printf("%d\n", towers[i]);
  }
  return 0;
}
