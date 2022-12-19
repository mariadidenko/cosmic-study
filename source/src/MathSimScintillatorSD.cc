#include "MathSimScintillatorSD.hh"
#include "MathSimScintillatorHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimScintillatorSD::MathSimScintillatorSD(G4String name)
: G4VSensitiveDetector(name), 
  fHitsCollection(nullptr), fHCID(-1)
{
  collectionName.insert("scintillatorColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimScintillatorSD::~MathSimScintillatorSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimScintillatorSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection 
    = new MathSimScintillatorHitsCollection(SensitiveDetectorName,collectionName[0]);

  if (fHCID<0) { 
     fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); 
  }
  hce->AddHitsCollection(fHCID,fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MathSimScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  //energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  
  auto charge = step->GetTrack()->GetDefinition()->GetPDGCharge();

  //if (charge!= 0. || (step->GetTrack()->GetParentID() <= 4 && step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==22)){
  if (charge!= 0){
    
  auto trackId = step->GetTrack()->GetTrackID();
  auto parentId = step->GetTrack()->GetParentID();
  auto pdg = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  auto preStepPoint = step->GetPreStepPoint();

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto motherPhysical = touchable->GetVolume(1); // mother
  auto copyNo = motherPhysical->GetCopyNo();
    
  auto worldPos = preStepPoint->GetPosition();
  auto localPos 
    = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);

  auto isPrimary = false;
  auto isSecondary = false;
  if(trackId == 1) isPrimary = true;
  if(parentId == 1) isSecondary = true;
  
  auto hit = new MathSimScintillatorHit(copyNo);
  hit->SetWorldPos(worldPos);
  hit->SetLocalPos(localPos);
  hit->SetTime(preStepPoint->GetGlobalTime());
  hit->SetEdep(edep);
  hit->SetPDG(pdg);
  hit->SetTrackID(trackId);
  hit->SetParentID(parentId);
  hit->SetIsPrimary(isPrimary); // truth information, is this hit from primary particle?
  hit->SetIsSecondary(isSecondary);
  hit->SetCharge(charge);
  fHitsCollection->insert(hit);
    }
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
