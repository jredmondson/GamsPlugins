// Fill out your copyright notice in the Description page of Project Settings.

#include "SGamsAgentsWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Templates/SharedPointer.h"
#include "Internationalization.h"

#define LOCTEXT_NAMESPACE "SGamsAgentsWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGamsAgentsWidget::Construct(const FArguments& InArgs)
{
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/

  // ++ Asign the argument to our local variable
        // name will be _OwnerHUDArg instead of OwnerHUDArg, see comments about SLATE_ARGUMENT before
  OwnerHUD = InArgs._OwnerHUDArg;

  // ++ Add all this Slate code
// If the code below doesn't look like C++ to you it's because it (sort-of) isn't,
// Slate makes extensive use of the C++ Prerocessor(macros) and operator overloading,
// Epic is trying to make our lives easier, look-up the macro/operator definitions to see why.

  ChildSlot
    .VAlign(VAlign_Fill)
    .HAlign(HAlign_Fill)
    [
      SNew(SOverlay)
      + SOverlay::Slot()
    .VAlign(VAlign_Top)
    .HAlign(HAlign_Center)
    [
      // Inside lies a text block with these settings
      SNew(STextBlock)
      .ShadowColorAndOpacity(FLinearColor::Black)
    .ColorAndOpacity(FLinearColor::Red)
    .ShadowOffset(FIntPoint(-1, 1))
    .Font(FSlateFontInfo("Veranda", 16))
    // localized text to be translated with a generic name HelloSlateText
    .Text(LOCTEXT("HelloSlateText", "Hello, Slate!"))
    ]
    ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
