/*Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: Fractals.c
 *
 * This file contains functions that are used to 
 * generate fractals, both affine and non-affine
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "Fractals.h"
#include "vecio.h"
#include "matvec_read.h"
#define DOTSIZE 1 //must be an odd positive integer

void func(double *x, double *y, double **genome, int funcnum){
    /* This function computes the transformation of a point (x, y) by a 
     * function in an IFS. The specific function in the IFS is given by
     * an integer, funcnum, then the parameters of that function are obtained 
     * from the IFS genome. The function type is then used to compute the 
     * transformation. 
     *
     * if functype is 0:    affine transformation   new_x = ax + by + c
     */
    double oldx = *x;
    double oldy = *y;
    int ind = 4 * funcnum;
    int addind = 2 * funcnum;
    (*x) = genome[0][ind]   * oldx + genome[0][ind+1] * oldy + genome[1][addind];
    (*y) = genome[0][ind+2] * oldx + genome[0][ind+3] * oldy + genome[1][addind+1];
}

double validranddouble(void){
    /* This function generates random values
     * within a specific range for IFS parameters
     */
    return (double)rand()/RAND_MAX * 2 - 1;
}

void generatemults(double **genome, int *multparams){
    /* This function generates the multiplicative parameters 
     * of each function in an IFS. ie., the parameters that are not
     * the +c or +e in the functions defined in the func() function
     *
     * The parameters are generated using validranddouble function
     * and the function is checked if it satisfies contractivity
     * conditions. If the function does not satisfy the contractivity
     * conditions, all parameters for that function are regenerated.
     */
    double specrad = 0;
    int i = *multparams; 
    while (specrad == 0){
        for (int j = i; j < i + 4; j++){
            genome[0][j] = validranddouble();
        }
        specrad = validatefunc(genome[0][i], genome[0][i+1], genome[0][i+2], genome[0][i+3]);
    }
    int funcind = (int)(i/4);
    genome[3][funcind] = specrad;
    *multparams += 4;
    return;
}

void generateadds(double **genome, int *addparams){
    /* This function generates the additive parameters
     * for each function in the IFS. ie., the +c or +e in the 
     * functions defined in the func() function.
     *
     * Since these parameters do not affect contractivity,
     * they are generated to be a random number between 
     * -1 and 1.
     */
    for (int i = *addparams; i < *addparams + 2; i++){
        genome[1][i] = (double)rand()/RAND_MAX*2. - 1.;
    }
    *addparams += 2;
    return;
}

void generategenome(struct Fractal *frac){
    /* This function generates the genome of a fractal, ie., the set of vectors that  
     * that contains all the parameters used to generate the fractal. 
     * NOTE: This function assumes that initializefrac has been called
     *
     * INPUTS:
     *      frac - the fractal struct for which the genome is being 
     *             generated and stored in 
     */
    int i;
    int multparams = 0;
    int addparams = 0;
    double **genome = frac -> genome;
    double sumspecrad = 0;
    for (i = 0; i < frac -> numfuncs; i++){
        generatemults(genome, &multparams);
        generateadds(genome, &addparams);
	sumspecrad += genome[3][i];
    }

    //Make the probabilities the normalized spectral 
    //radii for the functions
    for (i = 0; i < frac -> numfuncs; i++){
        genome[2][i] = genome[3][i]/sumspecrad;
    }
    ordergenome(frac->numfuncs, genome);
    return;
}

void ordergenome(int numfuncs, double **genome){
    /* This function is used to order the functions in the 
    * fractal genome. This is done by sorting rows of a
    * matrix where each row represents a function
    */
    int i,j;
    double **genomefuncs = dmatmem(numfuncs, 8);
    /* Setting values in the genomefuncs matrix */
    for (i = 0; i < numfuncs; i++){
    	genomefuncs[i][0] = genome[3][i];
    	genomefuncs[i][1] = genome[0][4*i+0];
    	genomefuncs[i][2] = genome[0][4*i+1];
    	genomefuncs[i][3] = genome[0][4*i+2];
    	genomefuncs[i][4] = genome[0][4*i+3];
    	genomefuncs[i][5] = genome[1][2*i+0];
    	genomefuncs[i][6] = genome[1][2*i+1];
    	genomefuncs[i][7] = genome[2][i];
    }
    dsortmatrows(numfuncs, 0, genomefuncs);
    /* Converting back to genome of the fractal */

    for (i = 0; i < numfuncs; i++){
        for (j = 0; j < 4; j++){
	    genome[0][4*i+j] = genomefuncs[i][j+1];
	}
	for (j = 0; j < 2; j++){
            genome[1][2*i+j] = genomefuncs[i][j+5];
	}
        genome[2][i] = genomefuncs[i][7];
        genome[3][i] = genomefuncs[i][0];
    }
    dfreemat(numfuncs, genomefuncs);
    return;
}

