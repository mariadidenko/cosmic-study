#ifndef MathSimEventAction_h
#define MathSimEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4DynamicParticle.hh"
#include "globals.hh"

#include <vector>
#include <array>


/// Event action class
///

// Victoria: check, these need to be removed
const G4int kDim = 1;
const G4int kH1 = 0;
const G4int kH2 = 1;


class MathSimEventAction : public G4UserEventAction
{
public:
  MathSimEventAction();
  virtual ~MathSimEventAction();
  
  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
  
  /// Vector of primary particles in an event: PDG type
  std::vector<G4int> fPrimariesPDG;
  /// Vector of primary particles in an event: particle energy (in GeV)
  std::vector<G4double> fPrimariesEnergy;
  /// Vector of primary particles in an event: vertex position x (in cm)
  std::vector<G4double> fPrimariesX;
  /// Vector of primary particles in an event: vertex position y (in cm)
  std::vector<G4double> fPrimariesY;
  /// Vector of primary particles in an event: vertex position z (in cm)
  std::vector<G4double> fPrimariesZ;
  std::vector<G4double> fPrimariesThetaDir;
  std::vector<G4double> fPrimariesPhiDir;

  G4int fKlongSecondaryID;
  
  G4bool  isCleanEvent;
  G4bool  isKLongEvent;
  G4bool  isKShortEvent;
  G4bool  isInteractEvent;
  
  /// Vector of hits in silicon sensors: hit ID
  std::vector<G4int> fSiHitsID;
  /// Vector of hits in silicon sensors: hit position x (in cm)
  std::vector<G4double> fSiHitsX;
  /// Vector of hits in silicon sensors: hit position y (in cm)
  std::vector<G4double> fSiHitsY;
  /// Vector of hits in silicon sensors: hit position z (in cm)
  std::vector<G4double> fSiHitsZ;
  /// Vector of hits in silicon sensors: hit energy (in keV)
  std::vector<G4double> fSiHitsEdep;
  std::vector<G4double> fSiHitsTime;
  std::vector<G4int> fSiHitsIsPrimary;
  std::vector<G4int>  fSiHitsIsSecondary;
  
  std::vector<G4int> fSiHitsTrackID;
  std::vector<G4int> fSiHitsPDG;
  std::vector<G4int> fSiHitsCharge;

  /// Vector of primary particle steps
  std::vector<G4double> fPrimaryPosX;
  std::vector<G4double> fPrimaryPosY;
  std::vector<G4double> fPrimaryPosZ;
  std::vector<G4double> fPrimaryEdep;
  std::vector<G4double> fPrimaryPx;
  std::vector<G4double> fPrimaryPy;
  std::vector<G4double> fPrimaryPz;
  std::vector<G4double> fPrimaryPtotal;
  std::vector<G4double> fPrimaryEtotal;
    
  std::vector<G4double> fPrimaryTrackLength;
  std::vector<G4double> fPrimaryTime;
    
  G4int fPrimaryTotalElectrons;
  G4int fPrimaryTotalPhotons;
    
  /// Particle gun information
  
  std::vector<G4double> fKinEnergyOfParticleGun;
  std::vector<G4double> fMomentumOfParticleGun;
  std::vector<G4double> fParticleGunMomentumDirectionU;
  std::vector<G4double> fParticleGunMomentumDirectionV;
  std::vector<G4double> fParticleGunMomentumDirectionW;
  std::vector<G4double> fParticleGunTheta;
  std::vector<G4double> fParticleGunPhi;
  std::vector<G4double> fParticleGunMag;
    
public:
  // Vector of secondary particle
  
  std::vector<G4int> fParentIDOfSec;
  std::vector<G4double> fSecondaryPDG;
  std::vector<G4int> fProcess;
    
  std::vector<G4double> fSecondaryEnergy;
  std::vector<G4double> fSecPosX;
  std::vector<G4double> fSecPosY;
  std::vector<G4double> fSecPosZ;
  std::vector<G4double> fSecXVertex;
  std::vector<G4double> fSecYVertex;
  std::vector<G4double> fSecZVertex;
  std::vector<G4double> fSecondaryPx;
  std::vector<G4double> fSecondaryPy;
  std::vector<G4double> fSecondaryPz;
  std::vector<G4double> fSecDirPx;
  std::vector<G4double> fSecDirPy;
  std::vector<G4double> fSecDirPz;
  std::vector<G4double> fSecEtotal;
  std::vector<G4double> fSecPtotal;
  std::vector<G4double> fSecEdep;
    
