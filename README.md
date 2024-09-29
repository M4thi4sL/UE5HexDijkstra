# Unreal Engine 5 Dijkstra Hexagonal Grid Pathfinding System

This project is a complete refactor of the [original DijkstraBlueprint repository](https://github.com/salvatorespoto/DijkstraBlueprint), updated to follow industry-standard coding practices and optimization.

# Features

### BP_GridManager
  
Responsible for managing the hexagonal grid system.

 * Random Grid Generation: Creates a random hex grid based on given dimensions.
 
 * Goal Assignment: Randomly sets a goal within the grid for pathfinding.
 
### BP_Hexagon

Primarily serves as the visual representation of hexagons in the grid. All logic and data are decoupled from this class and loaded from data assets, allowing for easy customization.

### PDA_Base

### PDA_Hexagon

Inherits from PDA_Base. It defines the hexagon's mesh, materials, and travel cost. It overrides the GetSoftReferences function to return all necessary soft references for asset initialization.
   
# Showcase
        
![Hex01](https://github.com/user-attachments/assets/9e7feb5e-4993-4cf0-b2ab-a1439467dbfd)

debug view:
![Hex02](https://github.com/user-attachments/assets/e0bff4db-23af-4940-8b9f-ee1e10838753)

