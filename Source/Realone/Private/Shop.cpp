// Shop.cpp

#include "Shop.h"
#include "Components/BillboardComponent.h"
#include "../Public/UShopWidget.h"
#include "GameFramework/Character.h"

AShop::AShop()
{
    // Initialize components
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitBoxExtent(FVector(50, 50, 50));  // Set size as needed
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AShop::BeginPlay()
{
    Super::BeginPlay();
}

void AShop::Interact_Implementation(AActor* Caller)
{
     ACharacter* PlayerCharacter = Cast<ACharacter>(Caller);
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Interact called by a non-character actor."));
        return;
    }

    APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("Interact called by a character without a valid player controller."));
        return;
    }

    if (!ShopWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ShopWidgetClass is not set on %s"), *GetName());
        return;
    }

    UUserWidget* ShopWidget = CreateWidget<UUserWidget>(PlayerController, ShopWidgetClass);
    if (ShopWidget)
    {
        ShopWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create shop widget."));
    }
}
