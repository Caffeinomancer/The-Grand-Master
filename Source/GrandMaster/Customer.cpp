// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "Customer.h"


// Sets default values
ACustomer::ACustomer()
{
	/*
	//https://answers.unrealengine.com/questions/53689/spawn-blueprint-from-c.html
	//http://pastebin.com/UcfF3f3i

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Items/Blueprints/BP_ItemTest.BP_ItemTest'"));
	if (ItemBlueprint.Object) 
	{
		mCustomerBP = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	*/
	mName = "Suspicious Character";
	mMoney = 500;
	mTrust = 1.0;
	mAppraisal = 1.0;
	mDominateTrait = TraitEnum::AESTHETICS;
	mSecondaryTrait = TraitEnum::INTELLIGENCE;
	mDesiredItem = ItemEnum::FOOD;
	mInitialDialog = "I'm looking for some hard to find items ;)";
	mLineCount = -1;
	mNum = 0;
}

//~ACustomer::ACustomer()
//{

//}

// Called when the game starts or when spawned
void ACustomer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACustomer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ACustomer::AdjustTrust(float change)
{
	mTrust += change;
}

void ACustomer::AdjustAppraisal(float change)
{
	mAppraisal += change;
}

void ACustomer::UpdateAppraisal()
{
	float difference = mTrust - mPrevTrust;
	UE_LOG(LogTemp, Log, TEXT("Old Appraisal: %f"), mAppraisal);
	mAppraisal += difference;
	UE_LOG(LogTemp, Log, TEXT("New Appraisal: %f"), mAppraisal);
}