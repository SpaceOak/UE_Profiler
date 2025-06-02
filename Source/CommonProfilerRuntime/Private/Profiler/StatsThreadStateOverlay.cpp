// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonProfilerRuntime/Public/Profiler/StatsThreadStateOverlay.h"

#if !UE_BUILD_SHIPPING

FStatsThreadStateOverlay::FStatsThreadStateOverlay()
{
}

int64 FStatsThreadStateOverlay::GetLastFullFrameProcessed() const
{
	return LastFullFrameProcessed;
}


FStatsThreadStateOverlay::~FStatsThreadStateOverlay()
{
}

#endif
