#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "PriorityQueue.generated.h"

// Order of priority: Ascending or Descending
UENUM(BlueprintType)
enum class EPriorityOrder : uint8 {
    Ascending UMETA(DisplayName = "Ascending"),
    Descending UMETA(DisplayName = "Descending")
};

// Node structure for the priority queue
template <typename InElementType>
struct TPriorityQueueNode {
    InElementType Element;
    float Priority;

    // Default constructor
    TPriorityQueueNode()
        : Element(nullptr), Priority(0.0f) {}

    // Parameterized constructor
    TPriorityQueueNode(InElementType InElement, float InPriority)
        : Element(InElement), Priority(InPriority) {}

    // Comparison based on the specified order
    bool Compare(const TPriorityQueueNode<InElementType>& Other, EPriorityOrder Order) const {
        if (Order == EPriorityOrder::Ascending) {
            return Priority < Other.Priority; // Lower priority value means higher precedence
        } else { // Descending
            return Priority > Other.Priority; // Higher priority value means higher precedence
        }
    }
};

// Priority Queue class
template <typename InElementType>
class TPriorityQueue {
public:
    // Constructor that sets the order of priority
    TPriorityQueue(EPriorityOrder InOrder = EPriorityOrder::Ascending)
        : Order(InOrder) {}

    // Set the order of the queue and re-heapify
    void SetOrder(EPriorityOrder NewOrder) {
        Order = NewOrder;
        Heapify();
    }

    InElementType Pop() {
        if (IsEmpty()) {
            return nullptr; // Handle empty queue case
        }
        TPriorityQueueNode<InElementType> Node = Array[0];
        Array.RemoveAt(0); // Remove the top element
        // Re-heapify after popping
        Heapify();
        return Node.Element;
    }

    void Push(InElementType Element, float Priority) {
        Array.Add(TPriorityQueueNode<InElementType>(Element, Priority));
        // Re-heapify after pushing
        Heapify();
    }

    InElementType Peek() const {
        if (IsEmpty()) {
            return nullptr; // Handle empty queue case
        }
        return Array[0].Element; // Return the top element without removing it
    }

    bool IsEmpty() const {
        return Array.Num() == 0;
    }

    int32 Size() const {
        return Array.Num(); // Returns the number of elements in the queue
    }

    void Clear() {
        Array.Empty(); // Clears all elements from the queue
    }

private:
    TArray<TPriorityQueueNode<InElementType>> Array;
    EPriorityOrder Order; // Store the order of the priority queue

    void Heapify() {
        for (int32 i = Array.Num() / 2 - 1; i >= 0; i--) {
            HeapifyDown(i);
        }
    }

    void HeapifyDown(int32 Index) {
        int32 LeftChild = 2 * Index + 1;
        int32 RightChild = 2 * Index + 2;
        int32 SmallestOrLargest = Index;

        if (LeftChild < Array.Num() && Array[LeftChild].Compare(Array[SmallestOrLargest], Order)) {
            SmallestOrLargest = LeftChild;
        }
        if (RightChild < Array.Num() && Array[RightChild].Compare(Array[SmallestOrLargest], Order)) {
            SmallestOrLargest = RightChild;
        }
        if (SmallestOrLargest != Index) {
            Swap(Array[Index], Array[SmallestOrLargest]);
            HeapifyDown(SmallestOrLargest);
        }
    }
};

// Blueprint-exposed wrapper class using UObject
UCLASS(Blueprintable)
class DIJKSTRA_API UPriorityQueue : public UObject {
    GENERATED_BODY()

public:
    // Default constructor
    UPriorityQueue();

    /**
     * Sets the sorting order of the priority queue.
     * 
     * @param NewOrder - The new order for the priority queue (Ascending or Descending).
     */
    UFUNCTION(BlueprintCallable, Category = "PriorityQueue")
    void SetOrder(EPriorityOrder NewOrder);

    /**
     * Pushes an element into the priority queue with a specified priority.
     * Lower priority values will have higher precedence in the queue.
     * 
     * @param Element - The UObject to add to the queue.
     * @param Priority - The priority of the element; lower values indicate higher priority.
     */
    UFUNCTION(BlueprintCallable, Category = "PriorityQueue")
    void Push(UObject* Element, float Priority);

    /**
     * Removes and returns the element with the highest priority (lowest priority value).
     * If the queue is empty, returns nullptr.
     * 
     * @return The UObject with the highest priority.
     */
    UFUNCTION(BlueprintCallable, Category = "PriorityQueue")
    UObject* Pop();

    /**
     * Retrieves the element with the highest priority without removing it from the queue.
     * If the queue is empty, returns nullptr.
     * 
     * @return The UObject with the highest priority in the queue.
     */
    UFUNCTION(BlueprintCallable, Category = "PriorityQueue")
    UObject* Peek() const;

    /**
     * Checks if the priority queue is empty.
     * 
     * @return True if the queue has no elements; otherwise, false.
     */
    UFUNCTION(BlueprintCallable, Category = "PriorityQueue")
    bool IsEmpty() const;

    /**
     * Returns the number of elements currently in the priority queue.
     * 
     * @return The size of the queue as an integer.
     */
    UFUNCTION(BlueprintCallable, Category = "PriorityQueue")
    int32 Size() const;

    /**
     * Clears all elements from the priority queue.
     */
    UFUNCTION(BlueprintCallable, Category = "PriorityQueue")
    void Clear();

private:
    TPriorityQueue<UObject*> PriorityQueue;
};

// Implementation of the UPriorityQueue functions
inline UPriorityQueue::UPriorityQueue()
    : PriorityQueue(EPriorityOrder::Ascending) // Default to ascending order
{}

inline void UPriorityQueue::SetOrder(EPriorityOrder NewOrder) {
    // Set the order in the priority queue
    PriorityQueue.SetOrder(NewOrder);
}

inline void UPriorityQueue::Push(UObject* Element, float Priority) {
    if (Element) { // Check if the Element is valid
        PriorityQueue.Push(Element, Priority);
    }
}

inline UObject* UPriorityQueue::Pop() {
    return PriorityQueue.Pop();
}

inline UObject* UPriorityQueue::Peek() const {
    return PriorityQueue.Peek();
}

inline bool UPriorityQueue::IsEmpty() const {
    return PriorityQueue.IsEmpty();
}

inline int32 UPriorityQueue::Size() const {
    return PriorityQueue.Size();
}

inline void UPriorityQueue::Clear() {
    PriorityQueue.Clear();
}
