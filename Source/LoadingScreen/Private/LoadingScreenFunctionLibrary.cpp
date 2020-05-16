// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenFunctionLibrary.h"


FLoadingScreenDescription ULoadingScreenFunctionLibrary::GetStartupLoadingScreen()
{
	return GetDefault<ULoadingScreenSettings>()->StartupScreen;
}

FLoadingScreenDescription ULoadingScreenFunctionLibrary::GetDefaultLoadingScreen()
{
	return GetDefault<ULoadingScreenSettings>()->DefaultScreen;
}

void ULoadingScreenFunctionLibrary::SetLoadingScreen(FLoadingScreenDescription InDescription)
{
	GetMutableDefault<ULoadingScreenSettings>()->DefaultScreen = InDescription;
}
