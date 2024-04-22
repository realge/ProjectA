// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Containers/Queue.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/RandomStream.h"  // Correct include statement



TArray<FVector> ADungeonGenerator::GetNeighbors(const FVector& NodePosition, const FVector& StartPos, const FVector& TargetPos, bool IsStairCase, FVector StairDirection)
{



     TArray<FVector> Neighbors;
    TArray<FVector> Directions = {
        FVector(1, 0, 0), FVector(-1, 0, 0),  // East, West
        FVector(0, 1, 0), FVector(0, -1, 0), // North, South
        // Up, Down
    };
        
    

    for (const FVector& Dir : Directions)
    {
        FVector NewPos = NodePosition + Dir;  
        if (NewPos.X >= 0 && NewPos.X < Width  && NewPos.Y >= 0 && NewPos.Y < Height  && NewPos.Z >= 0 && NewPos.Z < Length )
        {

            if(IsStairCase)
            {
                if(Dir.Y+StairDirection.Y==3||Dir.X+StairDirection.X==3||Dir.Y+StairDirection.Y==-3||Dir.X+StairDirection.X==-3)
                {
                
                }
                

                else
                continue;
            }

           

            if (IsWalkable(NewPos,StartPos,TargetPos))
            {
                Neighbors.Add(NewPos);
            }
        }
    }

   

    return Neighbors;
}

bool ADungeonGenerator::checkpath(TArray<FAStarNode*> path)
{

    return false;
}

TArray<FVector> ADungeonGenerator::GetStairNeighbors(const FVector& NodePosition, const FVector& StartPos, const FVector& TargetPos, bool IsStairCase,FVector Direction,bool IsStairCorridor,FAStarNode* node )
{
    

    TArray<FVector> StaircaseDirections = {
    FVector(2, 0, 1), FVector(-2, 0, 1), // Moving East/West, Ascending
    FVector(2, 0, -1), FVector(-2, 0, -1), // Moving East/West, Descending
    FVector(0, 2, 1), FVector(0, -2, 1), // Moving North/South, Ascending
    FVector(0, 2, -1), FVector(0, -2, -1) // Moving North/South, Descending
    };

     TArray<FVector> Neighbors;
   
   if(IsStairCorridor)
        {
            return Neighbors;
        }

    if(!IsInRoom(NodePosition,GetRoomFromPosition(StartPos))&&GetIndex(NodePosition.X,NodePosition.Y,NodePosition.Z))
    for (const FVector& Dir : StaircaseDirections)
    {
        FVector NewPos = NodePosition + Dir;

        if(IsStairCase)
        {
            if(Direction.Y+Dir.Y==4||Direction.X+Dir.X==4||Direction.Y+Dir.Y==-4||Direction.X+Dir.X==-4)
            {
               
            }
            
           else 
           continue;
           
        }

        
            FVector nodeDirection=node->CameFrom->Position;
           
            FVector result= nodeDirection+FVector(Dir.X/2,Dir.Y/2,Dir.Z);

            FVector check= node->Position+Dir;
          
            if(result.Equals(check, 1e-4f))
            {
               
                continue;
            }


        if(Direction+Dir==FVector(0,0,-2))
        {
            continue;
        }

        if (NewPos.X >= 0 && NewPos.X < Width && NewPos.Y >= 0 && NewPos.Y < Height && NewPos.Z >= 0 && NewPos.Z < Length)
        {
            if (IsStaircaseWalkable(NodePosition, Dir))
            {
                Neighbors.Add(NewPos);
            }
        }
    }

    return Neighbors;

}


bool ADungeonGenerator::IsStaircaseWalkable(const FVector& StartPos, const FVector& Direction)
{
   

    TArray<FVector> StaircaseCells;
   
   
    StaircaseCells.Add(StartPos + FVector(Direction.X / 2, Direction.Y / 2, 0));
     StaircaseCells.Add(StartPos + FVector(Direction.X , Direction.Y , 0));
    StaircaseCells.Add(StartPos + FVector(Direction.X / 2, Direction.Y / 2, Direction.Z ));
    StaircaseCells.Add(StartPos + Direction);

    for (const FVector& Point : StaircaseCells) {

                int32 X = Point.X ;
            int32 Y = Point.Y;
            int32 Z = Point.Z ;
            int32 Index = GetIndex(X, Y, Z);

        
            if (!Grid.IsValidIndex(Index))
            {
            
                return false;  // Out of bounds
            }
            if (Grid[Index] == 1||Grid[Index]==6||Grid[Index]==2)  // Assuming '0' means walkable/open space
            {
                
                return false;
            }
            
            
                
            }

    return true;
}

FVector ADungeonGenerator::RoomCenter(const FRoom& Room)
{
  

    return FVector(
        (Room.StartX + Room.Width / 2) ,
        (Room.StartY + Room.Height / 2) ,
        (Room.StartZ)
    );
}

