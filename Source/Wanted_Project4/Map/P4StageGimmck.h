// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P4StageGimmck.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	A,
	B,
	C,
	D,
};

//일단 해보자면..
UCLASS()
class WANTED_PROJECT4_API AP4StageGimmck : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP4StageGimmck();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//MapTiles Section

public:

private:
	//격자 스폰 로직
	//맵 파츠 A B C D가 원형으로 조립되는데 이걸 기준으로 월드를 위에서 내려다볼때를 기준으로 가상의 격자를 만들어서
	//이걸 기반으로 맵을 실시간으로 추가 및 삭제로 무한맵 로드및 관리.
	FIntPoint WorldToCell(const FVector& world) const; //월드 -> 셀인덱스.
	FVector CellToWorld(const FIntPoint& cell) const; //셀 인덱스 -> 월드위치.
	void RefreshTiles(); //맵 유지범위 갱신.(이걸 사용하면 현재 플레이어 위치기반으로 맵 로드 및 삭제를 처리하는 위치가 최신화됩니당.


	AActor* SpawnTileAt(const FIntPoint& cell); //맵 파츠 스폰.

	TSubclassOf<AActor> PickTileClass(const FIntPoint& cell) const;

	// A B C D 판정
	static FORCEINLINE ETileType GetCellType(const FIntPoint& cell) //입력된 인자를 기반으로 현재 타일타입 출력.
	{
		//홀수 짝수 체크 만약에 true면 홀수 false면 짝수.
		const bool CheckOddX = (cell.X & 1);
		const bool CheckOddY = (cell.Y & 1);

		if (CheckOddX == false && CheckOddY == false)
		{
			return ETileType::A;
		}

		if (CheckOddX == true && CheckOddY == false)
		{
			return ETileType::B;
		}

		if (CheckOddX == false && CheckOddY == true)
		{
			return ETileType::C;
		}

		return ETileType::D;
	}
public:
	//맵 타일의 상대좌표(타일 A가 기준점).
	UPROPERTY(EditAnywhere, Category = Grid)
	FVector TileOffsetCoordA = FVector::ZeroVector; //타일 A 좌표

	UPROPERTY(EditAnywhere, Category = Grid)
	FVector TileOffsetCoordB = FVector(3750.0f, 9950.0f, 1000.0f); //타일 B 좌표

	UPROPERTY(EditAnywhere, Category = Grid)
	FVector TileOffsetCoordC = FVector(11962.0f, -5474.0f, 100.0f); //타일 C 좌표

	UPROPERTY(EditAnywhere, Category = Grid)
	FVector TileOffsetCoordD = FVector::ZeroVector; //타일 D좌표 => 자동으로 계산.

	//UPROPERTY(EditAnywhere, Category = Grid)
	//FVector TileOffsetCoordD; //타일 D 좌표

	UPROPERTY(EditAnywhere, Category = Grid)
	int32 GridRadiusX = 1;

	UPROPERTY(EditAnywhere, Category = Grid)
	int32 GridRadiusY = 1;



	UPROPERTY(EditAnywhere, Category = Tiles)
	TSubclassOf<AActor> TileA;
	UPROPERTY(EditAnywhere, Category = Tiles)
	TSubclassOf<AActor> TileB;
	UPROPERTY(EditAnywhere, Category = Tiles)
	TSubclassOf<AActor> TileC;
	UPROPERTY(EditAnywhere, Category = Tiles)
	TSubclassOf<AActor> TileD;



private:
	//활성중인 타일 해시로 저장.
	TMap<FIntPoint, TWeakObjectPtr<AActor>> ActiveTilesMap;

	FIntPoint CurrentCenter = FIntPoint::ZeroValue; // 0,0 기본 시작지점 0,0인 A에 설정.

	FVector Vx;
	FVector Vy;

	//파츠 D 보정값.
	//FVector DCorrection = FVector::ZeroVector;
	FVector BaseAB, BaseAC; // A->B, A->C (Z는 0으로 맞춘 값)
};
