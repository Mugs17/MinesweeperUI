// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperCommands.h"

#define LOCTEXT_NAMESPACE "FTestPluginModule"

void FMinesweeperCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Minesweeper", "Bring up Minesweeper window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
