// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinefieldGridWidget.h"
#include "Math/UnrealMathUtility.h"

#define LOCTEXT_NAMESPACE "SMinefieldStartupWidget"

void SMinefieldGridWidget::Construct(const FArguments& InArgs)
{

	OwningWidget = InArgs._OwningWidget;

	ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.Padding(OwningWidget->VerticalBoxPadding)
			[
					SAssignNew(GridPanelContainer, SGridPanel)
			]
			
		];

	CreateMinefieldBaseGrid();

	
}

TArray<FString> SMinefieldGridWidget::FindNeighboringGridSlotIdentifiers(int32 Column, int32 Row)
{
		//Making sure minefield slots exist and then setting those neighboring slots for reference
		TArray<FString> NeighboringSlotIdentifiers;
		TArray<FString> NeighboringSlotIdentifiersToSpreadTo;
		int32 TotalChildrenCount = 0;
		// (-1 , 0)
		int32 NColumn = Column - 1;
		int32 NRow = Row;
		
		if (NColumn >= 0)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			NeighboringSlotIdentifiersToSpreadTo.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}
		//(+1 , 0)
		NColumn = Column + 1;
		NRow = Row;

		if (NColumn < OwningWidget->MinefieldHeight)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			NeighboringSlotIdentifiersToSpreadTo.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}

		//(0, -1))
		NColumn = Column;
		NRow = Row - 1;
		if (NRow >= 0)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			NeighboringSlotIdentifiersToSpreadTo.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}
		//(0 , +1)
		NColumn = Column;
		NRow = Row + 1;
		if (NRow < OwningWidget->MinefieldWidth)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			NeighboringSlotIdentifiersToSpreadTo.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}

		//(-1 , -1)
		NColumn = Column - 1;
		NRow = Row - 1;

		if (NColumn >= 0 && NRow >= 0)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}
		//(-1, +1)
		NColumn = Column - 1;
		NRow = Row + 1;

		if (NColumn >= 0 && NRow < OwningWidget->MinefieldWidth)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}

		//(+1, -1)
		NColumn = Column + 1;
		NRow = Row - 1;
		if (NColumn < OwningWidget->MinefieldHeight && NRow >= 0)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}

		//(+1, +1)
		NColumn = Column + 1;
		NRow = Row + 1;
		if  (NColumn < OwningWidget->MinefieldHeight && NRow < OwningWidget->MinefieldWidth)
		{
			NeighboringSlotIdentifiers.Add(GridSlotNumberToIdentifier(NColumn, NRow));
			TotalChildrenCount++;
		}

		FString CurrentGridSlotIdentifier = GridSlotNumberToIdentifier(Column, Row);
		FGridSlotInfo CurrentGridSlotInfo;

		CurrentGridSlotInfo.ColumnNumber = Column;
		CurrentGridSlotInfo.RowNumber = Row;

		//Setting all neighboring slots for reference;
		CurrentGridSlotInfo.NeighboringGridSlots = NeighboringSlotIdentifiers;

		CurrentGridSlotInfo.NeighboringGridSlotsToSpreadTo = NeighboringSlotIdentifiersToSpreadTo;


		//Saving current grid slot information
		GridSlotInfoMapContainer.Add(CurrentGridSlotIdentifier) = CurrentGridSlotInfo;

		return NeighboringSlotIdentifiers;
	 
}


FReply SMinefieldGridWidget::CreateMinefieldBaseGrid()
{
	//Creates buttons for every slot in minesweeper
	if (OwningWidget != nullptr)
	{
		TotalGridSlots = OwningWidget->MinefieldHeight * OwningWidget->MinefieldWidth;

		for (int32 i = 0; i < OwningWidget->MinefieldHeight; i++)
		{
			for (int32 j = 0; j < OwningWidget->MinefieldWidth; j++)
			{

				//Creating Grid Slot information struct
				//Each slot has a unique identifier for quick Map access
				FString StringIdentifier = GridSlotNumberToIdentifier(i, j);

				//Setting all neighboring slots for reference;
				FindNeighboringGridSlotIdentifiers(i, j);

				//Need this array to loop through grid slots and choose random slot to put mine.
				ArrayOfGridSlotIdentifiers.Add(StringIdentifier);

				GridPanelContainer->AddSlot(i, j)
					[
						SNew(SButton).ContentPadding(3.f).OnClicked_Lambda([=] ()
							{
									MinefieldGridSlotPressed(i, j);
									FString CurrentN = GridSlotNumberToIdentifier(i, j);
								//	UE_LOG(LogTemp, Warning, TEXT("This button was pressed : %d %d with this identifier: %s"), i, j, *CurrentN);

								return FReply::Handled();
							})
						//.RenderOpacity(1)
					];
			}
		}
		
	}
	return FReply::Handled();
}

