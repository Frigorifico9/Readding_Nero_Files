
#include <iostream>                 // class for inputoutput streams
#include <stdio.h>                  // printf, scanf, puts, NULL
#include <stdlib.h>                 // srand, rand, atof
#include <time.h>                   // time
#include <fstream>                  // class for inputoutput file streams
#include <dirent.h>
#include <string.h>           // class to manipulate strings
#include <sstream>                    // class for parsing strings
#include <vector>


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TPad.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TString.h>
#include <TGraph.h>
#include <TLatex.h>



#include <unistd.h>
#define GetCurrentDir getcwd

#include <TTree.h>

using namespace std;
int main(int argc, char* argv[])
{
  //Cargamos el archivo con los datos
  TFile* file = TFile::Open("/Users/Fer/Documents/traajo/samples/NeroNtuples_9.root"); // TFile::Open() instead of a constructor since it works over xrootd etc.
  //Obtenemos el árbol con los datos
  TTree *tree=(TTree*)file->Get("nero/events");
  //Creamos el lienzo donde se hará la imagen
  TCanvas* c1 = new TCanvas("c1", "Masa W", 600, 600);
  //Aquí es donde la magia sucede
  tree->Draw("sqrt((lepP4.Energy())^2-(lepP4.Px()+lepP4.Py()+lepP4.Pz())^2)+sqrt((metP4.Energy())^2-(metP4.Px()+metP4.Py()+metP4.Pz())^2)","lepPdgId>abs(11)");
  //Primero decimos que vamos a usar el método "Draw" con datos del objeto "tree"
  //Sigue específicar que datos vamos a utilizar y como
  //La fórmula de la masa transversa es "m=√(E^2-|p|^2)"
  //Especificaremos hasta el final que usaremos la energía y momentum de los electrones
  //el como se especifica se explica al final
  //Primero abrimos la raíz cuadrada
  //Dentro de ella calculamos el cuadrado de la energía de cada evento de leptones (lepP4)
  //y le restamos la magnitud del vector de momentum de los leptones (lepP4)
  //Luego hacemos los mismo con la energía y el momentum perdidos
  //Otenemos dos masas en cada raíz y las sumamos
  //Para especificar que usaremos la energía y momentum de los electrones usamos "lepPdgId>abs(11)"
  //Que dice que de "lepP4" use solo los eventos con identificador 11 o -11, por eso el abs() 
  //Guardamos la imagen
  c1->SaveAs("masaW.pdf");
  c1->Close();
  return 0;
}
