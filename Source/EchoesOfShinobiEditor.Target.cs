using UnrealBuildTool;
using System.Collections.Generic;

public class EchoesOfShinobiEditorTarget : TargetRules
{
	public EchoesOfShinobiEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("EchoesOfShinobi");
	}
}
