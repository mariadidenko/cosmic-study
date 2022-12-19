#ifndef MathSimDetectorConstruction_h
#define MathSimDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

class MathSimDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MathSimDetectorConstruction();
    virtual ~MathSimDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    // get methods
    //
    // To be removed:
    const G4VPhysicalVolume* GetAbsorberPV() const;
    const G4VPhysicalVolume* GetGapPV() const;
  
    const G4VPhysicalVolume* GetmodulePV() const;
    const G4VPhysicalVolume* GetscintillatorPV() const;
    const G4VPhysicalVolume* GetAlLayerPV() const;
    const G4VPhysicalVolume* GetSteelBarPV() const;
  
    const G4Material* GetMaterial() const;
    void SetContainMaterial (G4String);
  
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // data members
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // magnetic field messenger
     
    G4VPhysicalVolume*   fAbsorberPV; // the absorber physical volume
    G4VPhysicalVolume*   fGapPV;      // the gap physical volume

    G4LogicalVolume* moduleLV;
    G4LogicalVolume* layerLV;
    G4LogicalVolume* scintillatorLV;
    G4LogicalVolume* steelBarLV;
    G4LogicalVolume* barLV;
    G4LogicalVolume* flayerLV;
    G4LogicalVolume* rockLV;
  
    G4VPhysicalVolume* fmodulePV;
    G4VPhysicalVolume* fscintillatorPV;
    G4VPhysicalVolume* fAlLayerPV;
    G4VPhysicalVolume* steelBarPV;
    G4VPhysicalVolume* rockPV;
  
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};

// inline functions

inline const G4VPhysicalVolume* MathSimDetectorConstruction::GetAbsorberPV() const { 
  return fAbsorberPV; 
}

inline const G4VPhysicalVolume* MathSimDetectorConstruction::GetGapPV() const  { 
  return fGapPV; 
}

inline const G4VPhysicalVolume* MathSimDetectorConstruction::GetscintillatorPV() const {
  return fscintillatorPV;
}

inline const G4VPhysicalVolume* MathSimDetectorConstruction::GetmodulePV() const {
  return fmodulePV;
}

inline const G4VPhysicalVolume* MathSimDetectorConstruction::GetAlLayerPV() const {
  return fAlLayerPV;
}

inline const G4VPhysicalVolume* MathSimDetectorConstruction::GetSteelBarPV() const {
  return steelBarPV;
}

inline const G4Material* MathSimDetectorConstruction::GetMaterial() const {
  
    auto nistManager = G4NistManager::Instance();
    
    G4Material* scint_mat = nistManager ->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4Material* AlMat = nistManager ->FindOrBuildMaterial("G4_Al");
    G4Material* Galactic = nistManager ->FindOrBuildMaterial("G4_Galactic");
    G4Material* air = nistManager ->FindOrBuildMaterial("G4_AIR");
    
    return air;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

