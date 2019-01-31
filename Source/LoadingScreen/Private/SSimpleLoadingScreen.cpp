// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SSimpleLoadingScreen.h"

#include "SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "SSafeZone.h"
#include "SThrobber.h"
#include "SDPIScaler.h"
#include "Engine/Texture2D.h"
#include "Engine/UserInterfaceSettings.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

/////////////////////////////////////////////////////
// SSimpleLoadingScreen

void SSimpleLoadingScreen::Construct(const FArguments& InArgs, const FLoadingScreenDescription& InScreenDescription)
{
	LastComputedDPIScale = 1.0f;

	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();

	const FSlateFontInfo& TipFont = Settings->TipFont;
	const FSlateFontInfo& LoadingFont = Settings->LoadingFont;

	TSharedRef<SOverlay> Root = SNew(SOverlay);

	// If there's an image defined
	if ( InScreenDescription.Images.Num() > 0 )
	{
		int32 ImageIndex = FMath::RandRange(0, InScreenDescription.Images.Num() - 1);
		const FStringAssetReference& ImageAsset = InScreenDescription.Images[ImageIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		if ( UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject) )
		{
			FVector2D Size = FVector2D(LoadingImage->GetSizeX(), LoadingImage->GetSizeY());
			LoadingScreenBrush = MakeShareable(new FLoadingScreenBrush(LoadingImage, Size, FName(*ImageAsset.ToString())));

			Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SScaleBox)
				.Stretch(InScreenDescription.ImageStretch)
				[
					SNew(SImage)
					.Image(LoadingScreenBrush.Get())
				]
			];
		}
	}

	TSharedRef<SWidget> TipWidget = SNullWidget::NullWidget;
	if ( Settings->Tips.Num() > 0 )
	{
		int32 TipIndex = FMath::RandRange(0, Settings->Tips.Num() - 1);

		TipWidget = SNew(STextBlock)
			.WrapTextAt(Settings->TipWrapAt)
			.Font(TipFont)
			.Text(Settings->Tips[TipIndex]);
	}

	Root->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Bottom)
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderBackgroundColor(FLinearColor(0, 0, 0, 0.75))
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		[
			SNew(SSafeZone)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(25))
			.IsTitleSafe(true)
			[
				SNew(SDPIScaler)
				.DPIScale(this, &SSimpleLoadingScreen::GetDPIScale)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.Padding(FMargin(0))
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(SCircularThrobber)
						// Convert font size to pixels, pixel_size = point_size * resolution / 72, then half it to get radius
						.Radius((LoadingFont.Size * 96.0f/72.0f) / 2.0f)
					]

					+ SHorizontalBox::Slot()
					.Padding(FMargin(LoadingFont.Size * 0.5, 0.0f, 0, 0))
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(InScreenDescription.LoadingText)
						.Font(LoadingFont)
					]

					+ SHorizontalBox::Slot()
					.FillWidth(1)
					.HAlign(HAlign_Fill)
					[
						SNew(SSpacer)
						.Size(FVector2D(1.0f, 1.0f))
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.Padding(FMargin(10.0f))
					[
						TipWidget
					]
				]
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
	const FVector2D& LocalSize = AllottedGeometry.GetLocalSize();
	FIntPoint Size((int32)LocalSize.X, (int32)LocalSize.Y);
	const float NewScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);

	if ( NewScale != LastComputedDPIScale )
	{
		LastComputedDPIScale = NewScale;
		SlatePrepass(1.0f);
	}
}

float SSimpleLoadingScreen::GetDPIScale() const
{
	return LastComputedDPIScale;
}

#undef LOCTEXT_NAMESPACE