double validatefunc(double a, double b, double c, double d){
    /* This function checks if an affine transformation
     * satisfies contractivity conditions. This function
     * returns 0 if contractive, 1 otherwise.
     *
     * if an affine transformation is given by:
     *
     *       [x]   [a b][x]   [e]
     *       [y] = [c d][y] + [f]
     *
     * then these contractivity conditions are
     *  a^2 + c^2 < 1
     *  b^2 + d^2 < 1
     *  a^2 + b^2 + c^2 + d^2 - det(A)^2 < 1
     *
    double val1, val2;
    double det = funcdeterminant(a,b,c,d);
    if ((val1 = a*a + c*c)>=1) return 1;
    else if ((val2 = b*b + d*d)>=1) return 1;
    else if ((val1 + val2 - det*det) >= 1) return 1;
    else return 0;
    */
    // To be contractive all the function needs is the 
    // spectral radius less than 1. The above checks this,
    // but we will order the functions based on spectral
    // raidus anyway, so we might as well calculate it.
    double Tr = a + d;
    double det = a * d - b * c;
    double discr = Tr * Tr - 4 * det;
    if (discr < 0) return 0;
    else {
        double abslambda1 = fabs((Tr + sqrt(discr))*0.5);
    	double abslambda2 = fabs((Tr - sqrt(discr))*0.5);
	if (abslambda1 >= 1 || abslambda2 >= 1) return 0;
	else if (abslambda1 >= abslambda2) return abslambda1;
	else return abslambda2;
    }
}

double ** mallocgenome(int numfuncs){
    /* This function allocates memory for the genome of an IFS 
     * The genome consists of 4 vectors
     * genome[0] is the vector of multiplicative parameters
     * genome[1] is the vector of additive parameters
     * genome[2] is the vector of probabilities for each function
     * genome[3] is the spectral radius of the matrix in the function
     */
    double **genome;
    if ((genome = (double **)malloc(4*sizeof(double *))) == NULL){
        fprintf(stderr, "Malloc failed (generate genome)\n");
        exit(1);
    }
    if (((genome[0] = (double *)malloc(4*numfuncs*sizeof(double))) == NULL)||
        ((genome[1] = (double *)malloc(2*numfuncs*sizeof(double))) == NULL)||
        ((genome[2] = (double *)malloc(numfuncs*sizeof(double))) == NULL)||
	((genome[3] = (double *)malloc(numfuncs*sizeof(double))) == NULL)){
            fprintf(stderr, "Malloc failed (initializefrac)\n");
            exit(1);
    }
    return genome;
}

void initializefrac(struct Fractal *frac, int numfuncs, int numpoints){
    /* This function initializes a fractal structure. The number of
     * points and number of functions has to be defined before it 
     * can be called. This function then allocates memory for the
     * x and y points that will be generated as well as allocating 
     * memory for the matrix representing the picture of the fractal.
     *
     * All values corresponding to the fractal other than numfuncs,
     * numpoints, and whether the fractal is colours or not
     * are initialized to -1
     */
    int i;
    frac -> fracnum   = -1;
    frac -> numfuncs  = numfuncs;
    frac -> numpoints = numpoints;
    frac -> numb      = -1;
    frac -> avgx      = -1;
    frac -> avgy      = -1;
    frac -> stddevx   = -1;
    frac -> stddevy   = -1;
    frac -> dimension = -1;
    frac -> dist      = -1;
    frac -> coloured  = 1; //dont colour fractals by function by default
                           //to make them coloured by function by default
                           //change this to 0

    /* initialize genome */
    double **genome = mallocgenome(numfuncs);
    frac -> genome = genome;

    /* initialize xs, ys, colour vector, and specrad*/
    if ((frac -> xs = (double *)malloc(numpoints * sizeof(double))) == NULL){
        fprintf(stderr, "Malloc Failed. (initialize points)\n");
        exit(1);
    }
    if ((frac -> ys = (double *)malloc(numpoints * sizeof(double))) == NULL){
        fprintf(stderr, "Malloc Failed. (initialize points)\n");
        exit(1);
    }
    if ((frac -> colours = (int *)malloc(numpoints * sizeof(int))) == NULL){
        fprintf(stderr, "Malloc Failed. (initialize points)\n");
        exit(1);
    }
    /* initizlize the pixel map */
    int **bm;
    if ((bm = (int **)malloc(HEIGHT*sizeof(int *))) == NULL){
        fprintf(stdout, "Malloc Failed. (makematrix)\n");
        exit(1);
    }
    for (i = 0; i < HEIGHT; i++){
        if ((bm[i] = (int *)malloc(WIDTH*sizeof(int))) == NULL){
            fprintf(stderr, "Malloc Failed. (makematrix)\n");
            exit(1);
        }
    }
    frac -> bm = bm;
    return;
}

