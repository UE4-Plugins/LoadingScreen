// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FLoadingScreenModule : public IModuleInterface
{

public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	virtual bool IsGameModule() const override
	{
		return true;
	}

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline FLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FLoadingScreenModule>("LoadingScreen");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("LoadingScreen");
	}

private:

	void HandlePrepareLoadingScreen();

	void BeginLoadingScreen(const struct FLoadingScreenDescription& ScreenDescription);
};
