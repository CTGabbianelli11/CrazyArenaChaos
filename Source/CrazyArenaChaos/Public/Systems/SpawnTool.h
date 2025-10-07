#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/ISpawner.h" 

#include "SpawnTool.generated.h"

class USphereComponent;

USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> Class = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0"))
	int32 Count = 1;
};


UCLASS()
class CRAZYARENACHAOS_API ASpawnTool : public AActor, public ISpawner
{
	GENERATED_BODY()

public:

	ASpawnTool();

	virtual void Spawn_Implementation() override { SpawnAll(); }
	virtual void ResetSpawn_Implementation() override { ResetSpawn(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") float Radius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0.0")) float MinDistance = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "1")) int32 MaxTriesPerActor = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") bool bSpawnOnBeginPlay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") bool bAlignToGround = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0.0")) float GroundTraceHeight = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") int32 Seed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") bool bUseNavmesh = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0.0")) bool bAllowMultipleSpawns = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TArray<FSpawnEntry> Entries;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnAll();

	UFUNCTION(BlueprintImplementableEvent, Category = "Spawn")
	void OnSpawned(AActor* Spawned);

protected:
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY() USphereComponent* Area = nullptr;
	TOptional<FRandomStream> StreamOpt;
	bool bHasSpawned = false;
	bool FindValidLocation(const TArray<FVector>& Used, FVector& OutLocation) const;
	bool GetRandomLocation(FVector& OutLocation) const;
	bool ProjectToNavMesh(FVector& InOutLocation) const;
	bool AlignToGround(FVector& InOutLocation, FRotator& OutRot) const;
	static bool IsFarEnough(const FVector& A, const FVector& B, float MinDist);



	// --- Preview in Editor ---
	UPROPERTY(EditAnywhere, Category = "Spawn|Preview")
	float PreviewSphereRadius = 25.f;

	UPROPERTY(EditAnywhere, Category = "Spawn|Preview")
	float PreviewLifeTime = 10.f; 

	UPROPERTY(Transient)
	TArray<FVector> PreviewPoints;

	UFUNCTION(CallInEditor, Category = "Spawn|Preview")
	void PreviewSpawn();     //Draw Previews

	UFUNCTION(CallInEditor, Category = "Spawn|Preview")
	void ClearPreview();     // Clear Preview
	void ResetSpawn();    
};