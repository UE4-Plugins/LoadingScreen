// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SSimpleLoadingScreen.h"

#include "Slate/DeferredCleanupSlateBrush.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Images/SThrobber.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"
#include "Engine/UserInterfaceSettings.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

/////////////////////////////////////////////////////
// SSimpleLoadingScreen

static float PointSizeToSlateUnits(float PointSize)
{	
	const float SlateFreeTypeHorizontalResolutionDPI = 96.0f;
	const float FreeTypeNativeDPI = 72.0f;
	const float PixelSize = PointSize * (SlateFreeTypeHorizontalResolutionDPI / FreeTypeNativeDPI);

	return PixelSize;
}

void SSimpleLoadingScreen::Construct(const FArguments& InArgs, const FLoadingScreenDescription& InScreenDescription)
{	
	LastToolTipUpdate = -1.0f;

	ScreenDescriptionInfo = InScreenDescription;

	// Only show on construct if UI is true and we're not showing it after movies
	const bool bShowUiOnConstruct = (ScreenDescriptionInfo.bShowUiOverlay && !ScreenDescriptionInfo.bShowUiAfterMovies);
	bShowThrobber = InArgs._bShowThrobber;
	ThrobberType = InArgs._ThrobberType;	

	// Construct the root of this widget
	TSharedRef<SOverlay> Root = SNew(SOverlay);

	// If there's an image defined
	if (ScreenDescriptionInfo.Images.Num() > 0)
	{
		// Construct a random image to use for the loading screen
		const int32 ImageIndex = FMath::RandRange(0, ScreenDescriptionInfo.Images.Num() - 1);
		const FStringAssetReference& ImageAsset = ScreenDescriptionInfo.Images[ImageIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		if ( UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject) )
		{
			FVector2D Size = FVector2D(LoadingImage->GetSizeX(), LoadingImage->GetSizeY());
			LoadingScreenBrush = FDeferredCleanupSlateBrush::CreateBrush(LoadingImage, Size);
			//LoadingImage, Size, FName(*ImageAsset.ToString()))
			// Adds a slot to the root then add that image to the widget, renders over the movie if supposed to
			BackgroundImageWidget = SNew(SImage)
				.Visibility(ScreenDescriptionInfo.bShowImagesAfterMovies ? EVisibility::Hidden : EVisibility::SelfHitTestInvisible)
				.Image(LoadingScreenBrush->GetSlateBrush());

			Root->AddSlot(0)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)	
			[
				SNew(SScaleBox)
				.Stretch(ScreenDescriptionInfo.ImageStretch)
				[
					BackgroundImageWidget.ToSharedRef()
				]
			];
		}
	}	

	// Handles creating the throbber widget	
	{
		// Decides which throbber type to show
		switch (ThrobberType)
		{
		case EThrobberLoadingType::TLT_Circular:
		{
			// Construct circular throbber
			TSharedPtr<SCircularThrobber> ConstructedThrobber = SNew(SCircularThrobber)
				.Radius(ScreenDescriptionInfo.Throbber.ThrobberRadius)
				.Period(ScreenDescriptionInfo.Throbber.ThrobberPeriod)
				.NumPieces(ScreenDescriptionInfo.Throbber.NumPiecesThrobber)				
				.PieceImage(&ScreenDescriptionInfo.Throbber.ThrobberImage);

			ThrobberWidget = ConstructedThrobber;
			break;
		}
		case EThrobberLoadingType::TLT_Regular:		
		{			
			// Decide which animation to play for regular throbber
			const int32 AnimationParams = (ScreenDescriptionInfo.Throbber.AnimateVertically ? SThrobber::Vertical : 0) |
				(ScreenDescriptionInfo.Throbber.AnimateHorizontally ? SThrobber::Horizontal : 0) |
				(ScreenDescriptionInfo.Throbber.AnimateOpacity ? SThrobber::Opacity : 0);
			const SThrobber::EAnimation Animation = static_cast<SThrobber::EAnimation>(AnimationParams);

			// Construct regular throbber
			TSharedPtr<SThrobber> ConstructedThrobber = SNew(SThrobber)
				.Animate(Animation)
				.NumPieces(ScreenDescriptionInfo.Throbber.NumPiecesThrobber)				
				.PieceImage(&ScreenDescriptionInfo.Throbber.ThrobberImage);

			ThrobberWidget = ConstructedThrobber;
			break;
		}
		case EThrobberLoadingType::TLT_Image:
		default:
		{
			// Setup image to show
			const FSlateBrush* ImageBrush = &ScreenDescriptionInfo.Throbber.ImageBrush;

			// Construct an image in place of throbber
			TSharedPtr<SImage> ConstructedImage = SNew(SImage)
				.Image(ImageBrush)
				.ColorAndOpacity(ScreenDescriptionInfo.Throbber.ImageColorAndOpacity);

			ThrobberWidget = ConstructedImage;
			break;
		}
		}
				
		// Only adjust the the scaling if we're not using an image
		if (ThrobberType != EThrobberLoadingType::TLT_Image)
		{
			// Handles flipping the throbber if needed
			ThrobberWidget->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
			const float ThrobberScale = (float)((ScreenDescriptionInfo.Throbber.bFlipThrobberAnimation) ? -1 : 1);
			ThrobberWidget->SetRenderTransform(FSlateRenderTransform(FScale2D(ThrobberScale, 1.0f)));
		}
			// Show throbber if allowed
			ThrobberWidget->SetVisibility((bShowThrobber && bShowUiOnConstruct) ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);		
	}

	// Construct tooltip widget
	CurrentToolTipWidget = SNew(STextBlock)
		.Visibility(EVisibility::Hidden) // Default to hidden just incase
		.WrapTextAt(ScreenDescriptionInfo.LoadingScreenTips.SlotText.WrapAt)
		.Font(ScreenDescriptionInfo.LoadingScreenTips.SlotText.Font)
		.ColorAndOpacity(ScreenDescriptionInfo.LoadingScreenTips.SlotText.TextColor)
		.Justification(ScreenDescriptionInfo.LoadingScreenTips.SlotText.TextJustification);

	// Handles creating the tip text widget
	if (ScreenDescriptionInfo.LoadingScreenTips.Tips.Num() > 0)
	{					
		// Decide tool tip to show
		CurrentToolTipIndex = FMath::RandRange(0, ScreenDescriptionInfo.LoadingScreenTips.Tips.Num() - 1);
		
		// Update the text
		CurrentToolTipWidget->SetText(ScreenDescriptionInfo.LoadingScreenTips.Tips[CurrentToolTipIndex]);
		// Show tooltip widget if allowed
		CurrentToolTipWidget->SetVisibility((ScreenDescriptionInfo.LoadingScreenTips.SlotText.bShouldShowText && bShowUiOnConstruct)
			? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);

		// For deciding the time between tooltips
		LastToolTipUpdate = FPlatformTime::Seconds();
	}

	// Construct Description Text
	DescriptionTextWidget = SNew(STextBlock)
		.Text(ScreenDescriptionInfo.LoadingScreenDescription.Text)
		.Font(ScreenDescriptionInfo.LoadingScreenDescription.SlotText.Font)
		.ColorAndOpacity(ScreenDescriptionInfo.LoadingScreenDescription.SlotText.TextColor)
		.Justification(ScreenDescriptionInfo.LoadingScreenDescription.SlotText.TextJustification)
		.WrapTextAt(ScreenDescriptionInfo.LoadingScreenDescription.SlotText.WrapAt)
		.Visibility((ScreenDescriptionInfo.LoadingScreenDescription.SlotText.bShouldShowText && bShowUiOnConstruct)
			? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);

	// Construct Loading text		
	LoadingTextWidget = SNew(STextBlock)
		.Text(ScreenDescriptionInfo.LoadingScreenText.Text)
		.Font(ScreenDescriptionInfo.LoadingScreenText.SlotText.Font)
		.ColorAndOpacity(ScreenDescriptionInfo.LoadingScreenText.SlotText.TextColor)
		.Justification(ScreenDescriptionInfo.LoadingScreenText.SlotText.TextJustification)
		.Visibility((ScreenDescriptionInfo.LoadingScreenText.SlotText.bShouldShowText && bShowUiOnConstruct)
			? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);	

	// Adds a slot to the root of this widget to allow for other widgets to be added to it, renders over image/movie
	Root->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SSafeZone)
		[
			SNew(SConstraintCanvas)

			// Adds the circular throbber to the canvas
		+ SConstraintCanvas::Slot()
		.Anchors(ScreenDescriptionInfo.Throbber.ThrobberSlotPosition.Anchors)
		.Offset(FMargin(ScreenDescriptionInfo.Throbber.ThrobberSlotPosition.Offset))
		.Alignment(ScreenDescriptionInfo.Throbber.ThrobberSlotPosition.Alignment)
		.AutoSize(true)
		.ZOrder(1)
		[
			ThrobberWidget.ToSharedRef()
		]

	// Adds the tip text to the canvas
	+ SConstraintCanvas::Slot()
		.Anchors(ScreenDescriptionInfo.LoadingScreenTips.SlotPosition.Anchors)
		.Offset(FMargin(ScreenDescriptionInfo.LoadingScreenTips.SlotPosition.Offset))
		.Alignment(ScreenDescriptionInfo.LoadingScreenTips.SlotPosition.Alignment)
		.AutoSize(true)
		.ZOrder(1)
		[
			CurrentToolTipWidget.ToSharedRef()
		]

	// Adds the description text to the canvas
	+ SConstraintCanvas::Slot()
		.Anchors(ScreenDescriptionInfo.LoadingScreenDescription.SlotPosition.Anchors)
		.Offset(FMargin(ScreenDescriptionInfo.LoadingScreenDescription.SlotPosition.Offset))
		.Alignment(ScreenDescriptionInfo.LoadingScreenDescription.SlotPosition.Alignment)
		.AutoSize(true)
		.ZOrder(2)
		[
			DescriptionTextWidget.ToSharedRef()
		]

	// Adds the loading text to the canvas
	+ SConstraintCanvas::Slot()
		.Anchors(ScreenDescriptionInfo.LoadingScreenText.SlotPosition.Anchors)
		.Offset(FMargin(ScreenDescriptionInfo.LoadingScreenText.SlotPosition.Offset))
		.Alignment(ScreenDescriptionInfo.LoadingScreenText.SlotPosition.Alignment)
		.AutoSize(true)
		.ZOrder(3)
		[
			LoadingTextWidget.ToSharedRef()
		]
		]
	];
	
	ChildSlot
	[
		Root
	];
}

