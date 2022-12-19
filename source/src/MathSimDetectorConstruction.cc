#include "MathSimDetectorConstruction.hh"
#include "MathSimScintillatorSD.hh"
#include "MathSimLayerParameterisation.hh"
#include "MathSimConstants.hh"


#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4PVParameterised.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"
#include "G4Sphere.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* MathSimDetectorConstruction::fMagFieldMessenger = nullptr; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimDetectorConstruction::MathSimDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fAbsorberPV(nullptr),
   fGapPV(nullptr),
   fmodulePV(nullptr),
   fscintillatorPV(nullptr),
   fCheckOverlaps(true),
   fAlLayerPV(nullptr)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimDetectorConstruction::~MathSimDetectorConstruction()
{}

G4VPhysicalVolume* MathSimDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); // scintillator
  nistManager->FindOrBuildMaterial("G4_AIR");
  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_Galactic");    
  nistManager->FindOrBuildMaterial("G4_Steel");

  //Rock
  nistManager->FindOrBuildMaterial("G4_CONCRETE");
  
  G4Material* matAlHoneycomb = new G4Material("AlHoneycomb", 13, 27.0*g/mole, 30*kg/m3);
  
  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
    //for steel support
    
  G4int ncomponents, natoms;
    
    G4Element* Cr = new G4Element("Chromium" ,"Cr" , 24., 51.99*g/mole);
    G4Element* Fe = new G4Element("Iron"     ,"Fe" , 26., 55.84*g/mole);
    G4Element* Ni = new G4Element("Nickel"   ,"Ni" , 28., 58.69*g/mole);
    
    G4Material* inox =
    new G4Material("Stainless-Steel", 8*g/cm3, ncomponents=3);
    inox->AddElement(Fe, 74*perCent);
    inox->AddElement(Cr, 18*perCent);
    inox->AddElement(Ni,  8*perCent);
}

G4VPhysicalVolume* MathSimDetectorConstruction::DefineVolumes()
{
  // Geometry parameters in MathSimConstants.hh

  G4cout
    << G4endl
    << "------------------------------------------------------------" << G4endl
    << "---> The module length is " << moduleLength << G4endl
    << "------------------------------------------------------------" << G4endl;

  
  auto worldSizeXY = 1.2 * moduleSizeXY;
  auto worldSizeZ  = 1.2 * (moduleLength+rockLength+rockDistance);
  
  // Get materials
  auto defaultMaterial = G4Material::GetMaterial("G4_AIR");//AIR");
  auto absorberMaterial = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); //G4_Pb
  auto gapMaterial = G4Material::GetMaterial("G4_Galactic"); //AIR"); //liquidArgon
  auto supportLayerMaterial = G4Material::GetMaterial("AlHoneycomb");//G4_Al");
  auto supportBarMat = G4Material::GetMaterial("Stainless-Steel");//steel");
  auto rockMaterial = G4Material::GetMaterial("G4_CONCRETE");
  
  if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial || ! supportLayerMaterial || !supportBarMat) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("MathSimDetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  //     
  // World
  //
    auto worldS
      = new G4Box("World",           // its name
                   worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size
                         
  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 defaultMaterial,  // its material
                 "World");         // its name
                                   
  auto worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps

    // Rock
  //
  auto rockS
    = new G4Box("Rock",     // its name
                moduleSizeXY/2+barThickness, moduleSizeXY/2+barThickness, rockLength/2); // its size

  rockLV
    = new G4LogicalVolume(
                 rockS,     // its solid
                 rockMaterial,  // its material
                 "Rock");   // its name
  rockPV =
    new G4PVPlacement(
                 0,                // no rotation 
                 G4ThreeVector(0., 0., -worldSizeZ/2+0.1/1.2*worldSizeZ+rockLength/2),//G4ThreeVector(),  // at (0,0,0)
                 rockLV,          // its logical volume
                 "Rock",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number               
                 fCheckOverlaps);  // checking overlaps  
  
  // Module:
  //
  auto moduleS
    = new G4Box("Module",     // its name
		moduleSizeXY/2+barThickness, moduleSizeXY/2+barThickness, moduleLength/2); // its size
                         
  moduleLV
    = new G4LogicalVolume(
                 moduleS,     // its solid
                 defaultMaterial,  // its material
                 "Module");   // its name
  
  fmodulePV =
    new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., worldSizeZ/2-0.1/1.2*worldSizeZ-moduleLength/2),//G4ThreeVector(),  // at (0,0,0)
                 moduleLV,          // its logical volume                         
                 "Module",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
    // Actual material within bar
    auto steelBarS
          = new G4Box("Steel-Bar",            // its name
                      barThickness/2, barThickness/2, moduleLength/2); // its size
    
    steelBarLV = new G4LogicalVolume(
                                     steelBarS,        // its solid
                                     supportBarMat, // its material
                                    "Steel-Bar");          // its name
    // for steel bars:
    
    G4double x1,y1,z1;
    x1 = 0.0;
    y1 = 0.0;
    z1 = 0.0;
    std::array<G4double, 4> fXBars;
    std::array<G4double, 4> fYBars;
    std::array<G4double, 4> fZBars;
    
   for (auto i=0;i<nofBars;i++) {
    if(i/2 < 1){
    x1 = pow(-1, i) * (moduleSizeXY/2 + barThickness/2);
    y1 = pow(-1, i) * (moduleSizeXY/2 + barThickness/2);
    } else {
    x1 = pow(-1, i) * (moduleSizeXY/2 + barThickness/2);
    y1 = pow(-1, i+1) * (moduleSizeXY/2 + barThickness/2);
    }

    fXBars[i] = x1;
    fYBars[i] = y1;
    fZBars[i] = 0.0;
  }
    
    for (auto i=0;i<nofBars;i++) {
    steelBarPV = new G4PVPlacement(
                                   0,                // no rotation
    //                   G4ThreeVector(moduleSizeXY/2 + barThickness/2,moduleSizeXY/2 + barThickness/2,0),  // at (0,0,0)
                                   G4ThreeVector(fXBars[i],fYBars[i],fZBars[i]),  // at (0,0,0)
                                   steelBarLV,          // its logical volume
                                   "Steel-Bar",          // its name
                                   moduleLV,                // its mother  volume
                                   false,            // no boolean operation
                                   i,                // copy numbers
                                   false);  // checking overlaps
    }
    
  // LAYER --> including air
  //
  auto layerS
    = new G4Box("Layer",            // its name
                 moduleSizeXY/2, moduleSizeXY/2, moduleLength/2); // its size

  layerLV
    = new G4LogicalVolume(
                 layerS,        // its solid
                 defaultMaterial, // its material
                 "Layer");          // its name
  

  G4VPhysicalVolume* layerPV =
      new G4PVPlacement(
                   0,                // no rotation
                   G4ThreeVector(0., 0., 0.),//G4ThreeVector(),  // at (0,0,0)
                   layerLV,          // its logical volume
                   "Module",    // its name
                   moduleLV,          // its mother  volume
                   false,            // no boolean operation
                   0,                // copy number
                   fCheckOverlaps);  // checking overlaps

  // Actual scintillator material within layer
  
  auto flayerS
    = new G4Box("fLayer",            // its name
                   moduleSizeXY/2, moduleSizeXY/2, layerThickness/2); // its size

  flayerLV
    = new G4LogicalVolume(
			  flayerS,        // its solid
                   defaultMaterial, // its material
                   "fLayer");          // its name
    
  auto scintillatorS
    = new G4Box("Scintillator",            // its name
                 moduleSizeXY/2, moduleSizeXY/2, SCLayerThickness/2); // its size

  scintillatorLV
    = new G4LogicalVolume(
                 scintillatorS,        // its solid
                 absorberMaterial, // its material
                 "Scintillator");          // its name

  fscintillatorPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0,0,layerThickness/2 - SCLayerThickness/2),  // at (0,0,0)
                 scintillatorLV,          // its logical volume
                 "Scintillator",          // its name
                 flayerLV,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps

  // Layer alumium supporting structure
  auto AlLayerS
    = new G4Box("AlLayer",            // its name
                 moduleSizeXY/2, moduleSizeXY/2, SupLayerThickness/2); // its size

  auto AlLayerLV
    = new G4LogicalVolume(
                 AlLayerS,        // its solid
                 supportLayerMaterial, // its material
                 "AlLayer");          // its name
  fAlLayerPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0,0,-SupLayerThickness/2),  // at (0,0,0)
                 AlLayerLV,          // its logical volume
                 "AlLayer",          // its name
                 flayerLV,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps


  
