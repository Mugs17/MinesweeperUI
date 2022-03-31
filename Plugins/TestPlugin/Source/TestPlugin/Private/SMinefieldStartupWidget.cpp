// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinefieldStartupWidget.h"
#include "Slate/SObjectWidget.h"
#include "SMinefieldGridWidget.h"
#include "SlateFwd.h"
#include "Containers/UnrealString.h"

#define LOCTEXT_NAMESPACE "SMinefieldStartupWidget"

void SMinefieldStartupWidget::Construct(const FArguments& InArgs)
{	

	CreateMinefieldMenu();
	

}

void SMinefieldStartupWidget::EditableBoxTextHeight(const FText& Text)
{
	MinefieldHeight = CheckEditableBoxText(Text);
	
	UpdateTotalMineHintText();

	ResetTotalMines(false);

}

void SMinefieldStartupWidget::EditableBoxTextWidth(const FText& Text)
{
	MinefieldWidth = CheckEditableBoxText(Text);

	UpdateTotalMineHintText();

	ResetTotalMines(false);
}

void SMinefieldStartupWidget::EditableBoxTextTotalMines(const FText& Text)
{
	//Duplication of code needed here because I want a different max value for TotalMines
	if (Text.IsNumeric())

	{
		FString String = Text.ToString();
		int InputedNumber = FCString::Atoi(*String);

		//Making sure total mines never goes over a game breaking amount
		int MaxMines = (MinefieldHeight * MinefieldWidth * .8) - 9;

		if (InputedNumber && InputedNumber <= MaxMines && InputedNumber >= 5)
		{
			TotalMines = FCString::Atoi(*String);
			UE_LOG(LogTemp, Warning, TEXT("Total Mines: %d"), TotalMines);
		}
		else
		{
			//Resetting to default number
			ResetTotalMines(true);
		}
	}
	else
	{
		//Resetting to default number
		ResetTotalMines(true);
	}
}

void SMinefieldStartupWidget::UpdateTotalMineHintText()
{
	//Making sure total mines never goes over a game breaking amount
	int MaxMines = (MinefieldHeight * MinefieldWidth * .8) - 9;

	FString HintText = "Min: 5, Max: " + FString::FromInt(MaxMines);

	//Dynamically Updating TotalMine hint text
	EditableTextTotalMinesContainer->SetHintText(FText::FromString(HintText));
}

void SMinefieldStartupWidget::ResetTotalMines(bool bCalledFromTotalMines)
{
	//Clearing total mine text if it isn't current input field
	if (!bCalledFromTotalMines)
	{
		EditableTextTotalMinesContainer->SetText(FText::FromString(""));
	}
	
	TotalMines = (MinefieldHeight * MinefieldWidth) / Difficulty;
}

void SMinefieldStartupWidget::ChangeDifficulty(ECheckBoxState State)
{
	//Change to hard
	if (State == ECheckBoxState::Checked)
	{
		Difficulty = 3;
	}
	//change to easy
	else if (State == ECheckBoxState::Unchecked)
	{
		Difficulty = 5;
	}

	//Clearing total mine text when player selects a difficulty setting
	EditableTextTotalMinesContainer->SetText(FText::FromString(""));
}


FReply  SMinefieldStartupWidget::CreateMinefieldSection()
{
	//UE_LOG(LogTemp, Error, TEXT("Mine Height: %f, Mine Width: %f, TotalMines: %f"), MinefieldHeight, MinefieldWidth, TotalMines);

	//Make GameText invisible
	GameOverTextBlock->SetVisibility(EVisibility::Collapsed);
	GameWonTextBlock->SetVisibility(EVisibility::Collapsed);

	bGameIsOver = false;

	//Replace grid if it has already been generated
	if (MinefieldGridWidgetContainer)
	{
		MinefieldVerticalBoxContainer->RemoveSlot(MinefieldGridWidgetContainer.ToSharedRef());

		MinefieldVerticalBoxContainer->AddSlot()
			[
				SAssignNew(MinefieldGridWidgetContainer, SMinefieldGridWidget).OwningWidget(SharedThis(this))
			];

	}
	else
	{
		MinefieldVerticalBoxContainer->AddSlot()
			[
				SAssignNew(MinefieldGridWidgetContainer, SMinefieldGridWidget).OwningWidget(SharedThis(this))
			];
	}
		

		
	return FReply::Handled();
}

