// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonProfilerRuntime : ModuleRules
{
	public CommonProfilerRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				//"CommonProfilerRuntime/Public"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				//"CommonProfilerRuntime/Private"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",                
				"CoreUObject",
				"Engine",
				"RHI",          
				"RenderCore"  
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				"EditorFramework",
				"EditorStyle",
				"UnrealEd",
				"LevelEditor",
				"InteractiveToolsFramework",
				"RenderCore",
				"EditorInteractiveToolsFramework"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