FReply SMinefieldGridWidget::MinefieldGridSlotPressed(int32 Column, int32 Row)
{

	FString CurrentGridSlotIdentifier = GridSlotNumberToIdentifier(Column, Row);


	if (!bHasGameStarted)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Game has started"));
		//Set random Mines
		for (int32 i = 0; i < OwningWidget->TotalMines; i++)
		{

			FString RandomGridSlotIdentifier = GenerateRandomMines(CurrentGridSlotIdentifier);

			//UE_LOG(LogTemp, Error, TEXT("RandomGridSlotIdentifier = %s, CurrentGridSlotIdentifier = %s"), *RandomGridSlotIdentifier, *CurrentGridSlotIdentifier);

			FGridSlotInfo* RandomGridInfo = GridSlotInfoMapContainer.Find(RandomGridSlotIdentifier);

			RandomGridInfo->bIsBomb = true;
			//Adding mine to list of mines in game
			MineLocationIdentifiers.Add(RandomGridSlotIdentifier);

		}

		bHasGameStarted = true;
	}

	FindMines(CurrentGridSlotIdentifier);



	return FReply::Handled();
}

FString SMinefieldGridWidget::GenerateRandomMines(FString CurrentGridSlotIdentifier)
{
	TArray<FString> NeighboringGridSlotIdentifiers;
	//Need gridslotinfo to get children bc mines can't be in any of the first buttons children either
	NeighboringGridSlotIdentifiers = GridSlotInfoMapContainer.Find(CurrentGridSlotIdentifier)->NeighboringGridSlots;

	bool bNotSelf = true;
	bool bNotNeighbor = true;

	//random index for array that holds grid slots
	int32 RandomNumber = FMath::RandRange(0, TotalGridSlots - 1);

	//Getting identifier of randomly picked grid slot
	FString RandomGridSlotIdentifier = ArrayOfGridSlotIdentifiers[RandomNumber];

	if (RandomGridSlotIdentifier == CurrentGridSlotIdentifier)
	{
		bNotSelf = false;
	}

	for (int32 i = 0; i < NeighboringGridSlotIdentifiers.Num(); i++)
	{
		if (RandomGridSlotIdentifier == NeighboringGridSlotIdentifiers[i])
		{
			bNotSelf = false;
		}
	}

	if (MineLocationIdentifiers.Num() != 0)
	{
		for (int i = 0; i < MineLocationIdentifiers.Num(); i++)
		{
			if (RandomGridSlotIdentifier == MineLocationIdentifiers[i])
			{
				bNotSelf = false;
			}
		}
	}

	if (bNotSelf && bNotNeighbor)
	{
		return RandomGridSlotIdentifier;
	}
	else
	{
		return GenerateRandomMines(CurrentGridSlotIdentifier);
	}
}

