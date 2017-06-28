// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "CustomerManager.h"
#include "Customer.h"


// Sets default values
ACustomerManager::ACustomerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomerManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void ACustomerManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ACustomerManager::Init()
{
	mMaxPrice = 250;
	mRecurringWaitTime = 3;
	mAverageTrust = 0;

	mRecurringCustomersLoaded = false;

	mRandomEncounters = -1;
	mRecurringEncounters = 0;
	mAdventurerEncounters = 0;

	//load data from files
	//load in customer dialog
	LoadAllDialog();
	LoadRecurringSpecificDialog();

	//customer files
	FString customerListFileName = "Content/CustomerParameterFiles/Customers.txt";
	LoadCustomerData(customerListFileName, "customer_recurring");

	SetAverageTrust();
}

//gets a customer for use in the shop
ACustomer* ACustomerManager::GetCustomer()
{
	//4 strikes and the customer leaves
	mStrikes = 0;
	
	UE_LOG(LogTemp, Log, TEXT("GetCustomer()"));
	UE_LOG(LogTemp, Log, TEXT("mRecurringEncounters: %d"), mRecurringEncounters);

	if (mRecurringCustomersLoaded)
	{
		//if the recurring villager's trust has increased or decreased, their frequency will increase or decrease
		if (mRecurringEncounters >= 5)
		{
			CheckTrust();
			mRecurringEncounters = 0;
		}

		//check if there's been enough random characters for a recurring customer to show up
		//and give a random customer if not
		if (mRandomEncounters >= mRecurringWaitTime)
		{
			UE_LOG(LogTemp, Log, TEXT("Get Recurring Customer for GetCustomer()"));
			mRecurringEncounters++;
			mRandomEncounters = 0;
			return GetRecurringCustomer();
		}
		else if (mRandomEncounters == -1)
		{
			//gives Knarr
			mRandomEncounters++;
			SetGreeting(mRecurringCustomers[8]);
			SetStory(mRecurringCustomers[8]);
			SetInitialDialog(mRecurringCustomers[8]);
			SetResponses(mRecurringCustomers[8]);
			SetFarewell(mRecurringCustomers[8]);
			return mRecurringCustomers[8];
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Get Random Customer for GetCustomer()"));
			mRandomEncounters++;
			return GenerateRandomCustomer();
		}
	}
	return new ACustomer();
}

//Get next customer that will come up to the counter
ACustomer* ACustomerManager::GetRecurringCustomer()
{
	TArray<int> randomOrder;
	int listSize = mRecurringCustomers.Num();
	int triedCounter = 0;
	bool foundCustomer = false;
	int num = 0;
	float percentage;

	//get random order of recurring customers to try
	randomOrder = GetRandomCustomerOrder(listSize);

	//checking if customer is trusting enough to visit the shop
	while (triedCounter < listSize)
	{
		num = randomOrder[triedCounter];

		//roll for percentage for them to show up
		percentage = FMath::FRandRange(0, 1.0);
		if (mRecurringCustomers[num]->GetTrust() > percentage)
		{
			foundCustomer = true;
			triedCounter = listSize + 1;
		}
		else
		{
			triedCounter++;
		}
	}

	//if a willing customer is found, return them
	//if all recurring customers were checked to see if they'd come back and failed, make a random cutsomer

	if (foundCustomer)
	{
		UE_LOG(LogTemp, Log, TEXT("Got Recurring %d"), num);
		UE_LOG(LogTemp, Log, TEXT("%s Trust: %f"), *(mRecurringCustomers[num]->GetName()), mRecurringCustomers[num]->GetTrust());
		mRecurringCustomers[num]->UpdateAppraisal();
		mRecurringCustomers[num]->SetPrevTrust(mRecurringCustomers[num]->GetTrust());

		SetGreeting(mRecurringCustomers[num]);
		SetStory(mRecurringCustomers[num]);
		if (mRecurringCustomers[num]->GetLineCount() > 0)
			SetItemEnum(mRecurringCustomers[num], FMath::RandRange(0, 2));
		SetInitialDialog(mRecurringCustomers[num]);
		SetResponses(mRecurringCustomers[num]);
		SetFarewell(mRecurringCustomers[num]);
			

		return mRecurringCustomers[num];
	}
	else
	{
		//generate random customer
		UE_LOG(LogTemp, Log, TEXT("Got Random"), num);
		return GenerateRandomCustomer();
	}

}

//populates int array of non repeating numbers in an array's size range
TArray<int> ACustomerManager::GetRandomCustomerOrder(int size)
{
	TArray<int> randomOrder;
	int num;

	while (randomOrder.Num() < size)
	{
		num = FMath::RandRange(0, size - 1);
		randomOrder.AddUnique(num);
	}

	return randomOrder;
}

//creates a new random status customer
ACustomer* ACustomerManager::GenerateRandomCustomer()
{
	int maxMoney = 1000;
	int minMoney = 400;

	float maxAppraisal = 1.35;
	float minAppraisal = 1.15;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();

	//Note pointer is const, for added protection
	ACustomer* const customer = GetWorld()->SpawnActor<ACustomer>(ACustomer::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

	customer->SetIsRecurring(false);
	customer->SetName("Villager");
	customer->SetMoney(FMath::RandRange(minMoney, maxMoney));
	customer->SetTrust(0.0);
	customer->SetAppraisal(FMath::FRandRange(minAppraisal, maxAppraisal));
	
	SetTraitEnum(customer, FMath::RandRange(0, 3), true);
	SetTraitEnum(customer, FMath::RandRange(0, 3), false);
	SetItemEnum(customer, FMath::RandRange(0, 2));

	SetGreeting(customer);
	SetInitialDialog(customer);
	SetResponses(customer);
	SetFarewell(customer);
	int num = FMath::RandRange(0, mAngry.Num()-1);
	customer->SetAngry(mAngry[num]);

	return customer;
}

void ACustomerManager::SetAverageTrust()
{
	float averageTrust = 0;
	float size = mRecurringCustomers.Num();

	//get average trust
	for (int i = 0; i < size; i++)
	{
		averageTrust += mRecurringCustomers[i]->GetTrust();
	}

	averageTrust = (float)(averageTrust / size);
	mAverageTrust = averageTrust;

	UE_LOG(LogTemp, Log, TEXT("Average Trust: %f"), mAverageTrust);
}

//check if recurring customer frquency should be increased or decreased
void ACustomerManager::CheckTrust()
{
	float averageTrust = 0;
	float size = mRecurringCustomers.Num();
	float trustDifference;

	//get average trust
	for (int i = 0; i < size; i++)
	{
		averageTrust += mRecurringCustomers[i]->GetTrust();
	}

	averageTrust = (float)(averageTrust / size);
	trustDifference = averageTrust - mAverageTrust;
	mAverageTrust = averageTrust;

	UE_LOG(LogTemp, Log, TEXT("Trust Difference: %f"), trustDifference);
	UE_LOG(LogTemp, Log, TEXT("Average Trust: %f"), mAverageTrust);

	//if the recurring customers average trust is 20% more than before, they return to the shop more 
	if (trustDifference >= 0.15)
	{
		//min wait time is 1 random customer
		if (mRecurringWaitTime > 1)
			mRecurringWaitTime--;
	}
	//if the average trust has decreased at all, they return less
	else if (trustDifference < 0)
	{
		//max wait time is 5 random customers
		if(mRecurringWaitTime < 5)
			mRecurringWaitTime++;
	}

}

//adjust customer trust from what player choses for offered item
void ACustomerManager::AdjustCustomerTrustItem(ACustomer* customer, ItemEnum offeredItem)
{
	ItemEnum item = customer->GetDesiredItem();

	if (offeredItem == item)
	{
		customer->AdjustTrust(0.05);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
	}
	else
	{
		customer->AdjustTrust(-0.05);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
		mStrikes++;
	}

}

//adjust customer trust from player sales pitch
void ACustomerManager::AdjustCustomerTrustPitch(ACustomer* customer, TraitEnum pitchTrait)
{
	TraitEnum dominate = customer->GetDominateTrait();
	TraitEnum secondary = customer->GetSecondaryTrait();

	if (pitchTrait == dominate)
	{
		customer->AdjustTrust(0.25);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
	}
	else if (pitchTrait == secondary)
	{
		customer->AdjustTrust(0.15);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
	}
	else
	{
		customer->AdjustTrust(-0.15);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
		mStrikes++;
	}

}

//adjust customer trust from player haggle prices
//the greater the gap from the customer's max amount they're will to pay the greater the penalty or reward
void ACustomerManager::AdjustCustomerTrustHaggle(ACustomer* customer, float itemBasePrice, int askingPrice)
{
	float maxAmount = customer->GetAppraisal() * itemBasePrice;

	if (askingPrice > maxAmount * 2)
	{
		customer->AdjustTrust(-0.3);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
		mStrikes++;
	}
	else if (askingPrice > maxAmount * 1.5)
	{
		customer->AdjustTrust(-0.15);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
		mStrikes++;
	}
	else if (askingPrice > maxAmount)
	{
		customer->AdjustTrust(-0.05);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
		mStrikes++;
	}
	else if (askingPrice < maxAmount * 0.5)
	{
		customer->AdjustTrust(0.3);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
	}
	else if (askingPrice < maxAmount * 0.75)
	{
		customer->AdjustTrust(0.15);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
	}
	else
	{
		customer->AdjustTrust(0.05);
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customer->GetTrust());
	}
}

