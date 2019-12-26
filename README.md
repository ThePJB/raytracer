# Raytracer in C
Requirements:
* Linear algebra library
  * probably just do it on top of CBLAS
  * might be worth wrapping this one, its pretty ugly
* png output (libpng)
  * similar, well I just need to dump an rgba image so i can keep this one locked under the stairs

Features:
* Projective geometry/pinhole camera model
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


  could make some sick fractals with this same setup too