# Vector module contains all types and functions to manipulate vectors
type
  VectorElementType = SomeNumber | bool
  Vec*[N: static[int], T: VectorElementType] = array[N, T]

# type aliases for vec3, vec4, ...etc.
type
  Vec3*[T: VectorElementType] = Vec[3, T]

proc vec3*[T](): Vec3[T] {.inline.} = Vec3[T]()
proc vec3*[T](x, y, z: T): Vec3[T] {.inline.} = Vec3[T]([T(x), T(y), T(z)])

proc x*(v: Vec3): VectorElementType = v[0]
proc y*(v: Vec3): VectorElementType = v[1]
proc z*(v: Vec3): VectorElementType = v[2]