//Sets a maxumum for the haggling bar
//if the customer money is just used, sometimes the slider values are too large, making it difficult to ask for a price
int ACustomerManager::GetMaxPrice(ACustomer* customer, int itemBasePrice)
{
	int maxPrice = 0;
	int amount = itemBasePrice * 4;
	int customerMoney = customer->GetMoney();

	maxPrice = amount < customerMoney ? amount : customerMoney;

	return maxPrice;
}

int ACustomerManager::GetShopRating()
{
	int rating = 1;

	if (mAverageTrust < 1)
		rating = 1;
	else if (mAverageTrust >= 5)
		rating = 5;
	else
		rating = (int)mAverageTrust + 1;

	return rating;
}

//**************************************************************
//**
//**	Loading from file and setting data functions
//**
//**************************************************************

//load in data from text file for customers
//customer stats, dialog
void ACustomerManager::LoadCustomerData(FString fileName, FString fileContent)
{
	FString fileData;
	bool fileOpened = false;
	fileOpened = FFileHelper::LoadFileToString(fileData, *(FPaths::GameDir() + fileName));

	//check if loading file was a success
	if(fileOpened)
		UE_LOG(LogTemp, Log, TEXT("Opened file: %s, in LoadCustomerData()"), *(FPaths::GameDir() + fileName));
	if(!fileOpened)
		UE_LOG(LogTemp, Log, TEXT("Failed to Open file: %s, in LoadCustomerData()"), *(FPaths::GameDir() + fileName));

	TArray<FString> lines;
	int32 lineCount = fileData.ParseIntoArray(lines, TEXT("\n"), true);
	

	//fill array only if theres something in the file
	//engine breaks if you try to load in nothing
	if (lines.Num() > 0)
	{
		//customer stats
		if(fileContent == "customer_recurring")
			CreateRecurringCustomerList(lines, "customer_recurring");

		//dialog
		//recurring specific dialog
		if (fileContent == "recurring_greetings")
			CreateCustomerDialog(lines, "recurring_greetings");
		if (fileContent == "recurring_farewells")
			CreateCustomerDialog(lines, "recurring_farewells");
		if (fileContent == "recurring_story")
			CreateCustomerDialog(lines, "recurring_story");
		
		//Greetings
		if (fileContent == "greetings")
			CreateCustomerDialog(lines, "greetings");
		//Farewells
		if (fileContent == "farewells")
			CreateCustomerDialog(lines, "farewells");
		//Angry
		if (fileContent == "angry")
			CreateCustomerDialog(lines, "angry");

		//Responses
		//Item
		if (fileContent == "response_IP_F")
			CreateCustomerDialog(lines, "response_IP_F");
		if (fileContent == "response_IN_F")
			CreateCustomerDialog(lines, "response_IN_F");
		if (fileContent == "response_IP_W")
			CreateCustomerDialog(lines, "response_IP_W");
		if (fileContent == "response_IN_W")
			CreateCustomerDialog(lines, "response_IN_W");
		if (fileContent == "response_IP_A")
			CreateCustomerDialog(lines, "response_IP_A");
		if (fileContent == "response_IN_A")
			CreateCustomerDialog(lines, "response_IN_A");

		//Pitch
		if (fileContent == "response_PP_F")
			CreateCustomerDialog(lines, "response_PP_F");
		if (fileContent == "response_PN_F")
			CreateCustomerDialog(lines, "response_PN_F");
		if (fileContent == "response_PP_W")
			CreateCustomerDialog(lines, "response_PP_W");
		if (fileContent == "response_PN_W")
			CreateCustomerDialog(lines, "response_PN_W");
		if (fileContent == "response_PP_A")
			CreateCustomerDialog(lines, "response_PP_A");
		if (fileContent == "response_PN_A")
			CreateCustomerDialog(lines, "response_PN_A");

		//Haggle
		if (fileContent == "response_HP_F")
			CreateCustomerDialog(lines, "response_HP_F");
		if (fileContent == "response_HN_F")
			CreateCustomerDialog(lines, "response_HN_F");
		if (fileContent == "response_HP_W")
			CreateCustomerDialog(lines, "response_HP_W");
		if (fileContent == "response_HN_W")
			CreateCustomerDialog(lines, "response_HN_W");
		if (fileContent == "response_HP_A")
			CreateCustomerDialog(lines, "response_HP_A");
		if (fileContent == "response_HN_A")
			CreateCustomerDialog(lines, "response_HN_A");

		//initial dialog
		//Food
		if (fileContent == "dialog_AI_F")
			CreateCustomerDialog(lines, "dialog_AI_F");
		if (fileContent == "dialog_AS_F")
			CreateCustomerDialog(lines, "dialog_AS_F");
		if (fileContent == "dialog_FI_F")
			CreateCustomerDialog(lines, "dialog_FI_F");
		if (fileContent == "dialog_FS_F")
			CreateCustomerDialog(lines, "dialog_FS_F");

		if (fileContent == "dialog_IA_F")
			CreateCustomerDialog(lines, "dialog_IA_F");
		if (fileContent == "dialog_SA_F")
			CreateCustomerDialog(lines, "dialog_SA_F");
		if (fileContent == "dialog_IF_F")
			CreateCustomerDialog(lines, "dialog_IF_F");
		if (fileContent == "dialog_SF_F")
			CreateCustomerDialog(lines, "dialog_SF_F");

		//Weapons
		if (fileContent == "dialog_AI_W")
			CreateCustomerDialog(lines, "dialog_AI_W");
		if (fileContent == "dialog_AS_W")
			CreateCustomerDialog(lines, "dialog_AS_W");
		if (fileContent == "dialog_FI_W")
			CreateCustomerDialog(lines, "dialog_FI_W");
		if (fileContent == "dialog_FS_W")
			CreateCustomerDialog(lines, "dialog_FS_W");

		if (fileContent == "dialog_IA_W")
			CreateCustomerDialog(lines, "dialog_IA_W");
		if (fileContent == "dialog_SA_W")
			CreateCustomerDialog(lines, "dialog_SA_W");
		if (fileContent == "dialog_IF_W")
			CreateCustomerDialog(lines, "dialog_IF_W");
		if (fileContent == "dialog_SF_W")
			CreateCustomerDialog(lines, "dialog_SF_W");

		//Armor
		if (fileContent == "dialog_AI_A")
			CreateCustomerDialog(lines, "dialog_AI_A");
		if (fileContent == "dialog_AS_A")
			CreateCustomerDialog(lines, "dialog_AS_A");
		if (fileContent == "dialog_FI_A")
			CreateCustomerDialog(lines, "dialog_FI_A");
		if (fileContent == "dialog_FS_A")
			CreateCustomerDialog(lines, "dialog_FS_A");

		if (fileContent == "dialog_IA_A")
			CreateCustomerDialog(lines, "dialog_IA_A");
		if (fileContent == "dialog_SA_A")
			CreateCustomerDialog(lines, "dialog_SA_A");
		if (fileContent == "dialog_IF_A")
			CreateCustomerDialog(lines, "dialog_IF_A");
		if (fileContent == "dialog_SF_A")
			CreateCustomerDialog(lines, "dialog_SF_A");
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("File %s Empty"), *(fileContent));
	}
}

