﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"


class ATresor;
class ASoul;
class AItem;
UINTERFACE()
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IPickUpInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetOverlappingItem(AItem* Item);
	virtual void AddSouls(ASoul* Soul);
	virtual void AddGold(ATresor* Tresor);
};
