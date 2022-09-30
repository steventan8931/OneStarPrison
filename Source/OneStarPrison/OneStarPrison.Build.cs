// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OneStarPrison : ModuleRules
{
	public OneStarPrison(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//NEW
		//PrivatePCHHeaderFile = "MaulProtoPrecompiled.h";
		//MinFilesUsingPrecompiledHeaderOverride = 1;
		//bUseUnity = false;

		//bEnforceIWYU = true;

		//MinFilesUsingPrecompiledHeaderOverride = 1;
		//NEW ^
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "UMG",
            "Slate",
            "Sockets",
            "Niagara",
            "SlateCore",
            "ApplicationCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
 