//Setting values after switching scenes
//must be called after reloading customers and dialog!!!
void ACustomerManager::LoadCustomerSaveData(FCustomerSaveData data)
{
	mRandomEncounters = data.mRandomEncounters;
	mRecurringEncounters = data.mRecurringEncounters;
	mRecurringWaitTime = data.mRecurringWaitTime;
	mAverageTrust = data.mAverageTrust;

	//setting all recurring customers trust and line count
	for (int i = 0; i < mRecurringCustomers.Num(); i++)
	{
		mRecurringCustomers[i]->SetTrust(data.mCustomerTrust[i]);
		mRecurringCustomers[i]->SetLineCount(data.mCustomerLineCount[i]);
	}

	UE_LOG(LogTemp, Log, TEXT("LOADED DATA IN: LoadCustomerSaveData()"));
	UE_LOG(LogTemp, Log, TEXT("mRandomEncounters: %d"), mRandomEncounters);
	UE_LOG(LogTemp, Log, TEXT("mRecurringEncounters: %d"), mRecurringEncounters);
	UE_LOG(LogTemp, Log, TEXT("mRecurringWaitTime: %d"), mRecurringWaitTime);
	UE_LOG(LogTemp, Log, TEXT("mAverageTrust: %f"), mAverageTrust);

	for (int i = 0; i < mRecurringCustomers.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("%s Trust: %f"), *(mRecurringCustomers[i]->GetName()), mRecurringCustomers[i]->GetTrust());
	}

	for (int i = 0; i < mRecurringCustomers.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("%s Line Count: %d"), *(mRecurringCustomers[i]->GetName()), mRecurringCustomers[i]->GetLineCount());
	}
}

//Setting values for switching scenes to give to save data
FCustomerSaveData ACustomerManager::SaveCustomerSaveData()
{
	FCustomerSaveData data;

	data.mRandomEncounters = mRandomEncounters;
	data.mRecurringEncounters = mRecurringEncounters;
	data.mRecurringWaitTime = mRecurringWaitTime;
	data.mAverageTrust = mAverageTrust;

	//setting all recurring customers trust and line count
	for (int i = 0; i < mRecurringCustomers.Num(); i++)
	{
		data.mCustomerTrust.Add(mRecurringCustomers[i]->GetTrust());
		data.mCustomerLineCount.Add(mRecurringCustomers[i]->GetLineCount());
	}

	UE_LOG(LogTemp, Log, TEXT("SAVED DATA IN: SaveCustomerSaveData()"));
	UE_LOG(LogTemp, Log, TEXT("Struct mRandomEncounters: %d"), data.mRandomEncounters);
	UE_LOG(LogTemp, Log, TEXT("Struct mRecurringEncounters: %d"), data.mRecurringEncounters);
	UE_LOG(LogTemp, Log, TEXT("Struct mRecurringWaitTime: %d"), data.mRecurringWaitTime);
	UE_LOG(LogTemp, Log, TEXT("Struct mAverageTrust: %f"), data.mAverageTrust);

	for (int i = 0; i < mRecurringCustomers.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Struct Trust: %f"), data.mCustomerTrust[i]);
	}

	for (int i = 0; i < mRecurringCustomers.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Struct Line Count: %d"), data.mCustomerLineCount[i]);
	}

	return data;
}


//**************************************************************
//**	Customer Stats Functions
//**************************************************************

//sets customer's desired item category
//food, weapon, armor
void ACustomerManager::SetItemEnum(ACustomer* customer, int item)
{
	//UE_LOG(LogTemp, Log, TEXT("Set item: %d to enum"), item);
	switch (item)
	{
		case 0:
			customer->SetDesiredItem(ItemEnum::FOOD);
			break;
		case 1:
			customer->SetDesiredItem(ItemEnum::WEAPON);
			break;
		case 2:
			customer->SetDesiredItem(ItemEnum::ARMOR);
			break;
		default:
			customer->SetDesiredItem(ItemEnum::FOOD);
			break;
	}
	
}

