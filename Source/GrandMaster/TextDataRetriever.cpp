// Fill out your copyright notice in the Description page of Project Settings.

#include "GrandMaster.h"
#include "TextDataRetriever.h"


//To load data from text fille will make more diverse later
float UTextDataRetriever::FileLoadWormData(FString fileName, FString dataType, FString& loadText)
{
	FFileHelper::LoadFileToString(loadText, *(FPaths::GameDir() + fileName));
	
	if(dataType == "health")
	{
		FString healthParse = TEXT("Health:"), healthFromTextFile ="";
		bool shouldSetHealth = true;
		int charInc = 0;
		for (int i = 0; i < 7; i++)
		{
				if (loadText[i] != healthParse[i])
				{
					shouldSetHealth = false;	
				}
				charInc++;
		}
		if (shouldSetHealth)
		{
			while(loadText[charInc] != '~')
			{
				healthFromTextFile += loadText[charInc];
				charInc++;
			}

		}

		return FCString::Atof(*healthFromTextFile);
	}
	
	else if(dataType == "bullet_damage")
	{
		FString bulletDamageParse = TEXT("BulletDamage:"), bulletDamageFromTextFile ="";
		bool shouldSetBulletDamage = true;
		int charInc = 0;
		int sectionInc = 0;
		
		while(sectionInc < 1)
		{
			if(loadText[charInc] == '~')
			{
				sectionInc++;
				charInc++;
			}
			
			else
			{
				charInc++;
			}
		}
		
		charInc+=2;
		for(int i = 0; i < 13; i++)
		{
			if(loadText[charInc] != bulletDamageParse[i])
			{
				shouldSetBulletDamage = false;
			}
			
			charInc++;
		}
		
		if(shouldSetBulletDamage)
		{
			while(loadText[charInc] != '~')
			{
				bulletDamageFromTextFile += loadText[charInc];
				charInc++;
			}
		}
		
		
		//return 100.0;
		return FCString::Atof(*bulletDamageFromTextFile);
	}
	
	else if (dataType == "number_of_bullets")
	{
		FString numberOfBulletsParse = TEXT("NumberOfBullets:"), numberOfBulletsFromTextFile = "";
		bool shouldSetNumberOfBullets = true;
		int charInc = 0;
		int sectionInc = 0;

		while (sectionInc < 2)
		{
			if (loadText[charInc] == '~')
			{
				sectionInc++;
				charInc++;
			}

			else
			{
				charInc++;
			}
		}

		charInc += 2;
		for (int i = 0; i < 16; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("CharInc: %d"), loadText[charInc]);
			UE_LOG(LogTemp, Warning, TEXT("BulletInc: %d"), numberOfBulletsParse[i]);
			if (loadText[charInc] != numberOfBulletsParse[i])
			{
				shouldSetNumberOfBullets = false;
			}

			charInc++;
		}

		if (shouldSetNumberOfBullets)
		{
			while (loadText[charInc] != '~')
			{
				numberOfBulletsFromTextFile += loadText[charInc];
				charInc++;
			}
		}

		return FCString::Atof(*numberOfBulletsFromTextFile);
	}

	return 10.0f;
}
