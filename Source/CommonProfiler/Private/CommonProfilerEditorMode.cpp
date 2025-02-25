// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonProfilerEditorMode.h"
#include "CommonProfilerEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "CommonProfilerEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/CommonProfilerSimpleTool.h"
#include "Tools/CommonProfilerInteractiveTool.h"

// step 2: register a ToolBuilder in FCommonProfilerEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "CommonProfilerEditorMode"

const FEditorModeID UCommonProfilerEditorMode::EM_CommonProfilerEditorModeId = TEXT("EM_CommonProfilerEditorMode");

FString UCommonProfilerEditorMode::SimpleToolName = TEXT("CommonProfiler_ActorInfoTool");
FString UCommonProfilerEditorMode::InteractiveToolName = TEXT("CommonProfiler_MeasureDistanceTool");


UCommonProfilerEditorMode::UCommonProfilerEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UCommonProfilerEditorMode::EM_CommonProfilerEditorModeId,
		LOCTEXT("ModeName", "CommonProfiler"),
		FSlateIcon(),
		true);
}


UCommonProfilerEditorMode::~UCommonProfilerEditorMode()
{
}


void UCommonProfilerEditorMode::ActorSelectionChangeNotify()
{
}

void UCommonProfilerEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FCommonProfilerEditorModeCommands& SampleToolCommands = FCommonProfilerEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UCommonProfilerSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UCommonProfilerInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UCommonProfilerEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FCommonProfilerEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UCommonProfilerEditorMode::GetModeCommands() const
{
	return FCommonProfilerEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
