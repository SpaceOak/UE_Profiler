// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonProfilerRuntime/Public/Profiler/ProfileLog.h"
#include "Misc/DateTime.h"
#include "Algo/Reverse.h"

FArchive* UProfileLog::CreateOrOpenCameraStatsFile(const TArray<FProfilingStat>& Stats) const
{

	FString FilePath = FPaths::ProjectDir() / TEXT("Saved/Profiling") / CameraStatsFileName + TEXT(".csv");

	if (FPaths::FileExists(FilePath))
	{
		return IFileManager::Get().CreateFileWriter(*FilePath, FILEWRITE_Append);
	}

	FArchive* FileWriter = IFileManager::Get().CreateFileWriter(*FilePath);

	if (! FileWriter) 
	{
		return nullptr;
	}

	TArray<FString> NamesStrings;

	for (auto Stat : Stats)
	{
		NamesStrings.Add(Stat.StatName.ToString() + TEXT(" ") + StatTypesStrings[Stat.StatType]);
	}

	FString Result = TEXT("Camera, ") + FString::Join(NamesStrings, TEXT(",")) + TEXT("\n");

	FileWriter->Serialize(TCHAR_TO_ANSI(*Result), Result.Len());

	return FileWriter;

}

FString UProfileLog::GetCameraStatsFileName() const 
{
	FDateTime CurrentTime = FDateTime::Now();
	FString FormattedTime = CurrentTime.ToString(TEXT("%d%m%y%H%M%S"));
	return FString("camstats_") + FormattedTime;
}


bool UProfileLog::StartProfiling() 
{

	if (ProfilingRunning) 
	{
		return false;
	}

	ProfilingRunning = true;
	CameraStatsFileName = GetCameraStatsFileName();

	return true;
}


bool UProfileLog::FinishProfiling() 
{

	if (! ProfilingRunning) 
	{
		return false;
	}

	ProfilingRunning = false;
	CameraStatsFileName = FString();

	return true;
}


bool UProfileLog::TakeStatsSnapshotFromCameraToFile(const FString& CameraName, const TArray<FProfilingStat>& Stats) const
{

	if (! ProfilingRunning) 
	{
		return false;
	}

	FArchive* FileWriter = CreateOrOpenCameraStatsFile(Stats);

	if (! FileWriter) 
	{
		return false;
	}

	FString Result = FString(CameraName) + TEXT(",");

	for (auto Stat : Stats) 
	{
		Result += GetStatValue(Stat) + TEXT(",");
	}

	Result += TEXT("\n");

	FileWriter->Serialize(TCHAR_TO_ANSI(*Result), Result.Len());

	FileWriter->Close();
	delete FileWriter;

	return true;
}


TArray<FName> UProfileLog::GetStatGroupNames() const 
#if !UE_BUILD_SHIPPING
{

	CheckFrameValidity(Stats, TArray<FName>())

	TSet<FName> GroupNames = TSet<FName>();

	for (auto [key, value] : Stats.Groups)
	{
		GroupNames.Add(key);
	}

	return GroupNames.Array();
}
#else
{
	return TArray<FName>();
}
#endif

TArray<FName> UProfileLog::GetStatGroupStatNames(const FName& GroupName) const 
#if !UE_BUILD_SHIPPING
{

	CheckFrameValidity(Stats, TArray<FName>())

	TArray<FName> GroupItems;
	Stats.Groups.MultiFind(GroupName, GroupItems);

	return GroupItems;
}
#else
{
	return TArray<FName>();
}
#
#endif

TArray<FString> UProfileLog::GetStatValues(const TArray<FProfilingStat>& Stats) const
{

	//DO NOT USE!!!

	return TArray<FString>();
}


#if !UE_BUILD_SHIPPING
FString UProfileLog::GetStatValueFromMessage(const FComplexStatMessage& ComplexStat, const EComplexStatField::Type Type) const
{

	switch (ComplexStat.NameAndInfo.GetField<EStatDataType>())
	{
	case EStatDataType::ST_int64:
		return FormatStatValueInt64(ComplexStat.GetValue_int64(Type));
	case EStatDataType::ST_double:
		return FormatStatValueDouble(ComplexStat.GetValue_double(Type));
	default:
		check(0);
		return FString();
	}
}
#endif


FString UProfileLog::GetStatValue(const FProfilingStat& Stat) const
#if !UE_BUILD_SHIPPING
{

	FGameThreadStatsData* StatsData = FLatestGameThreadStatsData::Get().Latest;

	if (!StatsData) {
		return FString();
	}

	for (int32 GroupIndex = 0; GroupIndex < StatsData->ActiveStatGroups.Num(); ++GroupIndex)
	{

		const FActiveStatGroupInfo& StatGroup = StatsData->ActiveStatGroups[GroupIndex];

		if (!StatsData->GroupNames[GroupIndex].IsEqual(Stat.GroupName))
		{
			continue;
		}

		const bool bBudget = StatGroup.ThreadBudgetMap.Num() > 0;

		const int32 NumThreadsBreakdown = bBudget ? StatGroup.FlatAggregateThreadBreakdown.Num() : 1;

		TArray<FName> ThreadNames;
		StatGroup.FlatAggregateThreadBreakdown.GetKeys(ThreadNames);


		for (int32 ThreadBreakdownIdx = 0; ThreadBreakdownIdx < NumThreadsBreakdown; ++ThreadBreakdownIdx)
		{
			FName ThreadName;

			if (bBudget)
			{
				ThreadName = ThreadNames[ThreadBreakdownIdx];
			}

			if (StatGroup.FlatAggregate.Num() <= 0) 
			{
				continue;
			}

			TArray<FComplexStatMessage> Aggregates = bBudget ? StatGroup.FlatAggregateThreadBreakdown[ThreadName] : StatGroup.FlatAggregate;
			for (const auto& ComplexStat : Aggregates)
			{
				if (ComplexStat.NameAndInfo.GetShortName().IsEqual(Stat.StatName))
				{
					return GetStatValueFromMessage(ComplexStat, ConvertEnumType(Stat.StatType));
				}
			}

		}

		
		if (StatGroup.CountersAggregate.Num() <= 0) 
		{
			continue;
		}


		const TArray<FComplexStatMessage> Aggregates = StatGroup.CountersAggregate;
		for (const auto& ComplexStat : Aggregates)
		{
			if (ComplexStat.NameAndInfo.GetShortName().IsEqual(Stat.StatName))
			{
				return GetStatValueFromMessage(ComplexStat, ConvertEnumType(Stat.StatType));
			}
		}

	}

	return FString();
}
#else
{
	return FString();
}
#endif


#if !UE_BUILD_SHIPPING
EComplexStatField::Type UProfileLog::ConvertEnumType(EProfilingStatType Type) const
{
	switch (Type)
	{
	case EProfilingStatType::IncAve:
		return EComplexStatField::Type::IncAve;
	case EProfilingStatType::IncMin:
		return EComplexStatField::Type::IncMin;
	case EProfilingStatType::IncMax:
		return EComplexStatField::Type::IncMax;
	case EProfilingStatType::ExcAve:
		return EComplexStatField::Type::ExcAve;
	case EProfilingStatType::ExcMin:
		return EComplexStatField::Type::ExcMin;
	case EProfilingStatType::ExcMax:
		return EComplexStatField::Type::ExcMax;
	default:
		check(0);
		return EComplexStatField::Type::IncAve;
	}
}
#endif