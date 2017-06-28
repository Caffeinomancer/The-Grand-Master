// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SharedData.h"
#include "Drifter.generated.h"


UCLASS()
class GRANDMASTER_API ADrifter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADrifter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//accessors
	UFUNCTION(BlueprintCallable, Category = "Drifter")
		int GetSpawnNum() { return mSpawnEncounterNum; };
	UFUNCTION(BlueprintCallable, Category = "Drifter")
		int GetEncounterCount() { return mEncounterCount; };
	UFUNCTION(BlueprintCallable, Category = "Drifter")
		FString GetName() { return mName; };
	UFUNCTION(BlueprintCallable, Category = "Drifter")
		BossEnum GetBossType() { return mSpawnType; };
	UFUNCTION(BlueprintCallable, Category = "Drifter")
		int GetNumOfEncounters() { return mNumOfEncounters; };

	UFUNCTION(BlueprintCallable, Category = "Drifter")
		TArray<FString> GetDrifterDialog();
	UFUNCTION(BlueprintCallable, Category = "Drifter")
		TArray<FString> GetGMDialog();
	UFUNCTION(BlueprintCallable, Category = "Drifter")
		TArray<FString> GetDrifterDialogLite() { return mDrifterDialogLite; };
	
	//manipulators
	void SetName(FString name) { mName = name; };
	void SetSpawnType(BossEnum type) { mSpawnType = type; };
	void SetSpawnNum(int num) { mSpawnEncounterNum = num; };
	void SetEncounterCount(int num) { mEncounterCount = num; };
	void SetNumOfEncounters(int num) { mNumOfEncounters = num; };
	void SetDrifterDialog(TArray<TArray<FString>> dialog) { mDrifterDialog = dialog; };
	void SetGMDialog(TArray<TArray<FString>> dialog) { mGMDialog = dialog; };
	void SetDrifterDialogLite(TArray<FString> dialog) { mDrifterDialogLite = dialog; };

private:

	int mEncounterCount;
	int mNumOfEncounters;
	TArray<TArray<FString>> mDrifterDialog;
	TArray<TArray<FString>> mGMDialog;
	TArray<FString> mDrifterDialogLite;

	FString mName;
	BossEnum mSpawnType;
	int mSpawnEncounterNum;
};
