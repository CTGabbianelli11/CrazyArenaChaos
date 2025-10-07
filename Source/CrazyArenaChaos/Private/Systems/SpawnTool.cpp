#include "Systems/SpawnTool.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

#include "Interfaces/Spawnable.h"

ASpawnTool::ASpawnTool()
{
	PrimaryActorTick.bCanEverTick = false;

	Area = CreateDefaultSubobject<USphereComponent>(TEXT("Area"));
	Area->InitSphereRadius(Radius);
	Area->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Area;
}

void ASpawnTool::BeginPlay()
{
	Super::BeginPlay();
	if (Seed != 0) StreamOpt = FRandomStream(Seed);
	if (bSpawnOnBeginPlay) SpawnAll();
}

#if WITH_EDITOR
void ASpawnTool::PostEditChangeProperty(FPropertyChangedEvent& E)
{
	Super::PostEditChangeProperty(E);
	if (Area && E.Property && E.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ASpawnTool, Radius))
	{
		Area->SetSphereRadius(Radius);
	}
}
#endif

bool ASpawnTool::IsFarEnough(const FVector& A, const FVector& B, float MinDist)
{
	return FVector::DistSquaredXY(A, B) >= FMath::Square(MinDist);
}

bool ASpawnTool::GetRandomLocation(FVector& Out) const
{
	const FVector Origin = GetActorLocation();
	const float R = Radius;

	float Angle, Dist;
	if (StreamOpt.IsSet())
	{
		Angle = StreamOpt->FRandRange(0.f, 2 * PI); Dist = FMath::Sqrt(StreamOpt->FRand()) * R;
	}
	else
	{
		Angle = FMath::FRandRange(0.f, 2 * PI);     Dist = FMath::Sqrt(FMath::FRand()) * R;
	}

	Out = Origin + FVector(FMath::Cos(Angle) * Dist, FMath::Sin(Angle) * Dist, 0.f);
	return true;
}

bool ASpawnTool::ProjectToNavMesh(FVector& InOut) const
{
	if (!bUseNavmesh) return true;
	if (const UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation OutLoc;
		if (Nav->ProjectPointToNavigation(InOut, OutLoc, FVector(200.f)))
		{
			InOut = OutLoc.Location; return true;
		}
	}
	return false;
}

bool ASpawnTool::AlignToGround(FVector& InOut, FRotator& OutRot) const
{
	if (!bAlignToGround) { OutRot = FRotator::ZeroRotator; return true; }

	FVector Start = InOut + FVector(0, 0, GroundTraceHeight);
	FVector End = InOut - FVector(0, 0, GroundTraceHeight * 2.f);
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpawnTrace), false, this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		InOut = Hit.ImpactPoint; OutRot = Hit.ImpactNormal.Rotation(); return true;
	}
	return false;
}

bool ASpawnTool::FindValidLocation(const TArray<FVector>& Used, FVector& Out) const
{
	for (int32 Try = 0; Try < MaxTriesPerActor; ++Try)
	{
		FVector P; GetRandomLocation(P);
		if (!ProjectToNavMesh(P)) continue;

		bool bOk = true;
		for (const FVector& U : Used)
			if (!IsFarEnough(P, U, MinDistance)) { bOk = false; break; }
		if (!bOk) continue;

		FRotator Dummy;
		if (!AlignToGround(P, Dummy)) continue;

		Out = P; return true;
	}
	return false;
}

void ASpawnTool::SpawnAll()
{
	if (Entries.Num() == 0) return;
	if (!bAllowMultipleSpawns && bHasSpawned) return;
	TArray<FVector> Used; Used.Reserve(128);

	for (const FSpawnEntry& E : Entries)
	{
		if (!E.Class) continue;

		for (int32 i = 0; i < E.Count; ++i)
		{
			FVector Loc;
			if (!FindValidLocation(Used, Loc))
			{
				UE_LOG(LogTemp, Warning, TEXT("[SpawnTool] Failed to place %s"), *E.Class->GetName());
				continue;
			}

			FRotator Rot = FRotator::ZeroRotator;
			if (bAlignToGround) { FVector Tmp = Loc; AlignToGround(Tmp, Rot); }

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			Params.Owner = this;
			Params.Instigator = GetInstigator();

			if (AActor* NewA = GetWorld()->SpawnActor<AActor>(E.Class, Loc, Rot, Params))
			{
				Used.Add(Loc);
				if (NewA->GetClass()->ImplementsInterface(USpawnable::StaticClass()))
				{
					ISpawnable::Execute_OnSpawnedFromSpawner(NewA, this);
				}
				OnSpawned(NewA);
			}
		}
		bHasSpawned = true;
	}
}

void ASpawnTool::PreviewSpawn()
{
#if WITH_EDITOR
	// Genera gli stessi punti che useresti per lo spawn vero
	PreviewPoints.Reset();

	// Calcola quanti oggetti totali vogliamo “simulare”
	int32 Total = 0;
	for (const FSpawnEntry& E : Entries)
		if (E.Class) Total += FMath::Max(0, E.Count);

	TArray<FVector> Used;
	Used.Reserve(Total);

	for (const FSpawnEntry& E : Entries)
	{
		if (!E.Class) continue;

		for (int32 i = 0; i < E.Count; ++i)
		{
			FVector Loc;
			if (!FindValidLocation(Used, Loc))
				continue;

			Used.Add(Loc);
			PreviewPoints.Add(Loc);

			// Disegno: linea persistente per PreviewLifeTime secondi
			DrawDebugSphere(GetWorld(), Loc, PreviewSphereRadius, 12,
				FColor::Cyan, /*bPersistent*/ true, PreviewLifeTime);
		}
	}
#endif
}

void ASpawnTool::ClearPreview()
{
#if WITH_EDITOR
	PreviewPoints.Reset();
	// Cancella tutte le debug lines persistenti di questo world
	FlushPersistentDebugLines(GetWorld());
#endif
}

void ASpawnTool::ResetSpawn()
{
	bHasSpawned = false;
}
