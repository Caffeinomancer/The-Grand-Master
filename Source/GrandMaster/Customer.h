// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SharedData.h"
#include "Customer.generated.h"


UCLASS()
class GRANDMASTER_API ACustomer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomer();
	// Sets values of customer
	ACustomer(FString name, int appraise, int aesth, int intell, int trust, int money);

	//~ACustomer();

	//UPROPERTY(EditAnywhere, Category = "Customer")
		//TSubclassOf<class ACustomer> mCustomerBP;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//customer adjusting functions
	void AdjustTrust(float change);
	void AdjustAppraisal(float change);
	void IncreaseLineCount() { mLineCount++; };

	//Accessors
	//number, name, and money
	UFUNCTION(BlueprintCallable, Category = "Customer")
		int GetNum() { return mNum; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetName() { return mName; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		int GetMoney() { return mMoney; };

	//trust and appraisal
	UFUNCTION(BlueprintCallable, Category = "Customer")
	float GetTrust() { return mTrust; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
	float GetAppraisal() { return mAppraisal; };

	//traits and item
	UFUNCTION(BlueprintCallable, Category = "Customer")
		ItemEnum GetDesiredItem() { return mDesiredItem; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		TraitEnum GetDominateTrait() { return mDominateTrait; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		TraitEnum GetSecondaryTrait() { return mSecondaryTrait; };

	//get dialog
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetGreeting() { return mGreeting; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetInitialDialog() { return mInitialDialog; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetResponseNegativeItem() { return mResponseNegativeItem; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetResponsePositiveItem() { return mResponsePositiveItem; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetResponseNegativePitch() { return mResponseNegativePitch; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetResponsePositivePitch() { return mResponsePositivePitch; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetResponseNegativeHaggle() { return mResponseNegativeHaggle; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetResponsePositiveHaggle() { return mResponsePositiveHaggle; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetFarewell() { return mFarewell; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetAngry() { return mAngry; };

	//recurring customer specific dialog
	UFUNCTION(BlueprintCallable, Category = "Customer")
		int GetLineCount() { return mLineCount; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		FString GetStory() { return mStory; };
	UFUNCTION(BlueprintCallable, Category = "Customer")
		bool IsRecurring() { return mIsRecurring; };


	//Mutator
	//number, name, and money
	void SetNum(int num) { mNum = num; };
	void SetName(FString name) { mName = name; };
	void SetMoney(int money) { mMoney = money; };

	//trust and appraisal
	void SetPrevTrust(float trust) { mPrevTrust = trust; };
	void SetTrust(float trust) { mTrust = trust; };
	void SetAppraisal(float appraise) { mAppraisal = appraise; };
	void UpdateAppraisal();

	//traits, item
	void SetDesiredItem(ItemEnum item) { mDesiredItem = item; };
	void SetDominateTrait(TraitEnum trait) { mDominateTrait = trait; };
	void SetSecondaryTrait(TraitEnum trait) { mSecondaryTrait = trait; };

	//setting dialog
	void SetGreeting(FString dialog) { mGreeting = dialog; };
	void SetInitialDialog(FString dialog) { mInitialDialog = dialog; };
	void SetResponseNegativeItem(FString dialog) { mResponseNegativeItem = dialog; };
	void SetResponsePositiveItem(FString dialog) { mResponsePositiveItem = dialog; };
	void SetResponseNegativePitch(FString dialog) { mResponseNegativePitch = dialog; };
	void SetResponsePositivePitch(FString dialog) { mResponsePositivePitch = dialog; };
	void SetResponseNegativeHaggle(FString dialog) { mResponseNegativeHaggle = dialog; };
	void SetResponsePositiveHaggle(FString dialog) { mResponsePositiveHaggle = dialog; };
	void SetFarewell(FString dialog) { mFarewell = dialog; };
	void SetAngry(FString dialog) { mAngry = dialog; };

	//recurring customer specific dialog
	void SetStory(FString dialog) { mStory = dialog; };
	void SetLineCount(int count) { mLineCount = count; };
	void SetIsRecurring(bool recurring) { mIsRecurring = recurring; };
	

private:

	int mNum;
	FString mName;
	int mMoney;

	float mTrust;
	float mPrevTrust;
	float mAppraisal;
	
	ItemEnum mDesiredItem;
	TraitEnum mDominateTrait;
	TraitEnum mSecondaryTrait;
	
	FString mGreeting;
	FString mInitialDialog;
	FString mResponseNegativeItem;
	FString mResponsePositiveItem;
	FString mResponseNegativePitch;
	FString mResponsePositivePitch;
	FString mResponseNegativeHaggle;
	FString mResponsePositiveHaggle;
	FString mFarewell;
	FString mAngry;

	//recurring customer variables
	FString mStory;
	int mLineCount;
	bool mIsRecurring;
};
