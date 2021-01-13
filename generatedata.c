/*Created by:    Liam Graham
 * Last updated: Jun. 2020
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
    int i, j, numpoints, numfuncs, numrows, numtogenerate, tmpint;
    int pcomp = 0; 
    double window[4];
    char filename[50], dirname[50], fracname[124],filepath[100],tmp[50];
    FILE *fp;
    struct Fractal *frac;
    srand(time(NULL));
    
    fprintf(stdout, "How many fractals would you like to generate: ");
    scanf("%d", &numtogenerate);
    fprintf(stdout, "\n");
    
    fprintf(stdout, "What is the directory called (Note: it should already be created): ");
    scanf("%s", dirname);
    
    fprintf(stdout, "\nHow many points would you like to plot for each fractal: ");
    scanf("%d", &numpoints);
    strcpy(filepath, dirname); 
    
    fprintf(stdout, "\nHow many functions in each IFS: ");
    scanf("%d", &numfuncs);
    
    fprintf(stdout, "\nEnter a vector representing the viewing window (eg. minx,maxx,miny,maxy): ");
    scanf("%s", tmp);
    
    dstrtovec(tmp, window, &tmpint);
    sprintf(filename, "fracdata.dat");
    sprintf(filepath, "%s%s", dirname, filename);
    
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
        frac = makerandfrac(numpoints, numfuncs, window);
        stddev(frac);
        dimension(frac);
        int params = 0;
        /* fractal number, numfuncs, numpoints, numb, avgx, avgy, stddevx, stddevy, dimension, genome */
        fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\t%.15lf\t%.15lf\t%.15lf\t",
                numrows+i, frac->numfuncs, frac->numpoints, frac->numb, 
                frac->avgx, frac->avgy, frac->stddevx, frac->stddevy, frac -> dimension);
        for (j = 0; j < frac -> numfuncs; j++){
            for (int k = 0; k < 4; k++){
                fprintf(fp, "%.15lf\t", frac -> genome[0][params + k]);
            }
            params += 4;
        }
        params = 0;
        for (j = 0; j < frac -> numfuncs; j++){
            for (int k = 0; k < 2; k++){
                fprintf(fp, "%.15lf\t", frac -> genome[1][params + k]);
            }
            params += 2;
        }
        for (j = 0; j < frac -> numfuncs; j++){
            fprintf(fp, "%.15lf\t", frac -> genome[2][j]);
        }
        for (j = 0; j < frac -> numfuncs-1; j++){
            fprintf(fp, "%.15lf\t", frac -> genome[3][j]);
        }
        fprintf(fp, "%.15lf\n", frac -> genome[3][frac -> numfuncs -1]);
        sprintf(fracname, "%sfrac%d.png", dirname, numrows+i);
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



