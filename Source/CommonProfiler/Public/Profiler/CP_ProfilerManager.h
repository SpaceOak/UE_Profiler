#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CP_ProfilerManager.generated.h"

UCLASS()
class COMMONPROFILER_API UProfilerManager : public UObject
{
	GENERATED_BODY()

public:
	// Инициализация менеджера
	UFUNCTION(BlueprintCallable, Category="Performance Profiler")
	void Initialize();

	// Получение FPS
	UFUNCTION(BlueprintPure, Category="Performance Profiler")
	float GetFPS() const;

	// Получение CPU/GPU загрузки
	UFUNCTION(BlueprintPure, Category="Performance Profiler")
	float GetCPUUsage() const;

	UFUNCTION(BlueprintPure, Category="Performance Profiler")
	float GetGPUUsage() const;

	// Получение количества Draw Calls
	UFUNCTION(BlueprintPure, Category="Performance Profiler")
	int32 GetDrawCalls() const;

	// Получение текущей VRAM (в мегабайтах)
	UFUNCTION(BlueprintPure, Category="Performance Profiler")
	float GetVRAMUsage() const;

	// Обновление данных
	UFUNCTION()
	void UpdateStats();

private:
	float CurrentFPS;
	float CPUUsage;
	float GPUUsage;
	int32 DrawCalls;
	float VRAMUsage;

	// Таймер для обновления данных
	FTimerHandle UpdateTimerHandle;
};
