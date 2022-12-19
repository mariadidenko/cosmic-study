#include "MathSimRunAction.hh"
#include "MathSimAnalysis.hh"
#include "MathSimEventAction.hh"
#include "MathSimDetectorConstruction.hh"
#include "MathSimPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimRunAction::MathSimRunAction(MathSimEventAction *eventAction, MathSimDetectorConstruction* detConstruction, MathSimPrimaryGeneratorAction *primaryAction)
 : G4UserRunAction(),
fDetConstruction(detConstruction),
fEventAction(eventAction),
fPrimary(primaryAction)
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace in MathSimAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //

  // Creating ntuple
  //
  analysisManager->CreateNtuple("SimulationTree", "Edep and TrackL");

  analysisManager->CreateNtupleIColumn("event"); // column Id = 0
  analysisManager->CreateNtupleIColumn("pdgID", fEventAction->fPrimariesPDG); // column Id = 1
  analysisManager->CreateNtupleDColumn("beamEnergy_GeV", fEventAction->fPrimariesEnergy); // column Id = 2
  analysisManager->CreateNtupleDColumn("beamX_cm", fEventAction->fPrimariesX); // column Id = 3
  analysisManager->CreateNtupleDColumn("beamY_cm", fEventAction->fPrimariesY); // column Id = 4
  analysisManager->CreateNtupleDColumn("beamZ_cm", fEventAction->fPrimariesZ); // column Id = 5
  analysisManager->CreateNtupleDColumn("beamDir_Theta", fEventAction->fPrimariesThetaDir);//6
  analysisManager->CreateNtupleDColumn("beamDir_Phi", fEventAction->fPrimariesPhiDir);//7
  
  analysisManager->CreateNtupleIColumn("siliconHits_ID",fEventAction->fSiHitsID);//8
    
  analysisManager->CreateNtupleDColumn("siliconHits_x_cm",fEventAction->fSiHitsX);//9
  analysisManager->CreateNtupleDColumn("siliconHits_y_cm",fEventAction->fSiHitsY);//10
  analysisManager->CreateNtupleDColumn("siliconHits_z_cm",fEventAction->fSiHitsZ);//11
    
  analysisManager->CreateNtupleDColumn("siliconHits_Edep_MeV",fEventAction->fSiHitsEdep);//12
  analysisManager->CreateNtupleDColumn("siliconHits_Time_ns",fEventAction->fSiHitsTime);//13
    
  analysisManager->CreateNtupleIColumn("siliconHits_IsFromPrimary",fEventAction->fSiHitsIsPrimary);//14
  analysisManager->CreateNtupleIColumn("siliconHits_IsFromSecondary",fEventAction->fSiHitsIsSecondary);//15
  analysisManager->CreateNtupleIColumn("siliconHitsTrackID",fEventAction->fSiHitsTrackID);//16
  analysisManager->CreateNtupleIColumn("siliconHitsPDG",fEventAction->fSiHitsPDG);//17
  analysisManager->CreateNtupleIColumn("siliconHitsCharge",fEventAction->fSiHitsCharge);//18
  
  analysisManager->CreateNtupleDColumn("Primary_PosX_cm", fEventAction->fPrimaryPosX);//19
  analysisManager->CreateNtupleDColumn("Primary_PosY_cm", fEventAction->fPrimaryPosY);//20
  analysisManager->CreateNtupleDColumn("Primary_PosZ_cm", fEventAction->fPrimaryPosZ);//21
    
  analysisManager->CreateNtupleDColumn("Primary_Edep_MeV", fEventAction->fPrimaryEdep);//22
    
  analysisManager->CreateNtupleDColumn("Primary_Px_MeV", fEventAction->fPrimaryPx);//23
  analysisManager->CreateNtupleDColumn("Primary_Py_MeV", fEventAction->fPrimaryPy);//24
  analysisManager->CreateNtupleDColumn("Primary_Pz_MeV", fEventAction->fPrimaryPz);//25
    
  analysisManager->CreateNtupleDColumn("Primary_Ptotal_MeV", fEventAction->fPrimaryPtotal);//26
  analysisManager->CreateNtupleDColumn("Primary_Etotal_MeV", fEventAction->fPrimaryEtotal);//27
  analysisManager->CreateNtupleDColumn("Primary_time_ns", fEventAction->fPrimaryTime);//28
  analysisManager->CreateNtupleDColumn("PrimaryTrackLength_mm", fEventAction->fPrimaryTrackLength);//29
    
  analysisManager->CreateNtupleIColumn("Primary_TotalElectrons");//30
  analysisManager->CreateNtupleIColumn("Primary_TotalPhotons");//31

  // Material map variables:
  analysisManager->CreateNtupleDColumn("MatMap_Scintillator_length_cm");//32
  analysisManager->CreateNtupleDColumn("MatMap_SupportLayer_length_cm");//33
  analysisManager->CreateNtupleDColumn("MatMap_SteelBar_length_cm");//34
  analysisManager->CreateNtupleDColumn("MatMap_Scintillator_length_radLen");//35
  analysisManager->CreateNtupleDColumn("MatMap_SupportLayer_length_radLen");//36
  analysisManager->CreateNtupleDColumn("MatMap_SteelBar_length_radLen");//37
  
  analysisManager->CreateNtupleDColumn("ScattAngle1");//38
  analysisManager->CreateNtupleDColumn("ScattAngle2");//39
    
  analysisManager->CreateNtupleDColumn("SecPDG", fEventAction->fSecondaryPDG);//40
    
  analysisManager->CreateNtupleDColumn("SecVertexX_cm", fEventAction->fSecXVertex);//41
  analysisManager->CreateNtupleDColumn("SecVertexY_cm", fEventAction->fSecYVertex);//42
  analysisManager->CreateNtupleDColumn("SecVertexZ_cm", fEventAction->fSecZVertex);//43
  
  analysisManager->CreateNtupleDColumn("Secondary_PosX_cm", fEventAction->fSecPosX);//44
  analysisManager->CreateNtupleDColumn("Secondary_PosY_cm", fEventAction->fSecPosY);//45
  analysisManager->CreateNtupleDColumn("Secondary_PosZ_cm", fEventAction->fSecPosZ);//46
    
  analysisManager->CreateNtupleDColumn("SecMomDirX", fEventAction->fSecDirPx);//47
  analysisManager->CreateNtupleDColumn("SecMomDirY", fEventAction->fSecDirPy);//48
  analysisManager->CreateNtupleDColumn("SecMomDirZ", fEventAction->fSecDirPz);//49

  analysisManager->CreateNtupleDColumn("Secondary_Px_MeV", fEventAction->fSecondaryPx);//50
  analysisManager->CreateNtupleDColumn("Secondary_Py_MeV", fEventAction->fSecondaryPy);//51
  analysisManager->CreateNtupleDColumn("Secondary_Pz_MeV", fEventAction->fSecondaryPz);//52

  analysisManager->CreateNtupleDColumn("Secondary_Edep_MeV", fEventAction->fSecEdep);//53
    
  analysisManager->CreateNtupleDColumn("Secondary_Ptotal_MeV", fEventAction->fSecPtotal);//54
  analysisManager->CreateNtupleDColumn("Secondary_Etotal_MeV", fEventAction->fSecEtotal);//55
  
  analysisManager->CreateNtupleDColumn("Secondary_time_ns", fEventAction->fSecondaryTime);//56
    
  analysisManager->CreateNtupleIColumn("SecondaryTrackID",fEventAction->fSecTrackID);//57
  analysisManager->CreateNtupleIColumn("SecondaryParentID",fEventAction->fSecParentID);//58
  analysisManager->CreateNtupleDColumn("SecondaryTrackLength_cm", fEventAction->fSecTrackLength);//59
  analysisManager->CreateNtupleDColumn("SecondaryCharge", fEventAction->fSecCharge);//60
    
  analysisManager->CreateNtupleDColumn("isCleanEvent");//61
  analysisManager->CreateNtupleDColumn("isKLongEvent");//62
  analysisManager->CreateNtupleDColumn("isKShortEvent");//63
  analysisManager->CreateNtupleDColumn("isInteractEvent");//64
    
  analysisManager->CreateNtupleIColumn("NSecondaryFromNoClean");//65
  analysisManager->CreateNtupleIColumn("NSecondaryFromKLongEvent");//66
  analysisManager->CreateNtupleIColumn("NSecondaryOfKLongs");//67
  analysisManager->CreateNtupleIColumn("KlongSecondaryID");//68
  
  analysisManager->CreateNtupleDColumn("isThreePionsDecay");//69
  analysisManager->CreateNtupleDColumn("isPionAndLeptonDecay");//70
  analysisManager->CreateNtupleDColumn("isTwoPhotonsDecay");//71
  analysisManager->CreateNtupleDColumn("isOtherDecay");//72
  
  analysisManager->CreateNtupleIColumn("ProcessName", fEventAction->fProcess);//73
  analysisManager->CreateNtupleDColumn("NumDecayChildren");//74
  analysisManager->CreateNtupleDColumn("NumDecayChildrenAfterInteract");//75
  analysisManager->CreateNtupleDColumn("NumSecKShort");//76
  analysisManager->CreateNtupleDColumn("NumSecKLong");//77
  analysisManager->CreateNtupleDColumn("KinEnergyOfParticleGun", fEventAction->fKinEnergyOfParticleGun);//78
  analysisManager->CreateNtupleDColumn("MomentumOfParticleGun", fEventAction->fMomentumOfParticleGun);//79
  analysisManager->CreateNtupleDColumn("ParticleGunMomentumDirectionU", fEventAction->fParticleGunMomentumDirectionU);//80
  analysisManager->CreateNtupleDColumn("ParticleGunMomentumDirectionV", fEventAction->fParticleGunMomentumDirectionV);//81
  analysisManager->CreateNtupleDColumn("ParticleGunMomentumDirectionW", fEventAction->fParticleGunMomentumDirectionW);//82
  analysisManager->CreateNtupleDColumn("ParticleGunTheta", fEventAction->fParticleGunTheta);//83
  analysisManager->CreateNtupleDColumn("ParticleGunPhi", fEventAction->fParticleGunPhi);//84
  analysisManager->CreateNtupleDColumn("ParticleGunMag", fEventAction->fParticleGunMag);//85
   
  analysisManager->FinishNtuple();

  analysisManager
    ->CreateH1("Tracker1","Drift Chamber 1 # Hits", 50, 0., 50); // h1 Id = 0
  analysisManager
    ->CreateH2("Tracker1XY","Drift Chamber 1 X vs Y",           // h2 Id = 0
	       50, -1500., 1500, 50, -300., 300.);
  analysisManager
    ->CreateH1("SpaceAngle1","Space angle distribution of particle+",100, 0., 100); // h1 Id = 5
  analysisManager
    ->CreateH1("SpaceAngle2","Space angle distribution of particle-", 100, 0., 100); // h1 Id = 5

  //  analysisManager->CreateH1("xVertSec","x Vertex Position of secondary", 100, 0.,50*m);//6
  //  analysisManager->CreateH1("yVertSec","y Vertex Position of secondary", 100, 0.,50*m);//7
  //  analysisManager->CreateH1("xVertSec","z Vertex Position of secondary", 100, 0.,50*m);//8
  //  analysisManager->CreateH1("PDG sec","PDG of secondary", 100, 0.,);//9
  //  analysisManager->CreateH1("kinEnergySec","Kinematic Energy of secondary", 100, 0.,);//10
  //  analysisManager->CreateH1("MomentDirXSec","Momentum Direction X of secondary", 100, 0.,);//11
  //  analysisManager->CreateH1("MomentDirYSec","Momentum Direction Y of secondary", 100, 0.,);//12
  //  analysisManager->CreateH1("MomentDirZSec","Momentum Direction Z of secondary", 100, 0.,);//13
    
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimRunAction::~MathSimRunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    auto nistManager = G4NistManager::Instance();
    
    fTotalCount = 0.0;
    fStepLen = 0.0;
    fCurrentMat = nistManager->FindOrBuildMaterial("G4_Galactic");
  // Get analysis manager
  
     auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "MathuslaSimulation";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
    
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double MathSimRunAction::ComputeMscHighland(G4double pathLength)
{
 
 // https://pdg.lbl.gov/2005/reviews/passagerpp.pdf
 // page 11, formula 27.10
    
 G4double Radlen = fCurrentMat->GetRadlen();
 G4double t = pathLength/Radlen;
 
    if (t < DBL_MIN) return 0.;

 G4ParticleGun* particle = fPrimary->GetParticleGun();
 G4double T = particle->GetParticleEnergy();
 G4double M = particle->GetParticleDefinition()->GetPDGMass();
 G4double z = std::abs(particle->GetParticleDefinition()->GetPDGCharge()/eplus);

 G4double bpc = T*(T+2*M)/(T+M); // !!! check this formula
 G4double teta0 = 13.6*MeV*z*std::sqrt(t)*(1.+0.038*std::log(t))/bpc;
 
// G4cout<<"!!!!!!====== CurrentMat: "<< fCurrentMat << G4endl;
// G4cout<<"!!!!!!====== pathLength: "<< pathLength/cm << "/cm" << G4endl;
// G4cout<<"!!!!!!====== T: "<< T/MeV << G4endl;
// G4cout<<"!!!!!!====== M: "<< M/MeV << G4endl;
// G4cout<<"!!!!!!====== z: "<< z << G4endl;
    
    return teta0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
