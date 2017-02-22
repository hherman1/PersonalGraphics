Hello!

https://learnopengl.com/ 

We used learnopengl a LOT for guidance. They provide tutorials on how to implement just about everything we implemented here, 
from shadows to shaders, textures, materials and so on. This would be really hard to implement without them. 

Right now the system is kind of rigid. It only supports a single light and a single texture.

The scene and interactions (nearly) all happen in main.cpp, by preference.



## Cool Features

* Directional light + shadow mapping
* WASD: move the light's (x,z) coords. Note, the light isn't casting shadows from its location, instead its casting shadows as though it were the sun and its location about the origin determines the direction of the shadows.I just started to implement real point lights w/ shadows, but I haven't got them finished yet. Implementing these things would probbaly be much harder without good tutorials.
* Space: Launch the ball.
* Play against yourself by bouncing off the wall
* Or off the floor! (or other walls)
* Your volleys are automatically tracked!

##Building

This should build just like any other project. If it doesnt build when configuring with a build directory, it might work when using the source directory as a build dir. 

## Browsing Source

A lot of the files in `src/` are unused. `CMakeLists.txt` lists all the used files.
