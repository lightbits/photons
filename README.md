photons
====

Performs a 2D simulation of bouncing photons.
For each lightsource we emit a number of uniformly distributed photons.
As the photons collide with materials, the color of the photon is multiplied
with the texture color.
The color of the photon is continually added to a RGB lightbuffer.
At the end of the simulation we multiply the lightbuffer with the scene texture.
To compress the color data to displayable values, we apply tonemapping based on the
log-average luminance and the modified Reinhard operator.

Resources
----
* Real-Time Rendering Third Edition
* [tigsource](http://forums.tigsource.com/index.php?topic=31378.0)
* [A closer look at tonemapping](http://mynameismjp.wordpress.com/2010/04/30/a-closer-look-at-tone-mapping/)
* [Tonemapping and RGB-XYZ conversion](http://www.xnainfo.com/content.php?content=28)
* [The Reinhard tonemapping algorithm](http://www.cs.utah.edu/~reinhard/cdrom/)