void SMinefieldGridWidget::FindMines(FString SlotIdentifier)
{
	FGridSlotInfo* CurrentGridSlotInfo = GridSlotInfoMapContainer.Find(SlotIdentifier);


	//Turn off functionality if spot already visible
	if (CurrentGridSlotInfo->bHasSwitched == false)
	{
		if (CurrentGridSlotInfo->bIsBomb == false)
		{
			TArray<FString> NeighboringGridSlotIdentifiers = CurrentGridSlotInfo->NeighboringGridSlots;
			TArray<FString> NeighboringGridSlotIdentifiersToSpreadTo = CurrentGridSlotInfo->NeighboringGridSlotsToSpreadTo;

			int32 NeighboringMineCount = 0;

			bool bIsNextToMine = false;

			for (int32 i = 0; i < NeighboringGridSlotIdentifiers.Num(); i++)
			{
				FGridSlotInfo* CurrentNeighboringGridSlotInfo = GridSlotInfoMapContainer.Find(NeighboringGridSlotIdentifiers[i]);

				if (CurrentNeighboringGridSlotInfo->bIsBomb == true)
				{
					NeighboringMineCount++;
					bIsNextToMine = true;
				}
			}

			if (bIsNextToMine)
			{
				CurrentGridSlotInfo->bHasSwitched = true;
				SwitchCount++;

				FSlateFontInfo GameOverText = FCoreStyle::Get().GetFontStyle("EmbossedText");
				GameOverText.Size = 20.f;

				FColor NumberColor = PickNumberColor(NeighboringMineCount);
				GridPanelContainer->AddSlot(CurrentGridSlotInfo->ColumnNumber, CurrentGridSlotInfo->RowNumber)
					[
						SNew(SOverlay)
						+SOverlay::Slot()
						.Padding(1)
						[
							SNew(SImage)
							.ColorAndOpacity(FColor(170,170,170))
						]
						+SOverlay::Slot()
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Font(GameOverText)
							.Text(FText::FromString(FString::FromInt(NeighboringMineCount)))
							.ColorAndOpacity(NumberColor)
							.Visibility(EVisibility::Visible)
						]
						
					];
			}
			else
			{
				CurrentGridSlotInfo->bHasSwitched = true;
				SwitchCount++;

				GridPanelContainer->AddSlot(CurrentGridSlotInfo->ColumnNumber, CurrentGridSlotInfo->RowNumber)
					[
						SNew(SOverlay)
						+SOverlay::Slot()
						.Padding(1)
						[
							SNew(SImage)
							.ColorAndOpacity(FColor(170, 170, 170))
						]
						
					];
				//if this mine has no neighboring bombs keep searching
				//Game starting selection needs to show all neighboring slots
				if (bFirstCheckShowAllChildren == true)
				{
					for (int i = 0; i < NeighboringGridSlotIdentifiers.Num(); i++)
					{
						FindMines(NeighboringGridSlotIdentifiers[i]);
					}

					bFirstCheckShowAllChildren = false;
				}
				else
				{
					for (int i = 0; i < NeighboringGridSlotIdentifiersToSpreadTo.Num(); i++)
					{
						FindMines(NeighboringGridSlotIdentifiersToSpreadTo[i]);
					}
				}

			}
		}
		else if (CurrentGridSlotInfo->bIsBomb == true)
		{
			CurrentGridSlotInfo->bHasSwitched = true;
			GridPanelContainer->AddSlot(CurrentGridSlotInfo->ColumnNumber, CurrentGridSlotInfo->RowNumber)
				[
					SNew(SImage)
					.ColorAndOpacity(FColor::Red)


				];

			if (OwningWidget->bGameIsOver == false)
			{
				//Make Game Over text visible
				OwningWidget->GameOverTextBlock->SetVisibility(EVisibility::Visible);

				//if the player won but pressed a mine, reverses win to loss and hides game win message
				OwningWidget->GameWonTextBlock->SetVisibility(EVisibility::Collapsed);
			
				OwningWidget->bGameIsOver = true;
			}
			
		}
		//Wining Condition
		if (SwitchCount == OwningWidget->MinefieldHeight * OwningWidget->MinefieldWidth - OwningWidget->TotalMines && !OwningWidget->bGameIsOver)
		{
			OwningWidget->GameWonTextBlock->SetVisibility(EVisibility::Visible);
		}

	}
	
}

FString SMinefieldGridWidget::GridSlotNumberToIdentifier(int32 Column, int32 Row)
{
	//need comma to prevent duplicate grid slot identifiers. e.g. C = 11 R = 7 (Ident = 117), C = 1 R = 17 (Ident = 117);
	FString StringIdentifier = FString::FromInt(Column) + ", " + FString::FromInt(Row);
	return StringIdentifier;
}

FColor SMinefieldGridWidget::PickNumberColor(int32 Number)
{
	FColor Color;

	switch (Number)
	{
	case 1:

		Color = FColor::Blue;
		break;

	case 2:

		//Green
		Color = FColor(0, 100, 0);
		break;

	case 3:

		Color = FColor::Red;
		break;

	case 4:

		Color = FColor::Purple;
		break;

	case 5:

		Color = FColor::Orange;
		break;

	case 6:

		Color = FColor::Black;
		break;

	case 7:

		Color = FColor::Cyan;
		break;

	case 8:

		Color = FColor::White;
		break;
	}

	return Color;
}



#undef LOCTEXT_NAMESPACE