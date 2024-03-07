//SensitiveDetector::ProcessHits

G4bool EdepScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*) 
{
  G4double edep =  aStep->GetTotalEnergyDeposit();

  if (edep > 0)
  {
    G4int index = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    //Add the energy to the accumulable vector
    (static_cast<G4VectorAccumulable<G4double>*>(accumulableManager->GetAccumulable("doseAccumulable")))->AddToEntry(index, edep);
  }

  return 0;
}

//RunAction::RunAction()

RunAction::RunAction()
{

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  doseAccumulable = new G4VectorAccumulable<G4double>("doseAccumulable", 200); 
  accumulableManager->RegisterAccumulable(doseAccumulable);
}

//RunAction::EndOfRunAction(const G4Run*)

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  //Let's print the dose accumulable to see if it's working
  if(isMaster)
  {
    G4VectorAccumulable<G4double>* doseAccum = static_cast<G4VectorAccumulable<G4double>*>(accumulableManager->GetAccumulable("doseAccumulable"));
    auto doseVector = doseAccum->GetVector();
    for (const auto& item : doseVector)
    {
      std::cout << item << std::endl;
    }
  }
}
