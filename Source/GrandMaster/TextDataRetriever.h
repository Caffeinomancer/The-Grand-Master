// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextDataRetriever.generated.h"

/**
 * 
 */
UCLASS()
class GRANDMASTER_API UTextDataRetriever : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()public:
		
		//Ufunctions that can be referenced in blueprints
		UFUNCTION(BlueprintCallable, Category = "load")
			static float FileLoadWormData(FString fileName, FString dataType, FString& loadText);
			
	    //End Of UFUNCTIONS

protected:
		
};
