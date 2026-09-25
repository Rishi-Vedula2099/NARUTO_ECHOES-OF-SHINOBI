using UnrealBuildTool;
using System.Collections.Generic;

public class EchoesOfShinobiEditorTarget : TargetRules
{
	public EchoesOfShinobiEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		bOverrideBuildEnvironment = true;
		ExtraModuleNames.Add("EchoesOfShinobi");
	}
}
