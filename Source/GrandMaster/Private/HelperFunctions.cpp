// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "HelperFunctions.h"

bool UHelperFunctions::IsWithEditor() 
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}


