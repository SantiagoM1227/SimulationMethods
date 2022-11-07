#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

#define Lx 16
#define Nx 8

const int Mx = (Lx+Nx-1)/Nx;

//----------Programa del Device------------------
//-----------------KERNELS-----------------------
__global__ void AddTwoVectors(float * d_a, float * d_b, float * d_c){
  //¿Qué tarea me toca?
  int ix; ix = blockIdx.x*blockDim.x+threadIdx.x;
  d_c[ix]=d_a[ix]+d_b[ix];
}
//------------Código de Host---------------------
int main(void){
  int ix;
  //Declarar todas las variables-----------------
  float h_a[Lx],h_b[Lx],h_c[Lx];
  //----------En el device-----------------------
  float *d_a; cudaMalloc((void**)&d_a, Lx*sizeof(float));
  float *d_b; cudaMalloc((void**)&d_b, Lx*sizeof(float));
  float *d_c; cudaMalloc((void**)&d_c, Lx*sizeof(float));
  
  //Inicializar variables------------------------
  
  for (ix = 0; ix<Lx; ix++){
    h_a[ix]=ix; h_b[ix]=2*ix;
  }
  
  //Enviar al Device
  cudaMemcpy(d_a,h_a,Lx*sizeof(float),cudaMemcpyHostToDevice);
  cudaMemcpy(d_b,h_b,Lx*sizeof(float),cudaMemcpyHostToDevice);
  
  //Correr en el Device
  dim3 ThreadsPerBlock(Nx,1,1);
  dim3 BlocksPerGrid(Mx,1,1);
  
  AddTwoVectors<<<BlocksPerGrid,ThreadsPerBlock>>>(d_a,d_b,d_c);
  
  //Devolver el resultado al Host
  
  cudaMemcpy(h_c,d_c,Lx*sizeof(float),cudaMemcpyDeviceToHost);
  //Imprimir los resultados
  
  for (ix = 0; ix<Lx; ix++){
    cout<<h_c[ix]<<endl;
  }
  
  
  return 0;
}