bool ADungeonGenerator::IsWalkable(const FVector& Position, const FVector& StartPos, const FVector& TargetPos)
{
    int32 X = Position.X ;
    int32 Y = Position.Y;
    int32 Z = Position.Z ;
    int32 Index = GetIndex(X, Y, Z);

   
    if (!Grid.IsValidIndex(Index))
    {
       
        return false;  // Out of bounds
    }
    if (Grid[Index] == 0||Grid[Index]==2)  // Assuming '0' means walkable/open space
    {
        
        return true;
    }
    // Check if the position is within the start or target room
    bool isInStartRoom = IsInRoom(Position, GetRoomFromPosition(StartPos));
    bool isInTargetRoom = IsInRoom(Position, GetRoomFromPosition(TargetPos));

    

    return (isInStartRoom || isInTargetRoom);
}

FRoom ADungeonGenerator::GetRoomFromPosition(const FVector& Position)
{
    for (const FRoom& Room : Rooms)
    {
        if (IsInRoom(Position, Room))
            return Room;
    }
    return FRoom();  // Return an empty room if no room is found
}

bool ADungeonGenerator::IsInRoom(const FVector& Position, const FRoom& Room)
{
    int32 X = Position.X ;  // Convert from world coordinates to grid indices
    int32 Y = Position.Y ;
    int32 Z = Position.Z ;

    return X >= Room.StartX && X < Room.StartX + Room.Width &&
           Y >= Room.StartY && Y < Room.StartY + Room.Height &&
           Z == Room.StartZ;
}

bool checkpath( TArray<FAStarNode*> Path)
{
    TArray<FVector> staircasecells;
    for(FAStarNode* node:Path)
    {
        if(node->Istair)
        {
            FVector Direction= node->StairDirection;
            staircasecells.Add(node->Position);
        }
    }
    return false;
}

TArray<FAStarNode*> ADungeonGenerator::FindPath(const FVector& StartPos, const FVector& TargetPos)
{
    TArray<FAStarNode*> Path;
    TArray<FAStarNode*> OpenSet;
    TMap<FVector, FAStarNode*> AllNodes;  // No need for custom key comparators

    FAStarNode* StartNode = new FAStarNode(StartPos, 0, FVector::Dist(StartPos, TargetPos));
	
    OpenSet.Add(StartNode);
    AllNodes.Add(StartPos, StartNode);



    while (OpenSet.Num() > 0)
    {
        // Sort OpenSet based on FCost, then HCost for ties
        OpenSet.Sort([](const FAStarNode& A, const FAStarNode& B) {
            return A.FCost() == B.FCost() ? A.HCost < B.HCost : A.FCost() < B.FCost();
        });

        FAStarNode* CurrentNode = OpenSet[0];
        OpenSet.RemoveAt(0);  // Pop the first element (the node with the lowest F cost)
		
        
        
        if (CurrentNode->Position.Equals(TargetPos, 0.0f)&&CurrentNode->Istair==false)  // Check proximity within a small threshold
        {

        
            while (CurrentNode != nullptr)
            {
                
                Path.Add(CurrentNode);
                
                CurrentNode = CurrentNode->CameFrom;
            }
            Algo::Reverse(Path);
            break;
        }

        TArray<FVector> Neighbors = GetNeighbors(CurrentNode->Position,StartPos, TargetPos,CurrentNode->Istair,CurrentNode->StairDirection);

        TArray<FVector> StairNeighbors= GetStairNeighbors(CurrentNode->Position,StartPos, TargetPos,CurrentNode->Istair,CurrentNode->StairDirection,CurrentNode->IsStaircorridor,CurrentNode);

        for (const FVector& Neighbor : Neighbors)
        {
			
			
            float TentativeGCost = CurrentNode->GCost + (CurrentNode->Position - Neighbor).Size();  // Distance as cost
            FAStarNode* NeighborNode = AllNodes.FindRef(Neighbor);
			
            if (!NeighborNode)
            {
                NeighborNode = new FAStarNode(Neighbor, TentativeGCost, FVector::Dist(Neighbor, TargetPos), CurrentNode);
               
                
              if(CurrentNode->Istair)
                {
                    NeighborNode->IsStaircorridor=true;
                    NeighborNode->StairDirection=Neighbor-CurrentNode->Position;
                }
                
                OpenSet.Add(NeighborNode);
                AllNodes.Add(Neighbor, NeighborNode);
            }
            else if (TentativeGCost < NeighborNode->GCost)
            {
                NeighborNode->CameFrom = CurrentNode;
                NeighborNode->GCost = TentativeGCost;
            }
        }

        for (const FVector& Neighbor : StairNeighbors)
        {
			
			
            float TentativeGCost = CurrentNode->GCost + (CurrentNode->Position - Neighbor).Size();  // Distance as cost
            FAStarNode* NeighborNode = AllNodes.FindRef(Neighbor);
			
            if (!NeighborNode)
            {
                NeighborNode = new FAStarNode(Neighbor, TentativeGCost, FVector::Dist(Neighbor, TargetPos), CurrentNode);
                NeighborNode->Istair=true;
                NeighborNode->StairDirection=Neighbor-CurrentNode->Position;
                
                OpenSet.Add(NeighborNode);
                AllNodes.Add(Neighbor, NeighborNode);
            }
            else if (TentativeGCost < NeighborNode->GCost)
            {
                NeighborNode->CameFrom = CurrentNode;
                NeighborNode->GCost = TentativeGCost;
            }
        }


        
    }

    // Clean up
    /*for (const auto& NodePair : AllNodes)
    {
        if (!Path.Contains(NodePair.Value))
        {
            delete NodePair.Value;  // Delete nodes not in the final path to prevent memory leaks
        }
    }*/

    UE_LOG(LogTemp, Warning, TEXT("Path Length %d"), Path.Num());
    return Path;
}

