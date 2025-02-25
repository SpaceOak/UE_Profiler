// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonProfilerModule.h"
#include "CommonProfilerEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "CommonProfilerModule"

void FCommonProfilerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FCommonProfilerEditorModeCommands::Register();
}

void FCommonProfilerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FCommonProfilerEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCommonProfilerModule, CommonProfilerEditorMode)