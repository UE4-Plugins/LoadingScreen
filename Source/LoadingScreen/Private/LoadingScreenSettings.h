// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Widgets/Layout/SScaleBox.h"
#include "MoviePlayer.h"
#include "Engine/DeveloperSettings.h"

#include "LoadingScreenSettings.generated.h"

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenDescription
{
	GENERATED_USTRUCT_BODY()

	FLoadingScreenDescription();

	/** The minimum time that a loading screen should be opened for, -1 if there is no minimum time. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	float MinimumLoadingScreenDisplayTime = -1;
	
	/** If true, the loading screen will disappear as soon as loading is done. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	bool bAutoCompleteWhenLoadingCompletes = true;
	
	/** If true, movies can be skipped by clicking the loading screen as long as loading is done. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	bool bMoviesAreSkippable = true;
	
	/** If true, movie playback continues until Stop is called. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	bool bWaitForManualStop = false;

	/** Should we just play back, loop, etc.  NOTE: if playback type is MT_LoadingLoop, then MoviePlayer will auto complete when in the last movie and load finishes regardless of bAutoCompleteWhenLoadingCompletes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Loading)
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	/** The movie paths local to the game's Content/Movies/ directory without extension. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movies)
	TArray<FString> MoviePaths;

	/**  Should we show the images/tips/loading text?  Generally you'll want to set this to false if you just want to show a movie. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Display)
	bool bShowUIOverlay = true;

	/**  Text displayed beside the animated icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Display)
	FText LoadingText;

	/** The texture display while in the loading screen on top of the movie. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Images, meta=(AllowedClasses="Texture2D"))
	TArray<FSoftObjectPath> Images;

	/** The scaling type to apply to images. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Images)
	TEnumAsByte<EStretch::Type> ImageStretch = EStretch::ScaleToFit;

	/** The background color to use */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Images)
	FLinearColor BackgroundColor = FLinearColor::Black;

	/** The background color to use for tips */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Images)
	FLinearColor TipBackgroundColor = FLinearColor(0, 0, 0, 0.75f);
};

/**
 * Settings for the simple loading screen plugin.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Loading Screen"))
class LOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:

	/** The startup screen for the project. */
	UPROPERTY(config, EditAnywhere, Category=Screens)
	FLoadingScreenDescription StartupScreen;

	/** The default load screen between maps. */
	UPROPERTY(config, EditAnywhere, Category=Screens)
	FLoadingScreenDescription DefaultScreen;

	/** The font to display the tips in. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	FSlateFontInfo TipFont;

	/** The font to display on loading. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = Display)
	FSlateFontInfo LoadingFont;

	/** The size of the tip before it's wrapped to the next line. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	float TipWrapAt;

	/** The tips to display on the load screen. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice, meta = (MultiLine = "true"))
	TArray<FText> Tips;
};