//sets customer's traits
//aesthetics, functionality, intelligence, stupidity
void ACustomerManager::SetTraitEnum(ACustomer* customer, int trait, bool isDomTrait)
{
	//UE_LOG(LogTemp, Log, TEXT("Set trait: %d to enum"), trait);
	switch (trait)
	{
		case 0:
			if (isDomTrait)
				customer->SetDominateTrait(TraitEnum::AESTHETICS);
			else
				customer->SetSecondaryTrait(TraitEnum::AESTHETICS);
			break;
		case 1:
			if (isDomTrait)
				customer->SetDominateTrait(TraitEnum::FUNCTIONALITY);
			else
				customer->SetSecondaryTrait(TraitEnum::FUNCTIONALITY);
			break;
		case 2:
			if (isDomTrait)
				customer->SetDominateTrait(TraitEnum::INTELLIGENCE);
			else
				customer->SetSecondaryTrait(TraitEnum::INTELLIGENCE);
			break;
		case 3:
			if (isDomTrait)
				customer->SetDominateTrait(TraitEnum::STUPIDITY);
			else
				customer->SetSecondaryTrait(TraitEnum::STUPIDITY);
			break;
		default:
			if (isDomTrait)
				customer->SetDominateTrait(TraitEnum::AESTHETICS);
			else
				customer->SetSecondaryTrait(TraitEnum::AESTHETICS);
			break;
	}
}

//takes in data from files and a the customer array type that needs to be filled (eg. reaccuring, adventures)
//fills customer array with cutomers and sets customer data
void ACustomerManager::CreateRecurringCustomerList(TArray<FString> data, FString customerType)
{
	TArray<ACustomer*> customers;

	//j loops through the customers
	//i loops through the data read from the file

	int j = 0;
	int k = 0;
	//i keeps track of the customer stat that we are on (0-8)
	//k keeps track of the index of the data read in we are on
	//there could be comments in data file, so two index trackers are used (i and k)
	//j keeps track of which customer we are filling out
	for (int i = 0; i < data.Num(); i++, k++)
	{
		//parse over lines from file that were comments (comments marked with *)
		while(data[i][0] == '*')
		{
			i++;
		}

		switch ((k + 8) % 8)
		{
		case 0:
		{
			//add a new customer to the temp array
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			const FVector SpawnLocation = GetActorLocation();
			const FRotator SpawnRotation = GetActorRotation();

			//Note pointer is const, for added protection
			//UPROPERTY()
			ACustomer* const customer = GetWorld()->SpawnActor<ACustomer>(ACustomer::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
			//customer->SetOwner(this);

			UE_LOG(LogTemp, Log, TEXT("add new customer %d"), j);
			customers.Add(customer);
			
			//customer number
			customers[j]->SetNum(FCString::Atoi(*data[i]));
			customers[j]->SetIsRecurring(true);
			break;
		}
		case 1:
			//customer name
			customers[j]->SetName(data[i]);
			break;
		case 2:
			//customer money
			//note Atoi is unsafe; no way to indicate errors
			customers[j]->SetMoney(FCString::Atoi(*data[i]));
			break;
		case 3:
			//customer trust
			customers[j]->SetTrust(FCString::Atof(*data[i]));
			customers[j]->SetPrevTrust(customers[j]->GetTrust());
			break;
		case 4:
		{
			//customer appraisal
			float appraisal = FCString::Atof(*data[i]) + customers[j]->GetTrust();
			//increase if trait aesthetics or stupidity else decrease
			appraisal += (customers[j]->GetDominateTrait() == TraitEnum::AESTHETICS || customers[j]->GetDominateTrait() == TraitEnum::STUPIDITY) ? 0.1 : (-0.1);
			appraisal += (customers[j]->GetSecondaryTrait() == TraitEnum::AESTHETICS || customers[j]->GetSecondaryTrait() == TraitEnum::STUPIDITY) ? 0.05 : (-0.05);

			customers[j]->SetAppraisal(appraisal);
			break;
		}
		case 5:
			//customer dominate trait
			SetTraitEnum(customers[j], FCString::Atoi(*data[i]), true);
			break;
		case 6:
			//customer secondary trait
			SetTraitEnum(customers[j], FCString::Atoi(*data[i]), false);
			break;
		case 7:
		{
			//customer desired item
			SetItemEnum(customers[j], FCString::Atoi(*data[i]));

			//customer dialog
			customers[j]->SetLineCount(-1);
			//SetGreeting(customers[j]);
			//SetStory(customers[j]);
			//SetInitialDialog(customers[j]);
			//SetResponses(customers[j]);
			//SetFarewell(customers[j]);
			int num = FMath::RandRange(0, mAngry.Num()-1);
			customers[j]->SetAngry(mAngry[num]);
			j++;
			break;
		}
		default:
			break;
		}
	}

	//set member customer array's to customers
	if (customerType == "customer_recurring")
	{
		mRecurringCustomers = customers;
		DisplayCustomerData(mRecurringCustomers);
		UE_LOG(LogTemp, Log, TEXT("Finished DisplayCustomerData()"));
		mRecurringCustomersLoaded = true;
	}

}


//print to output log customer stats
void ACustomerManager::DisplayCustomerData(TArray<ACustomer*> customers)
{
	for (int i = 0; i < customers.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Name: %s"), *(customers[i]->GetName()));
		UE_LOG(LogTemp, Log, TEXT("Money: %d"), customers[i]->GetMoney());
		UE_LOG(LogTemp, Log, TEXT("Trust: %f"), customers[i]->GetTrust());
		UE_LOG(LogTemp, Log, TEXT("Appraisal: %f"), customers[i]->GetAppraisal());
		switch (customers[i]->GetDominateTrait())
		{
			case TraitEnum::AESTHETICS:
				UE_LOG(LogTemp, Log, TEXT("Dominate Trait: Aesthetics"));
				break;
			case TraitEnum::FUNCTIONALITY:
				UE_LOG(LogTemp, Log, TEXT("Dominate Trait: Functionality"));
				break;
			case TraitEnum::INTELLIGENCE:
				UE_LOG(LogTemp, Log, TEXT("Dominate Trait: Intelligence"));
				break;
			case TraitEnum::STUPIDITY:
				UE_LOG(LogTemp, Log, TEXT("Dominate Trait: Stupidity"));
				break;
			default:
				UE_LOG(LogTemp, Log, TEXT("Dominate Trait: Not Set"));
				break;
		}
		switch (customers[i]->GetSecondaryTrait())
		{
		case TraitEnum::AESTHETICS:
			UE_LOG(LogTemp, Log, TEXT("Secondary Trait: Aesthetics"));
			break;
		case TraitEnum::FUNCTIONALITY:
			UE_LOG(LogTemp, Log, TEXT("Secondary Trait: Functionality"));
			break;
		case TraitEnum::INTELLIGENCE:
			UE_LOG(LogTemp, Log, TEXT("Secondary Trait: Intelligence"));
			break;
		case TraitEnum::STUPIDITY:
			UE_LOG(LogTemp, Log, TEXT("Secondary Trait: Stupidity"));
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("Secondary Trait: Not Set"));
			break;
		}
		switch (customers[i]->GetDesiredItem())
		{
		case ItemEnum::FOOD:
			UE_LOG(LogTemp, Log, TEXT("Desired Item: Food"));
			break;
		case ItemEnum::WEAPON:
			UE_LOG(LogTemp, Log, TEXT("Desired Item: Weapon"));
			break;
		case ItemEnum::ARMOR:
			UE_LOG(LogTemp, Log, TEXT("Desired Item: Armor"));
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("Desired Item: Not Set"));
			break;
		}
		UE_LOG(LogTemp, Log, TEXT("Greeting Dialog: %s"), *(customers[i]->GetGreeting()));
		UE_LOG(LogTemp, Log, TEXT("Story Dialog: %s"), *(customers[i]->GetStory()));
		UE_LOG(LogTemp, Log, TEXT("Initial Dialog: %s"), *(customers[i]->GetInitialDialog()));
		UE_LOG(LogTemp, Log, TEXT("Item Positive Response Dialog: %s"), *(customers[i]->GetResponsePositiveItem()));
		UE_LOG(LogTemp, Log, TEXT("Item Negative Response Dialog: %s"), *(customers[i]->GetResponseNegativeItem()));
		UE_LOG(LogTemp, Log, TEXT("Pitch Positive Response Dialog: %s"), *(customers[i]->GetResponsePositivePitch()));
		UE_LOG(LogTemp, Log, TEXT("Pitch Negative Response Dialog: %s"), *(customers[i]->GetResponseNegativePitch()));
		UE_LOG(LogTemp, Log, TEXT("Haggle Positive Response Dialog: %s"), *(customers[i]->GetResponsePositiveHaggle()));
		UE_LOG(LogTemp, Log, TEXT("Haggle Negative Response Dialog: %s"), *(customers[i]->GetResponseNegativeHaggle()));
		UE_LOG(LogTemp, Log, TEXT("Farewell Dialog: %s"), *(customers[i]->GetFarewell()));
		UE_LOG(LogTemp, Log, TEXT("Angry Dialog: %s"), *(customers[i]->GetAngry()));
	}
}

