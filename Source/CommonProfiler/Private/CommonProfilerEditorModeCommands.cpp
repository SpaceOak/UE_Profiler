// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonProfilerEditorModeCommands.h"
#include "CommonProfilerEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "CommonProfilerEditorModeCommands"

FCommonProfilerEditorModeCommands::FCommonProfilerEditorModeCommands()
	: TCommands<FCommonProfilerEditorModeCommands>("CommonProfilerEditorMode",
		NSLOCTEXT("CommonProfilerEditorMode", "CommonProfilerEditorModeCommands", "CommonProfiler Editor Mode"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
{
}

void FCommonProfilerEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FCommonProfilerEditorModeCommands::GetCommands()
{
	return FCommonProfilerEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
