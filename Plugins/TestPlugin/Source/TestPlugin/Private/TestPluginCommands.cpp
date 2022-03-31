// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestPluginCommands.h"

#define LOCTEXT_NAMESPACE "FTestPluginModule"

void FTestPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "TestPlugin2", "Bring up TestPlugin window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
