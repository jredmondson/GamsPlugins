// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Types/SlateEnums.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "SlateOptMacros.h"


/**
 * 
 */
class GAMSAGENTS_API SGamsAgentsWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGamsAgentsWidget)
	{}
  SLATE_ARGUMENT(TWeakObjectPtr<class GamsEnvironmentsHUD>, OwnerHUDArg);
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

  TWeakObjectPtr<class GamsEnvironmentsHUD> OwnerHUD;
};
