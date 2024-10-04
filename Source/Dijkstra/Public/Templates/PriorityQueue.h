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

UENUM(BlueprintType)
enum class EPeekPosition : uint8 {
    Head UMETA(DisplayName = "Head"),
    Tail UMETA(DisplayName = "Tail")
};

// Node structure for non-UObject types
template <typename InElementType, typename = void>
struct TPriorityQueueNode {
    InElementType Element; // Directly store the element
    float Priority;

    TPriorityQueueNode()
        : Priority(-1.0f) {}

    TPriorityQueueNode(const InElementType& InElement, float InPriority)
        : Element(InElement), Priority(InPriority) {}

    bool Compare(const TPriorityQueueNode<InElementType>& Other, EPriorityOrder Order) const {
        return Order == EPriorityOrder::Ascending ? Priority < Other.Priority : Priority > Other.Priority;
    }
};

// Specialization for UObject types using TWeakObjectPtr
template <typename InElementType>
struct TPriorityQueueNode<InElementType, typename TEnableIf<TIsDerivedFrom<InElementType, UObject>::Value>::Type> {
    TWeakObjectPtr<InElementType> Element; // Use TWeakObjectPtr for UObjects
    float Priority;

    TPriorityQueueNode()
        : Priority(-1.0f) {}

    TPriorityQueueNode(InElementType* InElement, float InPriority)
        : Element(InElement), Priority(InPriority) {}

    bool Compare(const TPriorityQueueNode<InElementType>& Other, EPriorityOrder Order) const {
        return Order == EPriorityOrder::Ascending ? Priority < Other.Priority : Priority > Other.Priority;
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

    // Specialization for non-UObject types
    template <typename T = InElementType>
    typename TEnableIf<!TIsDerivedFrom<T, UObject>::Value, T>::Type
    Pop() {
        check(!IsEmpty());
        TPriorityQueueNode<T> Node = Array[0];
        Array[0] = Array.Last();
        Array.Pop();
        Heapify();

        return Node.Element;
    }

    // Specialization for UObject types
    template <typename T = InElementType>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value, T*>::Type
    Pop() {
        check(!IsEmpty());
        TPriorityQueueNode<T> Node = Array[0];
        Array[0] = Array.Last();
        Array.Pop();
        Heapify();

        return Node.Element.IsValid() ? Node.Element.Get() : nullptr;
    }

    // Push method for non-UObject types
    template <typename T = InElementType>
    typename TEnableIf<!TIsDerivedFrom<T, UObject>::Value>::Type
    Push(const T& Element, float Priority) {
        // Check if the element already exists in the queue
        for (int32 i = 0; i < Array.Num(); ++i) {
            if (Array[i].Element == Element) {
                // Update the priority of the existing element
                Array[i].Priority = Priority;
                Heapify();
                return;
            }
        }

        // Add new node for non-UObject type
        Array.Add(TPriorityQueueNode<T>(Element, Priority));
        Heapify();
    }

    // Push method for UObject types
    template <typename T = InElementType>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value>::Type
    Push(T* Element, float Priority) {
        // Check if the element already exists in the queue
        for (int32 i = 0; i < Array.Num(); ++i) {
            if (Array[i].Element.Get() == Element) {
                // Update the priority of the existing element
                Array[i].Priority = Priority;
                Heapify();
                return;
            }
        }

        // Add new node for UObject type
        Array.Add(TPriorityQueueNode<T>(Element, Priority));
        Heapify();
    }

    // Peek method for non-UObject types
    template <typename T = InElementType>
    typename TEnableIf<!TIsDerivedFrom<T, UObject>::Value, T>::Type
    Peek(EPeekPosition PeekPosition, float& OutPriority) const {
        check(!IsEmpty());

        const auto& Node = (PeekPosition == EPeekPosition::Head) ? Array[0] : Array.Last();
        OutPriority = Node.Priority;
        return Node.Element;
    }

    // Peek method for UObject types
    template <typename T = InElementType>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value, T*>::Type
    Peek(EPeekPosition PeekPosition, float& OutPriority) const {
        if (IsEmpty()) {
            OutPriority = -1.0f;
            return nullptr;
        }

        const auto& Node = (PeekPosition == EPeekPosition::Head) ? Array[0] : Array.Last();
        OutPriority = Node.Priority;
        return Node.Element.IsValid() ? Node.Element.Get() : nullptr;
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

    // Contains method for non-UObject types
    template <typename T = InElementType>
    typename TEnableIf<!TIsDerivedFrom<T, UObject>::Value, bool>::Type
    Contains(const T& Element) const {
        // Use TPriorityQueueNode<InElementType> to match the type in the array
        for (const TPriorityQueueNode<InElementType>& Node : Array) {
            if (Node.Element == Element) {
                return true;
            }
        }
        return false;
    }

    // Contains method for UObject types
    template <typename T = InElementType>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value, bool>::Type
    Contains(const T* Element) const {
        for (const TPriorityQueueNode<T>& Node : Array) {
            if (Node.Element.IsValid() && Node.Element.Get() == Element) {
                return true;
            }
        }
        return false;
    }

    // Find method for non-UObject types
    template <typename T = InElementType>
    typename TEnableIf<!TIsDerivedFrom<T, UObject>::Value, bool>::Type
    Find(const T& Element, float& OutPriority) const {
        for (const TPriorityQueueNode<T>& Node : Array) {
            if (Node.Element == Element) {
                OutPriority = Node.Priority;
                return true;
            }
        }
        OutPriority = -1.0f;
        return false;
    }

    // Find method for UObject types
    template <typename T = InElementType>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value, bool>::Type
    Find(const T* Element, float& OutPriority) const {
        for (const TPriorityQueueNode<T>& Node : Array) {
            if (Node.Element.IsValid() && Node.Element.Get() == Element) {
                OutPriority = Node.Priority;
                return true;
            }
        }
        OutPriority = -1.0f;
        return false;
    }

    // GetPosition method for non-UObject types
    template <typename T = InElementType>
    typename TEnableIf<!TIsDerivedFrom<T, UObject>::Value, bool>::Type
    GetPosition(const T& Element, int32& OutIndex) const {
        for (int32 Index = 0; Index < Array.Num(); ++Index) {
            if (Array[Index].Element == Element) {
                OutIndex = Index; // Set the index of the element if found
                return true;  // Return true to indicate it was found
            }
        }

        OutIndex = -1; // Set index to -1 if not found
        return false;  // Return false if the element is not found
    }

    // GetPosition method for UObject types
    template <typename T = InElementType>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value, bool>::Type
    GetPosition(const T* Element, int32& OutIndex) const {
        for (int32 Index = 0; Index < Array.Num(); ++Index) {
            if (Array[Index].Element.IsValid() && Array[Index].Element.Get() == Element) {
                OutIndex = Index; // Set the index of the element if found
                return true;  // Return true to indicate it was found
            }
        }

        OutIndex = -1; // Set index to -1 if not found
        return false;  // Return false if the element is not found
    }

    // Getter for internal array
    const TArray<TPriorityQueueNode<InElementType>>& GetArray() const {
        return Array;
    }

private:
    TArray<TPriorityQueueNode<InElementType>> Array;
    EPriorityOrder Order;

    void Heapify() {
        Array.Sort([this](const TPriorityQueueNode<InElementType>& A, const TPriorityQueueNode<InElementType>& B) {
            if (Order == EPriorityOrder::Ascending) {
                return A.Priority < B.Priority;
            }
            return A.Priority > B.Priority;
        });
    }
};
