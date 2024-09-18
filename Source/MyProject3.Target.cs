// Salvatore Spoto 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class MyProject3Target : TargetRules
{
	public MyProject3Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "MyProject3" } );
	}
}
