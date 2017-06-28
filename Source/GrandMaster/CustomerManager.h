// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "SharedData.h"
#include "CustomerManager.generated.h"

UCLASS()
class GRANDMASTER_API ACustomerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomerManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	

	//initialization
	UFUNCTION(BlueprintCallable, Category = "Customer")
		void Init();

	//returns a customer for player interaction
	UFUNCTION(BlueprintCallable, Category = "Customer")
		ACustomer* GetCustomer();

	UFUNCTION(BlueprintCallable, Category = "Customer")
		bool GetRecurringCustomersLoaded() { return mRecurringCustomersLoaded; };
	
	UFUNCTION(BlueprintCallable, Category = "Customer")
		int GetMaxPrice(ACustomer* customer, int itemBasePrice);

	//save data functions
	UFUNCTION(BlueprintCallable, Category = "Customer")
		void LoadCustomerSaveData(FCustomerSaveData data);
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FCustomerSaveData SaveCustomerSaveData();


	UFUNCTION(BlueprintCallable, Category = "Customer")
		int GetShopRating();

	//trust changing functions
	UFUNCTION(BlueprintCallable, Category = "Customer")
		void AdjustCustomerTrustItem(ACustomer* customer, ItemEnum offeredItem);
	UFUNCTION(BlueprintCallable, Category = "Customer")
		void AdjustCustomerTrustPitch(ACustomer* customer, TraitEnum pitchTrait);
	UFUNCTION(BlueprintCallable, Category = "Customer")
		void AdjustCustomerTrustHaggle(ACustomer* customer, float itemBasePrice, int askingPrice);
	UFUNCTION(BlueprintCallable, Category = "Customer")
		int GetStrikes() { return mStrikes; };

protected:

	

private:
	//load in data from text file for customers
	void LoadCustomerData(FString fileName, FString fileContent);

	//fills customer array with cutomers and sets customer data
	void CreateRecurringCustomerList(TArray<FString> data, FString customerType);
	void DisplayCustomerData(TArray<class ACustomer*> customers);

	//dialog functions
	void LoadAllDialog();
	void LoadRecurringSpecificDialog();
	void CreateCustomerDialog(TArray<FString> data, FString dialogType);
	void DisplayCustomerDialog(TArray<FString> dialog);

	void SetItemEnum(ACustomer* customer, int item);
	void SetTraitEnum(ACustomer* customer, int trait, bool isDomTrait);

	//Getting a new recurring or random customer for at the shop countertop
	ACustomer* GetRecurringCustomer();
	ACustomer* GenerateRandomCustomer();
	TArray<int> GetRandomCustomerOrder(int size);

	//adjusts frequency of recurring customers based on trust levels
	void CheckTrust();
	void SetAverageTrust();

	//setting dialog of customers
	void SetInitialDialog(ACustomer* customer);
	void SetResponses(ACustomer* customer);
	void SetGreeting(ACustomer* customer);
	void SetFarewell(ACustomer* customer);
	void SetStory(ACustomer* customer);

	int nNumOfCustomers;

	TArray<class ACustomer*> mRecurringCustomers;

	//check if all data is loaded in
	bool mRecurringCustomersLoaded;

	//counters for number of turns since last type of customer has shown
	int mRandomEncounters;
	int mRecurringEncounters;
	int mAdventurerEncounters;
	
	//how often recurring customers should appear
	int mRecurringWaitTime;
	float mAverageTrust;
	int mMaxPrice;
	int mStrikes;

	//dialog arrays set with food, weapon, then armor for positive and negative responses to player choices
	TArray<TArray<FString>> mPositiveItem;
	TArray<TArray<FString>> mNegativeItem;
	TArray<TArray<FString>> mPositivePitch;
	TArray<TArray<FString>> mNegativePitch;
	TArray<TArray<FString>> mPositiveHaggle;
	TArray<TArray<FString>> mNegativeHaggle;
	
	//dialog greeting
	TArray<FString> mGreetings;

	//dialog farewells
	TArray<FString> mFarewells;

	//dialog farewells
	TArray<FString> mAngry;

	//dialog arrays set with food, weapon, then armor for respective catagory
	TArray<TArray<FString>> mDialogAI;
	TArray<TArray<FString>> mDialogAS;
	TArray<TArray<FString>> mDialogFI;
	TArray<TArray<FString>> mDialogFS;

	TArray<TArray<FString>> mDialogIA;
	TArray<TArray<FString>> mDialogSA;
	TArray<TArray<FString>> mDialogIF;
	TArray<TArray<FString>> mDialogSF;

	//recurring customer specific dialog
	TArray<TArray<FString>> mRecurringGreetings;
	TArray<TArray<FString>> mRecurringFarewells;
	TArray<TArray<FString>> mRecurringStory;
};

//deathly slow intellisense
//https://forums.unrealengine.com/showthread.php?93546-For-those-who-suffer-from-Visual-Studio-IntelliSense-slowness