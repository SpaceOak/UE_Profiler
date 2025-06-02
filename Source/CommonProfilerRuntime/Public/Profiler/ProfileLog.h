// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Stats/StatsData.h"
#include "StatsThreadStateOverlay.h"
#include "ProfileLog.generated.h"

#define CheckFrameValidity(StateVarName, ReturnVar) \
   FStatsThreadStateOverlay& StateVarName = (FStatsThreadStateOverlay&)FStatsThreadState::GetLocalState(); \
   int64 LastGoodGameFrame = StateVarName.GetLastFullFrameProcessed(); \
   if (!StateVarName.IsFrameValid(LastGoodGameFrame)) { return ReturnVar; } 


UENUM(BlueprintType, Category = "Profiling")
enum class EProfilingStatType : uint8
{
	IncAve UMETA(DisplayName = "IncAve"),
	IncMin UMETA(DisplayName = "IncMin"),
	IncMax UMETA(DisplayName = "IncMax"),
	ExcAve UMETA(DisplayName = "ExcAve"),
	ExcMin UMETA(DisplayName = "ExcMin"),
	ExcMax UMETA(DisplayName = "ExcMax")
};


USTRUCT(BlueprintType, Category = "Profiling")
struct FProfilingStat {

	GENERATED_BODY()
	
	/**
	 * Stat's GroupName (for example STATGROUP_rhi)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling")
	FName GroupName;

	/**
	 * Stat's StatName (for example STAT_RHITriangles)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling")
	FName StatName;

	/**
	 * Stat's StatType (IncAve, IncMin, ExcAve etc...)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profiling")
	EProfilingStatType StatType = EProfilingStatType::IncAve;
};


UCLASS(Blueprintable, Category="Profiling")
class UProfileLog : public UObject
{

	GENERATED_BODY()


private:

	bool ProfilingRunning = false;
	
	FString CameraStatsFileName = FString();

	static inline TMap<EProfilingStatType, FString> StatTypesStrings = {
		{ EProfilingStatType::IncAve, TEXT("IncAve") },
		{ EProfilingStatType::IncMin, TEXT("IncMin") },
		{ EProfilingStatType::IncMax, TEXT("IncMax") },
		{ EProfilingStatType::ExcAve, TEXT("ExcAve") },
		{ EProfilingStatType::ExcMin, TEXT("ExcMin") },
		{ EProfilingStatType::ExcMax, TEXT("ExcMax") }
	};

	/**
	 * Create a filename of output .csv file for camera stats profiling based on current time
	 * Return: filename string
	 */
	FString GetCameraStatsFileName() const;


	/**
	 * Creates/opens the .csv file for camera stats profiling for write/append respectively
	 * Return: file handle pointer,  nullptr - if an error occured
	 */
	FArchive* CreateOrOpenCameraStatsFile(const TArray<FProfilingStat>& StatNames) const;


#if !UE_BUILD_SHIPPING
	/**
	 * Converts stat message to string, based on the innner data type
	 * Return: string representation of the stat value, empty string - if an error occured
	 */
	FString GetStatValueFromMessage(const FComplexStatMessage& ComplexStat, const EComplexStatField::Type Type) const;
#endif

	/**
	 * Converts stat message double value to formated string
	 * Return: string representation of the stat value
	 */
	inline FString FormatStatValueDouble(const double Value) const 
	{
		return FString::Printf(TEXT("%.2f"), Value);
	};


	/**
	 * Converts stat message int value to formated string
	 * Return: string representation of the stat value
	 */
	inline FString FormatStatValueInt64(const int64 Value) const
	{
		return FString::FromInt(Value);
	};


#if !UE_BUILD_SHIPPING
	/**
	 * Converts stat type used by  ProfileLog to stat type used by Unreal Engine.
	 * Return: converted stat type value
	 */
	EComplexStatField::Type ConvertEnumType(EProfilingStatType Type) const;
#endif


public:


	/**
	 * Starts profiling process
	 * Return: true - successfuly started, false - profiling is already running
	 */
	UFUNCTION(BlueprintCallable, Category="VVRProfiling")
	bool StartProfiling();


	/**
	 * Finishes profiling process
	 * Return: true - successfuly finished, false - profiling is not running
	 */
	UFUNCTION(BlueprintCallable, Category = "VVRProfiling")
	bool FinishProfiling();

	
	/**
	 * Appends the previous frame stats values to csv file in Saved/Profiling folder. Camera name is index column.
	 * Return: true - successfuly written to file, false - something went wrong
	 * 
	 * @param CameraName name of the camera of the frame
	 * @param StatNames stat names to record
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "VVRProfiling")
	bool TakeStatsSnapshotFromCameraToFile(const FString& CameraName, const TArray<FProfilingStat>& Stats) const;

	
	/**
	 * Get all existing stat group names
	 * Return: array of stat group names, empty array - if an error occured
	 */
	UFUNCTION(BlueprintCallable, Category = "VVRProfiling")
	TArray<FName> GetStatGroupNames() const;


	/**
	 * Get all existing stat names for given stat group
	 * Return: array of stat names for given stat group, empty array - if an error occured
	 *
	 * @param GroupName name of the stat group 
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "VVRProfiling")
	TArray<FName> GetStatGroupStatNames(const FName& GroupName) const;


	/**
	 * Get stat value string from previous frame for given ProfilingStat. ProfilingStat consists of StatGroup, StatName and StatType.
	 * Return:string value for the stat, empty string - if an error occured
	 *
	 * @param StatName name of the stat to get value of
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "VVRProfiling")
	FString GetStatValue(const FProfilingStat& Stat) const;


	/**
	 * DONT USE FOR NOW!
	 * 
	 * Get an array of stat value strings from previous frame for given ProfilingStats. ProfilingStat consists of StatGroup, StatName and StatType.
	 * Return: array of string values for the input stat names, empty array - if an error occured
	 *
	 * @param Stats ProfilingStats structs of the stats to get values of
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "VVRProfiling")
	TArray<FString> GetStatValues(const TArray<FProfilingStat>& Stats) const;
};
