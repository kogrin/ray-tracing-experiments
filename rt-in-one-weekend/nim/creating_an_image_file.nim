import strformat    # string formating functions    (nim)
import terminal     # colored output to tetminal    (nim)

import pixie        # cool 2d graphics library
                    # https://github.com/treeform/pixie     (installed via nimble)
import chroma       # cool `color api` library
                    # https://github.com/treeform/chroma/   (installed via nimble)

import include_lib/vec3

proc getGradient(width_px: int, height_px: int): seq[uint8] {. noSideEffect .} =
    var rgbPixelData: seq[uint8] = @[]
    for j in countdown(height_px - 1, 0):
        for i in 0 ..< width_px:
            # generate gradient with RGB colors 
            let red   = uint8(255.999 * float(i) / float(width_px - 1))
            let green = uint8(255.999 * float(j) / float(height_px - 1))
            let blue  = uint8(255.999 * 0.25)
            rgbPixelData.add(red)
            rgbPixelData.add(green)
            rgbPixelData.add(blue)
    # return saved
    # color values in seq[uint8]
    rgbPixelData

proc savePixelsToPNG(filename: string, width: int, height: int, pxColors: seq[uint8]) =
    var image = newImage(width, height)
    var i = 0
    for y in 0 ..< height:
        for x in 0 ..< width:
            image[x, y] = rgb(pxColors[i], pxColors[i + 1], pxColors[i + 2])
            i += 3
    echo &"Writing data to: ./{filename}.png ..."
    image.writeFile(filename & ".png")

proc writeColorData(width: int, height: int, pxColors: seq[uint8]) =
    echo &"P3\n{width} {height}\n255"
    let pxColors: seq[uint8] = getGradient(width, height)
    var i: int = 0
    while i < (width * height):
        stdout.styledWrite(fgRed,   $pxColors[i],       " ")
        stdout.styledWrite(fgGreen, $pxColors[i + 1],   " ")
        stdout.styledWrite(fgCyan,  $pxColors[i + 2],   "\n")
        i += 3

proc writeToStdout(width: int, height: int, pxColors: seq[uint8]) =
    echo &"P3\n{width} {height}\n255"
    let pxColors = getGradient(width, height)
    var i: int = 0
    while i < (width * height):
        echo &"{pxColors[i]} {pxColors[i + 1]} {pxColors[i + 2]}"
        i += 3

proc main() =
    let imageWidth  = 256
    let imageHeight = 256
    let pxColors = getGradient(imageWidth, imageHeight)

    # writeColorData(imageWidth, imageHeight, pxColors)
    # writeToStdout(imageWidth, imageHeight, pxColors)

    savePixelsToPNG("first_gradient", imageWidth, imageHeight, pxColors)
    echo "All done."

proc testVec3() =
    let vector3 = vec3(2.0, 3.0, 5.0)
    echo &"\nTest of vector3 repr: {vector3} (as generic array resolved with floats)"
    echo &"=>\tx is {vector3.x()}"
    echo &"=>\ty is {vector3.y()}"
    echo &"=>\tz is {vector3.z()}"

# main()
testVec3()
