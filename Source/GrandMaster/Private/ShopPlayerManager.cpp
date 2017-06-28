// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "ShopPlayerManager.h"


// Sets default values
AShopPlayerManager::AShopPlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShopPlayerManager::BeginPlay()
{
	Super::BeginPlay();
	
	ReadFiles();
}

// Called every frame
void AShopPlayerManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//*************************************
//*			Dialog Accessors		  *
//*************************************

//Greetings
FString AShopPlayerManager::GetGreeting()
{
	int num = FMath::RandRange(0, mGreetings.Num() - 1);
	return mGreetings[num];
}

//Aesthetics
FString AShopPlayerManager::GetPitchAesthetics(ItemEnum desiredItem)
{
	switch (desiredItem)
	{
		case ItemEnum::FOOD:
		{
			int num = FMath::RandRange(0, mPitchesAesthetics[0].Num() - 1);
			return mPitchesAesthetics[0][num];
			break;
		}
		case ItemEnum::WEAPON:
		{
			int num = FMath::RandRange(0, mPitchesAesthetics[1].Num() - 1);
			return mPitchesAesthetics[1][num];
			break;
		}
		case ItemEnum::ARMOR:
		{
			int num = FMath::RandRange(0, mPitchesAesthetics[2].Num() - 1);
			return mPitchesAesthetics[2][num];
			break;
		}
		default:
		{
			int num = FMath::RandRange(0, mPitchesAesthetics[0].Num() - 1);
			return mPitchesAesthetics[0][num];
			break;
		}
	}
	
}

//Functionality
FString AShopPlayerManager::GetPitchFunctionality(ItemEnum desiredItem)
{
	switch (desiredItem)
	{
	case ItemEnum::FOOD:
	{
		int num = FMath::RandRange(0, mPitchesFunctionality[0].Num() - 1);
		return mPitchesFunctionality[0][num];
		break;
	}
	case ItemEnum::WEAPON:
	{
		int num = FMath::RandRange(0, mPitchesFunctionality[1].Num() - 1);
		return mPitchesFunctionality[1][num];
		break;
	}
	case ItemEnum::ARMOR:
	{
		int num = FMath::RandRange(0, mPitchesFunctionality[2].Num() - 1);
		return mPitchesFunctionality[2][num];
		break;
	}
	default:
	{
		int num = FMath::RandRange(0, mPitchesFunctionality[0].Num() - 1);
		return mPitchesFunctionality[0][num];
		break;
	}
	}

}

//Intelligence
FString AShopPlayerManager::GetPitchIntelligence(ItemEnum desiredItem)
{
	switch (desiredItem)
	{
	case ItemEnum::FOOD:
	{
		int num = FMath::RandRange(0, mPitchesIntelligence[0].Num() - 1);
		return mPitchesIntelligence[0][num];
		break;
	}
	case ItemEnum::WEAPON:
	{
		int num = FMath::RandRange(0, mPitchesIntelligence[1].Num() - 1);
		return mPitchesIntelligence[1][num];
		break;
	}
	case ItemEnum::ARMOR:
	{
		int num = FMath::RandRange(0, mPitchesIntelligence[2].Num() - 1);
		return mPitchesIntelligence[2][num];
		break;
	}
	default:
	{
		int num = FMath::RandRange(0, mPitchesIntelligence[0].Num() - 1);
		return mPitchesIntelligence[0][num];
		break;
	}
	}

}

//Stupidity
FString AShopPlayerManager::GetPitchStupidity(ItemEnum desiredItem)
{
	switch (desiredItem)
	{
	case ItemEnum::FOOD:
	{
		int num = FMath::RandRange(0, mPitchesStupidity[0].Num() - 1);
		return mPitchesStupidity[0][num];
		break;
	}
	case ItemEnum::WEAPON:
	{
		int num = FMath::RandRange(0, mPitchesStupidity[1].Num() - 1);
		return mPitchesStupidity[1][num];
		break;
	}
	case ItemEnum::ARMOR:
	{
		int num = FMath::RandRange(0, mPitchesStupidity[2].Num() - 1);
		return mPitchesStupidity[2][num];
		break;
	}
	default:
	{
		int num = FMath::RandRange(0, mPitchesStupidity[0].Num() - 1);
		return mPitchesStupidity[0][num];
		break;
	}
	}

}


//*********************************
//*			Files & Data		  *
//*********************************

void AShopPlayerManager::ReadFiles()
{
	FString playerDialogPath = "Content/CustomerParameterFiles/Player Dialog";
	FString fileName;

	//greetings
	fileName = playerDialogPath + "/Player_Greetings.txt";
	LoadData(fileName, "greetings");

	//pitches
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Aesthetics_Food.txt";
	LoadData(fileName, "pitches_AF");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Aesthetics_Weapon.txt";
	LoadData(fileName, "pitches_AW");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Aesthetics_Armor.txt";
	LoadData(fileName, "pitches_AA");

	fileName = playerDialogPath + "/MerchantPitches/Pitches_Functionality_Food.txt";
	LoadData(fileName, "pitches_FF");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Functionality_Weapon.txt";
	LoadData(fileName, "pitches_FW");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Functionality_Armor.txt";
	LoadData(fileName, "pitches_FA");

	fileName = playerDialogPath + "/MerchantPitches/Pitches_Intelligence_Food.txt";
	LoadData(fileName, "pitches_IF");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Intelligence_Weapon.txt";
	LoadData(fileName, "pitches_IW");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Intelligence_Armor.txt";
	LoadData(fileName, "pitches_IA");

	fileName = playerDialogPath + "/MerchantPitches/Pitches_Stupidity_Food.txt";
	LoadData(fileName, "pitches_SF");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Stupidity_Weapon.txt";
	LoadData(fileName, "pitches_SW");
	fileName = playerDialogPath + "/MerchantPitches/Pitches_Stupidity_Armor.txt";
	LoadData(fileName, "pitches_SA");
}

