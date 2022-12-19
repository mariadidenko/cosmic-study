#include "MathSimPrimaryGeneratorAction.hh"
#include "MathSimConstants.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4PhysicalConstants.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "G4ThreeVector.hh"

#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimPrimaryGeneratorAction::MathSimPrimaryGeneratorAction(MathSimEventAction* eventAction)
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr),
   fEventAction(eventAction),
   fParticleDefinition(0),
   fParticleMass(0),
   fParticleEnergy(0),
   fParticlePosX(0),fParticlePosY(0),fParticlePosZ(0),
   fParticleMomentumDirectionU(0),fParticleMomentumDirectionV(0),fParticleMomentumDirectionW(0),
   fParticleTime(0)
{
  const char  *inputfile="setupCry.file";
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
//  mParticleGun = new G4GeneralParticleSource();
    
    // Read the cry input file
    std::ifstream inputFile;
    inputFile.open(inputfile,std::ios::in);
    
    if (!inputFile.good()){
          G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
          G4cout<<"File is in the folder?"<<G4endl;
          G4cout<<"EXITING"<<G4endl;
        exit(0);
        }
    char buffer[1000];

    if (inputFile.fail()) {
      if( *inputfile !=0)  //....only complain if a filename was given
        G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
      InputState=-1;
    }else{
      std::string setupString("");
      while ( !inputFile.getline(buffer,1000).eof()) {
        setupString.append(buffer);
        setupString.append(" ");
      }

      CRYSetup *setup=new CRYSetup(setupString,"/Users/mdidenko/Geant4/cry_v1.7/data");

      gen = new CRYGenerator(setup);

      // set random number generator
      RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
      setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
      InputState=0;
    }
    // create a vector to store the CRY particle properties
    vect=new std::vector<CRYParticle*>;

    // Create the table containing all particle names
    particleTable = G4ParticleTable::GetParticleTable();
    
  // default particle kinematic
  
//  auto particleDefinition
//    = G4ParticleTable::GetParticleTable()->FindParticle("kaon0L");//kaon0L"); //chargedgeantino");
//  fParticleGun->SetParticleDefinition(particleDefinition);
//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
//  fParticleGun->SetParticleEnergy(5000*MeV); // this is kinetic energy
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimPrimaryGeneratorAction::InputCRY()
{
  InputState=1;
}


//----------------------------------------------------------------------------//
void MathSimPrimaryGeneratorAction::UpdateCRY(std::string* MessInput)
{
  CRYSetup *setup=new CRYSetup(*MessInput,"/Users/mdidenko/Geant4/cry_v1.7/data");

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState=0;
}

//----------------------------------------------------------------------------//
void MathSimPrimaryGeneratorAction::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,"/Users/mdidenko/Geant4/cry_v1.7/data");

    gen = new CRYGenerator(setup);

  // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
}

MathSimPrimaryGeneratorAction::~MathSimPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Clear particle gun information after filling of ntuples:
    fEventAction->ClearKinEnergyOfParticleGun();
    fEventAction->ClearMomentumOfParticleGun();
    fEventAction->ClearParticleGunMomentumDirectionU();
    fEventAction->ClearParticleGunMomentumDirectionV();
    fEventAction->ClearParticleGunMomentumDirectionW();
    fEventAction->ClearParticleGunTheta();
    fEventAction->ClearParticleGunPhi();
    fEventAction->ClearParticleGunMag();
    
    // This function is called at the begining of event

    G4double moduleHalfLength = 0.;
    auto moduleLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Module");

    // Check that the world volume has box shape
    G4Box* moduleBox = nullptr;
    if (  moduleLV ) {
      moduleBox = dynamic_cast<G4Box*>(moduleLV->GetSolid());
    }
    
    if ( moduleBox ) {
      moduleHalfLength = moduleBox->GetZHalfLength();
    }
    
    // Set gun position
    //fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -moduleHalfLength-1*m));
    auto worldSizeZ  = 1.2 * (moduleLength+rockLength+rockDistance);
    G4double module_zposition = worldSizeZ/2-0.1/1.2*worldSizeZ-moduleLength/2;
