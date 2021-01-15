/*Created by:    Liam Graham
 * Last updated: Jan 2021
 *
 * FILE NAME: generatedata.c
 * 
 * This is the main file that, when run,
 * generates databases of fractals
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "Fractals.h"
#include "vecio.h"
#include "PNGio.h"
#include "fracfuncs.h"

int main(int argc, char *argv[]){
    int i, j, numpoints, numfuncs, numrows, numtogenerate;
    int pcomp = 0; 
    double window[4] = {-8,8,-8,8};
    char dirname[50], fracname[50],filepath[100];
    FILE *fp;
    srand(time(NULL));
	
    fprintf(stdout, "How many fractals would you like to generate: ");
    scanf("%d", &numtogenerate);
    fprintf(stdout, "\n");
    
    fprintf(stdout, "What is the directory called (Note: it should already be created): ");
    scanf("%s", dirname);
    
    fprintf(stdout, "\nHow many points would you like to plot for each fractal: ");
    scanf("%d", &numpoints);
    
    fprintf(stdout, "\nHow many functions in each IFS: ");
    scanf("%d", &numfuncs);
    
    //fprintf(stdout, "\nEnter a vector representing the viewing window (eg. minx,maxx,miny,maxy): ");
    //scanf("%s", tmp);	    
    //dstrtovec(tmp, window, &tmpint);

    sprintf(filepath, "./%s/fracdata.dat", dirname);

    if ((fp = fopen(filepath, "r")) == NULL){
        numrows = 0;
    }
    else {
        numrows = lenfile(filepath);
    }
    if ((fp = fopen(filepath, "a")) == NULL){
        fprintf(stderr, "Error, you must create the directory first\n");
        exit(1);
    }

    fprintf(stdout, "Generating fractals %d to %d\n", numrows, numrows+numtogenerate);
    for (i = 0; i < numtogenerate; i++){
    	struct Fractal *frac = makerandfrac(numpoints, numfuncs, window, 1);
        stddev(frac);
        dimension(frac);
        /* fractal number, numfuncs, numpoints, numb, avgx, avgy, stddevx, stddevy, dimension, genome */
        fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\t%.15lf\t%.15lf\t%.15lf\t", numrows+i, frac->numfuncs, frac->numpoints, frac->numb, frac->avgx, frac->avgy, frac->stddevx, frac->stddevy, frac -> dimension);
        for (j = 0; j < 4 * frac -> numfuncs; j++){
	    fprintf(fp, "%.15lf\t", frac -> genome[0][j]);
        }
        for (j = 0; j < 2 * frac -> numfuncs; j++){
            fprintf(fp, "%.15lf\t", frac -> genome[1][j]);
        }
	for (j = 0; j < frac -> numfuncs; j++){
            fprintf(fp, "%.15lf\t", frac -> genome[2][j]);
        }
        for (j = 0; j < frac -> numfuncs-1; j++){
            fprintf(fp, "%.15lf\t", frac -> genome[3][j]);
        }
        fprintf(fp, "%.15lf\n", frac -> genome[3][frac -> numfuncs -1]);
        sprintf(fracname, "%s/frac%d.png", dirname, numrows+i);
        WritePNG(fracname, frac);
    	freefrac(frac);
        if (numtogenerate >= 100 && (i%((int)(numtogenerate/100.)) == 0)){
            pcomp += 1;
            if (pcomp < 10){
                fprintf(stdout, "\rPercent Complete:\t%d%%", pcomp);
            }
            else if (pcomp < 100){
                fprintf(stdout, "\rPercent Complete:\t\b%d%%", pcomp);
            }
            else {
                fprintf(stdout, "\rPercent Complete:\t\b\b%d%%",pcomp);
            }
            fflush(stdout);
        }
    }
    fprintf(stdout, "\n"); 
    fclose(fp);
    exit(0);
}
