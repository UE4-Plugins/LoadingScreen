# LoadingScreen

Unreal Engine 4 plug-in that adds a simplified layer on top of the loading screen system
to make it easier to add a loading screen.

![Screenshot](Docs/screenshot.png)


## About

The LoadingScreen plug-in implements a module that allows you to configure a simple
Loading Screen system in the engine.  To goal is to make it more customizable over 
time to avoid needing to write a new loading screen manually.


## Supported Platforms

This plug-in was last built against **Unreal Engine 4.21**. It works on all platforms (probably).


## Dependencies

This plug-in requires Visual Studio and either a C++ code project or a the full
Unreal Engine 4 source code from GitHub. If you are new to programming in UE4,
please see the official [Programming Guide](https://docs.unrealengine.com/latest/INT/Programming/index.html)! 


## Usage

You can use this plug-in as a project plug-in, or an Engine plug-in.

If you use it as a project plug-in, clone this repository into your project's
*/Plugins* directory and compile your game in Visual Studio. A C++ code project
is required for this to work.

If you use it as an Engine plug-in, clone this repository into the
*/Engine/Plugins/Media* directory and compile your game. Full Unreal Engine 4
source code from GitHub (4.21 or higher) is required for this.

After compiling the plug-in, you have to **enable it** in Unreal Editor's
plug-in browser.

![plugin](Docs/plugins.png)


## Support

**Note: This plugin is not supported by Epic Games.**

Please [file an issue](https://github.com/nickdarnell/LoadingScreen/issues),
submit a [pull request](https://github.com/nickdarnell/LoadingScreen/pulls?q=is%3Aopen+is%3Apr)
or hit me up on twitter [@NickDarnell](https://twitter.com/NickDarnell)


## References

* [Introduction to UE4 Plugins](https://wiki.unrealengine.com/An_Introduction_to_UE4_Plugins)