  std::vector<G4int> fSecTrackID;
  std::vector<G4int> fSecParentID;
  std::vector<G4double> fSecTrackLength;
  std::vector<G4double> fSecondaryTime;
  std::vector<G4double> fSecCharge;

  //pi0->gamma gamma -> e+e- e+e-:
  std::vector<G4double> fpi0TrkID;
  std::vector<G4double> fpi0GammaTrkID;
  
  G4int fTotalSecondary;
    
  /// Material map variables:
  G4double fMatMapScintillator;
  G4double fMatMapLayerSupport;
  G4double fMatMapSteelBar;
  G4double fMatMapScintillator_radLen;
  G4double fMatMapLayerSupport_radLen;
  G4double fMatMapSteelBar_radLen;
  
  G4double fScatAngle1;
  G4double fScatAngle2;
  
 // to think ?????
  void AddEnergy(G4double te);
  void AddTrakLenCharg(G4double sl);
  void CountStepsCharg();
  void AddTrakLenNeutr(G4double sl);
  void CountStepsNeutr();

  void StorePrimaryPosition(G4double x, G4double y, G4double z);
  void StorePrimaryKineticInfo(const G4DynamicParticle* part);
  void StorePrimaryEdep(G4double Edep);
  void StorePrimaryTime(G4double time);
  void StorePrimaryTrackLength(G4double length);
  void AddSecondaryPhoton();
  void AddSecondaryElectron();
  void FillKlongSecondaryID(G4int trackID);
  G4int GetKlongSecondaryID();
  
  // store secondary particle
  void StoreProcessName(G4int ProcessName);
  void StoreSecPDG(G4double pdg);
  void StoreSecondaryPosition(G4double x, G4double y, G4double z);
  void StoreSecondaryKineticInfo(const G4DynamicParticle* part);
  void StoreSecondaryMomentumDir(G4double x, G4double y, G4double z);
  void StoreTotalSecondary(G4int count);
  void StoreVertexPosSecondary(G4double x, G4double y, G4double z);
  void StoreSecTrackID(G4int trackId);
  void StoreSecParentID(G4int parentId);
  void StoreSecTrackLength(G4double trackLength);
  void StoreSecondaryEdep(G4double Edep);
  void StoreSecondaryTime(G4double time);
  void StoreSecondaryCharge(G4double charge);
  
  /// store particle gun information
    
  void StoreKinEnergyOfParticleGun(G4double KinEnergyOfParticleGun);
  void StoreMomentumOfParticleGun(G4double MomentumOfParticleGun);
  void StoreParticleGunMomentumDirectionU(G4double ParticleGunMomentumDirectionU);
  void StoreParticleGunMomentumDirectionV(G4double ParticleGunMomentumDirectionV);
  void StoreParticleGunMomentumDirectionW(G4double ParticleGunMomentumDirectionW);
  void StoreParticleGunTheta(G4double ParticleGunTheta);
  void StoreParticleGunPhi(G4double ParticleGunPhi);
  void StoreParticleGunMag(G4double ParticleGunMag);
    
  void ClearKinEnergyOfParticleGun();
  void ClearMomentumOfParticleGun();
  void ClearParticleGunMomentumDirectionU();
  void ClearParticleGunMomentumDirectionV();
  void ClearParticleGunMomentumDirectionW();
  void ClearParticleGunTheta();
  void ClearParticleGunPhi();
  void ClearParticleGunMag();
    
  void StoreParentIDOfSec(G4int ParentID);
  std::vector<G4int> GetParentIDOfSec();

  void StorePi0TrkID(G4double trkID);
  std::vector<G4double> ListPi0TrkID();
  void StorePi0GammaTrkID(G4double trkID);
  std::vector<G4double> ListPi0GammaTrkID();
  
  // radiation length
  void StoreAngle1(G4double ang);
  void StoreAngle2(G4double ang);
    
  void MatMapScintillator(G4double dl, G4double dl2);
  void MatMapLayerSupport(G4double dl, G4double dl2);
  void MatMapSteelBar(G4double dl, G4double dl2);
  int StoreCountOfVolume();

  G4int fcount;

private:

  G4double fTotalEnergy;
  G4double fTrackLenCharge, fTrackLenNeutr;
  G4int fCountStepCharge, fCountStepNeutr;
  G4int fCountHitEachLayer;
  G4int fCountChildPos, fCountChildNeg;
  
  std::array<G4int, kDim> fDriftHCID;
  std::array<G4int, 2> fDriftHistoID;
};

// inline functions
inline void MathSimEventAction::StoreProcessName(G4int ProcessName){
    fProcess.push_back(ProcessName);
}

