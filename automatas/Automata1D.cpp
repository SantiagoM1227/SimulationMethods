#include <cmath>
#include "Random64.h"

using namespace std;

const int Lx = 1024; // Número de Celdas
const double p = 0.5; //Probabilidad de giro 

const int Q = 2; //Número de direcciones por casilla

//--------------------- Clase LatticeGas ------------
class LatticeGas{
private:
  int V[Q]; //V[i] i = 0(derecha) i = 1 (izquierda)
  int n[Lx][Q], nnew[Lx][Q]; //n[ix][i] indica ocupación en casilla[ix] y dirección[i]
public:
  LatticeGas(void);
  void Borrese(void);
  void Inicie(int N, double mu, double sigma, Crandom & ran64);
  void Show(void);
  void GrafiqueRho(void);
  void Colisione(Crandom & ran64);
  void Adveccione(void);
  double rho (int ix){return n[ix][0]+n[ix][1]:};
  double Varianza (void);
};
LatticeGas::LatticeGas(void){
  V[0] = 1; V[1]= -1;
}
void LatticeGas::Borrese(void){
  for (int ix=0, i = 0; ix< Lx, i<Q; ix++, i++){
    n[ix][i] = nnew[ix][i] =0;
  }
}
void LatticeGas::Inicie(int N, double mu, double sigma, Crandom & ran64){
  int ix, i;
  while (N>0){
    ix = (int) ran64.gauss(mu,sigma); //escoger celda al azar 
    if (ix<0) ix = 0; if (ix>Lx-1) ix = Lx-1;//bordes
    i = (int)Q*ram64.r(); //dirección al azar
    if (n[ix]==0){n[ix][i]=1; N--;}// se pone bolita y decrezco 
  }
}
void LatticeGas:Show(void){
  for (int ix = 0, i =0; ix<Lx, i<Q; ix++, i++){
	  cout n[ix][i];
	  cout<<endl;
  }
}
void LatticeGas::GrafiqueRho(void){
  for (int ix =0; ix<Lx; ix++)
	  cout <<ix<<"\t"<<rho(ix)<<endl;
}
void LatticeGas::Colisione(void){
  for (int ix  =0; ix<Lx; ix++)
	  if (ran64.r()>p)// numero al azar 
	    {nnew[ix][0]=n[ix][1]; nnew[ix][1]=n[ix][0];}//giro 180° si se cumple
          else
	    {nnew[ix][0]=n[ix][0]; nnew[ix][1]=n[ix][1];}

}

void LatticeGas::Adveccione(void){
  for (int ix = 0, i = 0; ix< Lx, i< Q; ix++, i++)
	  n[(ix+V[i]+Lx)%Lx][i]=nnew[ix][i];
}

double LatticeGas::Varianza(void){
  int ix; double N, Xprom, Sigma2;
  //Calcular N
  for (N = 0, ix = 0; ix<Lx; ix++)
	  N+=rho(ix);
  //Calcular Xprom
  for (Xprom = 0, ix=0; ix<Lx; ix++)
	 Xprom+= ix*rho(ix);
  Xprom/=N;
  //Calcular Sigma2
  for (Sigma2 = 0, ix = 0; ix<Lx; ix+)
	 Sigma2+= pow(ix-Xprom,2.0)*rho(ix);
  Sigma2/=(N-1); 
  return Sigma2;
}

