#ifndef MathSimScintillatorHit_h
#define MathSimScintillatorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// Scintillator layer hit
///
/// It records:
/// - the layer ID
/// - the particle time
/// - the particle local and global positions
/// - If the hit is from the primary or secondary particle
/// ...

class MathSimScintillatorHit : public G4VHit
{
  public:
    MathSimScintillatorHit();
    MathSimScintillatorHit(G4int layerID);
    MathSimScintillatorHit(const MathSimScintillatorHit &right);
    virtual ~MathSimScintillatorHit();

    const MathSimScintillatorHit& operator=(const MathSimScintillatorHit &right);
    G4bool operator==(const MathSimScintillatorHit &right) const;
    
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    void SetEdep(G4double de) { fEdep = de; }
    G4double GetEdep() const { return fEdep;}
  
    void SetLayerID(G4int z) { fLayerID = z; }
    G4int GetLayerID() const { return fLayerID; }

    void SetTime(G4double t) { fTime = t; }
    G4double GetTime() const { return fTime; }

    void SetLocalPos(G4ThreeVector xyz) { fLocalPos = xyz; }
    G4ThreeVector GetLocalPos() const { return fLocalPos; }

    void SetWorldPos(G4ThreeVector xyz) { fWorldPos = xyz; }
    G4ThreeVector GetWorldPos() const { return fWorldPos; }

    void SetIsPrimary(G4bool isPrimary) { fIsPrimary = isPrimary; }
    G4bool GetIsPrimary() const { return fIsPrimary; }
    
    void SetPDG(G4int pdg){fPdg = pdg;}
    G4int GetPDG(){ return fPdg;}
    
    void SetTrackID(G4int trackId){ fTrackID = trackId; }
    G4int GetTrackID() const { return fTrackID; }
    
    void SetParentID(G4int parentId){ fParentID = parentId; }
    G4int GetParentID() const { return fParentID; }
    
    void SetIsSecondary(G4bool isSecondary){ fIsSecondary =  isSecondary;}
    G4bool GetIsSecondary() const { return fIsSecondary; }
    
    void SetCharge(G4int charge){ fCharge = charge; }
    G4int GetCharge() const { return fCharge;}
  
  private:
    G4int fLayerID;
    G4double fTime;
    G4double fEdep;
    G4ThreeVector fLocalPos;
    G4ThreeVector fWorldPos;
    G4bool fIsPrimary; G4bool fIsSecondary;
    G4int fPdg; G4int fCharge;
    G4int fTrackID; G4int fParentID;
};

using MathSimScintillatorHitsCollection = G4THitsCollection<MathSimScintillatorHit>;

extern G4ThreadLocal G4Allocator<MathSimScintillatorHit>* MathSimScintillatorHitAllocator;

inline void* MathSimScintillatorHit::operator new(size_t)
{
  if (!MathSimScintillatorHitAllocator) {
       MathSimScintillatorHitAllocator = new G4Allocator<MathSimScintillatorHit>;
  }
  return (void*)MathSimScintillatorHitAllocator->MallocSingle();
}

inline void MathSimScintillatorHit::operator delete(void* aHit)
{
  MathSimScintillatorHitAllocator->FreeSingle((MathSimScintillatorHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
