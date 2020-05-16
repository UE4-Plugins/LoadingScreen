// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LoadingScreenSettings.h"
#include "LoadingScreenFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LOADINGSCREEN_API ULoadingScreenFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** Returns the startup loading screen settings */
	UFUNCTION(BlueprintPure, Category = "Loading Screen")
	static FLoadingScreenDescription GetStartupLoadingScreen();

	/** Returns the current default loading screen settings. */
	UFUNCTION(BlueprintPure, Category = "Loading Screen")
	static FLoadingScreenDescription GetDefaultLoadingScreen();

	/** 
	* Sets the current default loading screen settings to InDescription.
	* NOTE: This by default does not save those changes so when the application closes, it will reset those settings.
	*/
	UFUNCTION(BlueprintCallable, Category = "Loading Screen")
	static void SetLoadingScreen(FLoadingScreenDescription InDescription);

};
