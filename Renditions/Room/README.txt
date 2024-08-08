# Rendition *Hello World (OpenGL)*
Enter `Zeitgeist.Interpeter --rendition "Room (OpenGL)"` to start this rendition.
This rendition opens a window and renders a room into that window.
Use the arrow keys to move through the room.
The application will close of the window is closed or escape is pressed.

## Implementation details
For each technique the renderer supports, a material (definition) contains
a material technique (definition). The current renderer supports the Phong
and BlinnPhong techniques.

## Roadmap
*room* places the player in a building.
The player navigates the rooms in the building to find an exit of the building.
Once the player reaches an exit, he is placed in the next building.

The *map* maintains information on
- the level geometry and materials
- light sources
- the player
- the entry and the exit point