int32 ADungeonGenerator::GetCorridorType(const FVector& Direction)
{
    return 2;
   
}
FVector ADungeonGenerator::GetStaircaseDirectionFromIndex(const FVector& Location)
{
    // Example function to retrieve direction vector based on cell index, needs actual implementation based on data structure
    for (const FStair& Stair : Stairs)  // Assuming AllStairs is TArray<FStair>
    {
        if (Stair.StairCells.Contains(Location))  // Convert index back to FVector or adjust logic based on actual usage
        {
            return Stair.Direction;  // Scale direction for visualization purposes
        }
    }
    return FVector(0,0,0);  // Default to no direction if not found
}

int32 ADungeonGenerator::GetStairIndex(const FVector& Location)
{
    // Example function to retrieve direction vector based on cell index, needs actual implementation based on data structure
    for(int i=0;i<Stairs.Num();i++)  
    {
        if (Stairs[i].StairCells.Contains(Location))  // Convert index back to FVector or adjust logic based on actual usage
        {
            return i;  // Scale direction for visualization purposes
        }
    }
    return 0;  // Default to no direction if not found
}

void ADungeonGenerator::PlaceStaircase(const FVector& StartPosition, const FVector& Direction)
{
    TArray<FVector> StaircaseCells;
   
  

    StaircaseCells.Add(StartPosition + FVector(Direction.X / 2, Direction.Y / 2, 0));
     StaircaseCells.Add(StartPosition + FVector(Direction.X , Direction.Y , 0));
    StaircaseCells.Add(StartPosition + FVector(Direction.X / 2, Direction.Y / 2, Direction.Z ));
    StaircaseCells.Add(StartPosition + Direction);

    FStair newstair;

    newstair.Direction = Direction;
    for (int i = 0; i < StaircaseCells.Num(); i++)
    {
        
        
        int32 X = StaircaseCells[i].X;
        int32 Y = StaircaseCells[i].Y;
        int32 Z = StaircaseCells[i].Z;
        int32 Index = GetIndex(X, Y, Z);
       
        Grid[Index] = 6;  // 6, 7, 8, 9 represent parts of the staircase
        
        newstair.AddStairCell(X, Y, Z);

        
    }

    Stairs.Add(newstair);
  

    
}

void ADungeonGenerator::PlaceCorridor(const FVector& Position, int32 Type)
{
    int32 X = Position.X ;
    int32 Y = Position.Y ;
    int32 Z = Position.Z ;
    int32 Index = GetIndex(X, Y, Z);
    if (Grid[Index] != 6&&Grid[Index]!=1)
        Grid[Index] = Type;
}