//**************************************************************
//**	Dialog Functions
//**************************************************************
//There are 24 different dialog catagories
//4 main attributes: Aesthetic, Functionality, Intelligence, Stupidity
//8 main catagories with one attribute as the dominate one: AI, AS, FI, FS, IA, IF, SA, SF
//within the 8 main catagories each is specific to an item: Food, Weapon, Armor

//Loads in all 24 dialog files
void ACustomerManager::LoadAllDialog()
{
	FString customerDialogPath = "Content/CustomerParameterFiles/Customer Dialog";
	FString responseDialogPath = "Content/CustomerParameterFiles/Customer Dialog/Responses";
	FString fileName;

	//dialog files
	//Greetings
	fileName = customerDialogPath + "/Greetings.txt";
	LoadCustomerData(fileName, "greetings");
	//Farewells
	fileName = customerDialogPath + "/Farewells.txt";
	LoadCustomerData(fileName, "farewells");
	//Angry
	fileName = customerDialogPath + "/Angry.txt";
	LoadCustomerData(fileName, "angry");

	//Responses
	//Item
	fileName = responseDialogPath + "/Item/Item_Positive_Food.txt";
	LoadCustomerData(fileName, "response_IP_F");
	fileName = responseDialogPath + "/Item/Item_Negative_Food.txt";
	LoadCustomerData(fileName, "response_IN_F");
	fileName = responseDialogPath + "/Item/Item_Positive_Weapon.txt";
	LoadCustomerData(fileName, "response_IP_W");
	fileName = responseDialogPath + "/Item/Item_Negative_Weapon.txt";
	LoadCustomerData(fileName, "response_IN_W");
	fileName = responseDialogPath + "/Item/Item_Positive_Armor.txt";
	LoadCustomerData(fileName, "response_IP_A");
	fileName = responseDialogPath + "/Item/Item_Negative_Armor.txt";
	LoadCustomerData(fileName, "response_IN_A");

	//Pitch
	fileName = responseDialogPath + "/Pitch/Pitch_Positive_Food.txt";
	LoadCustomerData(fileName, "response_PP_F");
	fileName = responseDialogPath + "/Pitch/Pitch_Negative_Food.txt";
	LoadCustomerData(fileName, "response_PN_F");
	fileName = responseDialogPath + "/Pitch/Pitch_Positive_Weapon.txt";
	LoadCustomerData(fileName, "response_PP_W");
	fileName = responseDialogPath + "/Pitch/Pitch_Negative_Weapon.txt";
	LoadCustomerData(fileName, "response_PN_W");
	fileName = responseDialogPath + "/Pitch/Pitch_Positive_Armor.txt";
	LoadCustomerData(fileName, "response_PP_A");
	fileName = responseDialogPath + "/Pitch/Pitch_Negative_Armor.txt";
	LoadCustomerData(fileName, "response_PN_A");

	//Haggle
	fileName = responseDialogPath + "/Haggle/Haggle_Positive_Food.txt";
	LoadCustomerData(fileName, "response_HP_F");
	fileName = responseDialogPath + "/Haggle/Haggle_Negative_Food.txt";
	LoadCustomerData(fileName, "response_HN_F");
	fileName = responseDialogPath + "/Haggle/Haggle_Positive_Weapon.txt";
	LoadCustomerData(fileName, "response_HP_W");
	fileName = responseDialogPath + "/Haggle/Haggle_Negative_Weapon.txt";
	LoadCustomerData(fileName, "response_HN_W");
	fileName = responseDialogPath + "/Haggle/Haggle_Positive_Armor.txt";
	LoadCustomerData(fileName, "response_HP_A");
	fileName = responseDialogPath + "/Haggle/Haggle_Negative_Armor.txt";
	LoadCustomerData(fileName, "response_HN_A");


	//Attributes
	//Food
	//Aesthetics/Functionality Dominate
	fileName = customerDialogPath + "/Aesthetics/CustomerDialog_Aesthetics-Intelligence-Food.txt";
	LoadCustomerData(fileName, "dialog_AI_F");
	fileName = customerDialogPath + "/Aesthetics/CustomerDialog_Aesthetics-Stupidity-Food.txt";
	LoadCustomerData(fileName, "dialog_AS_F");
	fileName = customerDialogPath + "/Functionality/CustomerDialog_Functionality-Intelligence-Food.txt";
	LoadCustomerData(fileName, "dialog_FI_F");
	fileName = customerDialogPath + "/Functionality/CustomerDialog_Functionality-Stupidity-Food.txt";
	LoadCustomerData(fileName, "dialog_FS_F");
	//Inelligence/Stupidity Dominate
	fileName = customerDialogPath + "/Intelligence/CustomerDialog_Intelligence-Aesthetics-Food.txt";
	LoadCustomerData(fileName, "dialog_IA_F");
	fileName = customerDialogPath + "/Intelligence/CustomerDialog_Intelligence-Functionality-Food.txt";
	LoadCustomerData(fileName, "dialog_IF_F");
	fileName = customerDialogPath + "/Stupidity/CustomerDialog_Stupidity-Aesthetics-Food.txt";
	LoadCustomerData(fileName, "dialog_SA_F");
	fileName = customerDialogPath + "/Stupidity/CustomerDialog_Stupidity-Functionality-Food.txt";
	LoadCustomerData(fileName, "dialog_SF_F");

	//Weapons
	//Aesthetics/Functionality Dominate
	fileName = customerDialogPath + "/Aesthetics/CustomerDialog_Aesthetics-Intelligence-Weapon.txt";
	LoadCustomerData(fileName, "dialog_AI_W");
	fileName = customerDialogPath + "/Aesthetics/CustomerDialog_Aesthetics-Stupidity-Weapon.txt";
	LoadCustomerData(fileName, "dialog_AS_W");
	fileName = customerDialogPath + "/Functionality/CustomerDialog_Functionality-Intelligence-Weapon.txt";
	LoadCustomerData(fileName, "dialog_FI_W");
	fileName = customerDialogPath + "/Functionality/CustomerDialog_Functionality-Stupidity-Weapon.txt";
	LoadCustomerData(fileName, "dialog_FS_W");
	//Inelligence/Stupidity Dominate
	fileName = customerDialogPath + "/Intelligence/CustomerDialog_Intelligence-Aesthetics-Weapon.txt";
	LoadCustomerData(fileName, "dialog_IA_W");
	fileName = customerDialogPath + "/Intelligence/CustomerDialog_Intelligence-Functionality-Weapon.txt";
	LoadCustomerData(fileName, "dialog_IF_W");
	fileName = customerDialogPath + "/Stupidity/CustomerDialog_Stupidity-Aesthetics-Weapon.txt";
	LoadCustomerData(fileName, "dialog_SA_W");
	fileName = customerDialogPath + "/Stupidity/CustomerDialog_Stupidity-Functionality-Weapon.txt";
	LoadCustomerData(fileName, "dialog_SF_W");
	
	//Armor
	//Aesthetics/Functionality Dominate
	fileName = customerDialogPath + "/Aesthetics/CustomerDialog_Aesthetics-Intelligence-Armor.txt";
	LoadCustomerData(fileName, "dialog_AI_A");
	fileName = customerDialogPath + "/Aesthetics/CustomerDialog_Aesthetics-Stupidity-Armor.txt";
	LoadCustomerData(fileName, "dialog_AS_A");
	fileName = customerDialogPath + "/Functionality/CustomerDialog_Functionality-Intelligence-Armor.txt";
	LoadCustomerData(fileName, "dialog_FI_A");
	fileName = customerDialogPath + "/Functionality/CustomerDialog_Functionality-Stupidity-Armor.txt";
	LoadCustomerData(fileName, "dialog_FS_A");
	//Inelligence/Stupidity Dominate
	fileName = customerDialogPath + "/Intelligence/CustomerDialog_Intelligence-Aesthetics-Armor.txt";
	LoadCustomerData(fileName, "dialog_IA_A");
	fileName = customerDialogPath + "/Intelligence/CustomerDialog_Intelligence-Functionality-Armor.txt";
	LoadCustomerData(fileName, "dialog_IF_A");
	fileName = customerDialogPath + "/Stupidity/CustomerDialog_Stupidity-Aesthetics-Armor.txt";
	LoadCustomerData(fileName, "dialog_SA_A");
	fileName = customerDialogPath + "/Stupidity/CustomerDialog_Stupidity-Functionality-Armor.txt";
	LoadCustomerData(fileName, "dialog_SF_A");
}

