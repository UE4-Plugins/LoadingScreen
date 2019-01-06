// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenModule.h"
#include "LoadingScreenSettings.h"
#include "MoviePlayer/Public/MoviePlayer.h"
#include "SSimpleLoadingScreen.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "FLoadingScreenModule"

void FLoadingScreenModule::StartupModule()
{
	if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
	{
		// Load for cooker reference
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();

		for (const FStringAssetReference& Ref : Settings->StartupScreen.Images)
		{
			Ref.TryLoad();
		}

		for (const FStringAssetReference& Ref : Settings->DefaultScreen.Images)
		{
			Ref.TryLoad();
		}

		if (IsMoviePlayerEnabled())
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FLoadingScreenModule::HandlePrepareLoadingScreen);
		}

		// Prepare the startup screen, the PrepareLoadingScreen callback won't be called
		// if we've already explicitly setup the loading screen.
		BeginLoadingScreen(Settings->StartupScreen);
	}
}

void FLoadingScreenModule::ShutdownModule()
{
	if (!IsRunningDedicatedServer())
	{
		GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
	}
}

void FLoadingScreenModule::HandlePrepareLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	BeginLoadingScreen(Settings->DefaultScreen);
}

void FLoadingScreenModule::BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = ScreenDescription.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = ScreenDescription.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable = ScreenDescription.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop = ScreenDescription.bWaitForManualStop;
	LoadingScreen.MoviePaths = ScreenDescription.MoviePaths;
	LoadingScreen.PlaybackType = ScreenDescription.PlaybackType;

	if (ScreenDescription.bShowUIOverlay)
	{
		LoadingScreen.WidgetLoadingScreen = SNew(SSimpleLoadingScreen, ScreenDescription);
	}

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLoadingScreenModule, LoadingScreen)