void AShopPlayerManager::LoadData(FString fileName, FString responseType)
{
	FString fileData;
	bool fileOpened = false;
	fileOpened = FFileHelper::LoadFileToString(fileData, *(FPaths::GameDir() + fileName));

	//check if loading file was a success
	if (fileOpened)
		UE_LOG(LogTemp, Log, TEXT("Opened file: %s, in LoadResponses()"), *(FPaths::GameDir() + fileName));
	if (!fileOpened)
		UE_LOG(LogTemp, Log, TEXT("Failed to Open file: %s, in LoadResponses()"), *(FPaths::GameDir() + fileName));

	TArray<FString> lines;
	int32 lineCount = fileData.ParseIntoArray(lines, LINE_TERMINATOR, true);

	//fill array only if theres something in the file
	//engine breaks if you try to load in nothing
	if (lines.Num() > 0)
	{
		//Greetings
		if (responseType == "greetings")
			CreatePlayerDialog(lines, "greetings");

		//Pitches
		//Aesthetics
		if (responseType == "pitches_AF")
			CreatePlayerDialog(lines, "pitches_AF");
		if (responseType == "pitches_AW")
			CreatePlayerDialog(lines, "pitches_AW");
		if (responseType == "pitches_AA")
			CreatePlayerDialog(lines, "pitches_AA");
		//Functionality
		if (responseType == "pitches_FF")
			CreatePlayerDialog(lines, "pitches_FF");
		if (responseType == "pitches_FW")
			CreatePlayerDialog(lines, "pitches_FW");
		if (responseType == "pitches_FA")
			CreatePlayerDialog(lines, "pitches_FA");
		//Intelligence
		if (responseType == "pitches_IF")
			CreatePlayerDialog(lines, "pitches_IF");
		if (responseType == "pitches_IW")
			CreatePlayerDialog(lines, "pitches_IW");
		if (responseType == "pitches_IA")
			CreatePlayerDialog(lines, "pitches_IA");
		//Stupidity
		if (responseType == "pitches_SF")
			CreatePlayerDialog(lines, "pitches_SF");
		if (responseType == "pitches_SW")
			CreatePlayerDialog(lines, "pitches_SW");
		if (responseType == "pitches_SA")
			CreatePlayerDialog(lines, "pitches_SA");
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("File %s Empty"), *(responseType));
	}
}

//takes in data from files and dialog type that needs to be filled (eg. dialog_aesthetics, dialog_intelligence)
//fills dialog array and sets correct member dialog array to it
void AShopPlayerManager::CreatePlayerDialog(TArray<FString> data, FString dialogType)
{
	TArray<FString> dialog;

	//i loops through the data read from the file
	for (int i = 0; i < data.Num(); i++)
	{
		//checks for section lables (eg. **0-15**)
		if (data[i][0] != '*')
		{
			dialog.Add(data[i]);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Loaded Dialog Section: %s"), *(data[i]));
		}
	}

	//Greetings
	if (dialogType == "greetings")
		mGreetings = dialog;

	//Pitches
	//Aesthetics
	if (dialogType == "pitches_AF")
		mPitchesAesthetics.Insert(dialog, 0);
	if (dialogType == "pitches_AW")
		mPitchesAesthetics.Insert(dialog, 1);
	if (dialogType == "pitches_AA")
		mPitchesAesthetics.Insert(dialog, 2);
	//Functionality
	if (dialogType == "pitches_FF")
		mPitchesFunctionality.Insert(dialog, 0);
	if (dialogType == "pitches_FW")
		mPitchesFunctionality.Insert(dialog, 1);
	if (dialogType == "pitches_FA")
		mPitchesFunctionality.Insert(dialog, 2);
	//Intelligence
	if (dialogType == "pitches_IF")
		mPitchesIntelligence.Insert(dialog, 0);
	if (dialogType == "pitches_IW")
		mPitchesIntelligence.Insert(dialog, 1);
	if (dialogType == "pitches_IA")
		mPitchesIntelligence.Insert(dialog, 2);
	//Stupidity
	if (dialogType == "pitches_SF")
		mPitchesStupidity.Insert(dialog, 0);
	if (dialogType == "pitches_SW")
		mPitchesStupidity.Insert(dialog, 1);
	if (dialogType == "pitches_SA")
		mPitchesStupidity.Insert(dialog, 2);
	
}

//print to output log customer dialog
void AShopPlayerManager::DisplayPlayerDialog(TArray<FString> dialog)
{
	for (int i = 0; i < dialog.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Line %d: %s"), i, *(dialog[i]));
	}
}