// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperPlugin.h"
#include "MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SMinefieldStartupWidget.h"

static const FName TestPluginTabName("TestPlugin");

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

void FMinesweeperPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperStyle::Initialize();

	//Runs fine without this line. I'm guessing textures means icon images. 
	FMinesweeperStyle::ReloadTextures();

	//Registering Commands
	FMinesweeperCommands::Register();
	//Makes FUICommandList into a sharable variable for use with TSharedPtr<>
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperPluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestPluginTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMinesweeperTabTitle", "Minesweeper"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);
}

void FMinesweeperPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);
	
	UToolMenus::UnregisterOwner(this);

	FMinesweeperStyle::Shutdown();

	FMinesweeperCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TestPluginTabName);
}

TSharedRef<SDockTab> FMinesweeperPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
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

void FMinesweeperPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(TestPluginTabName);
}

FReply FMinesweeperPluginModule::CreateMinefieldSection()
{
	return FReply::Handled();
}

void FMinesweeperPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

//No idea what this does. Maybe it had something to do with missing PCH header seen in old plugin code.
IMPLEMENT_MODULE(FMinesweeperPluginModule, Minesweeper)