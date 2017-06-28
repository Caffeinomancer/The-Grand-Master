// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "PlayerShop.h"


// Sets default values
APlayerShop::APlayerShop()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerShop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerShop::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APlayerShop::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void APlayerShop::LoadResponses(FString fileName, FString responseType)
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
	int32 lineCount = fileData.ParseIntoArray(lines, TEXT("\n"), true);

	//customer stats
	if (responseType == "response_aesthetics")
		mResponsesAesthetics = lines;
	if (responseType == "response_functionality")
		mResponsesFunctionality = lines;
	if (responseType == "response_intelligence")
		mResponsesIntelligence = lines;
	if (responseType == "response_stupidity")
		mResponsesStupidity = lines;
}