//    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., module_zposition-moduleHalfLength));

    /////////////////////////
    
    if (InputState != 0) {
      G4String* str = new G4String("CRY library was not successfully initialized");
      //G4Exception(*str);
      G4Exception("MathSimPrimaryGeneratorAction", "1",
                  RunMustBeAborted, *str);
    }else{
        G4String* str = new G4String("CRY library was successfully initialized :)");
    }
    
    G4String particleName;
    bool passEKinThreshold = false;
    
    int countAttempt = 0;
    do{
        vect->clear();
        gen->genEvent(vect);
        
        for(unsigned j=0; j<vect->size(); j++){
            
            G4ParticleDefinition*    particle;
            particleName=CRYUtils::partName((*vect)[j]->id());
            particle = particleTable->FindParticle((*vect)[j]->PDGid());
            
            double kinEnergy = (*vect)[j]->ke()*MeV;
            if(kinEnergy >= 2670) passEKinThreshold = true;//2670
            else countAttempt ++;
        }
        
    }while(passEKinThreshold == false);
    
    //....debug output
    G4cout << "\nEvent=" << anEvent->GetEventID() << " "
           << "CRY generated nparticles=" << vect->size()
           << " pass Ekin threshold: "<< passEKinThreshold << G4endl;
    
    for ( unsigned j=0; j<vect->size(); j++) {
      particleName=CRYUtils::partName((*vect)[j]->id());

      auto particleDefinition=particleTable->FindParticle((*vect)[j]->PDGid());
        
      //....debug output
     std:: cout << "....debug output  "          << particleName << " "
           << "charge="      << (*vect)[j]->charge() << " "
           << std::setprecision(4)
           << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
           << "pT (MeV)="<< sqrt(((*vect)[j]->ke() * (*vect)[j]->ke())*MeV + (2*(*vect)[j]->ke())*MeV * (particleDefinition->GetPDGMass())*MeV)<< " "
           << "pos (m)"
           << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
           << " " << "direction cosines "
           << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
           << " " << std::endl;

      
      fParticleEnergy=(*vect)[j]->ke()*MeV;
      fParticleMass=particleDefinition->GetPDGMass()*MeV;
      fParticlePosX=(*vect)[j]->x()*m;
      fParticlePosY=(*vect)[j]->y()*m;
      fParticlePosZ=(*vect)[j]->z()*m;
      fParticleMomentumDirectionU=(*vect)[j]->u();
      fParticleMomentumDirectionV=(*vect)[j]->v();
      fParticleMomentumDirectionW=(*vect)[j]->w();
      fParticleTime=(*vect)[j]->t();
        
        std::cout<<"fParticleDefinition: "<<(*vect)[j]->PDGid()<<" "<<std::endl;
        std::cout<<"fParticleEnergy: "<<fParticleEnergy<<" MeV" <<std::endl;
        std::cout<<"fParticleMomentum: "<<sqrt(fParticleEnergy*fParticleEnergy + 2*fParticleEnergy*fParticleMass)<<" MeV" <<std::endl;
        std::cout<<"fParticlePosX: "<<fParticlePosX<<" m" <<std::endl;
        std::cout<<"fParticlePosY: "<<fParticlePosY<<" m" <<std::endl;
        std::cout<<"fParticlePosZ: "<<fParticlePosZ<<" m" <<std::endl;
        std::cout<<"fParticleMomentumDirectionU: "<<fParticleMomentumDirectionU<<"" <<std::endl;
        std::cout<<"fParticleMomentumDirectionV: "<<fParticleMomentumDirectionV<<"" <<std::endl;
        std::cout<<"fParticleMomentumDirectionW: "<<fParticleMomentumDirectionW<<"" <<std::endl;
        std::cout<<"fParticleTime: "<<fParticleTime<<"" <<std::endl;
        std::cout<<std::endl;
        
        fEventAction->StoreKinEnergyOfParticleGun(fParticleEnergy);
        fEventAction->StoreMomentumOfParticleGun(sqrt(fParticleEnergy*fParticleEnergy + 2*fParticleEnergy*fParticleMass));
        fEventAction->StoreParticleGunMomentumDirectionU(fParticleMomentumDirectionU);
        fEventAction->StoreParticleGunMomentumDirectionV(fParticleMomentumDirectionV);
        fEventAction->StoreParticleGunMomentumDirectionW(fParticleMomentumDirectionW);
        fEventAction->StoreParticleGunTheta(G4ThreeVector(fParticleMomentumDirectionU,fParticleMomentumDirectionV, fParticleMomentumDirectionW).theta());
        fEventAction->StoreParticleGunPhi(G4ThreeVector(fParticleMomentumDirectionU,fParticleMomentumDirectionV, fParticleMomentumDirectionW).phi());
        fEventAction->StoreParticleGunMag(G4ThreeVector(fParticleMomentumDirectionU,fParticleMomentumDirectionV, fParticleMomentumDirectionW).mag());
        
        fParticleGun->SetParticleDefinition(particleDefinition);
        fParticleGun->SetParticleMomentum(sqrt(fParticleEnergy*fParticleEnergy + 2*fParticleEnergy*fParticleMass));
//        fParticleGun->SetParticleEnergy(fParticleEnergy);
        fParticleGun->SetParticlePosition(G4ThreeVector(fParticlePosX,fParticlePosY, (module_zposition - moduleHalfLength)));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(fParticleMomentumDirectionU,fParticleMomentumDirectionV, fParticleMomentumDirectionW));
        fParticleGun->SetParticleTime(fParticleTime);
        fParticleGun->GeneratePrimaryVertex(anEvent);
    
        delete (*vect)[j];
        
        }
  
  /*
  G4double x0  = 0*m, y0  = 0*m, z0  = -moduleHalfLength;
  G4double dx0 = 10*m, dy0 = 10*m, dz0 = 0*mm;
  x0 += dx0*(G4UniformRand()-0.5);
  y0 += dy0*(G4UniformRand()-0.5);
  z0 += dz0*(G4UniformRand()-0.5);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  */
  // Set gun direction
  /*G4double theta = 2*CLHEP::pi*G4UniformRand();
  G4double phi = std::acos(1-2*G4UniformRand());
  G4double x_dir = std::sin(phi) * std::cos(theta);
  G4double y_dir = std::sin(phi) * std::sin(theta);
  G4double z_dir = std::abs(std::cos(phi));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x_dir,y_dir,z_dir));
   */
  
   
  // **************************************************************************************
  // *************************** New particle gun *****************************************
  // **************************************************************************************

//    G4int iEvent = anEvent->GetEventID() ;
//    if (iEvent == 0){
//
//      G4cout << mParticleGun->GetParticleDefinition()->GetParticleName()           << " " ;
//      G4cout << mParticleGun->GetCurrentSource()->GetEneDist()->GetEnergyDisType() << " " ;
//      G4cout << mParticleGun->GetCurrentSource()->GetPosDist()->GetPosDisType()    << G4endl ;
//
//  // Check if optical photon wavelength is within limits set for material optical properties tables.
//    }
//
//    mParticleGun->GeneratePrimaryVertex(anEvent);
//
//    if(mParticleGun->GetParticleDefinition()->GetParticleName() == "muon"){
//
//        G4double energy = mParticleGun->GetParticleEnergy() ;
//    }
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
