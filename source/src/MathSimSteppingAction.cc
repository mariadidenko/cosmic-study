#include "MathSimSteppingAction.hh"
#include "MathSimEventAction.hh"
#include "MathSimRunAction.hh"
#include "MathSimDetectorConstruction.hh"
#include "MathSimConstants.hh"

#include "G4VProcess.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4RunManager.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimSteppingAction::MathSimSteppingAction(
                      MathSimDetectorConstruction* detConstruction,
                      MathSimEventAction* eventAction,
                      MathSimRunAction* runAction)
  : G4UserSteppingAction(),
    fDetConstruction(detConstruction),
    fEventAction(eventAction),
    fRunAction(runAction)
{
  sum_teta = 0.;
  count = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimSteppingAction::~MathSimSteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimSteppingAction::UserSteppingAction(const G4Step* step)
{
  
  /// Check multiple scattering:
  MultipleScattering(step);

  /// Module Z position:
  auto worldSizeZ  = 1.2 * (moduleLength+rockLength+rockDistance);
  G4double module_zposition = worldSizeZ/2-0.1/1.2*worldSizeZ-moduleLength;
  
  /// Step information:
        // get volume of the current step
  
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    // energy deposit
    auto edep = step->GetTotalEnergyDeposit()/ CLHEP::MeV;
    auto epart = step->GetPreStepPoint()->GetKineticEnergy()/ CLHEP::MeV;
    // step length
    G4double stepLength = 0.;
    stepLength = step->GetStepLength()/CLHEP::cm;

  /// Store primary particle step information:
    
    /// store proton or muon depends of source we want: it is always GetParentID() == 0
    
    if(step->GetTrack()->GetTrackID()==1){
    
        auto PostStepPos = step->GetPostStepPoint()->GetPosition(); // Need to make sure, is it post step or pre step that we want to store?

        G4double PrimTime = step->GetTrack()->GetGlobalTime() / CLHEP::ns;
        fEventAction->StorePrimaryTime(PrimTime);
        fEventAction->StorePrimaryPosition(PostStepPos.x()/ CLHEP::cm, PostStepPos.y()/ CLHEP::cm, (PostStepPos.z() - module_zposition)/ CLHEP::cm);

        MaterialMap(step, stepLength); // build material map for primary only (geantinos dont interact)
        
      if(step->GetTrack()->GetCurrentStepNumber()==1){
          auto tracklength = step->GetTrack()->GetTrackLength()/CLHEP::mm;
          fEventAction->StorePrimaryTrackLength(tracklength);
          fEventAction->StorePrimaryEdep(epart);//clear store kinetic energy
          fEventAction->StorePrimaryKineticInfo(step->GetTrack()->GetDynamicParticle());

    // print information:
      
      G4cout<<std::setw(12)<<"Track#"<<std::setw(12)<<"Particle"<<std::setw(12)<<"PDG"<<std::setw(12)<<"Charge"<<std::setw(12)<<"VertexZ"<<std::setw(12)<<"ParentID"<<std::setw(14)<<"P total (MeV)"<<std::setw(12)<<"Step#"<<G4endl;
      G4cout <<std::setw(12)<<
        step->GetTrack()->GetTrackID()<<std::setw(12)<<
        step->GetTrack()->GetDefinition()->GetParticleName()<<std::setw(12)<<
        step->GetTrack()->GetDefinition()->GetPDGEncoding()<<std::setw(12)<<
        step->GetTrack()->GetDefinition()->GetPDGCharge()<<std::setw(12)<<
        step->GetTrack()->GetVertexPosition().z()-module_zposition<<std::setw(12)<<
        step->GetTrack()->GetParentID()<<std::setw(14)<<
        step->GetTrack()->GetDynamicParticle()->GetTotalMomentum()/CLHEP::MeV<<std::setw(12)<<
        step->GetTrack()->GetCurrentStepNumber();
      G4cout<<G4endl;
        
    }
  }

  /// store secondary/children particle step information:
    
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  const G4VProcess* CurrentProcess=preStepPoint->GetProcessDefinedStep();

  /// store secondary K-long track ID, if exists
    if(step->GetTrack()->GetCurrentStepNumber()==1 && step->GetTrack()->GetParentID() > 0  && (step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==130 || step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==310)){
        
        fEventAction->FillKlongSecondaryID(step->GetTrack()->GetTrackID());
        
    } else {
        fEventAction->FillKlongSecondaryID(-100);
    }
  
  bool isPi0Electron = false;

  if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==111 && (step->GetTrack()->GetParentID() == 1 || step->GetTrack()->GetParentID() == fEventAction->GetKlongSecondaryID() ) ){
    bool skip_pi0 = false;
    std::vector<G4double> pi0_trkID = fEventAction->ListPi0TrkID();
    for (int i = 0; i < (int) pi0_trkID.size(); i++){
      if(step->GetTrack()->GetTrackID() == pi0_trkID[i]) skip_pi0 = true;
    }
    if(!skip_pi0) fEventAction->StorePi0TrkID(step->GetTrack()->GetTrackID());
  }

  if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==22){
    bool skip_gamma = false;
    std::vector<G4double> pi0_trkID = fEventAction->ListPi0TrkID();
    std::vector<G4double> pi0gamma_trkID = fEventAction->ListPi0GammaTrkID();
    for (int i = 0; i < (int) pi0gamma_trkID.size(); i++){
      if(step->GetTrack()->GetTrackID() == pi0gamma_trkID[i]) skip_gamma = true;
    }
    if(!skip_gamma){
      for (int j = 0; j < (int) pi0_trkID.size(); j++){
	if(step->GetTrack()->GetParentID() == pi0_trkID[j]) {
	  fEventAction->StorePi0GammaTrkID(step->GetTrack()->GetTrackID());
	  break;
	}
      }
    }
  }
  if(abs(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding())==11){
    std::vector<G4double> pi0gamma_trkID = fEventAction->ListPi0GammaTrkID();
    for (int i = 0; i < (int) pi0gamma_trkID.size(); i++){
      if(step->GetTrack()->GetParentID() == pi0gamma_trkID[i]) {
        isPi0Electron = true;
	break;
      }
    }
  }
    
    bool KLongEvent = false;
    if((step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 130 || step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 310)){
        KLongEvent = true;
        auto seconda = step->GetSecondary();
        for(unsigned long i=0; i<seconda->size(); i++){
            fEventAction->StoreParentIDOfSec(seconda->at(i)->GetParentID());
        }
    }
    
    std::vector<int> ParentIDOfSec = fEventAction->GetParentIDOfSec();
    
    bool isChildKlong = false;
    for(int i = 0; i < ParentIDOfSec.size(); i++){
        if(step->GetTrack()->GetParentID()==ParentIDOfSec.at(i)) isChildKlong = true;
    }
