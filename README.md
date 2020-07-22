# AutoQuest
AutoQuest is just a hobbyists journey to creating a C++ OpenGL rendering engine. The goal of this engine is to eventually run on Android, Windows, and Linux 

_Currently tested in Windows and Linux, Android support should ideally work in a native C++ application as I've tested SFML there, as long as the code is OpenGL 4.3 or lower it should be cross compatible with OpenGL ES 3.2_

### Current State:
[Shuttle Radar Topography Mission Project] Currently the executable of this project renders an hgt file into a 3D model
![alt text](https://i.imgur.com/awMqACh.jpg "Loaded model!")
- [VAO/VBO/EBO/Buffer abstraction] Abstracts the logic that is the same to these openGL concepts and similifies the filling/streaming to these buffers
- Pretty flexible InputManager that allows binding an action to a event type and key/event_id
- Offers both a perspective and orthographic camera system

### Roadmap:
- Lighting is the next big todo, currently the color of the geography is based on the Z value of the geometry
- Collision with the terrain

### Far off Roadmap:
- Really need to figure out exactly how bones are able to transform other vertices to get animations working
- Android testing since the support right now is based on a seperate code base for the android NDK wrapper
