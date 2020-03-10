# AutoQuest
AutoQuest is just a hobbyists journey to creating a C++ OpenGL rendering engine. The goal of this engine is to eventually run on Android, Windows, and Linux 

_Currently tested in Windows and Linux, Android support should ideally work in a native C++ application as I've tested SFML there, as long as the code is OpenGL 4.3 or lower it should be cross compatible with OpenGL ES 3.2_

### Current State:
- [OBJ Loader] Can successfully load an obj as a model and actually understand the OpenGL concepts!
![alt text](https://i.imgur.com/9S6GQfn.png "Loaded model!")
- [VAO/VBO/EBO/Buffer abstraction] Abstracts the logic that is the same to these openGL concepts and similifies the filling/streaming to these buffers
- Pretty flexible InputManager that allows binding an action to a event type and key/event_id
- Ortho Camera System is probably all that will be supported at the moment since the goal of this engine is to create an isometric game [Note: this does not mean only 2D is supported!]
- AWS Cognito support for Login Server in C++

### Roadmap:
- Lighting and different texture types are the next two big todos
- Simlify mapping keys to actions in the input manager
- Collisions!

### Far off Roadmap:
- Really need to figure out exactly how bones are able to transform other vertices to get animations working
- MultiAuth w/ Steam + Google Play Games on the Cognito side of things
- Android testing since the support right now is all theoretical

