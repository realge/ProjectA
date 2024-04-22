// Shop.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Interactable.h"
#include "Shop.generated.h"


UCLASS()
class REALONE_API AShop : public AActor, public IInteractable
{
    GENERATED_BODY()

public:    
    // Sets default values for this actor's properties
    AShop();

    UPROPERTY(VisibleAnywhere, Category="Collision")
    UBoxComponent* CollisionComponent;
  
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> ShopWidgetClass;  // Set this in the editor to WBP_Shop

    virtual void Interact_Implementation(AActor* Caller) override;

protected:
    virtual void BeginPlay() override;
};