//    if(step->GetTrack()->GetParentID() > 1){
    if(KLongEvent || isChildKlong){
        
        G4double SecMomDirx = step->GetTrack()->GetMomentumDirection().x();
        G4double SecMomDiry = step->GetTrack()->GetMomentumDirection().y();
        G4double SecMomDirz = step->GetTrack()->GetMomentumDirection().z();
        
        G4double SecPosX = step->GetTrack()->GetPosition().x();
        G4double SecPosY = step->GetTrack()->GetPosition().y();
        G4double SecPosZ = step->GetTrack()->GetPosition().z();
        G4double SecTime = step->GetTrack()->GetGlobalTime()/CLHEP::ns;
        G4double secKinEnergy = step->GetTrack()->GetDynamicParticle()->GetKineticEnergy();
        
        fEventAction->StoreSecondaryMomentumDir(SecMomDirx, SecMomDiry, SecMomDirz);
        fEventAction->StoreSecondaryPosition(SecPosX/CLHEP::cm, SecPosY/CLHEP::cm, (SecPosZ - module_zposition)/CLHEP::cm);
        //fEventAction->StoreSecondaryTime(SecTime);
        
        if(KLongEvent && (SecPosZ - module_zposition) > 0){
            fEventAction->StoreSecondaryTime(secKinEnergy/CLHEP::MeV); //store in this variable Energy of particle;
        }
    
        if(step->GetTrack()->GetCurrentStepNumber()==1){
        
            G4double xVertex = step->GetTrack()->GetVertexPosition().x();
            G4double yVertex = step->GetTrack()->GetVertexPosition().y();
            G4double zVertex = step->GetTrack()->GetVertexPosition().z() - module_zposition;
            
            fEventAction->StoreSecPDG(step->GetTrack()->GetDefinition()->GetPDGEncoding());
            fEventAction->StoreVertexPosSecondary(xVertex/CLHEP::cm,yVertex/CLHEP::cm,(zVertex)/CLHEP::cm);
            fEventAction->StoreSecTrackID(step->GetTrack()->GetTrackID());
            fEventAction->StoreSecParentID(step->GetTrack()->GetParentID());
            fEventAction->StoreSecondaryCharge(step->GetTrack()->GetDefinition()->GetPDGCharge());
            
            fEventAction->StoreSecondaryEdep(secKinEnergy/CLHEP::MeV); //
            fEventAction->StoreSecondaryKineticInfo(step->GetTrack()->GetDynamicParticle());
           
            
            G4cout <<std::setw(12)<<
              step->GetTrack()->GetTrackID()<<std::setw(12)<<
              step->GetTrack()->GetDefinition()->GetParticleName()<<std::setw(12)<<
              step->GetTrack()->GetDefinition()->GetPDGEncoding()<<std::setw(12)<<
              step->GetTrack()->GetDefinition()->GetPDGCharge()<<std::setw(12)<<
              step->GetTrack()->GetVertexPosition().z()-module_zposition<<std::setw(12)<<
              step->GetTrack()->GetParentID()<<std::setw(14)<<
              step->GetTrack()->GetDynamicParticle()->GetTotalMomentum()/CLHEP::MeV<<std::setw(12)<<
              step->GetTrack()->GetCurrentStepNumber();
            G4cout<<G4endl;
        }
        
            // Store K Longs secondaries:
        
//        auto seconda = step->GetSecondary();
//
//        for(unsigned long i=0; i<seconda->size(); i++){
//
//                G4cout << std::setw(12)<<
//                "-"<<std::setw(12)<<
//                seconda->at(i)->GetDefinition()->GetParticleName()<<std::setw(12)<<
//                seconda->at(i)->GetDefinition()->GetPDGEncoding()<<std::setw(12)<<
//                seconda->at(i)->GetDefinition()->GetPDGCharge()<<std::setw(12)<<
//                seconda->at(i)->GetVertexPosition().z()-module_zposition<<std::setw(12)<<
//                seconda->at(i)->GetParentID()<<std::setw(14)<<
//                seconda->at(i)->GetDynamicParticle()->GetTotalMomentum()/CLHEP::MeV<<std::setw(12)<<
//                "-";
//                G4cout<<G4endl;
//
//    // store K Long/ K Short secondaries information:
//
//                SecMomDirx = seconda->at(i)->GetMomentumDirection().x();
//                SecMomDiry = seconda->at(i)->GetMomentumDirection().y();
//                SecMomDirz = seconda->at(i)->GetMomentumDirection().z();
//
//                SecPosX = seconda->at(i)->GetPosition().x();
//                SecPosY = seconda->at(i)->GetPosition().y();
//                SecPosZ = seconda->at(i)->GetPosition().z();
//                SecTime = seconda->at(i)->GetGlobalTime()/CLHEP::ns;
//
//                secKinEnergy = seconda->at(i)->GetDynamicParticle()->GetKineticEnergy();
//
//                G4double xVertex = seconda->at(i)->GetVertexPosition().x();
//                G4double yVertex = seconda->at(i)->GetVertexPosition().y();
//                G4double zVertex = seconda->at(i)->GetVertexPosition().z() - module_zposition;
//
//                fEventAction->StoreSecondaryMomentumDir(SecMomDirx,SecMomDiry, SecMomDirz);
//                fEventAction->StoreSecondaryKineticInfo(seconda->at(i)->GetDynamicParticle());
//                fEventAction->StoreSecondaryPosition(SecPosX/CLHEP::cm, SecPosY/CLHEP::cm, (SecPosZ - module_zposition)/CLHEP::cm);
//
//                fEventAction->StoreSecondaryEdep(secKinEnergy/CLHEP::MeV); //FUNCTION IS NOT DEFINED
//                fEventAction->StoreSecondaryTime(SecTime);
//
//                fEventAction->StoreSecPDG(seconda->at(i)->GetDefinition()->GetPDGEncoding());
//                fEventAction->StoreVertexPosSecondary(xVertex/CLHEP::cm,yVertex/CLHEP::cm,(zVertex)/CLHEP::cm);
//                fEventAction->StoreSecParentID(seconda->at(i)->GetParentID());
//                fEventAction->StoreSecondaryCharge(seconda->at(i)->GetDefinition()->GetPDGCharge());
//        }
            
//        G4cout<<G4endl;
    }
    
