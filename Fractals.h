/*Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: Fractals.h
 */
#define HEIGHT 640
#define WIDTH 640

struct Fractal{
        double dimension, stddevx, stddevy, *xs, *ys, **genome;
        int fracnum, numfuncs, numpoints, numb, dist, avgx, avgy, **bm, *colours, coloured;
};

void func(double *x, double *y, double **genome, int funcnum);
double validranddouble(void);
void generatemults(double **genome, int *multparams);
void generateadds(double **genome, int *addparams);
void generategenome(struct Fractal *frac);
void ordergenome(int numfuncs, double **genome);
double validatefunc(double a, double b, double c, double d);
double ** mallocgenome(int numfuncs);
void initializefrac(struct Fractal *frac, int numfuncs, int numpoints);
double generatepoints(struct Fractal *frac);
int generatefrac(struct Fractal *frac);
int * pointtocoord(double x, double y, double minx, double maxx, double miny, double maxy);
void generatematrix(struct Fractal *frac, double *window);
void freegenome(struct Fractal *frac);
void freefrac(struct Fractal *frac);
int lenfile(char *filename);



