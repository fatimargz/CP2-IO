#include <vector>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"

//Originally authored by the ROOT Team: https://root.cern/doc/master/hvector_8C.html


void write(int basketsize=4000, int N=1e6, int vecsize=50)
{
 
   TFile *f = TFile::Open(Form("hvector_%d_basket%d_%d_floats.root",N,basketsize,vecsize), "RECREATE");
   float percentComplete = 0.0f; 
   if (!f) { return; }
 
   std::vector<float> vpx;
   std::vector<float> vpy;
   std::vector<float> vpz;
   std::vector<float> vpt;   
   std::vector<float> vint;
 
   // Create a TTree
   TTree *t = new TTree("tvec","Tree with vectors");
   TBranch *b_vpx = t->Branch("vpx", &vpx);
   TBranch *b_vpy = t->Branch("vpy", &vpy);
   TBranch *b_vpz = t->Branch("vpz", &vpz);
   TBranch *b_vpt = t->Branch("vpt", &vpt);
   TBranch *b_vint = t->Branch("vint", &vint);

   // Apply basket size to each branch
   b_vpx->SetBasketSize(basketsize);
   b_vpy->SetBasketSize(basketsize);
   b_vpz->SetBasketSize(basketsize);
   b_vpt->SetBasketSize(basketsize);
   b_vint->SetBasketSize(basketsize);

   t->SetAutoFlush(0);

   gRandom->SetSeed();
   for (Int_t i = 0; i < N; i++) {
      Int_t npx = (Int_t)(gRandom->Rndm(1)*vecsize);
 
      vpx.clear();
      vpy.clear();
      vpz.clear();
      vpt.clear();
      vint.clear();            
 
      for (Int_t j = 0; j < npx; ++j) {
 
         Float_t px,py,pz,pt;
         Float_t int_2_float;
	 //Int_t integer;
         gRandom->Rannor(px,py);
         pt = sqrt(px*px + py*py);
         pz = gRandom->Rndm(1);
         //integer = (int)(gRandom->Rndm(1)*npx + npx);
 	 int_2_float = (gRandom->Rndm(1)*npx + npx);

         vpx.emplace_back(px);
         vpy.emplace_back(py);
         vpz.emplace_back(pz);
         
         /// this gets pushed back twice on purpose
         vpt.emplace_back(pt);
         vpt.emplace_back(pt);         
         //vint.emplace_back(integer);
	 vint.emplace_back(int_2_float);         
 
      }
      t->Fill();
   }
   
   t->Print();
   f->Write();
   
   delete f;
}
 
 
 
int main()
{
   int N = 1e6; // match this with N in readROOTFile.cpp
   int vecsize=50;
   std::vector<int> baskets;
   baskets.emplace_back(4000);
   baskets.emplace_back(8000);
   baskets.emplace_back(16000);
   baskets.emplace_back(32000);
   baskets.emplace_back(64000);
   baskets.emplace_back(128000);
   baskets.emplace_back(256000);
   baskets.emplace_back(512000);
   baskets.emplace_back(1024000);   
   const char* form;
   // Timed write()
   for (int basket : baskets) {
      std::cerr << "baskets = " << basket << " and N = " << N << " and vec size = " << vecsize << std::endl;
      gBenchmark = new TBenchmark();
      form =  Form("wr_hvec_N%d_F%d_Sz%d",N,basket,vecsize);
      gBenchmark->Start(form);
      write(basket,N); 
      std::cout << std::endl; 
      gBenchmark->Stop(form);
      //gBenchmark->Summary();
      delete gBenchmark;
   }

   return 0;
}