//    if(KLongEvent){
//
//      G4double secKinEnergy = step->GetTrack()->GetDynamicParticle()->GetKineticEnergy();
//
//      G4double SecMomDirx = step->GetTrack()->GetMomentumDirection().x();
//      G4double SecMomDiry = step->GetTrack()->GetMomentumDirection().y();
//      G4double SecMomDirz = step->GetTrack()->GetMomentumDirection().z();
//
//      G4double SecPosX = step->GetTrack()->GetPosition().x();
//      G4double SecPosY = step->GetTrack()->GetPosition().y();
//      G4double SecPosZ = step->GetTrack()->GetPosition().z();
//
//      G4double SecTime = step->GetTrack()->GetGlobalTime()/CLHEP::ns;
//
//      fEventAction->StoreSecondaryMomentumDir(SecMomDirx,SecMomDiry, SecMomDirz);
//      fEventAction->StoreSecondaryKineticInfo(step->GetTrack()->GetDynamicParticle());
//      fEventAction->StoreSecondaryPosition(SecPosX/CLHEP::cm, SecPosY/CLHEP::cm, (SecPosZ - module_zposition)/CLHEP::cm);
//
//      fEventAction->StoreSecondaryEdep(secKinEnergy/CLHEP::MeV); //FUNCTION IS NOT DEFINED
//      fEventAction->StoreSecondaryTime(SecTime);
//
//            if(step->GetTrack()->GetCurrentStepNumber()==1){
//    // Keep track of secondary electron and photons produced from primary particle:
//
//                if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==22){
//                    fEventAction->AddSecondaryPhoton();
//                }
//                else if (step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==11 && step->GetTrack()->GetDynamicParticle()->GetTotalEnergy()/CLHEP::keV > 511){
//                    fEventAction->AddSecondaryElectron();
//                }
//
//                int ProcessNumber = -1;
//
//              G4double secpdg = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
//              G4String process = step->GetTrack()->GetCreatorProcess()->GetProcessName();
//              if(process == "Decay") ProcessNumber = 0; // Decay process;
//              G4String secondaryName = step->GetTrack()->GetDefinition()->GetParticleName();
//
//              G4double xVertex = step->GetTrack()->GetVertexPosition().x();
//              G4double yVertex = step->GetTrack()->GetVertexPosition().y();
//              G4double zVertex = step->GetTrack()->GetVertexPosition().z()-module_zposition;
//
//              G4int trackId = step->GetTrack()->GetTrackID();
//              G4int parentId = step->GetTrack()->GetParentID();
//              G4double SecTrackLength = step->GetTrack()->GetTrackLength()/CLHEP::cm;
//              G4double SecCharge = step->GetTrack()->GetDefinition()->GetPDGCharge();
//
//
//              fEventAction->StoreProcessName(ProcessNumber);
//              fEventAction->StoreSecPDG(secpdg);
//              fEventAction->StoreSecTrackLength(SecTrackLength);
//              fEventAction->StoreVertexPosSecondary(xVertex/CLHEP::cm,yVertex/CLHEP::cm,(zVertex)/CLHEP::cm);
//              fEventAction->StoreSecTrackID(trackId);
//              fEventAction->StoreSecParentID(parentId);
//              fEventAction->StoreSecondaryCharge(SecCharge);
//
//              // print secondary information:
//
//              G4cout << std::setw(12)<<
//                step->GetTrack()->GetTrackID()<<std::setw(12)<<
//                step->GetTrack()->GetDefinition()->GetParticleName()<<std::setw(12)<<
//                step->GetTrack()->GetDefinition()->GetPDGEncoding()<<std::setw(12)<<
//                step->GetTrack()->GetDefinition()->GetPDGCharge()<<std::setw(12)<<
//                step->GetTrack()->GetVertexPosition().z()-module_zposition<<std::setw(12)<<
//                step->GetTrack()->GetParentID()<<std::setw(14)<<
//                step->GetTrack()->GetDynamicParticle()->GetTotalMomentum()/CLHEP::MeV<<std::setw(12)<<
//                step->GetTrack()->GetCurrentStepNumber();
//              G4cout<<G4endl;
//              G4cout<<"Process name: "<<process<<G4endl;
//              G4cout<<G4endl;
//              fEventAction->StoreTotalSecondary(1);
//
//
//                auto seconda= step->GetSecondaryInCurrentStep();
//
//                for(int i=0; i<seconda->size(); i++){
//
//                        G4cout << std::setw(12)<<
//                          "-"<<std::setw(12)<<
//                          seconda->at(i)->GetDefinition()->GetParticleName()<<std::setw(12)<<
//                          seconda->at(i)->GetDefinition()->GetPDGEncoding()<<std::setw(12)<<
//                          seconda->at(i)->GetDefinition()->GetPDGCharge()<<std::setw(12)<<
//                          seconda->at(i)->GetVertexPosition().z()-module_zposition<<std::setw(12)<<
//                          seconda->at(i)->GetParentID()<<std::setw(14)<<
//                          seconda->at(i)->GetDynamicParticle()->GetTotalMomentum()/CLHEP::MeV<<std::setw(12)<<
//                          "-";
//                        G4cout<<G4endl;
//
//                        // store K Long/ K Short secondaries information:
//
//                        G4double SecMomDirx = seconda->at(i)->GetMomentumDirection().x();
//                        G4double SecMomDiry = seconda->at(i)->GetMomentumDirection().y();
//                        G4double SecMomDirz = seconda->at(i)->GetMomentumDirection().z();
//
//                        G4double SecPosX = seconda->at(i)->GetPosition().x();
//                        G4double SecPosY = seconda->at(i)->GetPosition().y();
//                        G4double SecPosZ = seconda->at(i)->GetPosition().z();
//                        G4double SecTime = seconda->at(i)->GetGlobalTime()/CLHEP::ns;
//
//                        G4double secKinEnergy = seconda->at(i)->GetDynamicParticle()->GetKineticEnergy();
//
//                        G4double xVertex = seconda->at(i)->GetVertexPosition().x();
//                        G4double yVertex = seconda->at(i)->GetVertexPosition().y();
//                        G4double zVertex = seconda->at(i)->GetVertexPosition().z() - module_zposition;
//
//                        fEventAction->StoreSecondaryMomentumDir(SecMomDirx,SecMomDiry, SecMomDirz);
//                        fEventAction->StoreSecondaryKineticInfo(seconda->at(i)->GetDynamicParticle());
//                        fEventAction->StoreSecondaryPosition(SecPosX/CLHEP::cm, SecPosY/CLHEP::cm, (SecPosZ - module_zposition)/CLHEP::cm);
//
//                        fEventAction->StoreSecondaryEdep(secKinEnergy/CLHEP::MeV); //FUNCTION IS NOT DEFINED
//                        fEventAction->StoreSecondaryTime(SecTime);
//
//                        fEventAction->StoreSecPDG(seconda->at(i)->GetDefinition()->GetPDGEncoding());
//                        fEventAction->StoreVertexPosSecondary(xVertex/CLHEP::cm,yVertex/CLHEP::cm,(zVertex)/CLHEP::cm);
//                        fEventAction->StoreSecParentID(seconda->at(i)->GetParentID());
//                        fEventAction->StoreSecondaryCharge(seconda->at(i)->GetDefinition()->GetPDGCharge());
//
//                }
//
//                G4cout<<G4endl;
//
//          }
//  }
  
  
  if(volume == fDetConstruction->GetmodulePV()) return;
  
  // Is this being used??
  fEventAction->AddEnergy(step->GetTotalEnergyDeposit());
  G4double charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
  if (charge != 0.) {
    fEventAction->AddTrakLenCharg(step->GetStepLength());
    fEventAction->CountStepsCharg();
  } else {
    fEventAction->AddTrakLenNeutr(step->GetStepLength());
    fEventAction->CountStepsNeutr();
  }
}

