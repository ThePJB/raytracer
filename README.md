# Raytracer in C
Requirements:
* Linear algebra library
  * probably just do it on top of CBLAS
  * might be worth wrapping this one, its pretty ugly
* png output (libpng)
  * similar, well I just need to dump an rgba image so i can keep this one locked under the stairs

Features:
* Projective geometry/pinhole camera model
  * Note that angle of view is the primary thing we care about
  * Right now im doing image plane stuff but could just as easily calculate the different direction vecs with euler angles or something for each pixel. its not constant but probably a simple relationship
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


How to get perpendicular vectors (for defining image plane with rotated camera)

Do I even need to do this shit? scratchapixel mentions computing the rays from just the angle. If i just compute the angle for each ray that should do the trick. but not really because i dont know where it starts

swap 2 vals and make one negative

https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula






chuckb@milton.u.washington.edu (Chuck Bass) writes:

Graphics People:

I have the following problem. I have two unit vectors and I need to find the transformation matrix to go from one to the other. (I need to align an object to a normal vector)

    [x y z 1]      [T]    = [ x1 y1 z1 1]

I know that there is no translation in this transform so it is really the 3X3 problem.

    [x y z ]      [T']    = [ x1 y1 z1]

I have three equations here. Properties of [T'] give me three more equations from trigonometric relationships. Problem is, I need 3 more equations.

What am I missing?? Thanks for any insight.

From: andrey@cco.caltech.edu (Andre T. Yew)
Subject: Re: How to transform two normal vectors???
Sender: news@cco.caltech.edu
Date: Wed, 11 Mar 1992 01:06:56 GMT

Several weeks ago in a computer graphics class here, Blinn had us calculate the rotation matrix to go from EME50 to Voyager space so we could use actual JPL data and rotate Voyager's cameras so we could see Jupiter and its moons. Anyway, EME50 is this coordinate system defined based on something that happened in the solar system in 1950, and basically it was off from Voyager's coordinate system. The problem is the same as yours: Voyager had to keep its main antenna aimed at Earth all the time and its cameras were on a boom that was some weird angle off of the body. We were given EME50 coordinates, and to aim the camera, we had to first do the rotation matrix from EME50 to align vectors. In a long-winded way, here's what you do:

Find another vectors perpendicular to both vectors (x has xp, and x1 has x1p). Now form a third vector by taking x X xp (that's a cross-product of x and xp). Same deal with x1 and x1p. For notational simplicity, do this:

    x -- first vector
    m -- second vector perp. to x
    c -- the cross-product of x and m

    x1 -- the second vector
    m1 -- a vector perp to x1
    c1 -- the cross-product of x1 and m1

    T -- the transformation matrix you want to find.

Now write it out like this:

    |x|     |x1|
    |m| T = |m1| 
    |c|     |c1|,

Where [x, m, c] and [x1, m1, c1] are matrices. Now it should be obvious to the casual observer (:-), that T = [x, m, c]^-1 [x1, m1, c1]. I hope this is right and clear. Please, if I've made a mistake, correct me -- we wouldn't want to lose Voyager now.
From: zhu@sgi.com (Benjamin Zhu)
Newsgroups: comp.graphics
Subject: Re: How to transform two normal vectors???
Date: 11 Mar 92 01:06:25 GMT

A somewhat "nasty'' way to solve this problem:

Let V1 = [ x1, y1, z1 ], V2 = [ x2, y2, z2 ]. Assume V1 and V2 are already normalized.

    V3 = normalize(cross(V1, V2)). (the normalization here is mandatory.)
    V4 = cross(V3, V1).
             
         [ V1 ]
    M1 = [ V4 ]
         [ V3 ]

    cos = dot(V2, V1), sin = dot(V2, V4)
            
         [ cos   sin    0 ]
    M2 = [ -sin  cos    0 ]
         [ 0     0      1 ]

The sought transformation matrix is just M1^-1 * M2 * M1. This might well be a standard-text solution. 



 could make some sick fractals with this same setup too