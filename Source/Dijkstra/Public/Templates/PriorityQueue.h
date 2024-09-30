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

// What are we looking at?
UENUM(BlueprintType)
enum class EPeekPosition : uint8 {
    Head UMETA(DisplayName = "Head"),
    Tail UMETA(DisplayName = "Tail")
};

// Node structure for the priority queue
template <typename InElementType>
struct TPriorityQueueNode {
    TWeakObjectPtr<InElementType> Element; // Use TWeakObjectPtr to handle weak references to UObjects
    float Priority;

    // Default constructor
    TPriorityQueueNode()
        : Element(nullptr), Priority(0.0f) {}

    // Parameterized constructor
    TPriorityQueueNode(InElementType* InElement, float InPriority)
        : Element(InElement), Priority(InPriority) {}

    // Comparison based on the specified order
    bool Compare(const TPriorityQueueNode<InElementType>& Other, EPriorityOrder Order) const {
        // Ascending: lower priority value is higher precedence
        if (Order == EPriorityOrder::Ascending) {
            return Priority < Other.Priority;
        }
        // Descending: higher priority value is higher precedence
        return Priority > Other.Priority;
    }
};

// Priority Queue class
template <typename InElementType>
class TPriorityQueue {
public:
    TPriorityQueue(EPriorityOrder InOrder = EPriorityOrder::Ascending)
        : Order(InOrder) {}

    void SetSortOrder(EPriorityOrder NewOrder) {
        Order = NewOrder;
        Heapify();
    }

    EPriorityOrder GetOrder() const {
        return Order;
    }

    InElementType* Pop() {
        while (!IsEmpty()) {
            TPriorityQueueNode<InElementType> Node = Array[0];
            Array[0] = Array.Last();
            Array.Pop();
            Heapify();

            // Return valid element if not expired
            if (Node.Element.IsValid()) {
                return Node.Element.Get();
            }
        }
        return nullptr;
    }

    void Push(InElementType* Element, float Priority) {
        // Check if the element already exists in the queue
        for (int32 i = 0; i < Array.Num(); ++i) {
            if (Array[i].Element.Get() == Element) {
                // Update the priority of the existing element
                Array[i].Priority = Priority;

                // Sort the entire array after updating the priority
                Heapify();
                return; // Return since we updated the existing element
            }
        }

        // If the element doesn't exist, add it as a new node
        Array.Add(TPriorityQueueNode<InElementType>(Element, Priority));
        Heapify(); // Sort the array after adding the new element
    }

    
    InElementType* Peek(EPeekPosition PeekPosition , float& OutPriority) const {
        if (IsEmpty()) {
            OutPriority = -1.0f; // Set an invalid priority if the queue is empty
            return nullptr;
        }

        const auto& Node = (PeekPosition == EPeekPosition::Head) ? Array[0] : Array.Last();
        OutPriority = Node.Priority;
        return Node.Element.Get();
    }
    bool IsEmpty() const {
        return Array.Num() == 0;
    }

    int32 Length() const {
        return Array.Num();
    }

    void Clear() {
        Array.Empty();
    }
    

    bool Contains(const InElementType* Element) const {
        if (!Element) return false;

        for (const TPriorityQueueNode<InElementType>& Node : Array) {
            if (Node.Element.IsValid() && Node.Element.Get() == Element) {
                return true;
            }
        }
        return false;
    }

    // Getter for internal array
    const TArray<TPriorityQueueNode<InElementType>>& GetArray() const {
        return Array;
    }

    bool Find(const InElementType* Element, float& OutPriority) const {
        if (!Element) {
            return false; // Return false if the element is invalid
        }

        // Iterate over all nodes in the priority queue
        for (const auto& Node : Array) {
            if (Node.Element.Get() == Element) {
                OutPriority = Node.Priority; // Set the priority if found
                return true;  // Return true to indicate it was found
            }
        }

        OutPriority = -1.0f; // Optionally set an invalid priority if not found
        return false;  // Return false if the element is not found
    }

    bool GetPosition(const InElementType* Element, int32& OutIndex) const {
        if (!Element) {
            OutIndex = -1;
            return false; // Return false if the element is invalid
        }

        // Iterate over all nodes in the priority queue
        for (int32 Index = 0; Index < Array.Num(); ++Index) {
            if (Array[Index].Element.Get() == Element) {
                OutIndex = Index; // Set the index of the element if found
                return true;  // Return true to indicate it was found
            }
        }

        OutIndex = -1; // Set index to -1 if not found
        return false;  // Return false if the element is not found
    }
    
private:
    TArray<TPriorityQueueNode<InElementType>> Array;
    EPriorityOrder Order;
    
    void Heapify() {
        Array.Sort([this](const TPriorityQueueNode<InElementType>& A, const TPriorityQueueNode<InElementType>& B) {
            // Ascending: lower priority value is higher precedence
            if (Order == EPriorityOrder::Ascending) {
                return A.Priority < B.Priority;
            }
            // Descending: higher priority value is higher precedence
            return A.Priority > B.Priority;
        });
    }



};

