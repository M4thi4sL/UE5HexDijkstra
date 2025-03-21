# Unreal Engine 5 Dijkstra Hexagonal Grid Pathfinding System

This project is a complete refactor of the [original DijkstraBlueprint repository](https://github.com/salvatorespoto/DijkstraBlueprint), updated to follow industry-standard coding practices and optimization. It implements a hexagonal grid system with Dijkstra's algorithm for pathfinding, supporting both Blueprint and C++ workflows.

# Branches

✅ [Dijkstra_blueprint](https://github.com/M4thi4sL/UE5HexDijkstra/tree/Dijkstra_Blueprint) contains the latest version of the blueprint-only implementation.

🚧 [Dijkstra_C++](https://github.com/M4thi4sL/UE5HexDijkstra/tree/Dijkstra_C%2B%2B) contains the latest version of the C++ / Blueprint hybrid implementation.

You can also find a zipped-up version of either project in the release section.

🚧 TODO: Add speed comparison between Blueprint and C++ implementations.

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

![Dijkstra](https://github.com/user-attachments/assets/ff5e47fe-50e6-45f2-b740-4d7e56a8515c)

Debug view:
![hex_debug](https://github.com/user-attachments/assets/cbe40fa7-acb9-4f6f-857e-8a1812d0201a)

# How to Build

1. Open the project in Unreal Engine 5.
2. If using the C++ implementation, ensure you have Visual Studio installed with the necessary components for Unreal Engine development.
3. Build the project using the Unreal Engine Editor or Visual Studio.

# License

This project is licensed under the MIT License. See the `LICENSE` file for details.
