// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API ITargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void SetTarget(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent)
	void SetVisibilityTargetArrow(bool bIsVisible);
};
