# Vermeer Maya Plug-in

[Vermeer Light Tools](http://vermeerlt.com) is an open source 3D graphics rendering package.  Vermeer provides 
a ray tracing core, physically plausible shading and deterministic monte-carlo path tracing all
wrapped up into a production renderer.  

This is the plug-in for the Maya modelling/rendering package.  

Currently it is capable of exporting poly meshes and a range of Maya materials and lights, plus adds a 
specific Vermeer shader allowing full control over materials.

This version of the plug-in supports Vermeer version 0.3.1.

Installation
------------

Download the released .zip file or build from source with MS VC++.  Copy the vermeer-maya.mll to a plugin directory and the .mel files into your scripts folders.

Use
---

Within Maya, if it isn't auto loaded then go to Window->Settings/Preferences->Plugin Manager.  Click Browse and
find the vermeer-maya.mll.

You'll find a new shader called Vermeer Std Shader and new export options for VNF files.  Some nodes will also have some additional attributes.

Limitations
------------

This (along with Vermeer) is alpha quality software, there will be bugs and crashes.  Please make sure you've 
backed up any scenes or data you try with this plugin as it can't be guaranteed that data corruption won't 
occur.

That said, I've experienced Maya crashes while developing but no data loss and the current plugin is fairly 
stable. 

This plugin has only been tested with Maya 2015 (64 bit).  I'm lead to believe it should work with most 64 bit 
versions but would welcome any feedback on this.

Support
-------

Please report any issues no matter how small: [Issue Tracker](https://github.com/jamiec7919/vermeer-maya/issues)

Alternatively if you prefer, please feel free to contact me [Jamie](mailto:jamiec7919@gmail.com).

License
-------

vermeer-maya is released under the BSD license (same as the Vermeer LT license).  See the LICENSE file
for the full license.  All Maya SDK code and interfaces are Copyright their respective owners.
