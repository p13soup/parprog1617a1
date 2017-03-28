#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define N 400
#define M 400

void getWallTime(double *wct);


int main(int argc, char *argv[]) {
	
	float *pixels , *constants, *newPix;
	float timeStart , timeEnd;
	float up, same , down;
	int i,j,a;
	double ts, te;
	double mflops;
	
	
	pixels = (float *) malloc(N*M*sizeof(float));
	if(pixels == NULL)
	{
		exit(1);
	}
	
	constants = (float *) malloc(N*M*sizeof(float));
	if(constants == NULL)
	{
		free(pixels);
		exit(1);
	}
	
	newPix = (float *) malloc(N*M*sizeof(float));
	if(constants == NULL)
	{
		free(pixels);
		free(constants);
		exit(1);
	}
	

	for (i=0;i<N;i++)
	{
		for(j=0;j<M;j++)
		{
			pixels[i*N+j] = (rand()%16);
			constants[i*N+j] = 0.5;
			newPix[i*N+j]=0.0;
		}

	}
	
	getWallTime(&ts);

	for (i=1;i<N-1;i++)
	{
		for(j=1;j<M-1;j++)
		{
			up= pixels[(i-1)*M+(j-1)]*constants[(i-1)*M+(j-1)] + pixels[(i-1)*M+(j)]*constants[(i-1)*M+(j)] + pixels[(i-1)*M+(j+1)]*constants[(i-1)*M+(j+1)];
			same= pixels[(i)*M+(j-1)]*constants[(i)*M+(j-1)] + pixels[(i)*M+(j)]*constants[(i)*M+(j)] + pixels[(i)*M+(j+1)]*constants[(i)*M+(j+1)];
			down= pixels[(i+1)*M+(j-1)]*constants[(i+1)*M+(j-1)] + pixels[(i+1)*M+(j)]*constants[(i+1)*M+(j)] + pixels[(i+1)*M+(j+1)]*constants[(i+1)*M+(j+1)];
			
			newPix[i*N+j]= up+same+down;
			
		}
	
	}
	
	getWallTime(&te);
	
	free(pixels);
	free(constants);
	free(newPix);

	

	mflops=(te-ts);
	printf("XRONOS = %lf \n",mflops);
	
	return 0;
}

void getWallTime(double *wct)
{
	struct timeval tp;
	gettimeofday(&tp,NULL);
	*wct = (double)(tp.tv_sec+(tp.tv_usec/1000000.0));
}
