#pragma once

#include "CoreMinimal.h"
#include "Templates/PriorityQueue.h"
#include "PriorityQueueWrapper.generated.h"


// Blueprint-exposed wrapper class using UObject
UCLASS(Blueprintable)
class DIJKSTRA_API UPriorityQueue : public UObject {
    GENERATED_BODY()

public:
    UPriorityQueue();

    // Order of priority: Ascending or Descending (exposed on spawn)
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"), Category = "PriorityQueue")
    EPriorityOrder Order;
    
    // Sets the sort order of the priority queue (Ascending or Descending).
    UFUNCTION(BlueprintCallable, Category = "Utilities|PriorityQueue")
    void SetSortOrder(EPriorityOrder NewOrder);

    // Returns the current sort order of the priority queue (Ascending or Descending).
    UFUNCTION(BlueprintPure, Category = "Utilities|PriorityQueue")
    EPriorityOrder GetSortingOrder() const;

    // Adds an element to the priority queue with the specified priority.
    // If the element already exists, its priority is updated.
    UFUNCTION(BlueprintCallable,meta = (Keywords = "enqueue,add, push"), Category = "Utilities|PriorityQueue")
    void Push(UObject* Element, const float Priority);

    // Removes and returns the element with the highest priority from the queue.
    // Returns nullptr if the queue is empty.
    UFUNCTION(BlueprintCallable,  meta = (Keywords = "dequeue,remove,extract"), Category = "Utilities|PriorityQueue")
    UObject* Pop();

    // Returns the element with the highest priority without removing it from the queue.
    // Returns nullptr if the queue is empty.
    UFUNCTION(BlueprintCallable,meta = (Keywords = "peek front top"),  Category = "Utilities|PriorityQueue" )
    UObject* Peek(EPeekPosition PeekPosition, float& Priority) const;

    // Returns true if the priority queue is empty.
    UFUNCTION(BlueprintPure ,meta = (CompactNodeTitle="IS EMPTY", Keywords = "empty, isEmpty"), Category = "Utilities|PriorityQueue")
    bool IsEmpty() const;

    // Returns true if the priority queue is not empty.
    UFUNCTION(BlueprintPure ,meta = (CompactNodeTitle="IS NOT EMPTY", Keywords = "notEmpty"), Category = "Utilities|PriorityQueue")
    bool IsNotEmpty() const;

    // Returns the number of elements currently in the priority queue.
    UFUNCTION(BlueprintPure, meta = (CompactNodeTitle="LENGTH", Keywords = "Length, Size"), Category = "Utilities|PriorityQueue")
    int32 Length() const;

    // Clears all elements from the priority queue.
    UFUNCTION(BlueprintCallable, Category = "Utilities|PriorityQueue")
    void Clear();

    // Returns true if the priority queue contains the specified element.
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Contains", CompactNodeTitle = "CONTAINS"), Category = "Utilities|PriorityQueue")
    bool Contains(UObject* Element) const;

    // Prints all elements and their priorities in the queue to the log or screen for debugging.
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, Keywords = "log print", AdvancedDisplay = "2", DevelopmentOnly), Category = "Utilities|PriorityQueue")
    void PrintPriorityQueue(const UObject* WorldContextObject, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor TextColor = FLinearColor(0.0f, 0.66f, 1.0f), float Duration = 2.f) const;

    // Returns an array of all elements in the priority queue.
    UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Keys", CompactNodeTitle = "KEYS"), Category = "Utilities|PriorityQueue")
    void GetKeys(TArray<UObject*>& Keys);

    // Returns an array of all priorities in the priority queue.
    UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Priorities", CompactNodeTitle = "PRIORITIES"), Category = "Utilities|PriorityQueue")
    void GetPriorities(TArray<float>& Priorities);

    // Returns the priority value of the specified element in the priority queue if it exists.
    // Returns -1.0f if the element is not found.
    UFUNCTION(BlueprintPure, meta = (Keywords = "find", CompactNodeTitle = "FIND"), Category = "Utilities|PriorityQueue")
    bool Find(const UObject* Element, float& Priority) ;

    // Finds the specified element in the priority queue and outputs its index.
    // Returns true if the element is found.
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Pos, Position, Index, location", CompactNodeTitle = "POS"), Category = "Utilities|PriorityQueue")
    bool GetPosition(UObject* Element, int32& Index) const;
    
    // Custom thunks
    DECLARE_FUNCTION(execGetKeys);
    DECLARE_FUNCTION(execGetPriorities);

    // Exposed for access in thunk implementations
    TPriorityQueue<UObject> PriorityQueue;
};
