// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Toasty : ModuleRules
{
	public Toasty(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
        PublicDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    }
}