//loads from recurring customer files
//must add name to array if new folder is added!
void ACustomerManager::LoadRecurringSpecificDialog()
{
	FString storyDialogPath = "Content/CustomerParameterFiles/Customer Dialog/Recurring Dialog";
	FString fileName;
	TArray<FString> names = { "Dargos", "Ragnar", "Desna", "Zalk", "Han", "Kermet", "Ereka", "Loric", "Knarr", "Yngvar"};
	int size = names.Num();

	for (int i = 0; i < size; i++)
	{
		//dialog files
		//Greetings
		fileName = storyDialogPath + "/" + names[i] + "/Greetings.txt";
		LoadCustomerData(fileName, "recurring_greetings");

		//Farwells
		fileName = storyDialogPath + "/" + names[i] + "/Farewells.txt";
		LoadCustomerData(fileName, "recurring_farewells");

		//Story
		fileName = storyDialogPath + "/" + names[i] + "/LifeStory.txt";
		LoadCustomerData(fileName, "recurring_story");
	}
}

//takes in data from files and dialog type that needs to be filled (eg. dialog_aesthetics, dialog_intelligence)
//fills dialog array and sets correct member dialog array to it
void ACustomerManager::CreateCustomerDialog(TArray<FString> data, FString dialogType)
{
	TArray<FString> dialog;

	//i loops through the data read from the file
	for (int i = 0; i < data.Num(); i++)
	{
		//checks for comments
		if (data[i][0] != '*')
		{
			dialog.Add(data[i]);
		}
	}

	//recurring customers specific dialog
	if (dialogType == "recurring_greetings")
		mRecurringGreetings.Add(dialog);
	if (dialogType == "recurring_farewells")
		mRecurringFarewells.Add(dialog);
	if (dialogType == "recurring_story")
		mRecurringStory.Add(dialog);

	//Greetings
	if (dialogType == "greetings")
		mGreetings = dialog;
	//Farewells
	if (dialogType == "farewells")
		mFarewells = dialog;
	//Farewells
	if (dialogType == "angry")
		mAngry = dialog;

	//Responses
	//Item
	if (dialogType == "response_IP_F")
		mPositiveItem.Insert(dialog, 0);
	if (dialogType == "response_IN_F")
		mNegativeItem.Insert(dialog, 0);
	if (dialogType == "response_IP_W")
		mPositiveItem.Insert(dialog, 1);
	if (dialogType == "response_IN_W")
		mNegativeItem.Insert(dialog, 1);
	if (dialogType == "response_IP_A")
		mPositiveItem.Insert(dialog, 2);
	if (dialogType == "response_IN_A")
		mNegativeItem.Insert(dialog, 2);

	//Pitch
	if (dialogType == "response_PP_F")
		mPositivePitch.Insert(dialog, 0);
	if (dialogType == "response_PN_F")
		mNegativePitch.Insert(dialog, 0);
	if (dialogType == "response_PP_W")
		mPositivePitch.Insert(dialog, 1);
	if (dialogType == "response_PN_W")
		mNegativePitch.Insert(dialog, 1);
	if (dialogType == "response_PP_A")
		mPositivePitch.Insert(dialog, 2);
	if (dialogType == "response_PN_A")
		mNegativePitch.Insert(dialog, 2);

	//Haggle
	if (dialogType == "response_HP_F")
		mPositiveHaggle.Insert(dialog, 0);
	if (dialogType == "response_HN_F")
		mNegativeHaggle.Insert(dialog, 0);
	if (dialogType == "response_HP_W")
		mPositiveHaggle.Insert(dialog, 1);
	if (dialogType == "response_HN_W")
		mNegativeHaggle.Insert(dialog, 1);
	if (dialogType == "response_HP_A")
		mPositiveHaggle.Insert(dialog, 2);
	if (dialogType == "response_HN_A")
		mNegativeHaggle.Insert(dialog, 2);

	//Attributes
	//Food
	if (dialogType == "dialog_AI_F")
		mDialogAI.Insert(dialog, 0);
	if (dialogType == "dialog_AS_F")
		mDialogAS.Insert(dialog, 0);
	if (dialogType == "dialog_FI_F")
		mDialogFI.Insert(dialog, 0);
	if (dialogType == "dialog_FS_F")
		mDialogFS.Insert(dialog, 0);

	if (dialogType == "dialog_IA_F")
		mDialogIA.Insert(dialog, 0);
	if (dialogType == "dialog_SA_F")
		mDialogSA.Insert(dialog, 0);
	if (dialogType == "dialog_IF_F")
		mDialogIF.Insert(dialog, 0);
	if (dialogType == "dialog_SF_F")
		mDialogSF.Insert(dialog, 0);

	//Weapon
	if (dialogType == "dialog_AI_W")
		mDialogAI.Insert(dialog, 1);
	if (dialogType == "dialog_AS_W")
		mDialogAS.Insert(dialog, 1);
	if (dialogType == "dialog_FI_W")
		mDialogFI.Insert(dialog, 1);
	if (dialogType == "dialog_FS_W")
		mDialogFS.Insert(dialog, 1);

	if (dialogType == "dialog_IA_W")
		mDialogIA.Insert(dialog, 1);
	if (dialogType == "dialog_SA_W")
		mDialogSA.Insert(dialog, 1);
	if (dialogType == "dialog_IF_W")
		mDialogIF.Insert(dialog, 1);
	if (dialogType == "dialog_SF_W")
		mDialogSF.Insert(dialog, 1);

	//Armor
	if (dialogType == "dialog_AI_A")
		mDialogAI.Insert(dialog, 2);
	if (dialogType == "dialog_AS_A")
		mDialogAS.Insert(dialog, 2);
	if (dialogType == "dialog_FI_A")
		mDialogFI.Insert(dialog, 2);
	if (dialogType == "dialog_FS_A")
		mDialogFS.Insert(dialog, 2);

	if (dialogType == "dialog_IA_A")
		mDialogIA.Insert(dialog, 2);
	if (dialogType == "dialog_SA_A")
		mDialogSA.Insert(dialog, 2);
	if (dialogType == "dialog_IF_A")
		mDialogIF.Insert(dialog, 2);
	if (dialogType == "dialog_SF_A")
		mDialogSF.Insert(dialog, 2);
}