void ADungeonGenerator::ConnectRoomsUsingAStar(const TArray<FRoomConnection>& MST)
{
    for (const FRoomConnection& Connection : MST)
    {
        const FRoom& RoomA = Rooms[Connection.RoomIndexA];
        const FRoom& RoomB = Rooms[Connection.RoomIndexB];


        
          FVector StartPos = RoomCenter(RoomA);
        FVector TargetPos = RoomCenter(RoomB);

   
        TArray<FAStarNode*> Path = FindPath(StartPos, TargetPos);
         UE_LOG(LogTemp, Warning, TEXT("Path Generated between %d and %d"), Connection.RoomIndexA, Connection.RoomIndexB);
         
          
        FAStarNode* LastNode = nullptr;
        if (Path.Num() > 0)
        {
             
            for (FAStarNode* Node : Path)
            {
                if (LastNode != nullptr)
                {
                    FVector Direction = Node->Position - LastNode->Position;
                    int32 CorridorType = GetCorridorType(Direction);

                    UE_LOG(LogTemp, Warning, TEXT("path location %s"), *Node->Position.ToString());
                    
                    if(Node->Position.Z-LastNode->Position.Z>=1||Node->Position.Z-LastNode->Position.Z<=-1)
                    {
                       
                       PlaceStaircase(LastNode->Position, Direction);
                        PlaceCorridor(LastNode->Position, CorridorType);
                    }
                    else 
                     PlaceCorridor(LastNode->Position, CorridorType);
                    
                }
                LastNode = Node;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No path found between rooms %d and %d"), Connection.RoomIndexA, Connection.RoomIndexB);
        }
        UE_LOG(LogTemp, Warning, TEXT("Done"));
    }
}

void ADungeonGenerator::PlaceDoors()
{
    for (int32 i = 0; i < Rooms.Num(); i++)
    {
        const FRoom& room = Rooms[i];
        // Check potential door positions around the room perimeter
        for (int32 x = room.StartX; x < room.StartX + room.Width; x++) {
            // Top edge of the room
            if (room.StartY > 0 && Grid[(room.StartY - 1) * Width + x] == 2) {
                Grid[(room.StartY - 1) * Width + x] = 3;  // 3 for door
            }
            // Bottom edge of the room
            if (room.StartY + room.Height < Height && Grid[(room.StartY + room.Height) * Width + x] == 2) {
                Grid[(room.StartY + room.Height) * Width + x] = 3;  // 3 for door
            }
        }
        for (int32 y = room.StartY; y < room.StartY + room.Height; y++) {
            // Left edge of the room
            if (room.StartX > 0 && Grid[y * Width + (room.StartX - 1)] == 2) {
                Grid[y * Width + (room.StartX - 1)] = 3;  // 3 for door
            }
            // Right edge of the room
            if (room.StartX + room.Width < Width && Grid[y * Width + (room.StartX + room.Width)] == 2) {
                Grid[y * Width + (room.StartX + room.Width)] = 3;  // 3 for door
            }
        }
    }
}


int32 ADungeonGenerator::Find(int32 i, TArray<int32>& Parent)
{
    while (Parent[i] != i)
    {
        Parent[i] = Parent[Parent[i]];  // Path compression
        i = Parent[i];
    }
    return i;
}

void ADungeonGenerator::Union(int32 a, int32 b, TArray<int32>& Parent, TArray<int32>& Rank)
{
    int32 rootA = Find(a, Parent);
    int32 rootB = Find(b, Parent);
    if (rootA != rootB)
    {
        if (Rank[rootA] < Rank[rootB])
        {
            Parent[rootA] = rootB;
        }
        else if (Rank[rootA] > Rank[rootB])
        {
            Parent[rootB] = rootA;
        }
        else
        {
            Parent[rootB] = rootA;
            Rank[rootA]++;
        }
    }
}

TArray<FRoomConnection> ADungeonGenerator::KruskalsMST()
{
    TArray<FRoomConnection> AllConnections;
    GenerateAllRoomConnections(AllConnections);

    TArray<int32> Parent;
    TArray<int32> Rank;
    TArray<FRoomConnection> MST;

    for (int32 i = 0; i < Rooms.Num(); i++)
    {
        Parent.Add(i);
        Rank.Add(0);
    }

    for (const FRoomConnection& Conn : AllConnections)
    {
        if (Find(Conn.RoomIndexA, Parent) != Find(Conn.RoomIndexB, Parent))
        {
            MST.Add(Conn);
            Union(Conn.RoomIndexA, Conn.RoomIndexB, Parent, Rank);
            if (MST.Num() == Rooms.Num() - 1) break;
        }
    }

    if (MST.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No corridors to connect: MST is empty"));
    }
	
    return MST;
}

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    static ConstructorHelpers::FClassFinder<AActor> WallBPClass(TEXT("/Game/PathToBP_Wall.BP_Wall_C"));
    if (WallBPClass.Class != NULL)
    {
        WallClass = WallBPClass.Class;
    }

}
void ADungeonGenerator::GenerateAllRoomConnections(TArray<FRoomConnection>& OutConnections)
{
    OutConnections.Empty();
    for (int32 i = 0; i < Rooms.Num(); i++)
    {
        for (int32 j = i + 1; j < Rooms.Num(); j++)
        {
            const FRoom& RoomA = Rooms[i];
            const FRoom& RoomB = Rooms[j];
            FVector CenterA(RoomA.StartX + RoomA.Width / 2.0f, RoomA.StartY + RoomA.Height / 2.0f, RoomA.StartZ + RoomA.Length / 2.0f);
            FVector CenterB(RoomB.StartX + RoomB.Width / 2.0f, RoomB.StartY + RoomB.Height / 2.0f, RoomA.StartZ + RoomA.Length / 2.0f);
            float Distance = FVector::Dist(CenterA, CenterB);
            OutConnections.Add(FRoomConnection(i, j, Distance));
        }
    }

    // Sort connections by distance
    OutConnections.Sort();
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	 GenerateDungeon();
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 static int FrameCounter = 0;
    FrameCounter++;
    if (FrameCounter >= 400)
    {
        //DrawDebugGrid();
		
        FrameCounter = 0;  // reset counter after updating
    }
	
}

