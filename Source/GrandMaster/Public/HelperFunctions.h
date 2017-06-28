// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelperFunctions.generated.h"

/**
 * 
 */
UCLASS()
class GRANDMASTER_API UHelperFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()public:


		UFUNCTION(BlueprintPure, Category = Whatever)
		static bool IsWithEditor();
	
};
