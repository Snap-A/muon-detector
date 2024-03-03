#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"

#include "CRYGenerator.h"
#include "CRYSetup.h"

#ifndef CRY_DATA
#warn "Missing 'CRY_DATA' definition"
#endif

const int MAX_PARTICLES = 48;
const int TOTAL_EVENTS = 1000000;

int main(int argc, const char *argv[])
{
   if (argc < 2)
   {
      std::cout << "usage " << argv[0] << " <setup file name>" <<std::endl;
      return 0;
   }

   // Read the setup file into setupString
   std::ifstream inputFile;
   inputFile.open(argv[1], std::ios::in);
   char buffer[1000];

   std::string setupString("");
   while (!inputFile.getline(buffer, 1000).eof())
   {
      setupString.append(buffer);
      setupString.append(" ");
   }

   // Parse the contents of the setup file
   CRYSetup *setup=new CRYSetup(setupString, CRY_DATA);

   // Setup the CRY event generator
   CRYGenerator gen(setup);

   // Setup ROOT output
   TFile f("muon_cry.root", "recreate");

   // The TTree instance holding the event data
   TTree t1("cry", "Tree CRY event data");

   // Memory used to fill the data.
   Int_t npart;

   // Memory for origin and direction of particle
   Float_t x[MAX_PARTICLES];
   Float_t y[MAX_PARTICLES];
   Float_t z[MAX_PARTICLES];

   Float_t u[MAX_PARTICLES];
   Float_t v[MAX_PARTICLES];
   Float_t w[MAX_PARTICLES];

   // Total energy, time at origin, and particle ID
   Float_t kE[MAX_PARTICLES];
   Float_t tO[MAX_PARTICLES];
   Int_t   pdg[MAX_PARTICLES];

   // The total number of particles in a branch
   t1.Branch("npart", &npart, "npart/I");

   // Particle properties
   t1.Branch("x", &x, "x[npart]/F");
   t1.Branch("y", &y, "y[npart]/F");
   t1.Branch("z", &z, "z[npart]/F");

   t1.Branch("u", &u, "u[npart]/F");
   t1.Branch("v", &v, "v[npart]/F");
   t1.Branch("w", &w, "w[npart]/F");

   t1.Branch("kE", &kE, "kE[npart]/F");
   t1.Branch("tO", &tO, "tO[npart]/F");
   t1.Branch("pdg", &pdg, "pdg[npart]/I");

   // Generate the events
   std::vector<CRYParticle*> *ev=new std::vector<CRYParticle*>;
   for (int i=0; i<TOTAL_EVENTS; i++)
   {
      ev->clear();
      gen.genEvent(ev);

      // Save number of particles, and set limit
      npart = ev->size();
      if (npart > MAX_PARTICLES)
         npart = MAX_PARTICLES;
      
      for (unsigned j=0; j<npart; j++)
      {
         CRYParticle* p = (*ev)[j];

         // Create ROOT event data
         x[j] = (Float_t)p->x();
         y[j] = (Float_t)p->y();
         z[j] = (Float_t)p->z();

         u[j] = (Float_t)p->u();
         v[j] = (Float_t)p->v();
         w[j] = (Float_t)p->w();

         pdg[j] = (Int_t)p->PDGid();
         kE[j] = (Float_t)p->ke();
         tO[j] = (Float_t)p->t();

         // Delete event data
         delete (*ev)[j];
      }

      // Store current data values
      t1.Fill();
   }

   // Write all data
   f.cd();
   t1.Write();

   std::cout << "Done!" <<std::endl;
   return 0;
}
