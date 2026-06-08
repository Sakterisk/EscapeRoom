# Escape Room

A first-person **Escape Room** game built with **Unreal Engine 5** and C++. The player is placed inside a locked room and must explore the environment, inspect objects, solve puzzles, and find the way out. Developed as a learning project to understand Unreal Engine's component system, input handling, and C++ gameplay framework.

## Gameplay

The player navigates a 3D environment in first-person perspective and interacts with objects to progress:

- **Inspect** objects by picking them up and examining them up close
- **Grab** and move physical objects in the world
- **Click** interactive triggers (buttons, levers, switches)
- **Solve** logic puzzles involving locks and keys to escape the room

## Architecture

The project uses a **component-based interaction system** built entirely in C++:

```
Source/EscapeRoom/
├── EscapeRoomCharacter      # First-person character: movement, camera, input
├── EscapeRoomPlayerController
├── EscapeRoomGameMode
├── InteractComponent        # Core interaction system (line trace, range check)
├── InteractableObject        # Base class for all interactable actors
├── ClickableObject           # Trigger-style objects (buttons, levers)
├── GrabbableObject           # Physics-based grab and carry
├── InspectableObject         # Close-up object inspection
├── Lock / Key                # Puzzle lock & key mechanic
└── CrosshairUserWidget       # HUD crosshair UMG widget
```

## Tech Stack

| Technology | Purpose |
|---|---|
| Unreal Engine 5 | Game engine & rendering |
| C++ | Gameplay logic and component system |
| Unreal Build Tool (UBT) | Build system |
| Blueprints | Level design and asset setup |
| VS Code + UE Plugin | IDE |

## Key Concepts Implemented

- **Component-based design** — `InteractComponent` attached to the character handles all interaction logic, keeping actors decoupled
- **Interface-style inheritance** — `InteractableObject` as a base AActor subclass with virtual `Interact()`, extended by Clickable, Grabbable, Inspectable
- **Line trace interaction** — ray cast from camera to detect and interact with objects within reach
- **Physics grab** — `GrabbableObject` uses Unreal's `PhysicsHandleComponent` to pick up and carry rigid bodies
- **UMG HUD** — minimal crosshair widget rendered on screen during gameplay

## Getting Started

### Prerequisites

- Unreal Engine 5.x ([download from Epic Games Launcher](https://www.unrealengine.com/))
- Visual Studio 2022 or VS Code with the Unreal Engine extension

### Open the Project

1. Clone the repository
2. Right-click `EscapeRoom.uproject` → **Generate Visual Studio project files**
3. Open `EscapeRoom.uproject` in Unreal Engine 5
4. Press **Play** in the editor

```bash
git clone https://github.com/Sakterisk/EscapeRoom.git
```

> **Note:** Large binary assets (textures, meshes) are not included in the repository. Open the project in UE5 and let the engine generate required files on first launch.

## What I Learned

- Navigating the Unreal Engine 5 C++ project structure and build system
- Designing a reusable component-based interaction system with proper OOP inheritance
- Working with Unreal's `AActor`, `UActorComponent`, and `UUserWidget` classes
- Physics-based object manipulation using `UPhysicsHandleComponent`
- Binding input actions to C++ functions via the Enhanced Input system
- Integrating C++ gameplay classes with Blueprint-configured assets in the editor
