// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"

#define LOCTEXT_NAMESPACE "FLoadingScreenModule"

FLoadingScreenDescription::FLoadingScreenDescription()
	: MinimumLoadingScreenDisplayTime(-1)
	, bAutoCompleteWhenLoadingCompletes(true)
	, bMoviesAreSkippable(true)
	, bWaitForManualStop(false)
	, bShowUIOverlay(true)
	, LoadingText(LOCTEXT("Loading", "LOADING"))
	, ImageStretch(EStretch::ScaleToFit)
{
}

ULoadingScreenSettings::ULoadingScreenSettings(const FObjectInitializer& Initializer) : Super(Initializer)
{
	TipWrapAt = 1000.0f;

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
		TipFont = FSlateFontInfo(RobotoFontObj.Object, 20, FName("Normal"));
		LoadingFont = FSlateFontInfo(RobotoFontObj.Object, 32, FName("Bold"));
	}
}

#undef LOCTEXT_NAMESPACE