//   Place layers in module: -> parameterised

  G4VPVParameterisation* cellParam = new MathSimLayerParameterisation();
  new G4PVParameterised("cellPhysical",flayerLV,layerLV,
                        kZAxis,nofLayers_B0+nofLayers_B1+nofLayers_B2,cellParam);
  
  //                                        
  // Visualization attributes
  //
  //worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());
  auto worldVisAtt= new G4VisAttributes(G4Colour(0,1.0,1.0));
  worldVisAtt->SetVisibility(true);
  worldLV->SetVisAttributes(worldVisAtt);
  
  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  moduleLV->SetVisAttributes(simpleBoxVisAtt);
  
  auto SCVisAtt= new G4VisAttributes(G4Colour(0,1.0,0));
  SCVisAtt->SetVisibility(true);
  SCVisAtt->SetForceSolid(true);
  scintillatorLV->SetVisAttributes(SCVisAtt);

  auto AlLayerVisAtt = new G4VisAttributes(G4Colour(1,1,1));
  AlLayerVisAtt->SetVisibility(true);
  AlLayerVisAtt->SetForceSolid(true);
  AlLayerLV->SetVisAttributes(AlLayerVisAtt);

  auto steelBarVisAtt = new G4VisAttributes(G4Colour(1,0,1));
  steelBarVisAtt->SetVisibility(true);
  steelBarVisAtt->SetForceSolid(true);
  steelBarLV->SetVisAttributes(steelBarVisAtt);

  auto rockVisAtt = new G4VisAttributes(G4Colour(1,0.3,0.3));
  rockVisAtt->SetVisibility(true);
  rockVisAtt->SetForceSolid(true);
  rockLV->SetVisAttributes(rockVisAtt);
  
  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimDetectorConstruction::ConstructSDandField()
{
  // Sensitive detector
  auto sdManager = G4SDManager::GetSDMpointer();
  G4String SDname;
  auto chamber1 = new MathSimScintillatorSD(SDname="/chamber1");
  sdManager->AddNewDetector(chamber1);
  scintillatorLV->SetSensitiveDetector(chamber1);

  // Victoria: this needs to be removed:
  
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
