/*Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: vecio.h
 */
void dstrtovec(char *str, double *vector, int *len);
void istrtovec(char *str, int *vector, int *len);
int * ivecmem(int m);
double * dvecmem(int m);
double ** dmatmem(int m, int n);
void dvecout(int n, double * vec);
void dsortmatrows(int m, int col, double **mat);
void dsortvec(int m, double *vec);
void dfreemat(int m, double **mat);
void ifreemat(int m, int **mat);


