// MoneyItem.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "MoneyItem.generated.h"

UCLASS()
class REALONE_API AMoneyItem : public AActor
{
    GENERATED_BODY()

public:    
    // Sets default values for this actor's properties
    AMoneyItem();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
