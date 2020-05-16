// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/Anchors.h"
#include "Fonts/SlateFontInfo.h"
#include "MoviePlayer.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Framework/Text/TextLayout.h"
#include "Engine/DeveloperSettings.h"

#include "LoadingScreenSettings.generated.h"

UENUM(BlueprintType)
enum class EThrobberLoadingType : uint8
{
	/** Uses a regular throbber to indicate loading */
	TLT_Regular UMETA(DisplayName = "Regular"),
	/** Uses a circular throbber to indicate loading */
	TLT_Circular UMETA(DisplayName = "Circular"),
	/** Uses an image instead of a throbber type incase you want to use an image with a material or something to indicate loading */
	TLT_Image UMETA(DisplayName = "Image")
};

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenSlotPosition
{
	GENERATED_BODY()

public:

	FLoadingScreenSlotPosition();

	/** The anchor for the Widget
	* 0-X = Left Side
	* 1-X = Right Side
	* 0-Y = Top Side
	* 1-Y = Bottom Side
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Position")
		FAnchors Anchors;

	/** The offset for the Widget
	* -X = Left
	* +X = Right
	* -Y = Up
	* +Y = Down
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Position")
		FVector2D Offset;

	/** Alignment pivot point of the Widget with 0.5 being center of either axis
	* 0-X = Left Side
	* 1-X = Right Side
	* 0-Y = Top Side
	* 1-Y = Bottom Side
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Position")
		FVector2D Alignment;

};

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenSlotText
{
	GENERATED_BODY()

public:

	FLoadingScreenSlotText();

	/** Flag for showing the widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips")
		bool bShouldShowText;

	/** The justification of the text. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips")
		TEnumAsByte<ETextJustify::Type> TextJustification;

	/** The size of the text before it's wrapped to the next line. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips")
		float WrapAt;

	/** The color to use for the text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen Text")
		FSlateColor TextColor;

	/** The font to display for text. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips")
		FSlateFontInfo Font;
};

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenText
{
	GENERATED_BODY()

public:

	FLoadingScreenText();

	/** Text Information related to the text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen Text")
		FLoadingScreenSlotText SlotText;

	/** The slot position of the text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen Text")
		FLoadingScreenSlotPosition SlotPosition;

	/** The text to display on the loading screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen Text", meta = (MultiLine = "true"))
		FText Text;

};

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenThrobber
{
	GENERATED_BODY()

public:

	FLoadingScreenThrobber();

	/** Flag for showing the loading throbber if true, false will not show any throbber. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throbber")
		bool bShowThrobber;

	/** Decides which throbber type to show if true, false will not show any throbber. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throbber", meta = (EditCondition = "bShowThrobber"))
		EThrobberLoadingType ThrobberType;

	/** Should throbber animate in opposite direction? Works for both regular and circular throbber */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throbber", meta = (EditCondition = "bShowThrobber"))
		bool bFlipThrobberAnimation;

	/** The numbers of pieces in the throbber when it is shown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throbber", meta = (ClampMin = "1", ClampMax = "25", EditCondition = "bShowThrobber"))
		int NumPiecesThrobber;

	/** The image for each throbber piece */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throbber", meta = (EditCondition = "bShowThrobber"))
		FSlateBrush ThrobberImage;

	/** The slot position of the throbber */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throbber", meta = (EditCondition = "bShowThrobber"))
	FLoadingScreenSlotPosition ThrobberSlotPosition;

	/** Should the pieces animate horizontally? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regular", meta = (EditCondition = "bShowThrobber"))
		bool AnimateHorizontally;

	/** Should the pieces animate vertically? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regular", meta = (EditCondition = "bShowThrobber"))
		bool AnimateVertically;

	/** Should the pieces animate their opacity? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regular", meta = (EditCondition = "bShowThrobber"))
		bool AnimateOpacity;

	/** The amount of time for a full circle(in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circular", meta = (EditCondition = "bShowThrobber"))
		float ThrobberPeriod;

	/** The radius of the circle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circular", meta = (EditCondition = "bShowThrobber"))
		float ThrobberRadius;

	/** Image to draw (adjust image size here to adjust the size in X and Y) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image", meta = (EditCondition = "bShowThrobber"))
		FSlateBrush ImageBrush;

	/** Color and opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image", meta = (EditCondition = "bShowThrobber", sRGB = "true"))
		FLinearColor ImageColorAndOpacity;

};

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenTips
{
	GENERATED_BODY()

public:

	FLoadingScreenTips();

	/** Text Information related to the tips text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips")
		FLoadingScreenSlotText SlotText;

	/** The slot position of the tips */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips")
	FLoadingScreenSlotPosition SlotPosition;

	/** The intended time between tips before it automatically switches to a new tip, anything below zero will cause the tips to not change. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips", meta = (ClampMin = "0"))
		int TimeBetweenTips; // Using an integer because letting people be able to get it really low might be bad...

	/** The tips to display on the load screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tips", meta =(MultiLine = "true"))
		TArray<FText> Tips;

};

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenDescription
{
	GENERATED_BODY()

public:

	FLoadingScreenDescription();

	/** Flag for showing any ui elements and images if true(incase you only want to show movies and thats it then set this to false) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|Loading")
		bool bShowWidget;

	/** The minimum time that a loading screen should be opened for, -1 if there is no minimum time. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|Loading")
		float MinimumLoadingScreenDisplayTime;

	/** If true, the loading screen will disappear as soon as loading is done. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|Loading")
		bool bAutoCompleteWhenLoadingCompletes;

	/** If true, movies can be skipped by clicking the loading screen as long as loading is done. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|Loading")
		bool bMoviesAreSkippable;

	/** If true, movie playback continues until Stop is called. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|Loading")
		bool bWaitForManualStop;

	/** Should we just play back, loop, etc.  NOTE: if playback type is MT_LoadingLoop, then MoviePlayer will auto complete when in the last movie and load finishes regardless of bAutoCompleteWhenLoadingCompletes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|Movies")
		TEnumAsByte<EMoviePlaybackType> PlaybackType;

	/** The movie paths local to the game's Content/Movies/ directory without extension. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|Movies")
		TArray<FString> MoviePaths;

	/**  Should we show the throbber/loading text etc?  Generally you'll want to set this to false if you just want to show a movie. This will render over any movie/background image. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI")
		bool bShowUiOverlay;

	/** Flag for showing UI after all movies have been played successfully if true. False will show during movies. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI", meta = (EditCondition = "bShowUIOverlay"))
		bool bShowUiAfterMovies;

	/** Throbber to display when loading, can show circular and regular throbber types */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI|Throbber", meta = (EditCondition = "bShowUIOverlay"))
		FLoadingScreenThrobber Throbber;

	/**  Text to display to indicate that the game is loading */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI|Loading Text", meta = (EditCondition = "bShowUIOverlay"))
		FLoadingScreenText LoadingScreenText;

	/**  Optional text to display as a description. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI|Description", meta = (EditCondition = "bShowUIOverlay"))
	FLoadingScreenText LoadingScreenDescription;

	/** Optional text to display that will randomly swap out with other tips if applicable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI|Tips", meta = (EditCondition = "bShowUIOverlay"))
	FLoadingScreenTips LoadingScreenTips;

	/** Flag for showing images after all movies have been played successfully if true. False will show during movies. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI|Images")
		bool bShowImagesAfterMovies;

	/** The texture display while in the loading screen on top of the movie. Will render after and over any movies. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI|Images", meta = (AllowedClasses = "Texture2D"))
		TArray<FSoftObjectPath> Images;

	/** The scaling type to apply to images. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screens|UI|Images")
		TEnumAsByte<EStretch::Type> ImageStretch;
};

/**
 * Settings for the simple loading screen plugin.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Loading Screen"))
class LOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	ULoadingScreenSettings(const FObjectInitializer& Initializer);

	/** The startup screen for the project. */
	UPROPERTY(config, EditAnywhere, Category = "Screens")
	FLoadingScreenDescription StartupScreen;

	/** The default load screen between maps. */
	UPROPERTY(config, EditAnywhere, Category = "Screens")
	FLoadingScreenDescription DefaultScreen;

};
