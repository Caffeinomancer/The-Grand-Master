// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PlayerShop.generated.h"

UCLASS()
class GRANDMASTER_API APlayerShop : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerShop();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:

	void LoadResponses(FString fileName, FString responseType);

	TArray<FString> mResponsesAesthetics;
	TArray<FString> mResponsesFunctionality;
	TArray<FString> mResponsesIntelligence;
	TArray<FString> mResponsesStupidity;
	
	TArray<FString> mGreetings;
};
