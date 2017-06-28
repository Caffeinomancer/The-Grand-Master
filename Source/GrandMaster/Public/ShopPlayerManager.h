// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SharedData.h"
#include "ShopPlayerManager.generated.h"


UCLASS()
class GRANDMASTER_API AShopPlayerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopPlayerManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Accessors
	//Greetings
	UFUNCTION(BlueprintCallable, Category = "ShopPlayerDialog")
		FString GetGreeting();
	
	//Pitches
	UFUNCTION(BlueprintCallable, Category = "ShopPlayerDialog")
		FString GetPitchAesthetics(ItemEnum desiredItem);
	UFUNCTION(BlueprintCallable, Category = "ShopPlayerDialog")
		FString GetPitchFunctionality(ItemEnum desiredItem);
	UFUNCTION(BlueprintCallable, Category = "ShopPlayerDialog")
		FString GetPitchIntelligence(ItemEnum desiredItem);
	UFUNCTION(BlueprintCallable, Category = "ShopPlayerDialog")
		FString GetPitchStupidity(ItemEnum desiredItem);
		
private:

	void ReadFiles();
	void LoadData(FString fileName, FString responseType);
	void CreatePlayerDialog(TArray<FString> data, FString dialogType);
	void DisplayPlayerDialog(TArray<FString> dialog);

	TArray<TArray<FString>> mPitchesAesthetics;
	TArray<TArray<FString>> mPitchesFunctionality;
	TArray<TArray<FString>> mPitchesIntelligence;
	TArray<TArray<FString>> mPitchesStupidity;

	TArray<FString> mGreetings;
	
};
