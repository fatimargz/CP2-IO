#include <vector>
#include <cstdio> 
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TTreePerfStats.h"

//Originally authored by the ROOT Team: https://root.cern/doc/master/hvector_8C.html

int ntodo = 10;

void read(char *fin, char *fout)
{

   TH1F *hdisk = new TH1F("hdisk","hdisk",500,0,100);   

   for (int iter = 0; iter < ntodo; iter++) {
      TFile *f = TFile::Open(fin,"READ");
      if (!f) { return; }
     

      TTree *t; f->GetObject("tvec",t);
      int N = t->GetEntries();
      
      std::vector<float> *vpx = 0;
      std::vector<float> *vpy = 0;
      std::vector<float> *vpz = 0;
      std::vector<float> *vpt = 0;
      std::vector<float> *vint = 0;      
      
      TTreePerfStats* myIOStats = new TTreePerfStats("IOPerfStats", t);
      
      TBranch *bvpx = 0;
      t->SetBranchAddress("vpx",&vpx,&bvpx);
      
      TBranch *bvpy = 0;
      t->SetBranchAddress("vpy",&vpy,&bvpy);
      
      TBranch *bvpz = 0;
      t->SetBranchAddress("vpz",&vpz,&bvpz);
      
      TBranch *bvpt = 0;
      t->SetBranchAddress("vpt",&vpt,&bvpt);
      
      TBranch *bvint = 0;
      t->SetBranchAddress("vint",&vint,&bvint);
      
      
      for (Int_t i = 0; i < N; i++) {
         Long64_t tentry = t->LoadTree(i);
         bvpx->GetEntry(tentry);
         bvpy->GetEntry(tentry);
         bvpz->GetEntry(tentry);
         bvpt->GetEntry(tentry);
         bvint->GetEntry(tentry);            
      
      }

      hdisk->Fill(myIOStats->GetDiskTime());
      // Since we passed the address of a local variable we need
      // to remove it.
      t->ResetBranchAddresses();
      if (iter == 0) { 
	      std::cerr << std::endl;
	      myIOStats->SaveAs(fout); // save only once
      }
      f->Close();
   }
   std::cerr << std::endl;
   std::cerr << "Disk Mean = " << hdisk->GetMean() << " and RMS/sqrt(N) = " << hdisk->GetRMS()/sqrt(ntodo) << std::endl;   
   delete hdisk;
}
 
 
int main()
{
   int N = 1e6; // match this with N in writeROOTFile.cpp
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
   for (auto basket : baskets) {
   
      gBenchmark = new TBenchmark();
      // Timed read()
      gBenchmark->Start("read_hvector");
      std::cerr << std::endl;
      std::cerr << "<==== Basket Setting: @ " << basket << "====>" << std::endl;
      char *fname_in=Form("hvector_%d_basket%d_50_floats.root",N,basket);
      char *fname_out=Form("stats_%d_basket%d_50_floats.root",N,basket);            
      read(fname_in,fname_out);
      gBenchmark->Show("read_hvector");
      delete gBenchmark;
   }
   
   return 0;
}
