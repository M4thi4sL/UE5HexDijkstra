# Unreal Engine 5 Dijkstra Hexagonal Grid Pathfinding System

This project is a complete refactor of the [original DijkstraBlueprint repository](https://github.com/salvatorespoto/DijkstraBlueprint), updated to follow industry-standard coding practices and optimization. It implements a hexagonal grid system with Dijkstra's algorithm for pathfinding, supporting both Blueprint and C++ workflows.

Dijkstra_C++

# Features

### BP_GridManager

Responsible for managing the hexagonal grid system.

 * **Random Grid Generation**: Creates a random hex grid based on given dimensions.
 * **Goal Assignment**: Randomly sets a goal within the grid for pathfinding.

### BP_Hexagon

Primarily serves as the visual representation of hexagons in the grid. All logic and data are decoupled from this class and loaded from data assets, allowing for easy customization.

### PDA_Base

Base class for data assets used in the project.

### PDA_Hexagon

Inherits from `PDA_Base`. It defines the hexagon's mesh, materials, and travel cost. The project now uses Unreal Engine's internal Asset Manager for asset initialization, removing the need for the previously implemented `GetSoftReferences` function.

### C++ Implementation

The C++ implementation is located in the `Source/Dijkstra` folder and includes the following key components:

#### Core Classes

- **[`GridManager`](Source/Dijkstra/Public/GridManager.h)**: Manages the hexagonal grid and handles pathfinding logic.
- **[`PriorityQueueWrapper`](Source/Dijkstra/Public/PriorityQueueWrapper.h)**: A wrapper for a priority queue used in Dijkstra's algorithm.
- **[`Hexagon`](Source/Dijkstra/Public/Hexagon/Hexagon.h)**: Represents individual hexagons in the grid, including their state and type.

#### Enumerations

- **[`EHexState`](Source/Dijkstra/Public/Hexagon/EHexState.h)**: Defines the possible states of a hexagon (e.g., empty, occupied, goal).
- **[`EHexType`](Source/Dijkstra/Public/Hexagon/EHexType.h)**: Defines the types of hexagons (e.g., walkable, obstacle).

#### Data Assets

- **[`PDA_Base`](Source/Dijkstra/Public/DataAssets/PDA_Base.h)**: Base class for data assets.
- **[`PDA_Hex`](Source/Dijkstra/Public/DataAssets/PDA_Hex.h)**: Defines hexagon-specific data, such as mesh, materials, and travel cost.

#### Templates

- **[`PriorityQueue`](Source/Dijkstra/Public/Templates/PriorityQueue.h)**: A generic priority queue template used for efficient pathfinding.

# Showcase

![Hex01](https://github.com/user-attachments/assets/9e7feb5e-4993-4cf0-b2ab-a1439467dbfd)

debug view:
![Hex02](https://github.com/user-attachments/assets/e0bff4db-23af-4940-8b9f-ee1e10838753)


# How to Build

1. Open the project in Unreal Engine 5.
2. If using the C++ implementation, ensure you have Visual Studio installed with the necessary components for Unreal Engine development.
3. Build the project using the Unreal Engine Editor or Visual Studio.

# License

This project is licensed under the MIT License. See the `LICENSE` file for details.
=======

