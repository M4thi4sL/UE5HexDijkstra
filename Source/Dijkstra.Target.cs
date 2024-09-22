// Salvatore Spoto 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class DijkstraTarget : TargetRules
{
	public DijkstraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Dijkstra" } );
	}
}
