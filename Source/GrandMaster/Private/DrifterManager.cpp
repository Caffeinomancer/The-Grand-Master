// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "DrifterManager.h"
#include "Drifter.h"

// Sets default values
ADrifterManager::ADrifterManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADrifterManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADrifterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADrifterManager::Init(bool isLite)
{
	mIsLite = isLite;
	mDrifterCount = 0;
	ReadFiles();
}

bool ADrifterManager::ShouldSpawnBoss(ADrifter* drifter)
{
	int spawnNum = drifter->GetSpawnNum();
	int encounterCount = drifter->GetEncounterCount();

	if (spawnNum == encounterCount)
		return true;
	else
		return false;
}

//returns a drifter
//will get next drifter in line if all encounter dialog is read
ADrifter* ADrifterManager::GetDrifter()
{
	if (!mIsLite)
	{

		int encounter = mDrifters[mDrifterCount]->GetEncounterCount();
		int encounterMax = mDrifters[mDrifterCount]->GetNumOfEncounters() - 1;

		if (encounter == encounterMax)
		{
			mDrifterCount++;
		}

		return mDrifters[mDrifterCount];
	}
	else
	{
		return GenerateRandomDrifter();
	}
}

//*******************************
//*		Chacter Creation		*
//*******************************