void ADungeonGenerator::PlacePlayerStart()
{
    if (Rooms.Num() > 0)  // Check if there are any rooms defined
    {
        FRoom& FirstRoom = Rooms[0];  // Reference to the first room
        FVector RoomCenter = FirstRoom.GetCenter();  // Get the center point of the first room
        FVector WorldCenter = GetWorldLocation(RoomCenter);  // Convert grid coordinates to world coordinates

        // Assuming PlayerStart is an AActor class that you can place in the world
        APlayerStart* PlayerStartActor = GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), WorldCenter, FRotator(0.f, 0.f, 0.f));
       APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
       
        if (!PlayerStartActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to place PlayerStart at Room 0 center"));
        }
        
         if (PC)
            {
                PC->GetPawn()->SetActorLocation(WorldCenter);
            }
    }
}

FVector ADungeonGenerator::GetWorldLocation(const FVector& GridLocation)
{
    // Assuming each grid cell is 100 units wide
    return GetActorLocation() + GridLocation * CellSize;
}

void ADungeonGenerator::GenerateDungeon()
{

    UE_LOG(LogTemp, Warning, TEXT("Generating Dungeon..."));
  	 InitializeGrid();  // Set up the grid with default values
    PlaceMultipleRooms(NumofRoom);  // Place 10 rooms randomly

    TArray<FRoomConnection> MST = KruskalsMST();  // Generate the MST to find optimal room connections
    ConnectRoomsUsingAStar(MST);  // Connect rooms using corridors defined by A*

    //SpawnDungeonEnvironment();  // Spawn the physical dungeon based on the grid
    //SpawnRoomWalls();
    DrawDebugGrid();
    //PlacePlayerStart();
}

void ADungeonGenerator::SpawnFloorTile(const FVector& Location)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    FVector AdjustedLocation = Location - FVector(CellSize/2, CellSize/2, CellSize/2);
    AActor* FloorActor = GetWorld()->SpawnActor<AActor>(FloorTileClass, AdjustedLocation, FRotator::ZeroRotator, SpawnParams);
    if (!FloorActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn floor tile at Location: %s"), *Location.ToString());
    }

     if (FMath::RandRange(1, 100) <= 10)
    {
        AActor* MoneyActor = GetWorld()->SpawnActor<AActor>(MoneyItemClass, AdjustedLocation, FRotator::ZeroRotator, SpawnParams);
        if (!MoneyActor)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn money item at Location: %s"), *Location.ToString());
        }
    }
}

void ADungeonGenerator::SpawnDungeonEnvironment()
{

    float Elevation = 100.0f;
    for (int32 z = 0; z < Length; z++)
    for (int32 y = 0; y < Height; y++)
    {
        for (int32 x = 0; x < Width; x++)
        {
            
               int32 Index = GetIndex(x, y, z);
                FVector CellLocation = GetActorLocation() + FVector(x * CellSize, y * CellSize, z * CellSize);
                
                if (Grid[Index] == 1)  // Room
                {
                    SpawnFloorTile(CellLocation);
                }
                else if (Grid[Index] == 2)  // Corridors
                {
                    //SpawnCorridorTile(CellLocation, Grid[Index]);
                    SpawnCorridorWalls(x, y, z, Grid[Index]);
                }
                else  // Walls/empty space
                {
                    //SpawnWallTile(CellLocation);
            
                 }
        }
    }

    SpawnStairs();
}