inline void MathSimEventAction::StoreSecTrackID(G4int trackId){
    fSecTrackID.push_back(trackId);
}

inline void MathSimEventAction::StoreSecParentID(G4int parentId){
    fSecParentID.push_back(parentId);
}

inline void MathSimEventAction::StoreSecTrackLength(G4double length){
    fSecTrackLength.push_back(length);
}

inline void MathSimEventAction::StoreSecondaryEdep(G4double Edep){
    fSecEdep.push_back(Edep);
}

inline void MathSimEventAction::StoreSecondaryTime(G4double time){
    fSecondaryTime.push_back(time);
}

inline void MathSimEventAction::StoreSecondaryCharge(G4double charge){
    fSecCharge.push_back(charge);
}

// ************************************
// **** Function for particle gun: ****
// ************************************

/// Added in order to check particle gun properties: the energy, momentum and angular distribution

inline void MathSimEventAction::StoreKinEnergyOfParticleGun(G4double KinEnergyOfParticleGun){
    fKinEnergyOfParticleGun.push_back(KinEnergyOfParticleGun);
}
inline void MathSimEventAction::StoreMomentumOfParticleGun(G4double MomentumOfParticleGun){
    fMomentumOfParticleGun.push_back(MomentumOfParticleGun);
}
inline void MathSimEventAction::StoreParticleGunMomentumDirectionU(G4double ParticleGunMomentumDirectionU){
    fParticleGunMomentumDirectionU.push_back(ParticleGunMomentumDirectionU);
}
inline void MathSimEventAction::StoreParticleGunMomentumDirectionV(G4double ParticleGunMomentumDirectionV){
    fParticleGunMomentumDirectionV.push_back(ParticleGunMomentumDirectionV);
}
inline void MathSimEventAction::StoreParticleGunMomentumDirectionW(G4double ParticleGunMomentumDirectionW){
    fParticleGunMomentumDirectionW.push_back(ParticleGunMomentumDirectionW);
}
inline void MathSimEventAction::StoreParticleGunTheta(G4double ParticleGunTheta){
    fParticleGunTheta.push_back(ParticleGunTheta);
}
inline void MathSimEventAction::StoreParticleGunPhi(G4double ParticleGunPhi){
    fParticleGunPhi.push_back(ParticleGunPhi);
}
inline void MathSimEventAction::StoreParticleGunMag(G4double ParticleGunMag){
    fParticleGunMag.push_back(ParticleGunMag);
}
/// Clear vectors:

inline void MathSimEventAction::ClearKinEnergyOfParticleGun(){
    fKinEnergyOfParticleGun.clear();
}
inline void MathSimEventAction::ClearMomentumOfParticleGun(){
    fMomentumOfParticleGun.clear();
}
inline void MathSimEventAction::ClearParticleGunMomentumDirectionU(){
    fParticleGunMomentumDirectionU.clear();
}
inline void MathSimEventAction::ClearParticleGunMomentumDirectionV(){
    fParticleGunMomentumDirectionV.clear();
}
inline void MathSimEventAction::ClearParticleGunMomentumDirectionW(){
    fParticleGunMomentumDirectionW.clear();
}
inline void MathSimEventAction::ClearParticleGunTheta(){
    fParticleGunTheta.clear();
}
inline void MathSimEventAction::ClearParticleGunPhi(){
    fParticleGunPhi.clear();
}
inline void MathSimEventAction::ClearParticleGunMag(){
    fParticleGunMag.clear();
}
//// -------------------------------------------------------------------

//function for secondary position

inline void MathSimEventAction::StoreSecPDG(G4double pdg){
    fSecondaryPDG.push_back(pdg);
}

inline void MathSimEventAction::StoreSecondaryPosition(G4double x, G4double y, G4double z){
    fSecPosX.push_back(x);
    fSecPosY.push_back(y);
    fSecPosZ.push_back(z);
}
//function for Vertex position secondary
inline void MathSimEventAction::StoreVertexPosSecondary(G4double x, G4double y, G4double z){
    fSecXVertex.push_back(x);
    fSecYVertex.push_back(y);
    fSecZVertex.push_back(z);
}

inline void MathSimEventAction::StoreSecondaryMomentumDir(G4double x, G4double y, G4double z){
    fSecDirPx.push_back(x);
    fSecDirPy.push_back(y);
    fSecDirPz.push_back(z);
}

inline void MathSimEventAction::StoreParentIDOfSec(G4int ParentID){
    fParentIDOfSec.push_back(ParentID);
}
inline std::vector<G4int> MathSimEventAction::GetParentIDOfSec(){
    return fParentIDOfSec;
}

