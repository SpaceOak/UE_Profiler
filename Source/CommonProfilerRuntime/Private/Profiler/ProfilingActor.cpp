// Fill out your copyright notice in the Description page of Project Settings.



#include "CommonProfilerRuntime/Public/Profiler//ProfilingActor.h"

#if WITH_EDITOR
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "Templates/Casts.h"
#include "LevelInstance/LevelInstanceSubsystem.h"
#include "LevelInstance/LevelInstanceLevelStreaming.h"
#include "LevelEditorViewport.h"
#include "Editor/EditorEngine.h"
#include "Editor.h"
#include "Engine/World.h"
#endif


// Sets default values
AProfilingActor::AProfilingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AProfilingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProfilingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


#if WITH_EDITOR
UWorld* AProfilingActor::GetViewportWorld() const
{
	FLevelEditorViewportClient* viewport = nullptr;

	for (FLevelEditorViewportClient* EditorViewportClient : GEditor->GetLevelViewportClients())
	{
		if (EditorViewportClient && EditorViewportClient->IsVisible())
		{
			viewport = EditorViewportClient;
		}
	}

	if (!viewport) {
		return nullptr;
	}

	return viewport->GetWorld();
}

TArray <ULevelStreaming*> AProfilingActor::GetStreamingLevels() const 
{
	UWorld* world = GetViewportWorld();

	if(! world) {
		return TArray<ULevelStreaming*> ();
	}

	return world->GetStreamingLevels();
}

FTransform AProfilingActor::GetLevelInstanceLevelTransform(AActor* actor) const
{	
	ALevelInstance* levelInstance = Cast<ALevelInstance>(actor);
	UWorld* world = GetViewportWorld();

	if (!world || !levelInstance) {
		return FTransform();
	}

	ULevelInstanceSubsystem* LevelInstanceSubsystem = world->GetSubsystem<ULevelInstanceSubsystem>();

	if (!LevelInstanceSubsystem) {
		return FTransform();
	}

	ULevelStreamingLevelInstance* streaming = LevelInstanceSubsystem->GetLevelInstanceLevelStreaming(levelInstance);

	if (!streaming) {
		return FTransform();
	}

	return streaming->LevelTransform;
}

#endif