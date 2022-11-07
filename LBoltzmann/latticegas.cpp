#include <iostream>
#include <cmath>
#include "/home/santi/Documents/Simulación/Random64.h"


using std::cout;
using std::endl;

// probabilidad p de giro 0°
// probabilidad 1-p de giro 180°


const int Lx= 1024;
const double p = 0.5;

const int Q = 2; //# de posibles estados
//-------------------------Clase LatticeGas----------------
class Latticegas{
private:

  // EL autómata trabaja en paralelo
  // lee lel estado actual y actualiza globalmente
  
  int n[Lx][Q], nnew[Lx][Q];  //estados actual y nuevo del sistema
  
  //matriz de direcciones
  int V[Q]; //V[i] i = 0(derecha) i = 1 (izquierda)
  
   
public:
  Latticegas(void);
  void Borrese(void);
  void Inicie(int N, double mu, double sigma, Crandom & ran64);
  //se pasa el ran64 por referencia para generar DISTINTOS valores
  void Show(void);
  void Colisione(Crandom & ran64);
  void Adveccione(void);
  double rho(int ix){return n[ix][0]+n[ix][1];};
  double varianza(void);

};
//-------------funciones de la clase-----------------

Latticegas::Latticegas(void){
  V[0]=1; V[1]=-1;
}

void Latticegas::Borrese(void){
  for (int ix = 0; ix<Lx; ix++){
    for (int i=0; i<Q;i++){
      n[ix][i]=nnew[ix][i]=0;
    }
  }
}

void Latticegas::Inicie(int N, double mu, double sigma, Crandom & ran64){

  //Mietras (N>0)
  //  Escoger una celda al azar
  //  Escoger una dirección al azar;
  //  si (el sitio está vacío)
  //    int N, double mu, double sigma, Crandom & ran64)
  //  si (no) 
  //    busco otro sitio
  int ix, i;
  while (N>0){
    ix = (int) ran64.gauss(mu, sigma);
    if (ix<0) ix = 0; if (ix> Lx-1) ix = Lx-1;    
    i = (int) Q*ran64.r();
    if (n[ix][i]==0){
      n[ix][i]=1; N--;
    }
    
  }
}
  
void Latticegas::Show(void){
  for (int i = 0; i<Q; i++){
    for (int ix=0; ix<Lx;ix++){
      cout<<n[ix][i];
    }
    cout<<endl;
  }  
}


void Latticegas::Colisione(Crandom & ran64){
  for (int ix = 0; ix<Lx; ix++){
    if (ran64.r()>p){
      nnew[ix][0]= n[ix][1]; nnew [ix][1]=n[ix][0];
    }
  }  
}


void Latticegas::Adveccione(void){
  for (int i = 0; i<Q; i++){
    for (int ix=0; ix<Lx;ix++){
      n[(ix+V[i]+Lx)%Lx][i]=nnew[ix][i];
    }
  }  
}

double Latticegas::varianza(void){
  int ix; double N=0, Xprom=0, Sigma2=0;
  for(ix = 0; ix<Lx; ix++){
    N+=rho(ix);
  }
  for(ix = 0; ix<Lx; ix++){
    Xprom+= ix*rho(ix);
  }
  Xprom/=N;
  for (ix = 0; ix<Lx;ix++){
    Sigma2 += pow(ix-Xprom,2.0)*rho(ix);
  }
  Sigma2 /= (N-1);  
  return Sigma2;
}


//---------------Programa principal---------------------------



int main(void){
  Latticegas Difusion;
  Crandom ran64(1);
  int N=6; double mu = Lx/2, sigma = Lx/8; 
  int t, tmax= 400;
  
  Difusion.Borrese();
  Difusion.Inicie(N, mu, sigma, ran64); 
   
  for(t=0; t<tmax; t++){
    cout<<t<<"\t"<<Difusion.varianza()<<endl;
    Difusion.Colisione(ran64);
    Difusion.Adveccione();    
  }  
  return 0;

}