ADrifter* ADrifterManager::GenerateRandomDrifter()
{
	//add a new drifter to the temp array
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();

	//Note pointer is const, for added protection
	ADrifter* const drifter = GetWorld()->SpawnActor<ADrifter>(ADrifter::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

	//drifter->SetName("Drifter");
	SetBossEnum(drifter, FMath::RandRange(0, 4));
	SetDialogLite(drifter);

	return drifter;
}

void ADrifterManager::CreateDrifters(TArray<FString> data)
{
	TArray<ADrifter*> drifters;

	int j = 0;
	int k = 0;
	//i keeps track of the customer stat that we are on (0-8)
	//k keeps track of the index of the data read in we are on
	//there could be comments in data file, so two index trackers are used (i and k)
	//j keeps track of which customer we are filling out
	for (int i = 0; i < data.Num(); i++, k++)
	{
		//parse over lines from file that were comments (comments marked with *)
		while (data[i][0] == '*')
		{
			i++;
		}

		switch ((k + 4) % 4)
		{
		case 0:
		{
			//add a new drifter to the temp array
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			const FVector SpawnLocation = GetActorLocation();
			const FRotator SpawnRotation = GetActorRotation();

			//Note pointer is const, for added protection
			ADrifter* const drifter = GetWorld()->SpawnActor<ADrifter>(ADrifter::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			UE_LOG(LogTemp, Log, TEXT("add new drifter %d"), j);
			drifters.Add(drifter);

			//set name
			drifters[j]->SetName(data[i]);
			break;
		}
		case 1:
			//setting boss that spawns with drifter
			SetBossEnum(drifters[j], FCString::Atoi(*data[i]));
			break;
		case 2:
			//sets what encounter with this drifter will spawn boss
			drifters[j]->SetSpawnNum(FCString::Atoi(*data[i]));
			drifters[j]->SetEncounterCount(0);
			break;
		case 3:
			//set number of encounters
			drifters[j]->SetNumOfEncounters(FCString::Atoi(*data[i]));
			drifters[j]->SetEncounterCount(0);

			//set dialog
			drifters[j]->SetDrifterDialog(mDrifterDialog[j]);
			drifters[j]->SetGMDialog(mGMDialog[j]);

			j++;
			break;
		default:
			break;
		}
	}

	mDrifters = drifters;
	DisplayDrifterData();
}

//sets drifter boss type that they spawn from file's int given
void ADrifterManager::SetBossEnum(ADrifter* drifter, int type)
{
	switch (type)
	{
	case 0:
		drifter->SetSpawnType(BossEnum::FIRE_WORM);
		break;
	case 1:
		drifter->SetSpawnType(BossEnum::ICE_WORM);
		break;
	case 2:
		drifter->SetSpawnType(BossEnum::SMALL_TROLL);
		break;
	case 3:
		drifter->SetSpawnType(BossEnum::BIG_TROLL);
		break;
	case 4:
		drifter->SetSpawnType(BossEnum::FINAL_BOSS);
		break;
	default:
		drifter->SetSpawnType(BossEnum::SMALL_TROLL);
		break;
	}

}

TArray<FString> ADrifterManager::GetRandomDialogLite()
{
	int num = FMath::RandRange(0, mDrifterDialogLite.Num() - 1);

	return mDrifterDialogLite[num];
}

//set dialog to boss type
//be mindful of the order and the file's order
void ADrifterManager::SetDialogLite(ADrifter* drifter)
{

	switch (drifter->GetBossType())
	{
	case BossEnum::SMALL_TROLL:
		drifter->SetName("Karl");
		drifter->SetDrifterDialogLite(mDrifterDialogLite[0]);
		UE_LOG(LogTemp, Log, TEXT("Dialog: "), *(drifter->GetDrifterDialogLite()[0]));
		break;
	case BossEnum::BIG_TROLL:
		drifter->SetName("Tryggvar");
		drifter->SetDrifterDialogLite(mDrifterDialogLite[1]);
		UE_LOG(LogTemp, Log, TEXT("Dialog: "), *(drifter->GetDrifterDialogLite()[0]));
		break;
	case BossEnum::ICE_WORM:
		drifter->SetName("Tam");
		drifter->SetDrifterDialogLite(mDrifterDialogLite[2]);
		UE_LOG(LogTemp, Log, TEXT("Dialog: "), *(drifter->GetDrifterDialogLite()[0]));
		break;
	case BossEnum::FIRE_WORM:
		drifter->SetName("Tryggvar");
		drifter->SetDrifterDialogLite(mDrifterDialogLite[3]);
		UE_LOG(LogTemp, Log, TEXT("Dialog: "), *(drifter->GetDrifterDialogLite()[0]));
		break;
	case BossEnum::FINAL_BOSS:
		drifter->SetName("Karl");
		drifter->SetDrifterDialogLite(mDrifterDialogLite[4]);
		UE_LOG(LogTemp, Log, TEXT("Dialog: "), *(drifter->GetDrifterDialogLite()[0]));
		break;
	default:
		drifter->SetDrifterDialogLite(mDrifterDialogLite[0]);
		UE_LOG(LogTemp, Log, TEXT("Dialog: "), *(drifter->GetDrifterDialogLite()[0]));
		break;
	}

}


//***************************
//*		File Loading		*
//***************************

//read text from file
void ADrifterManager::ReadFiles()
{
	FString drifterPath = "Content/CustomerParameterFiles/Drifter/";
	FString fileName;
	TArray<FString> names = {  "Hus.txt", "Tam.txt", "Loric.txt", "Karl.txt"};

	for (int i = 0; i < names.Num(); i++)
	{
		//dialog
		fileName = drifterPath + names[i];
		LoadData(fileName, "dialog");
	}


	fileName = drifterPath + "Drifters.txt";
	LoadData(fileName, "drifters");

	fileName = drifterPath + "Dialog Lite.txt";
	LoadData(fileName, "dialog_lite");

}


void ADrifterManager::LoadData(FString fileName, FString type)
{
	FString fileData;
	bool fileOpened = false;
	fileOpened = FFileHelper::LoadFileToString(fileData, *(FPaths::GameDir() + fileName));

	//check if loading file was a success
	if (fileOpened)
		UE_LOG(LogTemp, Log, TEXT("Opened file: %s, in LoadData()"), *(FPaths::GameDir() + fileName));
	if (!fileOpened)
		UE_LOG(LogTemp, Log, TEXT("Failed to Open file: %s, in LoadData()"), *(FPaths::GameDir() + fileName));

	TArray<FString> lines;
	int32 lineCount = fileData.ParseIntoArray(lines, TEXT("\n"), true);

	//fill array only if theres something in the file
	//engine breaks if you try to load in nothing
	if (lines.Num() > 0)
	{
		if (type == "drifters")
			CreateDrifters(lines);

		if(type == "dialog")
			CreateDialog(lines);

		if (type == "dialog_lite")
			CreateDialogLite(lines);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("File %s Empty"), *(fileName));
	}
}

//takes in data from files and makes array of dialog
void ADrifterManager::CreateDialogLite(TArray<FString> data)
{
	TArray<FString> dialog;
	int k = -1;

	for (int i = 0; i < data.Num(); i++)
	{
		//checks for section lables (eg. **0-15**)
		if (data[i][0] != '*')
		{
			//k keeps track of encounter number
			while (data[i][0] == '$')
			{
				k++;
				i++;

				if (k > 0)
				{
					//adding dialog array of lines to dialog collection
					mDrifterDialogLite.Add(dialog);
					dialog.Empty();
				}
			}

			//adding lines to array
			dialog.Add(data[i]);
		}
	}

	//adding dialog array of lines to dialog collection for the last one
	mDrifterDialogLite.Add(dialog);
}


//takes in data from files
//fills dialog array and sets correct member dialog array to it
void ADrifterManager::CreateDialog(TArray<FString> data)
{
	TArray<FString> linesD;
	TArray<FString> linesGM;
	TArray<TArray<FString>> dialogD;
	TArray<TArray<FString>> dialogGM;
	int j = 0;
	int k = -1;

	//i loops through the data read from the file
	for (int i = 0; i < data.Num(); i++)
	{
		//checks for section lables (eg. **0-15**)
		if (data[i][0] != '*')
		{
			//k keeps track of encounter number
			while(data[i][0] == '$')
			{
				k++;
				i++;
				j = 0;

				if (k > 0)
				{
					//adding dialog array of lines to dialog collection for drifters
					dialogD.Add(linesD);
					linesD.Empty();

					//adding dialog array of lines to dialog collection for grand master
					dialogGM.Add(linesGM);
					linesGM.Empty();
				}
			}
			//every other line is the drifter's and the other is the Grand Master's
			//use j for mod division because there may be comments in file
			switch ((j + 2) % 2)
			{
			case 0:
				linesD.Add(data[i]);
				j++;
				break;
			case 1:
				linesGM.Add(data[i]);
				j++;
				break;
			default:
				break;
			}
		}
	}

	//add last array of lines to dialog array
	dialogD.Add(linesD);
	dialogGM.Add(linesGM);

	//add dialog to member arrays
	mDrifterDialog.Add(dialogD);
	mGMDialog.Add(dialogGM);
}

//print to output log customer stats
void ADrifterManager::DisplayDrifterData()
{
	
	for (int i = 0; i < mDrifters.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Name: %s"), *(mDrifters[i]->GetName()));

		switch (mDrifters[i]->GetBossType())
		{
		case BossEnum::SMALL_TROLL:
			UE_LOG(LogTemp, Log, TEXT("Boss Type: Small Troll"));
			break;
		case BossEnum::BIG_TROLL:
			UE_LOG(LogTemp, Log, TEXT("Boss Type: Big Troll"));
			break;
		case BossEnum::ICE_WORM:
			UE_LOG(LogTemp, Log, TEXT("Boss Type: Ice Worm"));
			break;
		case BossEnum::FIRE_WORM:
			UE_LOG(LogTemp, Log, TEXT("Boss Type: Fire Worm"));
			break;
		case BossEnum::FINAL_BOSS:
			UE_LOG(LogTemp, Log, TEXT("Boss Type: Final Boss"));
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("Boss Type: Not Set"));
			break;
		}
		UE_LOG(LogTemp, Log, TEXT("Spawn Number: %d"), mDrifters[i]->GetSpawnNum());
		UE_LOG(LogTemp, Log, TEXT("Drifter Line: %s"), *(mDrifters[i]->GetDrifterDialog()[0]));
		UE_LOG(LogTemp, Log, TEXT("GM Line: %s"), *(mDrifters[i]->GetGMDialog()[0]));
		mDrifters[i]->SetEncounterCount(0);
	}
	
}