FReply SMinefieldStartupWidget::CreateMinefieldMenu()
{

	FText WidgetText = LOCTEXT("WindowWidgetText", "Category Text:");

	VerticalBoxPadding = FMargin(50.f, 5.f, 0.f, 5.f);
	const FMargin HorizontalContentPadding = FMargin(10.f, 0.f);

	FSlateFontInfo GameOverText = FCoreStyle::Get().GetFontStyle("EmbossedText");
	GameOverText.Size = 40.f;


	float TextInputWidth = 200.f;
	float GridBoxSize = 50.f;


	FSlateFontInfo CategoryText = FCoreStyle::Get().GetFontStyle("EmbossedText");
	CategoryText.Size = 15.f;

	ChildSlot
		[
			
			SAssignNew(MinefieldVerticalBoxContainer, SVerticalBox)
			//Red line
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(VerticalBoxPadding)
			[
				SNew(SImage)
				.ColorAndOpacity(FColor::Red)
			]
			//Container for entire minesweeper menu
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(VerticalBoxPadding)
			[
				//Line 1
				SNew(SHorizontalBox)
				//Line 1.1 Width text
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[

					SNew(STextBlock)
					.Font(CategoryText)
					.Text(LOCTEXT("Width Text", "Width:"))

				]
				//Line 1.2 Width input
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.Padding(0)
				.AutoWidth()
				.MaxWidth(TextInputWidth)
				.Padding(HorizontalContentPadding)
				[

					SNew(SEditableTextBox)
					.Font(CategoryText)
					.MinDesiredWidth(TextInputWidth)
					.OnTextChanged(this, &SMinefieldStartupWidget::EditableBoxTextWidth)
					.HintText(LOCTEXT("HintText", "Min: 5, Max: 50"))
				]
				//Line 1.3 Height text
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.Padding(0)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Font(CategoryText)
					.Text(LOCTEXT("Height Text", "Height:"))
					]
				//Line 1.4 Height input
				+ SHorizontalBox::Slot()
				.Padding(0)
				.HAlign(HAlign_Left)
				.AutoWidth()
				.MaxWidth(TextInputWidth)
				.Padding(HorizontalContentPadding)
				[
					SNew(SEditableTextBox)
					.Font(CategoryText)
					.MinDesiredWidth(TextInputWidth)
					.OnTextChanged(this, &SMinefieldStartupWidget::EditableBoxTextHeight)
					.HintText(LOCTEXT("HintText", "Min: 5, Max: 50"))
				]
			]
			//Line 2
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(VerticalBoxPadding)
			[
				//Line 2.1 Total mines text
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Font(CategoryText)
					.Text(LOCTEXT("Total Mines Text", "Number of Mines:   "))
				]
				//Line 2.2 Total mines input
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				.MaxWidth(TextInputWidth)
				[
					SAssignNew(EditableTextTotalMinesContainer, SEditableTextBox)
					.Font(CategoryText)
					.MinDesiredWidth(TextInputWidth)
					.OnTextChanged(this, &SMinefieldStartupWidget::EditableBoxTextTotalMines)
					.HintText(LOCTEXT("HintTextMineCount", "Min: 5"))
				]
			]
			//Line 3
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(VerticalBoxPadding)
			[
				//Line 3.1 Choose difficulty text
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Font(CategoryText)
					.Text(LOCTEXT("Difficulty Text", "Hard Difficulty:   "))
				]
				//Line 3.2 Difficulty checkbox
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				.MaxWidth(TextInputWidth)
				[
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &SMinefieldStartupWidget::ChangeDifficulty)
				]
			]
			//Line 3 Generate grid button
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(VerticalBoxPadding)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				[
					SNew(SButton)
					.OnClicked(this, &SMinefieldStartupWidget::CreateMinefieldSection)
					[
						SNew(STextBlock)
						.Font(CategoryText)
						.Text(LOCTEXT("Start Game Text", "Generate New Grid"))
					]
				]
			]
			//Line 4 Game result text (Default: Invisible)
			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(VerticalBoxPadding)
			[
				SAssignNew(GameResultTextContainer, SOverlay)
				//Game over result
				+SOverlay::Slot()
				[
					SAssignNew(GameOverTextBlock, STextBlock)
					.Visibility(EVisibility::Collapsed)
					.Font(GameOverText)
					.ColorAndOpacity(FColor::Red)
					.Text(LOCTEXT("GameOverText", "Game Over!"))
					
				]
				//Game won result
				+ SOverlay::Slot()
				[
					SAssignNew(GameWonTextBlock, STextBlock)
					.Visibility(EVisibility::Collapsed)
					.Font(GameOverText)
					.ColorAndOpacity(FColor::Green)
					.Text(LOCTEXT("GameWonText", "You Won!"))

				]
			]
		];
		
	return FReply::Handled();
}


int SMinefieldStartupWidget::CheckEditableBoxText(const FText& Text)
{
	//Checking player input to make sure it is in required range
	FString String = Text.ToString();
	int MaxGridLines = 50;

	if (String.IsNumeric())
	{
		int InputedNumber = FCString::Atoi(*String);
		if (InputedNumber && InputedNumber <= MaxGridLines && InputedNumber >= 5)
		{
			return FCString::Atoi(*String);
		}
		else
		{
			//Default number
			return 5;
		}
	}
	else
	{
		//Default number
		return 5;
	}

}


#undef LOCTEXT_NAMESPACE