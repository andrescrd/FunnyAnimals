
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FTypes.generated.h"

USTRUCT(BlueprintType)
struct FCharacterColor : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor ColorBase;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor ColorLight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor ColorDark;
};

USTRUCT(BlueprintType)
struct FLevelConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MapName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int MaxTime;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int MaxItemToSpawn;

    FLevelConfig()
    {
        MapName = TEXT("NONE");
        MaxTime = 10;
        MaxItemToSpawn = 10;
    }
};


// USTRUCT(BlueprintType)
// struct FPlayerInfo
// {
//     GENERATED_USTRUCT_BODY() 
// };

UENUM(BlueprintType)
enum class EGameState : uint8
{
    PREPARING UMETA(DisplayName = "Preparing"),
    PLAYING UMETA(DisplayName = "Playing"),
    FINISH UMETA(DisplayName = "Finish"),
    GAME_OVER UMETA(DisplayName = "GameOver"),
    UNKNOW UMETA(DisplayName = "Unknow")
};

