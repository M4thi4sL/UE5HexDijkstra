#include "PriorityQueueWrapper.h"
#include "Kismet/KismetSystemLibrary.h"

// Constructor
UPriorityQueue::UPriorityQueue()
    : PriorityQueue(EPriorityOrder::Ascending) // Default to ascending order
{}

// Regular function implementations (no changes needed for these)
void UPriorityQueue::SetSortOrder(EPriorityOrder NewOrder) {
    Order = NewOrder;
    PriorityQueue.SetSortOrder(NewOrder);
}

EPriorityOrder UPriorityQueue::GetSortingOrder() const {
    return PriorityQueue.GetOrder();
}

void UPriorityQueue::Push(UObject* Element, const float Priority) {
    if (Element) {
        PriorityQueue.Push(Element, Priority);
    }
}

UObject* UPriorityQueue::Pop() {
    return PriorityQueue.Pop();
}

UObject* UPriorityQueue::Peek(EPeekPosition PeekPosition, float& Priority) const {
    return PriorityQueue.Peek(PeekPosition, Priority);
}

bool UPriorityQueue::IsEmpty() const {
    return PriorityQueue.IsEmpty();
}

bool UPriorityQueue::IsNotEmpty() const
{
    return !PriorityQueue.IsEmpty();
}

int32 UPriorityQueue::Length() const {
    return PriorityQueue.Length();
}

void UPriorityQueue::Clear() {
    PriorityQueue.Clear();
}

bool UPriorityQueue::Contains(UObject* Element) const {
    return Element ? PriorityQueue.Contains(Element) : false;
}

void UPriorityQueue::PrintPriorityQueue(const UObject* WorldContextObject, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration) const {
#if WITH_EDITOR
    const auto& Nodes = PriorityQueue.GetArray();

    FString Output = "Priority Queue Elements:\n";

    for (const auto& Node : Nodes) {
        if (UObject* Element = Node.Element.Get()) {
            FString ElementName = Element->GetName();
            FString Entry = FString::Printf(TEXT("Element: %s, Priority: %f"), *ElementName, Node.Priority);
            Output += Entry + "\n";
        } else {
            Output += "Invalid element in priority queue (possibly garbage collected)\n";
        }
    }

    Output += "\n---------------------------------------------------------------------------";

    // Print to screen
    if (bPrintToScreen && WorldContextObject) {
        UKismetSystemLibrary::PrintString(WorldContextObject, Output, bPrintToScreen, bPrintToLog, TextColor, Duration);
    }

    // Print to log
    if (bPrintToLog) {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Output);
    }
#endif
}

// Custom thunk implementations

// GetKeys helper function
void UPriorityQueue::GetKeys(TArray<UObject*>& OutKeys) {
    OutKeys.Empty();  // Clear the array to ensure no residual data
    for (const auto& Node : PriorityQueue.GetArray()) {
        if (UObject* Object = Node.Element.Get()) { // Use Get() and check if the result is not null
            OutKeys.Add(Object); // Add the valid UObject* to the array
        }
    }
}

// GetPriorities helper function
void UPriorityQueue::GetPriorities(TArray<float>& OutPriorities) {
    OutPriorities.Empty();  // Clear the array to ensure no residual data
    for (const auto& Node : PriorityQueue.GetArray()) {
        OutPriorities.Add(Node.Priority); // Add each priority to the array
    }
}

bool UPriorityQueue::Find(const UObject* Element, float& Priority) 
{
    return PriorityQueue.Find(Element, Priority);
}

bool UPriorityQueue::GetPosition(UObject* Element, int32& Index) const
{
    // Return false if the input element is not valid
    if (!Element) {
        Index = -1;
        return false;
    }

    return PriorityQueue.GetPosition(Element, Index);
}
// Custom thunk for GetKeys
void UPriorityQueue::execGetKeys(UObject* Context, FFrame& Stack, void* const Result) {
    P_GET_TARRAY_REF(UObject*, Keys);
    P_FINISH;

    UPriorityQueue* ThisQueue = Cast<UPriorityQueue>(Context);
    if (ThisQueue) {
        ThisQueue->GetKeys(Keys);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Context is not a valid UPriorityQueue in execGetKeys"));
        Keys.Empty();
    }
}

// Custom thunk for GetPriorities
void UPriorityQueue::execGetPriorities(UObject* Context, FFrame& Stack, void* const Result) {
    P_GET_TARRAY_REF(float, Priorities);
    P_FINISH;

    UPriorityQueue* ThisQueue = Cast<UPriorityQueue>(Context);
    if (ThisQueue) {
        ThisQueue->GetPriorities(Priorities);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Context is not a valid UPriorityQueue in execGetPriorities"));
        Priorities.Empty();
    }
}