void generatepoints(struct Fractal *frac, double *extrema){
    /* This function generates the points corresponding 
     * to a fractal. That is, it randomly picks a function
     * in the fractal and uses it to transform a random 
     * point. A new function is then picked and transforms 
     * the output from the last point. This continues until
     * numpoints points are generated. Additionally, the 
     * first 100 points are thrown away to ensure that all 
     * (or close to all) points correspond to the fractal. 
     */
    int i,j;
    int funcnum;
    double p, num;
    double x = (double)rand()/RAND_MAX; 
    double y = (double)rand()/RAND_MAX;
    double maxx = 0;
    double minx = 0;
    double maxy = 0;
    double miny = 0;
    for (i = 0; i < 100; i++){
        funcnum = rand()%frac -> numfuncs;
        func(&x, &y, frac -> genome, funcnum);
    }
    for (i = 0; i < frac -> numpoints; i++){
        num = (double)rand()/RAND_MAX;
        p = 0.0;
        funcnum = 0;
        for (j = 0; j < frac -> numfuncs; j++){
            p += frac -> genome[2][j];
            if (num < p) {
                func(&x,&y,frac -> genome, funcnum);
                frac -> xs[i] = x;
                frac -> ys[i] = y;
                
                //note: colours get put to pixels in generatebm (below)
                //      and colours chosen are in PNGio.c
                frac -> colours[i] = funcnum;
		if (i == 0){
		    maxx = x;
		    minx = x;
		    maxy = y;
		    miny = y;
		}
                if (x > maxx) maxx = x;
                if (x < minx) minx = x;
                if (y > maxy) maxy = y;
                if (y < miny) miny = y;
                break;
            }
            else funcnum ++;
        }
    }
    extrema[0] = minx;
    extrema[1] = maxx;
    extrema[2] = miny;
    extrema[3] = maxy;
}

void generatefrac(struct Fractal *frac, double *extrema){
    /* This function calls the generate points function.
     * The commented section is used to resized the affine
     * fractals if they were too large to fit into the -1 to 1 
     * square, however, this is no longer used as non-affine
     * IFSs are not as simple to resize
     */
    generatepoints(frac, extrema);
}

void pointtocoord(int *coords, double x, double y, double minx, double maxx, double miny, double maxy){
    /* This function is used to convert a point (x,y) to pixel coordinates
     * on a screen with viewing region [minx,maxx]x[miny,maxy]
     */
    coords[0] = (int)(WIDTH/2  + WIDTH/2  * ((x - minx)/(maxx - minx)*2 - 1));
    coords[1] = (int)(HEIGHT/2 - HEIGHT/2 * ((y - miny)/(maxy - miny)*2 - 1));
}

void generatematrix(struct Fractal *frac, double *window){
    /* This function is used to transform the points of a fractal
     * to a matrix of size HEIGHT x WIDTH which will be used to
     * generate an image of the fractal.
     */
    int i,j,k,x,y;
    int dotsize = DOTSIZE; //positive odd integer - defines the size of a point
    int white = 255;
    int **bm = frac -> bm;
    
    //start off with a fully white image
    for (i = 0; i < HEIGHT; i++){
        for (j = 0; j < WIDTH; j++){
            bm[i][j] = white;
        }
    }
    //numb is the number of pixels corresponding to the attractor
    //avgx and avgy are the pixel centroid coordinates
    int numb = 0;
    int avgx = 0;
    int avgy = 0;
    int *coords = ivecmem(2);
    for (i = 0; i < frac -> numpoints; i++){
	pointtocoord(coords, frac ->xs[i], frac->ys[i], window[0], 
                              window[1], window[2], window[3]);
        x = coords[0];
        y = coords[1];
        if (dotsize %2 != 0) {
             for (j = -1 * (dotsize -1)/2; j <= (dotsize - 1)/2; j++){
                 for (k = -1 * (dotsize -1)/2; k <= (dotsize -1)/2; k++){
                     if (x >= WIDTH  - dotsize/2 - 1) x = WIDTH  - dotsize/2 - 1;
                     if (y >= HEIGHT - dotsize/2 - 1) y = HEIGHT - dotsize/2 - 1;
                     if (x <= dotsize/2) x = dotsize;
                     if (y <= dotsize/2) y = dotsize;
                     if (bm[y+j][x+k] == 255){
                         avgx += x+k;
                         avgy += y+j;
                         numb += 1;
                     }
                     bm[y+j][x+k] = frac -> colours[i];
                }
            }
        }
    }
    frac -> avgx = avgx/(int)numb;
    frac -> avgy = avgy/(int)numb;
    frac -> numb = numb;
    free(coords);
    return;
}

void freegenome(struct Fractal *frac){
    /* This function frees the genome memory */
    free(frac -> genome[0]);
    free(frac -> genome[1]);
    free(frac -> genome[2]);
    free(frac -> genome[3]);
    free(frac -> genome);
    return;
}

void freefrac(struct Fractal *frac){
    /* This function frees the memory of a fractal structure */
    freegenome(frac);
    ifreemat(HEIGHT, frac -> bm);
    free(frac -> xs);
    free(frac -> ys);
    free(frac -> colours);
    free(frac);
    return;
}

int lenfile(char *filename){
    /* Thise function calculates the length of a file */
    int len = 0;
    char ch;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "Failed to open file (lenfile): %s (%c)\n", filename, 'r');
        exit(1);
    }
    while (!feof(fp)){
        ch = fgetc(fp);
        if (ch == '\n'){
            len++;
        }
    }
    fclose(fp);
    return len;
}
