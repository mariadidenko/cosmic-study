#include "MathSimScintillatorHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<MathSimScintillatorHit>* MathSimScintillatorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimScintillatorHit::MathSimScintillatorHit()
: G4VHit(), 
  fLayerID(-1), fTime(0.), fLocalPos(0), fWorldPos(0), fEdep(0.), fIsPrimary(false),
  fIsSecondary(false), fPdg(0), fTrackID(-1), fParentID(-1),fCharge(100)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimScintillatorHit::MathSimScintillatorHit(G4int layerID)
: G4VHit(), 
  fLayerID(layerID), fTime(0.), fLocalPos(0), fWorldPos(0), fEdep(0.), fIsPrimary(false),
  fIsSecondary(false), fPdg(0), fTrackID(-1), fParentID(-1),fCharge(100)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimScintillatorHit::~MathSimScintillatorHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimScintillatorHit::MathSimScintillatorHit(const MathSimScintillatorHit &right)
: G4VHit(),
  fLayerID(right.fLayerID),
  fTime(right.fTime),
  fLocalPos(right.fLocalPos),
  fWorldPos(right.fWorldPos),
  fEdep(right.fEdep),
  fIsPrimary(right.fIsPrimary),
  fIsSecondary(right.fIsSecondary),
  fPdg(right.fPdg),
  fTrackID(right.fTrackID),
  fParentID(right.fParentID),
  fCharge(right.fCharge)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MathSimScintillatorHit& MathSimScintillatorHit::operator=(const MathSimScintillatorHit &right)
{
  fLayerID = right.fLayerID;
  fTime = right.fTime;
  fLocalPos = right.fLocalPos;
  fWorldPos = right.fWorldPos;
  fEdep = right.fEdep;
  fIsPrimary = right.fIsPrimary;
  fIsSecondary = right.fIsSecondary;
  fPdg = right.fPdg;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  fCharge = right.fCharge;
  
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MathSimScintillatorHit::operator==(const MathSimScintillatorHit &/*right*/) const
{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimScintillatorHit::Draw()
{
  auto visManager = G4VVisManager::GetConcreteInstance();
  if (! visManager) return;

  G4Circle circle(fWorldPos);
  circle.SetScreenSize(2);
  circle.SetFillStyle(G4Circle::filled);
  G4Colour colour(1.,1.,0.);
  G4VisAttributes attribs(colour);
  circle.SetVisAttributes(attribs);
  visManager->Draw(circle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* MathSimScintillatorHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("MathSimScintillatorHit",isNew);

  if (isNew) {
      (*store)["HitType"] 
        = G4AttDef("HitType","Hit Type","Physics","","G4String");
      
      (*store)["ID"] 
        = G4AttDef("ID","ID","Physics","","G4int");
      
      (*store)["Time"] 
        = G4AttDef("Time","Time","Physics","G4BestUnit","G4double");
      
      (*store)["LocalPos"] 
        = G4AttDef("LocalPos", "Local Position", "Physics","G4BestUnit","G4ThreeVector");

      (*store)["WorldPos"]
        = G4AttDef("WorldPos", "World Position", "Physics","G4BestUnit","G4ThreeVector");

      (*store)["Edep"]
	= G4AttDef("Edep", "Energy deposited", "Physics","G4BestUnit","G4double");

      (*store)["IsPrimary"]
        = G4AttDef("IsPrimary","IsPrimary","Physics","","G4bool");
      
      (*store)["IsSecondary"]
        = G4AttDef("IsSecondary","IsSecondary","Physics","","G4bool");
      
      (*store)["pdg"]
        = G4AttDef("pdg","pdg","Physics","","G4int");
      
      (*store)["trackId"]
        = G4AttDef("trackId","trackId","Physics","","G4int");
      
      (*store)["parentId"]
        = G4AttDef("parentId","parentId","Physics","","G4int");
      
      (*store)["charge"]
        = G4AttDef("charge","charge","Physics","","G4int");

  }
  
  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* MathSimScintillatorHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;
  
  values
    ->push_back(G4AttValue("HitType","MathSimScintillatorHit",""));
  values
    ->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(fLayerID),""));
  values
    ->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));
  values
    ->push_back(G4AttValue("WorldPos",G4BestUnit(fWorldPos,"Length"),""));
   values
    ->push_back(G4AttValue("LocalPos",G4BestUnit(fLocalPos,"Length"),""));
  values
    ->push_back(G4AttValue("Edep",G4BestUnit(fEdep,"Energy"),""));
  values
    ->push_back(G4AttValue("IsPrimary",G4UIcommand::ConvertToString(fIsPrimary),""));
  values
    ->push_back(G4AttValue("IsSecondary",G4UIcommand::ConvertToString(fIsSecondary),""));
  values
    ->push_back(G4AttValue("pdg",G4UIcommand::ConvertToString(fPdg),""));
  values
    ->push_back(G4AttValue("trackId",G4UIcommand::ConvertToString(fTrackID),""));
  values
    ->push_back(G4AttValue("parentId",G4UIcommand::ConvertToString(fParentID),""));
  values
    ->push_back(G4AttValue("charge",G4UIcommand::ConvertToString(fCharge),""));
    
  return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimScintillatorHit::Print()
{
  if(fIsPrimary || fIsSecondary){
    G4cout << "  Layer[" << fLayerID << "] : time " << fTime/ns
	   << " (nsec) --- local (x,y) " << fLocalPos.x()/mm
	   << ", " << fLocalPos.y() <<" PDG: "<< fPdg << " Edep: " << fEdep/keV << " TrackID: "<< fTrackID
	   << " ParentID: "<< fParentID << " Prim: "<< fIsPrimary << " Sec: "<< fIsSecondary;
    G4cout<<G4endl;
  }
//    G4int id0 = 8;
//    G4int id1 = 20;
//    G4cout<<std::setw(id0)<<std::right<<
//           "Layer"<<std::setw(id0)<<std::right<<
//           "Time(nsec)"<<std::setw(id1)<<std::right<<
//           "local (x,y)"<<std::setw(id0)<<std::right<<
//           "PDG"<<std::setw(id0)<<std::right<<
//           "TrackID"<<std::setw(id0)<<std::right<<
//           "ParentID"<<std::setw(id0)<<std::right<<
//           "Prim"<<std::setw(id0)<<std::right<<
//           "Sec";
//    G4cout<<G4endl;
//
//    G4cout<<std::setw(id0)<<std::right<<
//           fLayerID<<std::setw(id0)<<std::right<<
//           fTime/ns<<std::setw(id1)<<std::right<<
//           fLocalPos.x()/mm<<","<<fLocalPos.y()<<std::setw(id0)<<std::right<<
//           fPdg<<std::setw(id0)<<std::right<<
//           fTrackID<<std::setw(id0)<<std::right<<
//           fParentID<<std::setw(id0)<<std::right<<
//           fIsPrimary<<std::setw(id0)<<std::right<<
//           fIsSecondary;
//    G4cout<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
