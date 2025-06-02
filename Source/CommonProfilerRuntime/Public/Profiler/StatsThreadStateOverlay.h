// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stats/StatsData.h"

#if !UE_BUILD_SHIPPING
/**
 * 
 */
class FStatsThreadStateOverlay : public FStatsThreadState
{

public:

	FStatsThreadStateOverlay();

	int64 GetLastFullFrameProcessed() const;

	~FStatsThreadStateOverlay();
};
#endif