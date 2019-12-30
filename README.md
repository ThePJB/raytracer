# Raytracer in C
Requirements:
* Linear algebra library
  * CBLAS (its quite ugly, worth wrapping. Also not sure how much of a difference it makes for a bunch of vec3s only)
* png output (libpng)
  * A full on library, I only need to dump an image so this one can be kept locked under the stairs

Features:
* Projective geometry/pinhole camera model
  * View plane size <> Angle of view <> Focal length trio (specify in terms of AOV)
* Backward stuff
  * reflection
  * refraction
  * fresnel (how much reflection vs refraction based on angle, get lense action)
  * shadow
  * specular
* Forward stuff?
  * GI?
* shading models
  * phong
* Material
  * Glass
  * Conductive (metal, colours its specular highlights)
  * emitter/luminescent?
  * translucent?
* Geometry
  * Plane / square
  * sphere
  * arbitrary mesh


# Geometry Stage (projection matrices etc. notes)
(this is from GPU pipeline, this part applies somewhat here as well)

## Model Transform

mat4 that controls position and orientation of the vertices of a model

## View Transform

mat4 for camera position and orientation
inverse of this takes you to the origin of the coordinate system (convention: facing negative z)

This gets applied to entire scene

These get composed so V^-1M is modelview transform, which takes model verts to view land

this most likely takes you to a view frustum (truncated pyramid) with near and far clipping planes

My question: How to calculate V from starting vec3 position and direction?

depends how you represent camera. Options include:
* euler angles
* unit vectors? (me) (View Plane Normal VPN, View Plane UP VPU)

How to calculate world to camera matrix from VPN, VPU and aov_h, aov_w?

We can consider position and orientation separately and multiply them together. I have to wonder as well about focal length though.

This might be enough to get started and then slap focal length on top with the image plane and stuff


actually, knowing VPN and VPU makes this thing i was doing simple. VPL is VPN x VPU. If camera isnt rolled, VPU is like probably a simple inversion or something away? Calculating VPU from VPN might be the hard bit

I can see how with vector there are infinte orthogonal vectors. Is there a formula that can take 1 vector and 1 angle and produce a second orthogonal vector?


It seems like we kind of need to specify VPN and VPU.

## Projection Transform

perspective vs. orthographic

This transform takes the frustum and flattens it out into a unit cube

### skip clipping for raytracer

## Viewport Transform / Screen mapping

from unit cube to flat image with correct resolution, no depth but depth appropriately represented

(convention: verts at 1 and -1)
