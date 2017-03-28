#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>
#define N 400
#define M 400


void getWallTime(double *wct);


int main(int argc, char *argv[]) {
	
	int i,j,a,r,count;
	float *fPix,*fConst,*fNewPix,*fUp,*fSame,*fDown,sum,*m;
	__m128 *vfPix,*vfConst,*vfUp,*vfSame,*vfDown,*vfm;
	double ts, te;
	double mflops;
	
	
	i=posix_memalign((void**)&fPix, 16, N*M*sizeof(float));

	if(i!=0)
	{
		printf("error fPix");
		exit (1);
	
	}

	i=posix_memalign((void**)&fConst, 16, N*M*sizeof(float));

	if(i!=0)
	{
		printf("error fConst");
		free (fPix);
		exit (1);
	}

	i=posix_memalign((void**)&fNewPix, 16, N*M*sizeof(float));

	if(i!=0)
	{
		printf("error fNewPix");
		free (fPix);
		free (fConst);
		exit (1);
	}
	i=posix_memalign((void**)&fUp, 16, 4*sizeof(float));

	if(i!=0)
	{
		printf("error fNewPix");
		free (fPix);
		free (fConst);
		free (fNewPix);
		exit (1);
	}
	i=posix_memalign((void**)&fSame, 16, 4*sizeof(float));

	if(i!=0)
	{
		printf("error fNewPix");
		free (fPix);
		free (fConst);
		free (fNewPix);
		free (fUp);
		exit (1);
	}
	i=posix_memalign((void**)&fDown, 16, 4*sizeof(float));

	if(i!=0)
	{
		printf("error fNewPix");
		free (fPix);
		free (fConst);
		free (fNewPix);
		free (fUp);
		free (fSame);
		exit (1);
	}
	i=posix_memalign((void**)&m, 16, 4*sizeof(float));

	if(i!=0)
	{
		printf("error fNewPix");
		free (fPix);
		free (fConst);
		free (fNewPix);
		free (fUp);
		free (fSame);
		free (fDown);
		exit (1);
	}
	
	vfPix=(__m128*)fPix;
	vfConst=(__m128*)fConst;
	vfUp=(__m128*)fUp;
	vfSame=(__m128*)fSame;
	vfDown=(__m128*)fDown;
	vfm=(__m128*)m;
	
	for (i=0;i<N;i++)
	{
		for(j=0;j<M;j++)
		{
			fPix[i*M+j] = (rand()%16);
			fConst[i*M+j] = 0.5;
			fNewPix[i*M+j]=0.0;
		}

	}
	
	for(a=3;a<3;a++)
	{
		fUp[a]=0.0;
		fSame[a]=0.0;
		fDown[a]=0.0;
	}

	getWallTime(&ts);
	
	count=0;

	while(count<N*M)
	{
		*vfConst= _mm_mul_ps(*vfPix,*vfConst);
		vfPix++;
		vfConst++;
		count+=4;
	}

	r= count%(N*M);
	i=N-1;
	for(j=(M-r);j<M;j++)
	{
		fConst[i*M+j] = fPix[i*M+j]*fConst[i*M+j];
	}

	/*for (i=0;i<N;i+=4)
	{
			if (count<N*M)
			{
				*vfConst= _mm_mul_ps(*vfPix,*vfConst);
				vfPix++;
				vfConst++;
				count+=4;
			}
			else
			{
				r= count%(N*M);
				i=N-1;
				for(j=(M-r);j<M;j++)
				{
					fConst[i*M+j] = fPix[i*M+j]*fConst[i*M+j];
				}
			}
	
	}*/

	for (i=1;i<N-1;i++)
	{
		for(j=1;j<M-1;j++)
		{
			fUp[0]= fConst[(i-1)*M+(j-1)];
			fUp[1]= fConst[(i-1)*M+(j)];
			fUp[2]= fConst[(i-1)*M+(j+1)];
			fSame[0]= fConst[(i)*M+(j-1)];
			fSame[1]= fConst[(i)*M+(j)];
			fSame[2]= fConst[(i)*M+(j+1)];
			fDown[0]= fConst[(i+1)*M+(j-1)];
			fDown[1]= fConst[(i+1)*M+(j)];
			fDown[2]= fConst[(i+1)*M+(j+1)];

			*vfm=_mm_add_ps(*vfUp,*vfSame);
			*vfm=_mm_add_ps(*vfm,*vfDown);
	
			fNewPix[(i)*M+(j)]=m[0]+m[1]+m[2];
		}
	
	}

	
	getWallTime(&te);

	
	free(fPix);
	free(fConst);
	free(fNewPix);
	free(fUp);
	free(fSame);
	free(fDown);
	free(m);

	

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
