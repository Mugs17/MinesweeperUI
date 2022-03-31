// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateExtras.h"
#include "SlateBasics.h"

#include "SMinefieldStartupWidget.h"

/**
 *
 */

struct FGridSlotInfo
{
	
	bool bIsBomb = false;

	TArray<FString> NeighboringGridSlots;

	TArray<FString> NeighboringGridSlotsToSpreadTo;

	int ColumnNumber;

	int RowNumber;

	bool bHasSwitched = false;

};
class SMinefieldGridWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMinefieldGridWidget) {}

	//Reference to the Minefield widget that calls this grid
	SLATE_ARGUMENT(TSharedPtr<SMinefieldStartupWidget>, OwningWidget)

	SLATE_END_ARGS()

	//Construction function for the widget
	void Construct(const FArguments& InArgs);

	//	TSharedPtr<class SMinefieldStartupWidget> OwningWidget;
	TSharedPtr<SMinefieldStartupWidget> OwningWidget;

	//Reference to the minefield grid for dynamic use
	TSharedPtr<SGridPanel> GridPanelContainer;

	TMap<FString, FGridSlotInfo> GridSlotInfoMapContainer;

	//Array of identifiers for picking random slot to put mine
	TArray<FString> ArrayOfGridSlotIdentifiers;

	//needed to make sure new mines arent spawned on top of old ones
	TArray<FString> MineLocationIdentifiers;

	//Creates buttons for every slot in minesweeper
	FReply CreateMinefieldBaseGrid();

	void FindMines(FString SlotIdentifier);

	//recrusive function to make sure random number isn't the same as current and neighboring buttons number
	FString GenerateRandomMines(FString CurrentGridSlotIdentifier);

	TArray<FString> FindNeighboringGridSlotIdentifiers(int Column, int Row);

	FString GridSlotNumberToIdentifier(int Column, int Row);

	FReply MinefieldGridSlotPressed(int Column, int Row);

	//Chooses color of text based on number of neighboring mines
	FColor PickNumberColor(int Number);


	FString CurrentGridSlotPressed;

	bool bFirstCheckShowAllChildren = true;

	int SwitchCount;

	int32 TotalGridSlots;

	bool bHasGameStarted = false;
};
