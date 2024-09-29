# Unreal Engine 5 Dijkstra Hexagonal Grid Pathfinding System

This project is a complete refactor of the [original DijkstraBlueprint repository](https://github.com/salvatorespoto/DijkstraBlueprint), updated to follow industry-standard coding practices and optimization.

# Branches

✅ [Dijkstra_blueprint](https://github.com/M4thi4sL/UE5HexDijkstra/tree/Dijkstra_Blueprint) contains the latest version of the blueprint only implementation.

✅ [Dijkstra_C++](https://github.com/M4thi4sL/UE5HexDijkstra/tree/Dijkstra_C%2B%2B) contains the latest version of the C++ / Blueprint implementation. 

You can also find a zipped up version of either project in the release section.

🚧 TODO: add speed comparison between blueprint and c++ versions

# Features

### BP_GridManager
  
Responsible for managing the hexagonal grid system.

 * Random Grid Generation: Creates a random hex grid based on given dimensions.
 
 * Goal Assignment: Randomly sets a goal within the grid for pathfinding.
   
 * Path-finding implementation based on Dijkstra.
 
### BP_Hexagon

Primarily serves as the visual representation of hexagons in the grid. All logic and data are decoupled from this class and loaded from data assets, allowing for easy customization.

### PDA_Base

The base class for all future data assets. It contains a single function that returns all soft references that need to be loaded. This function is intended to be overridden by child classes if needed.

![softreferences](https://github.com/user-attachments/assets/19154116-e999-4a31-bd06-ea2cfa3594e0)

### PDA_Hexagon

Inherits from PDA_Base. It defines the hexagon's mesh, materials, and travel cost. It overrides the GetSoftReferences function to return all necessary soft references for asset initialization.
   
# Showcase
        
![Dijkstra](https://github.com/user-attachments/assets/ff5e47fe-50e6-45f2-b740-4d7e56a8515c)

debug view:
![hex_debug](https://github.com/user-attachments/assets/cbe40fa7-acb9-4f6f-857e-8a1812d0201a)