void SSimpleLoadingScreen::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{	
	// Dont continue if we only have 1 tip to show
	if (ScreenDescriptionInfo.LoadingScreenTips.Tips.Num() == 1)
	{
		return;
	}
	
	const int TipDelay = ScreenDescriptionInfo.LoadingScreenTips.TimeBetweenTips;

	// Dont continue if the delay time is at zero
	if (TipDelay == 0)
	{
		return;
	}
	// If the time between now and last tooltip update is longer than the intended delay
	else if ((InCurrentTime - LastToolTipUpdate) > TipDelay)
	{
		// Record the time
		LastToolTipUpdate = InCurrentTime;

		// Valid check
		if (CurrentToolTipWidget)
		{
			// Update the text to a new random tool tip
			CurrentToolTipWidget->SetText(GetRandomToolTip());
		}		
	}
}

void SSimpleLoadingScreen::HandleMoviesFinishedPlaying()
{	
	// Show the background if we're allowed to
	if (ScreenDescriptionInfo.bShowImagesAfterMovies)
	{
		if (BackgroundImageWidget)
		{			
			BackgroundImageWidget->SetVisibility(EVisibility::SelfHitTestInvisible);
		}
	}

	// Show ui elements if we're showing ui and we can show it after movies
	if (ScreenDescriptionInfo.bShowUiAfterMovies && ScreenDescriptionInfo.bShowUiOverlay)
	{
		if (ThrobberWidget)
		{
			ThrobberWidget->SetVisibility(ScreenDescriptionInfo.Throbber.bShowThrobber 
				? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);
		}

		if (LoadingTextWidget)
		{
			LoadingTextWidget->SetVisibility(ScreenDescriptionInfo.LoadingScreenText.SlotText.bShouldShowText 
				? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);
		}

		if (DescriptionTextWidget)
		{
			DescriptionTextWidget->SetVisibility(ScreenDescriptionInfo.LoadingScreenDescription.SlotText.bShouldShowText 
				? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);
		}

		// Only show tooltip widget if we have any tips to show
		if (CurrentToolTipWidget && ScreenDescriptionInfo.LoadingScreenTips.Tips.Num() != 0)
		{					
			// Reset the time so incase if when we're visible its not shorter than its supposed to be
			LastToolTipUpdate = FPlatformTime::Seconds();
		

			CurrentToolTipWidget->SetVisibility(ScreenDescriptionInfo.LoadingScreenTips.SlotText.bShouldShowText 
				? EVisibility::SelfHitTestInvisible : EVisibility::Hidden);
		}
	}
}

float SSimpleLoadingScreen::GetDPIScale() const
{
	const FVector2D& DrawSize = GetCachedGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32)DrawSize.X, (int32)DrawSize.Y);
	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}

FText SSimpleLoadingScreen::GetRandomToolTip() 
{	
	// Decides a random tip to show thats not the current tooltip
	{
		const int32 Total = ScreenDescriptionInfo.LoadingScreenTips.Tips.Num();
		int32 RandomTip = FMath::RandRange(0, Total - 1);
		// If there's only one then do nothing
		if (Total == 1)
		{
			RandomTip = 0;
		}
		else
		{
			// Find a random index that is not currently used
			while (RandomTip == CurrentToolTipIndex)
			{
				// Randomize the index
				RandomTip = FMath::RandRange(0, Total - 1);
			}
		}

		// Update current index to be this randomized one
		CurrentToolTipIndex = RandomTip;
	}

	return ScreenDescriptionInfo.LoadingScreenTips.Tips[CurrentToolTipIndex];
}

bool SSimpleLoadingScreen::CanShowToolTip() const
{	
	return (ScreenDescriptionInfo.LoadingScreenTips.Tips.Num() > 0);
}

#undef LOCTEXT_NAMESPACE