void ADungeonGenerator::SpawnWallTile(const FVector& Location, const FRotator& Rotation)
{
    // Assuming WallClass is a UClass* that points to BP_Wall
    if (!WallClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Wall class not set."));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    // Adjust Location to be centered based on your grid cell size; assume CellSize is height of the wall
    FVector AdjustedLocation = Location + FVector(CellSize/2, CellSize/2, -CellSize/2);

    // Spawn the wall
    AActor* WallActor = GetWorld()->SpawnActor<AActor>(WallClass, AdjustedLocation, Rotation, SpawnParams);
    if (!WallActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn wall at Location: %s"), *Location.ToString());
    }
}


void ADungeonGenerator::SpawnStairs()
{

    FVector BaseLocation = GetActorLocation();
    for(const FStair& Stair:Stairs)
    {
        
            // Assume the first cell is the base of the staircase
            UE_LOG(LogTemp, Warning, TEXT("Staircase at Location: %s"), *Stair.StairCells[0].ToString());
            FVector CellLocation = BaseLocation + FVector(Stair.StairCells[0].X * CellSize, Stair.StairCells[0].Y * CellSize, Stair.StairCells[0].Z*CellSize);
              FVector Direction = Stair.Direction;

            
            
            if(Direction.Z==1)
            {
                   //Normalize direction to ignore the Z component for the rotator
                int temp=Direction.Z;
                Direction.Z = 0;
                Direction.Normalize();

                FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

                CellLocation = CellLocation - FVector(Direction.X*CellSize/2, Direction.Y*CellSize/2,temp*CellSize/2 );  // Adjust the location to the top of the staircase
                // Spawn the staircase blueprint at the base location with the calculated rotation

                AActor* SpawnedStair = GetWorld()->SpawnActor<AActor>(StairBlueprint, CellLocation, Rotation);

                if (!SpawnedStair)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to spawn staircase at Location: %s"), *BaseLocation.ToString());
                }
            }
            else
            {
                //Normalize direction to ignore the Z component for the rotator
                int temp=Direction.Z;
                Direction.Z = 0;
                Direction.Normalize();

                FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

                CellLocation = CellLocation + FVector(Direction.X*CellSize, Direction.Y*CellSize,temp*CellSize );  // Adjust the location to the top of the staircase
                // Spawn the staircase blueprint at the base location with the calculated rotation
                 CellLocation = CellLocation + FVector(Direction.X*CellSize/2, Direction.Y*CellSize/2,temp*CellSize/2 ); 
                AActor* SpawnedStair = GetWorld()->SpawnActor<AActor>(StairBlueprint2, CellLocation, Rotation);

                if (!SpawnedStair)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to spawn staircase at Location: %s"), *BaseLocation.ToString());
                }
            }

           
            
          
        
    }
}

void ADungeonGenerator::SpawnCorridorWalls(int x, int y, int z, int32 CorridorType)
{
    FVector BaseLocation = GetActorLocation();
    FVector CellLocation = BaseLocation + FVector(x * CellSize, y * CellSize, z*CellSize);
    bool shouldSpawnEastWall = true, shouldSpawnWestWall = true, shouldSpawnNorthWall = true, shouldSpawnSouthWall = true;


    FRotator r1(0.0f, -180.0f, 0.0f);  // Vertical walls, faces East/West
    FRotator r2(0.0f, -90.0f, 0.0f);  
     FRotator r3(0.0f, -180.0f, 0.0f);  
     FRotator r4(0.0f, -90.0f, 0.0f);  
    // Check the grid boundaries and neighboring cells
     TArray<FVector> Directions = {
        FVector(1, 0, 0), FVector(-1, 0, 0),   // East, West
        FVector(0, 1, 0), FVector(0, -1, 0),  // North, South
    };




    if (x > 0) // Check West
    {
        int32 WestIndex = GetIndex(x - 1, y, z);
        shouldSpawnWestWall = (Grid[WestIndex] == 0 ); // Empty or door
    }
    if (x < Width - 1) // Check East
    {
        int32 EastIndex = GetIndex(x + 1, y, z);
       
        shouldSpawnEastWall = (Grid[EastIndex] == 0 ); // Empty or door
    }
    if (y > 0) // Check North
    {
        int32 NorthIndex = GetIndex(x, y - 1, z);
        shouldSpawnNorthWall = (Grid[NorthIndex] == 0 ); // Empty or door
    }
    if (y < Height - 1) // Check South
    {
        int32 SouthIndex = GetIndex(x, y + 1, z);
        shouldSpawnSouthWall = (Grid[SouthIndex] == 0 ); // Empty or door
    }

    // Spawn walls where needed
    if (shouldSpawnEastWall)//done
    {
        FVector WallLocation = CellLocation;
        SpawnWallTile(WallLocation,r4);
    }
    if (shouldSpawnWestWall)//done
    {
        FVector WallLocation = CellLocation + FVector(-CellSize, 0, 0);
        SpawnWallTile(WallLocation,r2);
    }
    if (shouldSpawnNorthWall)//done
    {
        FVector WallLocation = CellLocation + FVector(0, -CellSize, 0);
        SpawnWallTile(WallLocation,r3);
    }
    if (shouldSpawnSouthWall)
    {
        FVector WallLocation = CellLocation;
        SpawnWallTile(WallLocation,r1);
    }

    SpawnFloorTile(CellLocation);
}


void ADungeonGenerator::InitializeGrid()
{
    Grid.SetNum(Width * Height*Length);
    for (int32& Cell : Grid)
    {
        Cell = 0; // Initialize all grid cells to 0
    }
}

