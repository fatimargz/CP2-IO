#include <vector>
 
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"

void read(char *filename, char *plot_title)
{

   TFile *f = TFile::Open(filename,"READ");

   if (!f) { return; }

   TTree *t; f->GetObject("tvec",t);

   std::vector<float> *vpx = 0;

  // Create a new canvas.
   TCanvas *c1 = new TCanvas("c1","Dynamic Filling Example",200,10,700,500);

   const Int_t kUPDATE = 1000;

   TBranch *bvpx = 0;
   t->SetBranchAddress("vpx",&vpx,&bvpx);


   // Create one histograms
   TH1F *h = new TH1F("h","This is the px distribution",100,-4,4);
   h->SetFillColor(48);

   for (Int_t i = 0; i < 25000; i++) {

      Long64_t tentry = t->LoadTree(i);
      bvpx->GetEntry(tentry);

      for (UInt_t j = 0; j < vpx->size(); ++j) {

         h->Fill(vpx->at(j));

      }
      if (i && (i%kUPDATE) == 0) {
         if (i == kUPDATE) h->Draw();
         c1->Modified();
         c1->Update();
         if (gSystem->ProcessEvents())
            break;
      }
   }
   // Since we passed the address of a local variable we need
   // to remove it.
   h->Draw();
   c1->SaveAs(plot_title);
   t->ResetBranchAddresses();
}

int main(){
	int N = 1e6; // match this with N in writeROOTFile.cpp
   	std::vector<int> flushes;
   	flushes.emplace_back(0);
   	flushes.emplace_back(-30000000);
   	flushes.emplace_back(-1000000);
   	flushes.emplace_back(1e4);
   	flushes.emplace_back(1e2);
   	for (auto flush : flushes) {
		char *fname=Form("hvector_%d_%d_50.root",N,flush);
		char *title=Form("vpx_%d_%d_50.png",N,flush);
		read(fname, title);
	}
}


