// estudio de las condiciones para emisión sincronizada
// de una colectividad de espines (modelo de ising)
//
// sergio rivera - versión inicial diciembre 2012. updated dec 2021
//
// ffmpeg -framerate 30 -i ising_%d.png -c:v libx264 -fs 1M out.mp4
//

#include "../include/ising.h"
#include "../include/img.h"

static int ***SpinMat; // heap
static int *Iindexes;
static int *Jindexes;

// shuffle was taken from https://stackoverflow.com
void shuffle(int *array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

// interpolate x within a range
float linear(float A, float B, float x, float xMax) {
  return A + (B - A) * ((float)(xMax - 1.0f - x)) / ((float)(xMax - 1.0f));
}

// inverse of thermodynamic temperature
float beta(int i, int j, int t) {
  float kb = 1.380649e-23;
  return 1.0f / (kb * params.temperature);
}

float J(int i, int j, int t) {
  // int period = 200;
  // return sin((float)i/1000.0f + ((float)j)*2.0f*3.14159f/(float)period) *
  // (params.JCteMax - params.JCteMin)- params.JCteMin; return
  // linear(params.JCteMin, params.JCteMax, (float)(j%period), (float)period);
  return linear(params.JCteMin, params.JCteMax, (float)j, (float)params.height);
}

float mu(int i, int j, int t) {
  // int period = 10+(int)(i/10.0);
  // return sin((float)j/100.0f + ((float)i)*2.0f*3.14159f/(float)period) *
  // (params.muMax - params.muMin)- params.muMin; return linear(params.muMin,
  // params.muMax, (float)(i%period), (float)period);
  return linear(params.muMin, params.muMax, (float)i, (float)params.width);
}

// print a matrix
void printSpinMat(int t) {
  int i, j;
  for (i = 0; i < params.width; i++) {
    for (j = 0; j < params.height; j++)
      printf("%d ", SpinMat[t][i][j] + 1);
    printf("\n");
  }
  printf("\n");
}

// energy() contains the interaction model
float energy(int i, int j, int t) {
  float e = 0;
  int ir = (i + 1) % params.width;
  int jr = (j + 1) % params.height;
  int il = (i - 1 + params.width) % params.width;
  int jl = (j - 1 + params.height) % params.height;

  // next comes the "hamiltonian":
  e +=
      // 1) interaction with nearest neighbors
      -5e-1 * J(i, j, t) *
          (params.spinNearestNeighbors *
           SpinMat[params.time % params.T][i][j]) *
          (SpinMat[(params.time) % params.T][il][j] +
           SpinMat[(params.time) % params.T][ir][j] +
           SpinMat[(params.time) % params.T][i][jl] +
           SpinMat[(params.time) % params.T][i][jr])

      // 2) magnetic field interaction
      - params.spinMagnetic * 5e-2 * (4.0f * J(i, j, t) - mu(i, j, t)) *
            SpinMat[params.time % params.T][i][j] +

      // 3) time dependency
      params.spinTime * SpinMat[params.time % params.T][i][j] *
          SpinMat[(params.time + params.T - (params.T - 1)) % params.T][i][j];

  // each term should have its own constants. But for the time being,
  // just making a dirty rough guess... e's value is given in eV.
  // To convert it to joules:
  return e * 1.602176634e-19;
}

// update the spin state
static inline void spinUpdate(int i, int j, int t) {
  float e0, e1, de, r;
  e0 = energy(i, j, t);
  SpinMat[t][i][j] = -SpinMat[t][i][j];
  e1 = energy(i, j, t);
  de = e1 - e0;
  if (de > 0.0f) {
    r = rand() / (float)RAND_MAX;
    if (r > exp(-beta(i, j, t) * de)) {
      SpinMat[t][i][j] = -SpinMat[t][i][j];
    } else {
      // spin change might bring cooling...
      params.temperature *= params.cooling;
    }
  }
  // if there is reason for warming
  params.temperature *= params.warming;
}

// each step update all the spins
int step(void) {
  int ii, jj, i, j;
  int z = params.time % params.T;
  int z1 = (params.time + 1) % params.T;
  // each step updates spins in random order
  shuffle(Iindexes, params.width);
  shuffle(Jindexes, params.height);
  for (ii = 0; ii < params.width; ii++)
    for (jj = 0; jj < params.height; jj++) {
      i = Iindexes[ii];
      j = Jindexes[jj];
      spinUpdate(i, j, z);
      SpinMat[z1][i][j] = SpinMat[z][i][j];
    }
  params.time++;
  return (params.time + 1) % params.T;
}

// default initial values
void init(void) {
  // vertical lattice size
  params.height = 640;
  // horizontal lattice size
  params.width = 480;
  // time derivative - delay in the feedback
  params.T = 20;
  // time (each step is one time unit)
  params.time = 0;
  // simulation steps
  params.Nsteps = 500;
  //  spin interactions
  params.JCteMax = 0.35f;
  params.JCteMin = -0.10f;
  // magnetic moment (interaction with external field)
  params.muMax = 5.30f;
  params.muMin = -5.30f;
  // temperature
  params.temperature = 293.15e0f; // in kelvin
  // temperature evolution factors
  params.cooling = 1.0e0f; // 1.0e0f; 0.9999;
  params.warming = 1.0e0f; // 1.0e0f; 1.0000001
  // spin coupling parameters
  params.spinNearestNeighbors = 1.0f;
  params.spinMagnetic = 1.0e0;
  params.spinTime = 1.0e-1;
}

void matrixAllocation(void) {
  int i, j, t;
  SpinMat = malloc(params.T * params.width * params.height * sizeof(int *));
  for (t = 0; t < params.T; t++) {
    SpinMat[t] = malloc(params.width * params.height * sizeof(int));
    for (i = 0; i < params.width; i++) {
      SpinMat[t][i] = malloc(params.height * sizeof(int));
      for (j = 0; j < params.height; j++) {
        if (t == 0) {
          float r = rand() / (float)RAND_MAX;
          if (r > 0.5)
            SpinMat[t][i][j] = 1;
          else
            SpinMat[t][i][j] = -1;
        } else
          SpinMat[t][i][j] = SpinMat[0][i][j];
      }
    }
  }

  Iindexes = malloc(params.width * sizeof(int));
  Jindexes = malloc(params.height * sizeof(int));
  for (int i = 0;
       i < (params.width < params.height ? params.height : params.width); i++) {
    if (i < params.width)
      Iindexes[i] = i;
    if (i < params.height)
      Jindexes[i] = i;
  }
}

// launch the simulation and save images
int lauchSimulation(void) {

  int t = 0;
  saveImg(params.width, params.height, SpinMat, params.time, t);
  for (int i = 0; i < params.Nsteps; i++) {
    t = step();
    saveImg(params.width, params.height, SpinMat, params.time, t);
  }
  free(SpinMat);
  free(Iindexes);
  free(Jindexes);

  return 0;
}

// house keeping:

static int xoptind = 1;
static int xopterr = 1;
static int xoptopt;
static char *xoptarg;

static void usage(const char *name, FILE *f) {
  fprintf(f,
          "usage: %s [-H int] [-J float] [-M float] [-S int] [-T int] [-V int] "
          "[-c float] [-h] [-j float] [-m float] [-n float] [-r float] [-s "
          "float] [-t float] [-w float]\n",
          name);
  fprintf(f, "  -H int         horizontal lattice size\n");
  fprintf(f, "  -J float       max J\n");
  fprintf(f, "  -M float       max magnetic moment\n");
  fprintf(f, "  -S int         simulation steps\n");
  fprintf(f, "  -T int         steps of delay for the feedback\n");
  fprintf(f, "  -V int         vertical lattice size\n");
  fprintf(f, "  -c float       temperature cooling factor\n");
  fprintf(f, "  -h             print this message\n");
  fprintf(f, "  -j float       min J\n");
  fprintf(f, "  -m float       min magnetic moment\n");
  fprintf(f, "  -n float       spin coupling parameter - nearest\n");
  fprintf(f, "  -r float       spin coupling parameter - magnetic\n");
  fprintf(f, "  -s float       spin coupling parameter - time\n");
  fprintf(f, "  -t float       temperature\n");
  fprintf(f, "  -w float       temperature warming factor\n");
  fprintf(f, "\n");
}

static int xgetopt(int argc, char *const argv[], const char *optstring) {
  static int optpos = 1;
  const char *arg;
  (void)argc;

  if (xoptind == 0) {
    xoptind = 1;
    optpos = 1;
  }

  arg = argv[xoptind];
  if (arg && strcmp(arg, "--") == 0) {
    xoptind++;
    return -1;
  } else if (!arg || arg[0] != '-' || !isalnum(arg[1])) {
    return -1;
  } else {
    const char *opt = strchr(optstring, arg[optpos]);
    xoptopt = arg[optpos];
    if (!opt) {
      if (xopterr && *optstring != ':')
        fprintf(stderr, "%s: illegal option: %c\n", argv[0], xoptopt);
      return '?';
    } else if (opt[1] == ':') {
      if (arg[optpos + 1]) {
        xoptarg = (char *)arg + optpos + 1;
        xoptind++;
        optpos = 1;
        return xoptopt;
      } else if (argv[xoptind + 1]) {
        xoptarg = (char *)argv[xoptind + 1];
        xoptind += 2;
        optpos = 1;
        return xoptopt;
      } else {
        if (xopterr && *optstring != ':')
          fprintf(stderr, "%s: option requires an argument: %c\n", argv[0],
                  xoptopt);
        return *optstring == ':' ? ':' : '?';
      }
    } else {
      if (!arg[++optpos]) {
        xoptind++;
        optpos = 1;
      }
      return xoptopt;
    }
  }
}

int getParams(int argc, char **argv) {
  int option;
  while ((option = xgetopt(argc, argv, "hV:H:T:S:J:j:M:m:c:w:n:s:t:r:")) != -1) {
    int n;
    float f;
    switch (option) {
    case 'h':
      usage(argv[0], stdout);
      exit(EXIT_SUCCESS);
    case 'V':
      n = atoi(xoptarg);
      params.height = n;
      break;
    case 'H':
      n = atoi(xoptarg);
      params.width = n;
      break;
    case 'T':
      n = atoi(xoptarg);
      params.T = n;
      break;
    case 'S':
      n = atoi(xoptarg);
      params.Nsteps = n;
      break;
    case 'J':
      f = atof(xoptarg);
      params.JCteMax = f;
      break;
    case 'j':
      f = atof(xoptarg);
      params.JCteMin = f;
      break;
    case 'M':
      f = atof(xoptarg);
      params.muMax = f;
      break;
    case 'm':
      f = atof(xoptarg);
      params.muMin = f;
      break;
    case 't':
      f = atof(xoptarg);
      params.temperature = f;
      break;
    case 'c':
      f = atof(xoptarg);
      params.cooling = f;
      break;
    case 'w':
      f = atof(xoptarg);
      params.warming = f;
      break;
    case 'n':
      f = atof(xoptarg);
      params.spinNearestNeighbors = f;
      break;
    case 's':
      f = atof(xoptarg);
      params.spinMagnetic = f;
      break;
    case 'r':
      f = atof(xoptarg);
      params.spinTime = f;
      break;
    default:
      usage(argv[0], stderr);
      exit(EXIT_FAILURE);
    }
  }
}