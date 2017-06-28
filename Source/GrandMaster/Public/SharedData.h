#pragma once

#include "SharedData.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class TraitEnum : uint8
{
	AESTHETICS		UMETA(DisplayName = "Aesthetics"),
	FUNCTIONALITY 	UMETA(DisplayName = "Functionality"),
	INTELLIGENCE	UMETA(DisplayName = "Intelligence"),
	STUPIDITY		UMETA(DisplayName = "Stupidity")
};

UENUM(BlueprintType)
enum class ItemEnum : uint8
{
	FOOD	UMETA(DisplayName = "Food"),
	WEAPON 	UMETA(DisplayName = "Weapon"),
	ARMOR	UMETA(DisplayName = "Armor")
};

UENUM(BlueprintType)
enum class BossEnum : uint8
{
	FIRE_WORM		UMETA(DisplayName = "Fire Worm"),
	ICE_WORM 		UMETA(DisplayName = "Ice Worm"),
	SMALL_TROLL		UMETA(DisplayName = "Tutorial Troll"),
	BIG_TROLL		UMETA(DisplayName = "Mountain Troll"),
	FINAL_BOSS		UMETA(DisplayName = "Final Boss")
};

//If you don't want this to appear in BP, make sure to use this instead
//USTRUCT()
USTRUCT(BlueprintType)
struct FCustomerSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "SaveDataStruct")
		int mRandomEncounters;
	UPROPERTY(EditAnywhere, Category = "SaveDataStruct")
		int mRecurringEncounters;
	UPROPERTY(EditAnywhere, Category = "SaveDataStruct")
		int mRecurringWaitTime;
	UPROPERTY(EditAnywhere, Category = "SaveDataStruct")
		float mAverageTrust;
	UPROPERTY(EditAnywhere, Category = "SaveDataStruct")
		TArray<float> mCustomerTrust;
	UPROPERTY(EditAnywhere, Category = "SaveDataStruct")
		TArray<int> mCustomerLineCount;
};