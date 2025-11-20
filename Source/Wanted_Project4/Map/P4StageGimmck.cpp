// P4StageGimmck.cpp

#include "Map/P4StageGimmck.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

AP4StageGimmck::AP4StageGimmck()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP4StageGimmck::BeginPlay()
{
	Super::BeginPlay();

	// 1) 격자 기본 벡터 설정 (Z=0으로 평면화)
	BaseAB = TileOffsetCoordB - TileOffsetCoordA;
	BaseAC = TileOffsetCoordC - TileOffsetCoordA;

	//BaseAB.Z = 0.0f;
	//BaseAC.Z = 0.0f;

	//FVector ComputeD = (TileOffsetCoordB + TileOffsetCoordC - TileOffsetCoordA);
	//TileOffsetCoordD = ComputeD;

	// 2) 시작 셀 계산
	if (APawn* P = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		CurrentCenter = WorldToCell(P->GetActorLocation());
	}

	// 3) 초기 로드
	RefreshTiles();

	// 디버그(경계 aliasing 확인)
	const FVector P00 = CellToWorld({ 0,0 });
	const FVector P10 = CellToWorld({ 1,0 });
	const FVector P01 = CellToWorld({ 0,1 });
	const FVector P11 = CellToWorld({ 1,1 });
	const FVector P20 = CellToWorld({ 2,0 });
	const FVector P02 = CellToWorld({ 0,2 });
	UE_LOG(LogTemp, Warning, TEXT("P00=%s P10=%s P01=%s P11=%s P20=%s P02=%s"),
		*P00.ToString(), *P10.ToString(), *P01.ToString(), *P11.ToString(), *P20.ToString(), *P02.ToString());
	UE_LOG(LogTemp, Warning, TEXT("|AB|=%f |AC|=%f |2AB|=%f |2AC|=%f"),
		(P10 - P00).Size(), (P01 - P00).Size(), (P20 - P00).Size(), (P02 - P00).Size());
}

void AP4StageGimmck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* P = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!P) return;

	const FIntPoint NewCenter = WorldToCell(P->GetActorLocation());
	if (NewCenter != CurrentCenter)
	{
		CurrentCenter = NewCenter;
		RefreshTiles();
	}
}

// 월드 → 셀인덱스 : 반드시 월드→로컬로 바꾼 뒤, 로컬 XY 평면에서 계산
FIntPoint AP4StageGimmck::WorldToCell(const FVector& World) const
{
	// 월드 → 로컬
	const FVector Local = GetActorTransform().InverseTransformPosition(World);

	// 로컬 XY에서 선형결합 풀기
	const FVector2D vx(BaseAB.X, BaseAB.Y);
	const FVector2D vy(BaseAC.X, BaseAC.Y);
	const FVector2D p(Local.X, Local.Y);

	const float det = vx.X * vy.Y - vx.Y * vy.X;
	if (FMath::IsNearlyZero(det))
	{
		// 거의 평행할 일은 없겠지만 안전장치
		const float SafeVx = FMath::IsNearlyZero(BaseAB.X) ? 1.f : BaseAB.X;
		const float SafeVy = FMath::IsNearlyZero(BaseAC.Y) ? 1.f : BaseAC.Y;

		int32 i = FMath::FloorToInt(Local.X / SafeVx);
		int32 j = FMath::FloorToInt(Local.Y / SafeVy);
		return { i, j };
	}

	const float invDet = 1.0f / det;
	const float iFloat = (p.X * vy.Y - p.Y * vy.X) * invDet;         // p · vY⟂ / det
	const float jFloat = (-p.X * vx.Y + p.Y * vx.X) * invDet;        // p · vX⟂ / det

	return FIntPoint(FMath::FloorToInt(iFloat), FMath::FloorToInt(jFloat));
}

// 셀인덱스 → 월드 : 로컬에서 좌표 만든 뒤 액터 트랜스폼으로 월드 변환
FVector AP4StageGimmck::CellToWorld(const FIntPoint& Cell) const
{
	// 2x2 슈퍼셀 단위 이동 벡터
	const FVector Ux = BaseAB * 2.f;   // 두 칸
	const FVector Uy = BaseAC * 2.f;

	// 음수 인덱스 안정화용 parity
	auto Parity = [](int32 v) { return (v % 2 + 2) % 2; };
	const int32 px = Parity(Cell.X);
	const int32 py = Parity(Cell.Y);

	// 슈퍼셀 좌표(정수)
	const int32 sx = (Cell.X - px) / 2;
	const int32 sy = (Cell.Y - py) / 2;

	// 슈퍼셀 기준 로컬 위치
	FVector Local = Ux * sx + Uy * sy;

	// 슈퍼셀 내부의 A/B/C/D 오프셋을 한 번만 적용
	switch (GetCellType(Cell))
	{
	case ETileType::A: Local += (TileOffsetCoordA - TileOffsetCoordA); break;
	case ETileType::B: Local += (TileOffsetCoordB - TileOffsetCoordA); break;
	case ETileType::C: Local += (TileOffsetCoordC - TileOffsetCoordA); break;
	case ETileType::D: Local += (TileOffsetCoordD - TileOffsetCoordA); break;
	}

	// 로컬 → 월드
	return GetActorTransform().TransformPosition(Local);
}

void AP4StageGimmck::RefreshTiles()
{
	TSet<FIntPoint> Wanted;

	for (int32 j = -GridRadiusY; j <= GridRadiusY; ++j)
	{
		for (int32 i = -GridRadiusX; i <= GridRadiusX; ++i)
		{
			const FIntPoint Cell = CurrentCenter + FIntPoint(i, j);
			Wanted.Add(Cell);
			SpawnTileAt(Cell);
		}
	}

	TArray<FIntPoint> Keys;
	ActiveTilesMap.GenerateKeyArray(Keys);
	for (const FIntPoint& K : Keys)
	{
		if (!Wanted.Contains(K))
		{
			if (AActor* A = ActiveTilesMap[K].Get())
			{
				A->Destroy();
			}
			ActiveTilesMap.Remove(K);
		}
	}
}

AActor* AP4StageGimmck::SpawnTileAt(const FIntPoint& Cell)
{
	if (TWeakObjectPtr<AActor>* Found = ActiveTilesMap.Find(Cell))
	{
		if (Found->IsValid()) return Found->Get();
	}

	TSubclassOf<AActor> Cls = PickTileClass(Cell);
	if (!Cls) return nullptr;

	const FVector  Loc = CellToWorld(Cell);
	const FRotator Rot = FRotator::ZeroRotator;

	FActorSpawnParameters Params;
	Params.Owner = this;

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(Cls, Loc, Rot, Params);
	if (Spawned)
	{
		ActiveTilesMap.Add(Cell, Spawned);
	}
	return Spawned;
}

TSubclassOf<AActor> AP4StageGimmck::PickTileClass(const FIntPoint& Cell) const
{
	switch (GetCellType(Cell))
	{
	case ETileType::A: return TileA;
	case ETileType::B: return TileB;
	case ETileType::C: return TileC;
	case ETileType::D: return TileD;
	default:           return TileD;
	}
}
