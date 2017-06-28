// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DrifterManager.generated.h"


UCLASS()
class GRANDMASTER_API ADrifterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrifterManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Drifter")
		bool ShouldSpawnBoss(ADrifter* drifter);

	UFUNCTION(BlueprintCallable, Category = "Drifter")
		ADrifter* GetDrifter();

	UFUNCTION(BlueprintCallable, Category = "Drifter")
		void Init(bool isLite);

private:
	//sets enum from int
	void SetBossEnum(ADrifter* drifter, int type);

	void DisplayDrifterData();

	void ReadFiles();
	void LoadData(FString fileName, FString type);

	void CreateDialog(TArray<FString> data);
	void CreateDialogLite(TArray<FString> data);
	TArray<FString> GetRandomDialogLite();
	void SetDialogLite(ADrifter* drifter);

	void CreateDrifters(TArray<FString> data);
	ADrifter* GenerateRandomDrifter();
	
	TArray<TArray<TArray<FString>>> mDrifterDialog;
	TArray<TArray<TArray<FString>>> mGMDialog;

	TArray<class ADrifter*> mDrifters;
	int mDrifterCount;
	bool mShouldSpawn;

	bool mIsLite;
	TArray<class ADrifter*> mDriftersRandom;
	TArray<TArray<FString>> mDrifterDialogLite;
};
