// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonProfilerEditorModeToolkit.h"
#include "CommonProfilerEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "CommonProfilerEditorModeToolkit"

FCommonProfilerEditorModeToolkit::FCommonProfilerEditorModeToolkit()
{
}

void FCommonProfilerEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FCommonProfilerEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FCommonProfilerEditorModeToolkit::GetToolkitFName() const
{
	return FName("CommonProfilerEditorMode");
}

FText FCommonProfilerEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "CommonProfilerEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
