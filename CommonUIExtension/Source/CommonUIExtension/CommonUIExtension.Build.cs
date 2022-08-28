// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CommonUIExtension : ModuleRules
{
	public CommonUIExtension(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
                "CommonUIExtension/Private",
			}   
			);

		// HoldTime �̏㏑���������s�����߁A
		// ������� CommonUI �� Private �w�b�_�[���Q�Ɖ\�ɂ���ݒ�
		PublicDefinitions.Add("COMMON_UI_PRIVATE_ACCESS");

		if (PublicDefinitions.Contains("COMMON_UI_PRIVATE_ACCESS"))
        {
			string engine_path = Path.GetFullPath(Target.RelativeEnginePath);
			string common_ui_path = engine_path + "Plugins/Experimental/CommonUI/Source/CommonUI/";
			PrivateIncludePaths.Add(common_ui_path + "Private");
		}


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"CommonUI",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
				"UMG",
				"GameplayTags",
				"CommonInput",
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
