// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateExtras.h"
#include "SlateBasics.h"

/**
 * 
 */
class SMinefieldStartupWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMinefieldStartupWidget) {}

	SLATE_ARGUMENT(TWeakPtr<class FTestPluginModule>, PluginModule)

	SLATE_END_ARGS()

	//Construction function for the widget
	void Construct(const FArguments& InArgs);

	TSharedPtr<SOverlay> GameResultTextContainer;
	TSharedPtr<STextBlock> GameOverTextBlock;
	TSharedPtr<STextBlock> GameWonTextBlock;

	FMargin VerticalBoxPadding;

	int32 MinefieldHeight = 15;

	int32 MinefieldWidth = 15;

	int32 TotalMines = 50;

	bool bGameIsOver;

private:

	//Creates Minesweeper game
	FReply CreateMinefieldSection();

	FReply CreateMinefieldMenu();


	//Makes sure player input is within required range
	int32 CheckEditableBoxText(const FText& Text);

	//Setting minefield grid column number
	void EditableBoxTextHeight(const FText& Text);

	//Setting minefield grid row number
	void EditableBoxTextWidth(const FText& Text);

	//Setting total mines
	void EditableBoxTextTotalMines(const FText& Text);

	//Updates max mines for inputed height and width for hint text
	void UpdateTotalMineHintText();

	//Resets total mines to default amount
	void ResetTotalMines(bool bCalledFromTotalMines);

	void ChangeDifficulty(ECheckBoxState State);

	int32 Difficulty = 5;

	//Needed to dynamically update hint text
	TSharedPtr<SEditableTextBox> EditableTextTotalMinesContainer;
	
	//Mindsweeper game widget
	TSharedPtr<class SMinefieldGridWidget> MinefieldGridContainer;

	//Holds both the menu and the minefield widget. Only the minefield widget gets added and removed for new games
	TSharedPtr<SVerticalBox> MinefieldVerticalBoxContainer;

	//Container for the minesweeper game widget
	TSharedPtr<SMinefieldGridWidget> MinefieldGridWidgetContainer;

//	TWeakObjectPtr<SMinefieldStartupWidget> SelfReference;
	//The module that created this widget
	TWeakObjectPtr<class FTestPluginModule> PluginModule;


};