//print to output log customer dialog
void ACustomerManager::DisplayCustomerDialog(TArray<FString> dialog)
{
	for (int i = 0; i < dialog.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Line %d: %s"), i, *(dialog[i]));
	}
}

//sets customer's greeting
void ACustomerManager::SetGreeting(ACustomer* customer)
{		
	//note: lineCount starts at -1, so increase first
	//lineCount is only increased in greetings, but is used in farewells and story
	//so, there must be a greeting
	customer->IncreaseLineCount();

	int size = mRecurringGreetings[customer->GetNum()].Num();
	
	//if all recurring dialog is exhausted, use generic
	if (customer->IsRecurring() && size > 0 && customer->GetLineCount() < size)
	{
		//specific greeting
		UE_LOG(LogTemp, Log, TEXT("First Greeting: %s"), *(mRecurringGreetings[customer->GetNum()][customer->GetLineCount()]));
		customer->SetGreeting( mRecurringGreetings[customer->GetNum()][customer->GetLineCount()] );
	}
	else
	{
		//generic greeting
		int num = FMath::RandRange(0, mGreetings.Num() - 1);
		customer->SetGreeting(mGreetings[num]);
	}
	
}

//sets recurring customer's story dialog
void ACustomerManager::SetStory(ACustomer* customer)
{
	int size = mRecurringStory[customer->GetNum()].Num();

	//Note: lineCount is increased in greetings
	if (size > 0 && customer->GetLineCount() < size)
	{
		customer->SetStory(mRecurringStory[customer->GetNum()][customer->GetLineCount()]);
	}
	else
	{
		customer->SetStory("");
	}
}

//sets customer's farewell dialog
void ACustomerManager::SetFarewell(ACustomer* customer)
{
	int size = mRecurringFarewells[customer->GetNum()].Num();

	//if all recurring dialog is exhausted, use generic
	if (customer->IsRecurring() && size > 0 && customer->GetLineCount() < size)
	{
		//specific greeting
		customer->SetFarewell(mRecurringFarewells[customer->GetNum()][customer->GetLineCount()]);
	}
	else
	{
		int num = FMath::RandRange(0, mFarewells.Num() - 1);
		customer->SetFarewell(mFarewells[num]);
	}
}

//set a customer's response dialog
void ACustomerManager::SetResponses(ACustomer* customer)
{
	switch (customer->GetDesiredItem())
	{
		case ItemEnum::FOOD:
		{
			int num = FMath::RandRange(0, mPositiveItem[0].Num() - 1);
			customer->SetResponsePositiveItem(mPositiveItem[0][num]);
			num = FMath::RandRange(0, mNegativeItem[0].Num() - 1);
			customer->SetResponseNegativeItem(mNegativeItem[0][num]);

			num = FMath::RandRange(0, mPositivePitch[0].Num() - 1);
			customer->SetResponsePositivePitch(mPositivePitch[0][num]);
			num = FMath::RandRange(0, mNegativePitch[0].Num() - 1);
			customer->SetResponseNegativePitch(mNegativePitch[0][num]);

			num = FMath::RandRange(0, mPositiveHaggle[0].Num() - 1);
			customer->SetResponsePositiveHaggle(mPositiveHaggle[0][num]);
			num = FMath::RandRange(0, mNegativeHaggle[0].Num() - 1);
			customer->SetResponseNegativeHaggle(mNegativeHaggle[0][num]);
			break;
		}
		case ItemEnum::WEAPON:
		{
			int num = FMath::RandRange(0, mPositiveItem[1].Num() - 1);
			customer->SetResponsePositiveItem(mPositiveItem[1][num]);
			num = FMath::RandRange(0, mNegativeItem[1].Num() - 1);
			customer->SetResponseNegativeItem(mNegativeItem[1][num]);

			num = FMath::RandRange(0, mPositivePitch[1].Num() - 1);
			customer->SetResponsePositivePitch(mPositivePitch[1][num]);
			num = FMath::RandRange(0, mNegativePitch[1].Num() - 1);
			customer->SetResponseNegativePitch(mNegativePitch[1][num]);

			num = FMath::RandRange(0, mPositiveHaggle[1].Num() - 1);
			customer->SetResponsePositiveHaggle(mPositiveHaggle[1][num]);
			num = FMath::RandRange(0, mNegativeHaggle[1].Num() - 1);
			customer->SetResponseNegativeHaggle(mNegativeHaggle[1][num]);
			break;
		}
		case ItemEnum::ARMOR:
		{
			int num = FMath::RandRange(0, mPositiveItem[2].Num() - 1);
			customer->SetResponsePositiveItem(mPositiveItem[2][num]);
			num = FMath::RandRange(0, mNegativeItem[2].Num() - 1);
			customer->SetResponseNegativeItem(mNegativeItem[2][num]);

			num = FMath::RandRange(0, mPositivePitch[2].Num() - 1);
			customer->SetResponsePositivePitch(mPositivePitch[2][num]);
			num = FMath::RandRange(0, mNegativePitch[2].Num() - 1);
			customer->SetResponseNegativePitch(mNegativePitch[2][num]);

			num = FMath::RandRange(0, mPositiveHaggle[2].Num() - 1);
			customer->SetResponsePositiveHaggle(mPositiveHaggle[2][num]);
			num = FMath::RandRange(0, mNegativeHaggle[2].Num() - 1);
			customer->SetResponseNegativeHaggle(mNegativeHaggle[2][num]);
			break;
		}
	}
}

