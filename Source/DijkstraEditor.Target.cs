
using UnrealBuildTool;
using System.Collections.Generic;

public class DijkstraEditorTarget : TargetRules
{
	public DijkstraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Dijkstra" } );
	}
}
