// MoneyItem.cpp

#include "MoneyItem.h"
#include "MyGameState.h"

AMoneyItem::AMoneyItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize MeshComponent
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Initialize CollisionComponent
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(100.0f);
    CollisionComponent->SetupAttachment(RootComponent);

    // Set up the overlap function
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMoneyItem::OnOverlapBegin);
}

void AMoneyItem::BeginPlay()
{
    Super::BeginPlay();
}

void AMoneyItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMoneyItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherActor && (OtherActor != this))
    {
        AMyGameState* GameState = GetWorld()->GetGameState<AMyGameState>();
        if (GameState)
        {
            GameState->AddCurrency(20); // Add 20 to global currency
			 FString Message = FString::Printf(TEXT("Picked up Money Item! New Total: %d"), GameState->GlobalCurrency);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
            Destroy(); // Destroy the item
        }
    }
}