//sets a customer's initial dialog
//there are eight different types of dialog each with spesific item types
//0 = Aesthetics, 1 = Fuctionality, 2 = Intelligence, 3 = Stupidity
void ACustomerManager::SetInitialDialog(ACustomer* customer)
{
	TraitEnum dom = customer->GetDominateTrait();
	TraitEnum sec = customer->GetSecondaryTrait();
	ItemEnum item = customer->GetDesiredItem();

	switch (dom)
	{
		case TraitEnum::AESTHETICS:
			if (sec == TraitEnum::INTELLIGENCE) //AESTHETICS-INTELLIGENCE
			{
				switch (item)
				{
					case ItemEnum::FOOD:
					{
						int num = FMath::RandRange(0, mDialogAI[0].Num() - 1);
						customer->SetInitialDialog(mDialogAI[0][num]);
						break;
					}
					case ItemEnum::WEAPON:
					{
						int num = FMath::RandRange(0, mDialogAI[1].Num() - 1);
						customer->SetInitialDialog(mDialogAI[1][num]);
						break;
					}
					case ItemEnum::ARMOR:
					{
						int num = FMath::RandRange(0, mDialogAI[2].Num() - 1);
						customer->SetInitialDialog(mDialogAI[2][num]);
						break;
					}
					default:
						break;
				}
			}
			else //AESTHETICS-STUPIDITY
			{
				switch (item)
				{
				case ItemEnum::FOOD:
				{
					int num = FMath::RandRange(0, mDialogAS[0].Num() - 1);
					customer->SetInitialDialog(mDialogAS[0][num]);
					break;
				}
				case ItemEnum::WEAPON:
				{
					int num = FMath::RandRange(0, mDialogAS[1].Num() - 1);
					customer->SetInitialDialog(mDialogAS[1][num]);
					break;
				}
				case ItemEnum::ARMOR:
				{
					int num = FMath::RandRange(0, mDialogAS[2].Num() - 1);
					customer->SetInitialDialog(mDialogAS[2][num]);
					break;
				}
				default:
					break;
				}
			}
			break;
		case TraitEnum::FUNCTIONALITY:
			if (sec == TraitEnum::INTELLIGENCE) //FUNCTIONALITY-INTELLIGENCE
			{
				switch (item)
				{
				case ItemEnum::FOOD:
				{
					int num = FMath::RandRange(0, mDialogFI[0].Num() - 1);
					customer->SetInitialDialog(mDialogFI[0][num]);
					break;
				}
				case ItemEnum::WEAPON:
				{
					int num = FMath::RandRange(0, mDialogFI[1].Num() - 1);
					customer->SetInitialDialog(mDialogFI[1][num]);
					break;
				}
				case ItemEnum::ARMOR:
				{
					int num = FMath::RandRange(0, mDialogFI[2].Num() - 1);
					customer->SetInitialDialog(mDialogFI[2][num]);
					break;
				}
				default:
					break;
				}
			}
			else //FUNCTIONALITY-STUPIDITY
			{
				switch (item)
				{
				case ItemEnum::FOOD:
				{
					int num = FMath::RandRange(0, mDialogFS[0].Num() - 1);
					customer->SetInitialDialog(mDialogFS[0][num]);
					break;
				}
				case ItemEnum::WEAPON:
				{
					int num = FMath::RandRange(0, mDialogFS[1].Num() - 1);
					customer->SetInitialDialog(mDialogFS[1][num]);
					break;
				}
				case ItemEnum::ARMOR:
				{
					int num = FMath::RandRange(0, mDialogFS[2].Num() - 1);
					customer->SetInitialDialog(mDialogFS[2][num]);
					break;
				}
				default:
					break;
				}
			}
			break;
		case TraitEnum::INTELLIGENCE:
			if (sec == TraitEnum::AESTHETICS) //INTELLIGENCE-AESTHETICS
			{
				switch (item)
				{
				case ItemEnum::FOOD:
				{
					int num = FMath::RandRange(0, mDialogIA[0].Num() - 1);
					customer->SetInitialDialog(mDialogIA[0][num]);
					break;
				}
				case ItemEnum::WEAPON:
				{
					int num = FMath::RandRange(0, mDialogIA[1].Num() - 1);
					customer->SetInitialDialog(mDialogIA[1][num]);
					break;
				}
				case ItemEnum::ARMOR:
				{
					int num = FMath::RandRange(0, mDialogIA[2].Num() - 1);
					customer->SetInitialDialog(mDialogIA[2][num]);
					break;
				}
				default:
					break;
				}
			}
			else //INTELLIGENCE-FUNCTIONALITY
			{
				switch (item)
				{
				case ItemEnum::FOOD:
				{
					int num = FMath::RandRange(0, mDialogIF[0].Num() - 1);
					customer->SetInitialDialog(mDialogIF[0][num]);
					break;
				}
				case ItemEnum::WEAPON:
				{
					int num = FMath::RandRange(0, mDialogIF[1].Num() - 1);
					customer->SetInitialDialog(mDialogIF[1][num]);
					break;
				}
				case ItemEnum::ARMOR:
				{
					int num = FMath::RandRange(0, mDialogIF[2].Num() - 1);
					customer->SetInitialDialog(mDialogIF[2][num]);
					break;
				}
				default:
					break;
				}
			}
			break;
		case TraitEnum::STUPIDITY:
			if (sec == TraitEnum::AESTHETICS) //STUPIDITY-AESTHETICS
			{
				switch (item)
				{
				case ItemEnum::FOOD:
				{
					int num = FMath::RandRange(0, mDialogSA[0].Num() - 1);
					customer->SetInitialDialog(mDialogSA[0][num]);
					break;
				}
				case ItemEnum::WEAPON:
				{
					int num = FMath::RandRange(0, mDialogSA[1].Num() - 1);
					customer->SetInitialDialog(mDialogSA[1][num]);
					break;
				}
				case ItemEnum::ARMOR:
				{
					int num = FMath::RandRange(0, mDialogSA[2].Num() - 1);
					customer->SetInitialDialog(mDialogSA[2][num]);
					break;
				}
				default:
					break;
				}
			}
			else //STUPIDITY-FUNCTIONALITY
			{
				switch (item)
				{
				case ItemEnum::FOOD:
				{
					int num = FMath::RandRange(0, mDialogSF[0].Num() - 1);
					customer->SetInitialDialog(mDialogSF[0][num]);
					break;
				}
				case ItemEnum::WEAPON:
				{
					int num = FMath::RandRange(0, mDialogSF[1].Num() - 1);
					customer->SetInitialDialog(mDialogSF[1][num]);
					break;
				}
				case ItemEnum::ARMOR:
				{
					int num = FMath::RandRange(0, mDialogSF[2].Num() - 1);
					customer->SetInitialDialog(mDialogSF[2][num]);
					break;
				}
				default:
					break;
				}
			}
			break;
		default:
			break;
	}
}
