// MyGameState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

UCLASS()
class REALONE_API AMyGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AMyGameState();

    // Currency accessible throughout the game
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Currency")
    int32 GlobalCurrency;

    void AddCurrency(int32 Amount);
};
