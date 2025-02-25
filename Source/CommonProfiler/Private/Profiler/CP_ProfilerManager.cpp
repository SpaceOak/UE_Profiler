#include "Profiler/CP_ProfilerManager.h"
#include "Engine/Engine.h"
#include "Stats/StatsData.h"
#include "HAL/PlatformProcess.h"
#include "RHI.h"

// Инициализация менеджера
void UProfilerManager::Initialize()
{
	if (GWorld)
	{
		GWorld->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UProfilerManager::UpdateStats, 1.0f, true);
	}
}

// Получение FPS
float UProfilerManager::GetFPS() const
{
	return CurrentFPS;
}

// Получение CPU/GPU загрузки
float UProfilerManager::GetCPUUsage() const
{
	return CPUUsage;
}

float UProfilerManager::GetGPUUsage() const
{
	return GPUUsage;
}

// Получение Draw Calls
int32 UProfilerManager::GetDrawCalls() const
{
	return DrawCalls;
}

// Получение VRAM
float UProfilerManager::GetVRAMUsage() const
{
	return VRAMUsage;
}

// Обновление всех метрик
void UProfilerManager::UpdateStats()
{
	// Получаем FPS через движковую систему
	if (GEngine)
	{
		//CurrentFPS = FApp::GetCurrentFPS();
	}

	// CPU Usage (используем Platform API)
	//CPUUsage = FPlatformProcess::GetCPUUsage();

	// GPU Usage (примерная оценка)
	//GPUUsage = float(RHIGetGPUFrameCycles()) / 1000.0f;

	// Draw Calls
	//DrawCalls = GRenderThreadStats->DrawCalls;

	// VRAM (получаем данные из RHI)
	//VRAMUsage = GRHIMemoryStats.TotalAllocated / (1024.0f * 1024.0f);
}