void MathSimSteppingAction::MultipleScattering(const G4Step* step){
  
  // for MSC:
  
  if (count < 26){
    prePoint = step->GetPreStepPoint();
    postPoint = step->GetPostStepPoint();
    stepL = step->GetStepLength();
    matPreStep = prePoint -> GetMaterial(); matPostStep = postPoint -> GetMaterial();
    
    if (matPreStep == matPostStep ){
      fRunAction->sumPathLen(stepL,1);
    } else {
      
      if(step->GetTrack()->GetTrackID() == 1){
	
	//        **** Compute the angle from momentum:
	
        G4ThreeVector ang = step->GetPostStepPoint()->GetMomentumDirection();
        G4ThreeVector *centerVector = new G4ThreeVector(0, 0, 1);
        G4double angle = ang.angle(*centerVector);
        
	//G4cout<<"\n !!!!!!====== angle from momentum: "<< angle / deg << G4endl;
        
	//        **** end
        
	//        **** Compute the angle from 27.10 Formula:
        
        fRunAction->sumPathLen(stepL,1);
        fRunAction->GetCurrentMat(matPreStep);
        
        if (fRunAction->GetTotalCount() > 0) {
	  
          //compute path length and related quantities
          
	  G4double MeanTPL  = fRunAction->GetPathLength();//fRunAction->GetTotalCount();
	  sum_teta += fRunAction->ComputeMscHighland(MeanTPL);
          
	  //G4cout<<"!!!!!!====== MeanTPL: "<< MeanTPL/cm <<"/cm"<<G4endl;
	  
	  //G4cout << "compute theta0 from Highland :\t"
	  //       << "!!!!!!====== "<< fRunAction->ComputeMscHighland(MeanTPL)/deg << " deg"
	  //       << G4endl;
	  
        }
	//G4cout<<"!!!!!!====== sum_angle from formula: "<< sum_teta / deg << G4endl;
        
	fRunAction->sumPathLen(0.0, 0);
        
	//        **** end
	
	//        **** Compute the angle from arctg:
        
        auto PreStepPos = step->GetPreStepPoint()->GetPosition();
        auto PostStepPos = step->GetPostStepPoint()->GetPosition();
        auto z = PostStepPos.z() + 15680; //15660;
        auto x = PostStepPos.x();
        auto y = PostStepPos.y();
        angle = std::atan(sqrt(x*x + y*y)/z);
        
	//G4cout<<"!!!!!!====== angle from atan: "<< angle/deg << G4endl;
        
        if (count == 25) {fEventAction->StoreAngle1(angle/deg);
	  fEventAction->StoreAngle2(sum_teta/deg);
	  sum_teta = 0;
        } else { if (count > 25) sum_teta=0; }
	
	//        **** end
	
	//        G4cout<<"!!!!!!====== z axis: "<< z/cm <<"/cm"<< G4endl;
	//        G4cout<<"!!!!!!====== x: "<< x/cm <<"/cm"<< G4endl;
	//        G4cout<<"!!!!!!====== y: "<< y/cm <<"/cm"<<G4endl;
	//
	
	//                        G4cout<<"!!!!!!====== matPreStep: "<< matPreStep  << G4endl;
	//                        G4cout<<"!!!!!!====== matPostStep: "<< matPostStep << count << G4endl;
	
	//G4cout<<"!!!!!!====== count of vol: "<< count << G4endl;
        count = fEventAction->StoreCountOfVolume();
        
      }
    }
  }
}

void MathSimSteppingAction::MaterialMap(const G4Step* step, G4double stepLength){
  
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  auto nistManager = G4NistManager::Instance();
  G4Material* scint_mat = nistManager ->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material* matAlHoneycomb = G4Material::GetMaterial("AlHoneycomb");
  G4Material* supportBarMat = G4Material::GetMaterial("Stainless-Steel");
  if (volume == fDetConstruction->GetscintillatorPV()) {
    
    fEventAction->MatMapScintillator(stepLength, stepLength/(scint_mat->GetRadlen()/CLHEP::cm));
    
  } else {
    if (volume == fDetConstruction->GetAlLayerPV()){
      
      fEventAction->MatMapLayerSupport(stepLength, stepLength/(matAlHoneycomb->GetRadlen()/CLHEP::cm));
      
    } else{
      if(step->GetPreStepPoint()->GetMaterial() == G4Material::GetMaterial("Stainless-Steel")){//volume == fDetConstruction->GetSteelBarPV()){
	
	fEventAction->MatMapSteelBar(stepLength, stepLength/(supportBarMat->GetRadlen()/CLHEP::cm));
	
      }
    }
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
