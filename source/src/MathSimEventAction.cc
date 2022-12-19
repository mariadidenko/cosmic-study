#include "MathSimEventAction.hh"
#include "MathSimRunAction.hh"
#include "MathSimAnalysis.hh"
#include "MathSimScintillatorHit.hh"
#include "MathSimConstants.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


using std::array;
using std::vector;


namespace {

// Utility function which finds a hit collection with the given Id
// and print warnings if not found
G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
    auto hce = event->GetHCofThisEvent();
    if (!hce) {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found." << G4endl;
        G4Exception("MathSimEventAction::EndOfEventAction()",
                    "MathSimCode001", JustWarning, msg);
        return nullptr;
    }

    auto hc = hce->GetHC(collId);
    if ( ! hc) {
      G4ExceptionDescription msg;
      msg << "Hits collection " << collId << " of this event not found." << G4endl;
      G4Exception("MathSimEventAction::EndOfEventAction()",
                  "MathSimCode001", JustWarning, msg);
    }
    return hc;
  }
}


MathSimEventAction::MathSimEventAction()
  : G4UserEventAction(),
    fDriftHCID  {{ -1 }},
    fDriftHistoID {{ -1 }}
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
   
MathSimEventAction::~MathSimEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimEventAction::BeginOfEventAction(const G4Event* )
{  
  // initialisation per event

  // Beam variables
  fPrimariesPDG.clear();
  fPrimariesEnergy.clear();
  fPrimariesX.clear();
  fPrimariesY.clear();
  fPrimariesZ.clear();
  fPrimariesThetaDir.clear();
  fPrimariesPhiDir.clear();
  fParentIDOfSec.clear();
  fKlongSecondaryID = 0;
  
 //angle variables
  fScatAngle1 = 0.0;
  fScatAngle2 = 0.0;
  
  //count hits each layer
  fCountHitEachLayer = 0;
    
  // Classification of events
    
  isCleanEvent = false;
  isKLongEvent = false;
  isKShortEvent = false;
  isInteractEvent = false;
    
  // Hit variables
  fSiHitsID.clear();
  fSiHitsX.clear();
  fSiHitsY.clear();
  fSiHitsZ.clear();
  fSiHitsEdep.clear();
  fSiHitsTime.clear();
  fSiHitsIsPrimary.clear();
  fSiHitsIsSecondary.clear();
  fSiHitsTrackID.clear();
  fSiHitsPDG.clear();
  fSiHitsCharge.clear();
  

  // Primary particle steps:   
  fPrimaryPosX.clear();
  fPrimaryPosY.clear();
  fPrimaryPosZ.clear();
  fPrimaryEdep.clear();
  fPrimaryPx.clear();
  fPrimaryPy.clear();
  fPrimaryPz.clear();
  fPrimaryPtotal.clear();
  fPrimaryEtotal.clear();
  fPrimaryTime.clear();
  fPrimaryTrackLength.clear();
  fPrimaryTotalElectrons=0;
  fPrimaryTotalPhotons=0;
  
  //secondary particle information:
  fSecondaryPDG.clear();
  fProcess.clear();
  
  fSecPosX.clear();
  fSecPosY.clear();
  fSecPosZ.clear();
  //
  fSecXVertex.clear();
  fSecYVertex.clear();
  fSecZVertex.clear();
  //
  fSecDirPx.clear();
  fSecDirPy.clear();
  fSecDirPz.clear();
  //
  fSecEdep.clear();
  //
  fSecondaryPx.clear();
  fSecondaryPy.clear();
  fSecondaryPz.clear();
  //
  fSecondaryTime.clear();
  fSecCharge.clear();
  fSecTrackID.clear();
  fSecParentID.clear();
  fSecTrackLength.clear();
    
  fSecPtotal.clear();
  fSecEtotal.clear();

  fpi0TrkID.clear();
  fpi0GammaTrkID.clear();
  
  //
  fTotalSecondary = 0;
  
  //
  fcount=0;
  
  // Material map variables:
  fMatMapScintillator=0.0;
  fMatMapLayerSupport=0.0;
  fMatMapSteelBar=0.0;
  fMatMapScintillator_radLen=0.0;
  fMatMapLayerSupport_radLen=0.0;
  fMatMapSteelBar_radLen=0.0;

  // Find hit collections and histogram Ids by names (just once)
  // and save them in the data members of this class   
  
  if (fDriftHCID[0] == -1) {
    auto sdManager = G4SDManager::GetSDMpointer();
    auto analysisManager = G4AnalysisManager::Instance();
    //
    //    // Hits collection names0
    array<G4String, kDim> dHCName = {"chamber1/scintillatorColl"};

    // histograms names
    array<G4String, 2> histoName =  {{ "Tracker1", "Tracker1XY"}};
    
    for (G4int iDet = 0; iDet < kDim; ++iDet) {
      // hit collections IDs
      fDriftHCID[iDet] = sdManager->GetCollectionID(dHCName[iDet]);
      fDriftHistoID[kH1] = analysisManager->GetH1Id(histoName[kH1]);
      fDriftHistoID[kH2] = analysisManager->GetH2Id(histoName[kH2]);
    }
  }//end if
} // end

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimEventAction::EndOfEventAction(const G4Event* event)
{
  // Accumulate statistics
  
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Module z position:
  auto worldSizeZ  = 1.2 * (moduleLength+rockLength+rockDistance);
  G4double module_zposition = worldSizeZ/2-0.1/1.2*worldSizeZ-moduleLength;

  // Primary input particles
  //
  analysisManager->FillNtupleIColumn(0, event->GetEventID());
  // fill for all primary input particles
  for (G4int iVertex = 0; iVertex < event->GetNumberOfPrimaryVertex();
       iVertex++) {
    auto vertex = event->GetPrimaryVertex(iVertex);
    fPrimariesX.push_back(vertex->GetX0() / CLHEP::cm);
    fPrimariesY.push_back(vertex->GetY0() / CLHEP::cm);
    fPrimariesZ.push_back(vertex->GetZ0() / CLHEP::cm);
    
    for (G4int iParticle = 0; iParticle < vertex->GetNumberOfParticle();
         iParticle++) {
      auto particle = vertex->GetPrimary(iParticle);
      fPrimariesPDG.push_back(particle->GetPDGcode());
      fPrimariesEnergy.push_back(particle->GetTotalEnergy() / CLHEP::GeV);

      auto beamDir = particle->GetMomentumDirection();
      fPrimariesThetaDir.push_back(beamDir.theta());
      fPrimariesPhiDir.push_back(beamDir.phi());      
    }
  }
    
    // *******
    // ******* Classification:
    // *******
    
    // Classification of all events:
    
    int decay[3] = {0, 0, 0};
    int vertex[3] = {0, 0, 0};//vertex for pi+ pi- pi 0
    int vertex2[3] = {0, 0, 0};//vertex for pi+/- and lep
    int decay2[3] = {0, 0, 0};
    int decay3[3] = {0, 0, 0};
    vector<double> vertex3;
    int num_pi0 = 0;
    
    bool ThreePionsDecay = false; // pion+, pion-, pion0
    bool PionAndLeptonDecay = false;//pion+/- and lepton
    bool TwoPhotonsDecay = false;// photons decay
    bool OtherDecay = false;
    
    for(int i = 0; i<fSecondaryPDG.size(); i++){
        
        if(fSecondaryPDG.at(i) == 211) {decay[0] = 1; vertex[0] = fSecZVertex.at(i);}
        if(fSecondaryPDG.at(i) == -211) {decay[1] = 1; vertex[1] = fSecZVertex.at(i);}
        if(fSecondaryPDG.at(i) == 111) {decay[2] = 1; vertex[2] = fSecZVertex.at(i);}
            
        if(abs(fSecondaryPDG.at(i)) == 211) {decay2[0] = 1; vertex2[0] = fSecZVertex.at(i);}
        if(abs(fSecondaryPDG.at(i)) == 11) {decay2[1] = 1; vertex2[1] = fSecZVertex.at(i);}
        if(abs(fSecondaryPDG.at(i)) == 13) {decay2[1] = 1; vertex2[1] = fSecZVertex.at(i);}
        if(abs(fSecondaryPDG.at(i)) == 12) {decay2[2] = 1; vertex2[2] = fSecZVertex.at(i);}
        if(abs(fSecondaryPDG.at(i)) == 14) {decay2[2] = 1; vertex2[2] = fSecZVertex.at(i);}
              
        if(abs(fSecondaryPDG.at(i)) == 111){
          num_pi0 ++;
          vertex3.push_back(fSecZVertex.at(i));
            }
    }
    
    //****************
    //**************** Is it K long decay to pi+ pi- pi0 ?
    //****************
    
    if((decay[0] + decay[1]  + decay[2]) == 3 && (vertex[0] == vertex[1] && vertex[1] == vertex[2])) ThreePionsDecay = true;
    
    //****************
    //**************** Is it K long decay to pi+/pi- and el/mu ?
    //****************
    
    if((decay2[0] + decay2[1]  + decay2[2]) == 3 && vertex2[0] == vertex2[1] && vertex2[1] == vertex2[2]) PionAndLeptonDecay = true;
    
     //****************
     //**************** Is it K long decay to 3pi0->6gammas ?
     //****************
     
    if(num_pi0 == 3 && vertex3.at(0) == vertex3.at(1) && vertex3.at(1) == vertex3.at(2)) TwoPhotonsDecay = true;
     
    if (!ThreePionsDecay && !PionAndLeptonDecay && !TwoPhotonsDecay) OtherDecay = true;
        
    // define clean event:
    
    int N_sec = 0;
    if(fKlongSecondaryID == 1 && (ThreePionsDecay || PionAndLeptonDecay || TwoPhotonsDecay)){// trackID of K long
        
        // How many secondaries we have with parent ID = 1:

        bool atom_part = false;
        for(int i = 0; i<fSecParentID.size(); i++){
            if(fSecParentID.at(i)==1) N_sec ++;
//            if (fSecondaryPDG.at(i) == 1000060120 || fSecondaryPDG.at(i) == 2212 || fSecondaryPDG.at(i) == 1000070140) atom_part = true;
            
        }
        if (N_sec == 3 || N_sec == 4) isCleanEvent = true;
        
        // to check the particles which produced with main decays:
        
        if (N_sec == 4){
            std::cout<<"Particles which produced with main modes"<<std::endl;
//            std::cout<<"New decay: "<<std::endl;
            for (int i = 0; i < fSecondaryPDG.size(); i++){
                std::cout<<"PDGs: "<<fSecondaryPDG.at(i)<<" ";
            }
            std::cout<<std::endl;
            
        }
    }
        
    // define K longs event:
    
    int N_sec_with_KLong = 0;
    int NSecondaryOfKLongs = 0;
    
    if(fKlongSecondaryID > 1) {
    
        for(int i = 0; i<fSecParentID.size(); i++){
            if(fSecParentID.at(i)==1) N_sec_with_KLong ++;
        }
        
        // how many secondaries we have from KLong case:
        
    //        int vertexKLong = 10000;
    //
    //        for(int i = 0; i<fSecondaryPDG.size(); i++){
    //            if(fSecondaryPDG.at(i)==130) vertexKLong = fSecZVertex.at(i);
    //        }
            
            for(int i = 0; i<fSecondaryPDG.size(); i++){
                if(fSecParentID.at(i)>1 && fSecondaryPDG.at(i)!=130) NSecondaryOfKLongs ++;
            }
            
        if((NSecondaryOfKLongs == 3 || NSecondaryOfKLongs == 4) && (ThreePionsDecay || PionAndLeptonDecay || TwoPhotonsDecay)) {
            isCleanEvent = true;
        } else {
            isKLongEvent = true;
        }
    }
    
    // define K short event:
    
    if (isCleanEvent == false && isKLongEvent == false){
        for(int i = 0; i<fSecondaryPDG.size(); i++){
            if(fSecondaryPDG.at(i)==310) isKShortEvent = true;
        }
    }
    
    
    if (!isCleanEvent && !isKLongEvent && !isKShortEvent){
        if (ThreePionsDecay || PionAndLeptonDecay || TwoPhotonsDecay) isInteractEvent = true;
    }

    std::cout<<"General modes "<<std::endl;
    
    std::cout<<"ThreePionsDecay: "<<ThreePionsDecay<<std::endl;
    std::cout<<"PionAndLeptonDecay: "<<PionAndLeptonDecay<<std::endl;
    std::cout<<"TwoPhotonsDecay: "<<TwoPhotonsDecay<<std::endl;
    std::cout<<"OtherDecay: "<<OtherDecay<<std::endl;
    
//    std::cout<<std::endl;
//    std::cout<<"What exact event we have: "<<std::endl;
//    std::cout << "Clean event: " << isCleanEvent <<std::endl;
//    //if (N_sec > 3) std::cout << "No clean event: N seconadries" << N_sec <<std::endl;
//    std::cout<<"isInteractEvent: "<<isInteractEvent<<std::endl;
//    std::cout << "K long event: " << isKLongEvent <<std::endl;
//    //std::cout << "Number of secondaries: " << N_sec_with_KLong <<std::endl;
//    std::cout <<"NSecondaryOfKLongs: " <<  NSecondaryOfKLongs <<std::endl;
//    std::cout << "KShort event: " <<  isKShortEvent <<std::endl;

    // ********* for New New New clasification  with Decay Children:
    
    int NumDecayChildren = 0;
    int NumDecayChildrenAfterInteract = 0;
    int NumSecKShort = 0;
    int NumSecKLong = 0;
    
    for (int i = 0; i < fProcess.size(); i++){
        if (fProcess.at(i) == 0 && fSecParentID.at(i)==1) NumDecayChildren ++;
        if (fProcess.at(i) == 0 && fSecParentID.at(i)>1) NumDecayChildrenAfterInteract ++;
        if (fSecondaryPDG.at(i) == 310 && fSecParentID.at(i) > 0 ) NumSecKShort ++;
        if (fSecondaryPDG.at(i) == 130 && fSecParentID.at(i) > 0 ) NumSecKLong ++;
    }
    
    std::cout<<std::endl;
    std::cout<<"How many decay particles we have: "<<std::endl;
    std::cout << "NumDecayChildren: " << NumDecayChildren <<std::endl;
    //if (N_sec > 3) std::cout << "No clean event: N seconadries" << N_sec <<std::endl;
    std::cout<<"NumDecayChildrenAfterInteract: "<<NumDecayChildrenAfterInteract<<std::endl;
    std::cout << "NumSecKShort " << NumSecKShort <<std::endl;
    //std::cout << "Number of secondaries: " << N_sec_with_KLong <<std::endl;
    std::cout <<"NumSecKLong: " <<  NumSecKLong <<std::endl;
    
  analysisManager->FillNtupleIColumn(30, fPrimaryTotalElectrons);
  analysisManager->FillNtupleIColumn(31, fPrimaryTotalPhotons);
  analysisManager->FillNtupleDColumn(32, fMatMapScintillator);
  analysisManager->FillNtupleDColumn(33, fMatMapLayerSupport);
  analysisManager->FillNtupleDColumn(34, fMatMapSteelBar);
  analysisManager->FillNtupleDColumn(35, fMatMapScintillator_radLen);
  analysisManager->FillNtupleDColumn(36, fMatMapLayerSupport_radLen);
  analysisManager->FillNtupleDColumn(37, fMatMapSteelBar_radLen);
  analysisManager->FillNtupleDColumn(38, fScatAngle1);
  analysisManager->FillNtupleDColumn(39, fScatAngle2);

  analysisManager->FillNtupleDColumn(61, isCleanEvent);
  analysisManager->FillNtupleDColumn(62, isKLongEvent);
  analysisManager->FillNtupleDColumn(63, isKShortEvent);
  analysisManager->FillNtupleDColumn(64, isInteractEvent);
    
  analysisManager->FillNtupleIColumn(65, N_sec);
  analysisManager->FillNtupleIColumn(66, N_sec_with_KLong);
  analysisManager->FillNtupleIColumn(67, NSecondaryOfKLongs);
  analysisManager->FillNtupleIColumn(68, fKlongSecondaryID);
    
  analysisManager->FillNtupleDColumn(69, ThreePionsDecay);
  analysisManager->FillNtupleDColumn(70, PionAndLeptonDecay);
  analysisManager->FillNtupleDColumn(71, TwoPhotonsDecay);
  analysisManager->FillNtupleDColumn(72, OtherDecay);
    
  analysisManager->FillNtupleDColumn(74, NumDecayChildren);
  analysisManager->FillNtupleDColumn(75, NumDecayChildrenAfterInteract);
  analysisManager->FillNtupleDColumn(76, NumSecKShort);
  analysisManager->FillNtupleDColumn(77, NumSecKLong);
  
  // Scintillator layer hits
  for (G4int iDet = 0; iDet < kDim; ++iDet) {
    auto hc = GetHC(event, fDriftHCID[iDet]);
    if ( ! hc ) return;

    auto nhit = hc->GetSize();
    analysisManager->FillH1(fDriftHistoID[kH1], nhit );
    
    for (unsigned long i = 0; i < nhit; ++i) {
      auto hit = static_cast<MathSimScintillatorHit*>(hc->GetHit(i));
      auto localPos = hit->GetLocalPos();
      auto worldPos = hit->GetWorldPos();
      
      analysisManager->FillH2(fDriftHistoID[kH2], localPos.x()/ CLHEP::cm, localPos.y()/CLHEP::cm);
      fSiHitsID.push_back(hit->GetLayerID());
      fSiHitsX.push_back(localPos.x()/ CLHEP::cm);
      fSiHitsY.push_back(localPos.y()/ CLHEP::cm);
      fSiHitsZ.push_back((worldPos.z() - module_zposition )/ CLHEP::cm);
      fSiHitsEdep.push_back(hit->GetEdep()/CLHEP::MeV);
      fSiHitsTime.push_back(hit->GetTime()/CLHEP::ns);
      fSiHitsIsPrimary.push_back(int(hit->GetIsPrimary()));
        
        fSiHitsIsSecondary.push_back(int(hit->GetIsSecondary()));
        fSiHitsTrackID.push_back(hit->GetTrackID());
        fSiHitsPDG.push_back(hit->GetPDG());
        fSiHitsCharge.push_back(hit->GetCharge());
    }
  }//end of Drift scintillator layer hits

  for (G4int iDet = 0; iDet < kDim; ++iDet) {
    auto hc = GetHC(event, fDriftHCID[iDet]);
    if ( ! hc ) return;
    G4cout << "Module " << iDet + 1 << " has " <<  hc->GetSize()  << " hits." << G4endl;
    for (auto layer = 0; layer < kNofChambers; ++layer) {
     
      fCountHitEachLayer = 0;
      fCountChildPos = 0;
      fCountChildNeg = 0;
        
      for (unsigned int i = 0; i < hc->GetSize(); i++) {
        auto hit = static_cast<MathSimScintillatorHit*>(hc->GetHit(i));
          if (hit->GetLayerID() == layer){
              hit->Print();
              fCountHitEachLayer ++;
              if(hit->GetParentID() == 1){
                  
                  if(hit->GetCharge() == -1) fCountChildPos ++;
                  if(hit->GetCharge() == 1) fCountChildNeg ++;
              }
              
          }
      }
      G4cout<<"--- Count hits of "<<"layer["<<layer<<"] = "<< fCountHitEachLayer << G4endl;
      G4cout<<"--- Secondary particle hits: "<<" positive - "<< fCountChildPos <<", negative - "<< fCountChildNeg << G4endl;
        
    }
  }
  
  analysisManager->AddNtupleRow();
  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
