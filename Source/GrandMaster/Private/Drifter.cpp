// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "Drifter.h"


// Sets default values
ADrifter::ADrifter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mName = "Dusty Drifter";
}

// Called when the game starts or when spawned
void ADrifter::BeginPlay()
{
	Super::BeginPlay();

	mEncounterCount = 0;
	
}

// Called every frame
void ADrifter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADrifter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//returns array of dialog for drifter and increases counter
TArray<FString> ADrifter::GetDrifterDialog()
{
	return mDrifterDialog[mEncounterCount];
}

//returns array of dialog for GM
TArray<FString> ADrifter::GetGMDialog()
{
	int count = mEncounterCount;
	mEncounterCount++;

	return mGMDialog[count];
}


