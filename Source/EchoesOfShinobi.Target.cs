using UnrealBuildTool;
using System.Collections.Generic;

public class EchoesOfShinobiTarget : TargetRules
{
	public EchoesOfShinobiTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		bOverrideBuildEnvironment = true;
		ExtraModuleNames.Add("EchoesOfShinobi");
	}
}
