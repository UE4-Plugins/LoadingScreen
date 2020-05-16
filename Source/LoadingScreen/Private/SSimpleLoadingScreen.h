// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateCore/Public/Widgets/SCompoundWidget.h"
#include "Styling/CoreStyle.h"
#include "LoadingScreenSettings.h"

class FDeferredCleanupSlateBrush;
class STextBlock;
class SImage;

class SSimpleLoadingScreen : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SSimpleLoadingScreen) :
		_bShowThrobber(false),
		_ThrobberType(EThrobberLoadingType::TLT_Regular)
	{}
		SLATE_ARGUMENT(bool, bShowThrobber);
		SLATE_ARGUMENT(EThrobberLoadingType, ThrobberType);
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FLoadingScreenDescription& ScreenDescription);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	/** Handles setting visibility on objects that should be showing after all loading movies finished showing */
	void HandleMoviesFinishedPlaying();

private:
	float GetDPIScale() const;

	/** Returns a random tool tip that is not currently used and sets the current index to that new one */
	FText GetRandomToolTip();

	/** Checks if we're able to show any tool tips */
	bool CanShowToolTip() const;
	
	TSharedPtr<FDeferredCleanupSlateBrush> LoadingScreenBrush;	

	bool bShowThrobber;
	EThrobberLoadingType ThrobberType;

	double LastToolTipUpdate;
	int CurrentToolTipIndex;
	TSharedPtr<STextBlock> CurrentToolTipWidget;
	TSharedPtr<STextBlock> LoadingTextWidget;
	TSharedPtr<STextBlock> DescriptionTextWidget;
	TSharedPtr<SWidget> ThrobberWidget = SNullWidget::NullWidget;
	TSharedPtr<SImage> BackgroundImageWidget;

	FLoadingScreenDescription ScreenDescriptionInfo;
};
