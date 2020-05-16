// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenSettings.h"
#include "Styling/CoreStyle.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

FLoadingScreenSlotPosition::FLoadingScreenSlotPosition()
	: Anchors(0.5f)
	, Offset(NoInit)
	, Alignment(NoInit)
{ }

FLoadingScreenSlotText::FLoadingScreenSlotText()
	: bShouldShowText(true)
	, TextJustification(ETextJustify::Center)
	, WrapAt(1000.0f)
	, TextColor(FSlateColor(FLinearColor::White))
{
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
		Font = FSlateFontInfo(RobotoFontObj.Object, 20, FName("Normal"));;
	}
}

FLoadingScreenText::FLoadingScreenText()
	: SlotText(FLoadingScreenSlotText())
	, SlotPosition(FLoadingScreenSlotPosition())
{ }

FLoadingScreenThrobber::FLoadingScreenThrobber()
	: bShowThrobber(true)
	, ThrobberType(EThrobberLoadingType::TLT_Regular)
	, bFlipThrobberAnimation(false)
	, NumPiecesThrobber(6)
	, ThrobberImage(*FCoreStyle::Get().GetBrush("Throbber.Chunk"))
	, ThrobberSlotPosition(FLoadingScreenSlotPosition())
	, AnimateHorizontally(true)
	, AnimateVertically(true)
	, AnimateOpacity(true)
	, ThrobberPeriod(0.75f)
	, ThrobberRadius(16.0f)
	, ImageBrush(*FCoreStyle::Get().GetDefaultBrush())
	, ImageColorAndOpacity(FLinearColor::White)
{ }

FLoadingScreenTips::FLoadingScreenTips()
	: SlotText(FLoadingScreenSlotText())
	, SlotPosition(FLoadingScreenSlotPosition())
	, TimeBetweenTips(0)
{ }

FLoadingScreenDescription::FLoadingScreenDescription()
	: bShowWidget(true)
	, MinimumLoadingScreenDisplayTime(-1.0f)
	, bAutoCompleteWhenLoadingCompletes(true)
	, bMoviesAreSkippable(true)
	, bWaitForManualStop(false)	
	, bShowUiOverlay(true)
	, bShowUiAfterMovies(true)
	, Throbber(FLoadingScreenThrobber())
	, LoadingScreenText(FLoadingScreenText())
	, LoadingScreenDescription(FLoadingScreenText())
	, LoadingScreenTips(FLoadingScreenTips())
	, bShowImagesAfterMovies(true)
	, ImageStretch(EStretch::ScaleToFit)
{ 
	LoadingScreenText.Text = LOCTEXT("Loading", "LOADING");
}

ULoadingScreenSettings::ULoadingScreenSettings(const FObjectInitializer& Initializer)
	: Super(Initializer)
{	

}

#undef LOCTEXT_NAMESPACE
