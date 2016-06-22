// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenPrivatePCH.h"
#include "ILoadingScreenModule.h"

#include "LoadingScreenSettings.h"
#include "MoviePlayer.h"

#include "SSimpleLoadingScreen.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

class FLoadingScreenModule : public ILoadingScreenModule
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool IsGameModule() const override
	{
		return true;
	}

private:
	void HandlePreLoadMap();
	void HandlePostLoadMap();

	void BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription);
	void EndLoadingScreen();
};

IMPLEMENT_MODULE(FLoadingScreenModule, LoadingScreen)

void FLoadingScreenModule::StartupModule()
{
	// Load for cooker reference
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	for ( const FStringAssetReference& Ref : Settings->StartupScreen.Images )
	{
		Ref.TryLoad();
	}
	for ( const FStringAssetReference& Ref : Settings->DefaultScreen.Images )
	{
		Ref.TryLoad();
	}

	FCoreUObjectDelegates::PreLoadMap.AddRaw(this, &FLoadingScreenModule::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMap.AddRaw(this, &FLoadingScreenModule::HandlePostLoadMap);

	if ( IsMoviePlayerEnabled() )
	{
		BeginLoadingScreen(Settings->StartupScreen);
	}
}

void FLoadingScreenModule::ShutdownModule()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMap.RemoveAll(this);
}

void FLoadingScreenModule::HandlePreLoadMap()
{
	//if ( GEngine && GEngine->IsInitialized() )
	{
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
		BeginLoadingScreen(Settings->DefaultScreen);
	}

	// workaround for PreLoadMap call order issue
	GetMoviePlayer()->PlayMovie();
}

void FLoadingScreenModule::HandlePostLoadMap()
{
}

void FLoadingScreenModule::BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = ScreenDescription.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = ScreenDescription.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable = ScreenDescription.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop = ScreenDescription.bWaitForManualStop;

	//TODO if overridden...

	if ( ScreenDescription.IsValid() )
	{
		LoadingScreen.MoviePaths = ScreenDescription.MoviePaths;

		if ( ScreenDescription.bShowUIOverlay )
		{
			LoadingScreen.WidgetLoadingScreen = SNew(SSimpleLoadingScreen, ScreenDescription);
		}
	}
	else
	{
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	}
	
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void FLoadingScreenModule::EndLoadingScreen()
{

}

#undef LOCTEXT_NAMESPACE