inline void MathSimEventAction::StoreSecondaryKineticInfo(const G4DynamicParticle* part){
    fSecondaryPx.push_back(part->GetMomentum().x()/CLHEP::MeV); //Momentum in Geant4 returned in energy equivalent
    fSecondaryPy.push_back(part->GetMomentum().y()/CLHEP::MeV);
    fSecondaryPz.push_back(part->GetMomentum().z()/CLHEP::MeV);
    fSecPtotal.push_back(part->GetTotalMomentum()/CLHEP::MeV); //Momentum in Geant4 returned in energy equivalent
    fSecEtotal.push_back(part->GetTotalEnergy()/CLHEP::MeV);
}

inline void MathSimEventAction::StorePi0TrkID(G4double trkID){
  fpi0TrkID.push_back(trkID);
}

inline std::vector<G4double> MathSimEventAction::ListPi0TrkID(){
  return fpi0TrkID;
}

inline void MathSimEventAction::StorePi0GammaTrkID(G4double trkID){
  fpi0GammaTrkID.push_back(trkID);
}

inline std::vector<G4double> MathSimEventAction::ListPi0GammaTrkID(){
  return fpi0GammaTrkID;
}

inline void MathSimEventAction::StoreTotalSecondary(G4int count){
    fTotalSecondary += count;
}

inline void MathSimEventAction::AddEnergy(G4double te){
    fTotalEnergy += te;
}

inline void MathSimEventAction::AddTrakLenCharg(G4double sl){
    fTrackLenCharge += sl;
}

inline void MathSimEventAction::CountStepsCharg(){
    fCountStepCharge ++;
}

inline void MathSimEventAction::AddTrakLenNeutr(G4double sl){
    fTrackLenNeutr += sl;
}

inline void MathSimEventAction::CountStepsNeutr(){
    fCountStepNeutr ++;
}

inline int MathSimEventAction::StoreCountOfVolume() {
    fcount += 1;
    if(fcount > 26) fcount = 0;
    return fcount;
}

inline void MathSimEventAction::StoreAngle1(G4double ang) {
    fScatAngle1 = ang;
}

inline void MathSimEventAction::StoreAngle2(G4double ang) {
    fScatAngle2 = ang;
}

// Primary particle step functions:
inline void MathSimEventAction::StorePrimaryPosition(G4double x, G4double y, G4double z){
  fPrimaryPosX.push_back(x);
  fPrimaryPosY.push_back(y);
  fPrimaryPosZ.push_back(z);
}
inline void MathSimEventAction::StorePrimaryKineticInfo(const G4DynamicParticle* part){
  fPrimaryPx.push_back(part->GetMomentum().x()/ CLHEP::MeV); //Momentum in Geant4 returned in energy equivalent
  fPrimaryPy.push_back(part->GetMomentum().y()/ CLHEP::MeV);
  fPrimaryPz.push_back(part->GetMomentum().z()/ CLHEP::MeV);
  fPrimaryPtotal.push_back(part->GetTotalMomentum()/ CLHEP::MeV); //Momentum in Geant4 returned in energy equivalent
  fPrimaryEtotal.push_back(part->GetTotalEnergy()/ CLHEP::MeV);
}

inline void MathSimEventAction::StorePrimaryEdep(G4double edep){
  fPrimaryEdep.push_back(edep);
}

inline void MathSimEventAction::StorePrimaryTime(G4double time){
  fPrimaryTime.push_back(time);
}

inline void MathSimEventAction::StorePrimaryTrackLength(G4double length){
    fPrimaryTrackLength.push_back(length);
}

inline void MathSimEventAction::FillKlongSecondaryID(G4int trackID){
  fKlongSecondaryID = trackID;
}

inline G4int MathSimEventAction::GetKlongSecondaryID(){
  return fKlongSecondaryID;
}

inline void MathSimEventAction::AddSecondaryPhoton(){
  fPrimaryTotalPhotons+=1;
}
inline void MathSimEventAction::AddSecondaryElectron(){
  fPrimaryTotalElectrons+=1;
}

// Add length for material map:
inline void MathSimEventAction::MatMapScintillator(G4double dl, G4double dl2){
  fMatMapScintillator += dl;
  fMatMapScintillator_radLen += dl2;
}
inline void MathSimEventAction::MatMapLayerSupport(G4double dl, G4double dl2){
  fMatMapLayerSupport += dl;
  fMatMapLayerSupport_radLen += dl2;
}
inline void MathSimEventAction::MatMapSteelBar(G4double dl, G4double dl2){
  fMatMapSteelBar += dl;
  fMatMapSteelBar_radLen += dl2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