void ADungeonGenerator::PlaceMeshes()
{
    FVector Origin = GetActorLocation();
    float TileSize = 100.0f;  // Assuming each tile is 100x100 units

    for (int32 Y = 0; Y < Height; Y++)
    {
        for (int32 X = 0; X < Width; X++)
        {
            int32 Index = Y * Width + X;
            FVector Location = Origin + FVector(X * TileSize, Y * TileSize, 0);
            FRotator Rotation = FRotator(0, 0, 0);
            FActorSpawnParameters SpawnParams;

          if (Grid[Index] == 1 && RoomMesh)  // Check if the grid cell is a room
				{
					AStaticMeshActor* RoomActor = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation, SpawnParams);
					if (RoomActor)
					{
						RoomActor->GetStaticMeshComponent()->SetStaticMesh(RoomMesh);
					}
				}
				else if (Grid[Index] == 2 && CorridorMesh)  // Check if the grid cell is a corridor
				{
					AStaticMeshActor* CorridorActor = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation, SpawnParams);
					if (CorridorActor)
					{
						CorridorActor->GetStaticMeshComponent()->SetStaticMesh(CorridorMesh);
					}
				}
        }
    }
}

void ADungeonGenerator::SpawnRoomWalls()
{
    for (const FRoom& Room : Rooms)
    {
        // Calculate bounds for easier looping
        int32 MinX = Room.StartX;
        int32 MaxX = Room.StartX + Room.Width;
        int32 MinY = Room.StartY;
        int32 MaxY = Room.StartY + Room.Height;
        int32 MinZ = Room.StartZ;
        int32 MaxZ = Room.StartZ + Room.Length;

        // Loop through the perimeter of the room
        for (int32 z = MinZ; z < MaxZ; z++)
        {
            for (int32 y = MinY; y <= MaxY; y++)
            {
                for (int32 x = MinX; x <= MaxX; x++)
                {
                    if (x == MinX || x == MaxX || y == MinY || y == MaxY)
                    {
                        FVector WallLocation(x, y, z);
                        SpawnWallAt(WallLocation, true);  
                        // Don't spawn floor/ceiling walls unless it's the first/last layer

                        UE_LOG(LogTemp, Warning, TEXT("Wall at Location: %s"), *WallLocation.ToString());
                    }
                }
            }
        }
    }
}

void ADungeonGenerator::SpawnWallAt(const FVector& Location, bool bSpawnVerticalWalls)
{
    if (bSpawnVerticalWalls)
    {
        // Check if there's no room cell at this location (empty or corridor)
        int32 Index = GetIndex(Location.X, Location.Y, Location.Z);
        
            FVector WorldLocation = GetWorldLocation(Location);
            // Assuming WallBlueprint is a UProperty that points to the wall's blueprint class
            AActor* WallActor = GetWorld()->SpawnActor<AActor>(WallClass, WorldLocation, FRotator(0.f));
            if (!WallActor)
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to spawn wall at: %s"), *WorldLocation.ToString());
            }
        
    }
}

void ADungeonGenerator::DrawDebugGrid()
{
    FVector BaseLocation = GetActorLocation(); // Base location of the dungeon generator actor // Size of each cell in units
    float Elevation = CellSize;  // Height of one floor above another

    for (int32 z = 0; z < Length; z++)
    {
        for (int32 y = 0; y < Height; y++)
        {
            for (int32 x = 0; x < Width; x++)
            {
                int32 Index = GetIndex(x, y, z);
                FVector CellLocation = BaseLocation + FVector(x * CellSize, y * CellSize, z * Elevation);
                FString IndexString = FString::Printf(TEXT("%d"), Index);
                FString In= FString::Printf(TEXT("%d"), GetStairIndex(FVector(x,y,z)));  

                if (Grid[Index] == 1)  // Room
                {
                    DrawDebugBox(GetWorld(), CellLocation, FVector(CellSize/2, CellSize/2, Elevation/2), FColor::Turquoise, true, -1.0f, 0, 5);
                }
                else if (Grid[Index] >= 2 && Grid[Index] <= 5)  // Corridor
                {
                    DrawDebugBox(GetWorld(), CellLocation, FVector(CellSize/2, CellSize/2, Elevation/2), FColor::Yellow, true, -1.0f, 0, 5);
                    DrawDebugString(GetWorld(), CellLocation + FVector(0, 0, Elevation/2 + 10), IndexString, nullptr, FColor::White, -1.0f, true);
                }
                else if (Grid[Index] == 6)  // stairs
                {
                     FVector Direction = GetStaircaseDirectionFromIndex(FVector(x,y,z)); // Assume a helper function to get direction
                        FVector ArrowHeadLocation = CellLocation + Direction*50.0f;  // Calculate where the arrow should point
                          DrawDebugString(GetWorld(), CellLocation + FVector(0, 0, Elevation/2 + 10), In, nullptr, FColor::White, -1.0f, true);
                        DrawDebugDirectionalArrow(GetWorld(), CellLocation + FVector(0, 0, Elevation/2), ArrowHeadLocation, -1.0f, FColor::Red, true, -1.0f, 0, 5);

                     DrawDebugBox(GetWorld(), CellLocation, FVector(CellSize/2, CellSize/2, Elevation/2), FColor::Blue, true, -1, 0, 5);
                       // Adjusted duration
                }
            }
        }
    }

    // Optionally draw labels for direction indicators at grid edges
    FVector NorthLabelLocation = BaseLocation + FVector(Width * CellSize / 2, -CellSize, Length * Elevation / 2);
    FVector SouthLabelLocation = BaseLocation + FVector(Width * CellSize / 2, Height * CellSize + CellSize, Length * Elevation / 2);
    FVector EastLabelLocation  = BaseLocation + FVector(Width * CellSize + CellSize, Height * CellSize / 2, Length * Elevation / 2);
    FVector WestLabelLocation  = BaseLocation + FVector(-CellSize, Height * CellSize / 2, Length * Elevation / 2);

    DrawDebugString(GetWorld(), NorthLabelLocation, "N", nullptr, FColor::Red, -1.0f, true);
    DrawDebugString(GetWorld(), SouthLabelLocation, "S", nullptr, FColor::Red, -1.0f, true);
    DrawDebugString(GetWorld(), EastLabelLocation, "E", nullptr, FColor::Red, -1.0f, true);
    DrawDebugString(GetWorld(), WestLabelLocation, "W", nullptr, FColor::Red, -1.0f, true);
}

