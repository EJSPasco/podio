#include "ParticleCollection.h"
#include "EventInfoCollection.h"
#include "JetCollection.h"
#include "JetParticleAssociationCollection.h"

#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TROOT.h"
#include <vector>
#include <iostream>

// albers specific includes
#include "albers/EventStore.h"
#include "albers/Reader.h"
#include "albers/Registry.h"


void processEvent(albers::EventStore& store, bool verbose) {

  // read event information
  EventInfoCollection* evinfocoll(nullptr);
  bool evinfo_available = store.get("EventInfo",evinfocoll);
  if(evinfo_available) {
    const EventInfoHandle& evinfo = evinfocoll->get(0);
    if(verbose)
      std::cout << "event number " << evinfo.Number() << std::endl;
    // COLIN avoid bug at first event
    if(evinfo.Number()==0) return;
  }

  // read particles
  ParticleCollection* refs(nullptr);
  bool particles_available = store.get("Particle",refs);
  if (particles_available){
    if(verbose)
      std::cout << "particle collection:" << std::endl;
    for(const auto& ref : *refs){
      if(verbose)
	std::cout << "\tparticle: " << ref.ID() << " " << ref.P4().Pt << std::endl;
    }
  }

  // read particles
  JetCollection* jrefs(nullptr);
  bool jets_available = store.get("Jet",jrefs);
  if (jets_available){
    JetParticleAssociationCollection* jprefs(nullptr);
    bool assoc_available = store.get("JetParticleAssociation",jprefs);
    if(verbose)
      std::cout << "jet collection:" << std::endl;
    for(const auto& jet : *jrefs){
      if(verbose) {
	std::cout << "\tjet: " << jet.P4().Pt << std::endl;
	if(assoc_available) {
	  for(const auto& assoc : *jprefs){
	    if(assoc.Jet().isAvailable() && assoc.Particle().isAvailable()) {
	      if(assoc.Jet() == jet) {
		std::cout<<"\t\tassoc jet "<<assoc.Jet().P4().Pt<<std::endl;
	       	std::cout<<"\t\tassoc particle "<<assoc.Particle().ID()<<std::endl;
	      }
	    }
	  }
	}
      }
    }
  }
}


int main(){
  gSystem->Load("libDataModelExample.so");
  albers::Reader reader;
  albers::EventStore store(nullptr);
  store.setReader(&reader);
  reader.openFile("example.root");

  bool verbose = true;

  // unsigned nEvents = 10;
  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if(i%1000==0)
      std::cout<<"reading event "<<i<<std::endl;
    processEvent(store, verbose);
    store.endOfEvent();
    reader.endOfEvent();
  }
  return 0;
}
