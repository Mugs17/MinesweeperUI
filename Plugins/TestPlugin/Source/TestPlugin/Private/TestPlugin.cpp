// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestPlugin.h"
#include "TestPluginStyle.h"
#include "TestPluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SMinefieldStartupWidget.h"

static const FName TestPluginTabName("TestPlugin");

#define LOCTEXT_NAMESPACE "FTestPluginModule"

void FTestPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTestPluginStyle::Initialize();

	//Runs fine without this line. I'm guessing textures means icon images. 
	FTestPluginStyle::ReloadTextures();

	//Registering Commands
	FTestPluginCommands::Register();
	//Makes FUICommandList into a sharable variable for use with TSharedPtr<>
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTestPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FTestPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTestPluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestPluginTabName, FOnSpawnTab::CreateRaw(this, &FTestPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FTestPluginTabTitle", "TestPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);
}

void FTestPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);
	
	UToolMenus::UnregisterOwner(this);

	FTestPluginStyle::Shutdown();

	FTestPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TestPluginTabName);
}

TSharedRef<SDockTab> FTestPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
//	FText WidgetText = FText::Format(
	//	LOCTEXT("WindowWidgetText", "Addx xCODEXx to {0} in {1} to override this window's contents"),
	//	FText::FromString(TEXT("FTestPluginModule::OnSpawnPluginTab")),
	//	FText::FromString(TEXT("TestPlugin.cpp"))
	//	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SAssignNew(MinefieldStartupContainer, SMinefieldStartupWidget)
		];
}

void FTestPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(TestPluginTabName);
}

FReply FTestPluginModule::CreateMinefieldSection()
{
	return FReply::Handled();
}

void FTestPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FTestPluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FTestPluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

//No idea what this does. Maybe it had something to do with missing PCH header seen in old plugin code.
IMPLEMENT_MODULE(FTestPluginModule, TestPlugin)