void ADungeonGenerator::InitializeSeed(int32 Seed)
{
    RandomStream.Initialize(Seed);
}

void ADungeonGenerator::PlaceMultipleRooms(int32 NumberOfRooms)
{
    int32 Attempts = 0;
    int32 PlacedRooms = 0;

    // Resetting the RandomStream should generally be done in the initialization or before the generation loop
    // RandomStream.Reset(); // Consider resetting this where you set the seed instead of here unless resetting is required every time this function is called

    while (PlacedRooms < NumberOfRooms && Attempts < NumberOfRooms * 10) {
        FRoom NewRoom;
        NewRoom.Width = RandomStream.RandRange(minRoomsize, maxRoomsize);
        NewRoom.Height = RandomStream.RandRange(minRoomsize, maxRoomsize);
        NewRoom.Length = RandomStream.RandRange(1, 3);  // Adjusted to allow variation in room height spanning 1 to 3 levels

        NewRoom.StartX = RandomStream.RandRange(0, Width - NewRoom.Width);
        NewRoom.StartY = RandomStream.RandRange(0, Height - NewRoom.Height);
        NewRoom.StartZ = RandomStream.RandRange(0, Length - NewRoom.Length);

        if (CanPlaceRoom(NewRoom)) {
            PlaceRoom(NewRoom);
            PlacedRooms++;
        }
        Attempts++;
    }
}
int32 ADungeonGenerator::GetIndex(int32 x, int32 y, int32 z)
{
    return x + y * Width + z * Width * Height;
}

bool ADungeonGenerator::CanPlaceRoom(const FRoom& Room) 
{
    for (int z = Room.StartZ; z < Room.StartZ + Room.Length; ++z) {
        for (int y = Room.StartY; y < Room.StartY + Room.Height; ++y) {
            for (int x = Room.StartX; x < Room.StartX + Room.Width; ++x) {
                if (x < 0 || x >= Width || y < 0 || y >= Height || z < 0 || z >= Length) {
                    return false;  // Check if the room is out of the grid bounds
                }
                int32 Index = GetIndex(x, y, z);
              
                if (Grid[Index] != 0) {
                    return false;  // Check if the cell is already occupied
                }
            }
        }
    }
    return true;
}

void ADungeonGenerator::PlaceRoom(const FRoom& Room)
{
    for (int z = Room.StartZ; z < Room.StartZ + Room.Length; ++z) {
        for (int y = Room.StartY; y < Room.StartY + Room.Height; ++y) {
            for (int x = Room.StartX; x < Room.StartX + Room.Width; ++x) {
                int32 Index = GetIndex(x, y, z);
                Grid[Index] = 1;  // Assume '1' marks cells occupied by rooms
            }
        }
    }
    Rooms.Add(Room);
}


void ADungeonGenerator::FinalizeDungeon()
{
    // Set the entry point
    Grid[0] = 3;  // 3 could signify an entry point

    // Set the exit point
    Grid[Width * Height - 1] = 4;  // 4 could signify an exit point

    // Potentially place other elements like traps (5) and treasure (6)
    for (const FRoom& Room : Rooms)
    {
        if (FMath::RandBool())  // Random chance to place a treasure
        {
            int32 TreasureX = FMath::RandRange(Room.StartX, Room.StartX + Room.Width - 1);
            int32 TreasureY = FMath::RandRange(Room.StartY, Room.StartY + Room.Height - 1);
            Grid[TreasureY * Width + TreasureX] = 6;
        }
    }
}

