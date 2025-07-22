// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class CAS : ModuleRules
{
	public CAS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","GameplayAbilities",
			"GameplayTags","GameplayTasks","UMG", "NavigationSystem", "AIModule", "Niagara", "Json", "JsonUtilities" });
        
        PrivateDependencyModuleNames.AddRange(new string[] { });

        PrivateIncludePaths.AddRange(
        new string[]
        {
        "CAS"
        });
    }

}
