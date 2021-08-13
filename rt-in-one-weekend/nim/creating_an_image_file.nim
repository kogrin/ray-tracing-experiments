import stb_image/write as stbiw
import strformat
import bitops # bitwise operations
# import freeimage

let image_width_px = 256
let image_height_px = 256

echo &"P3\n{image_width_px} {image_height_px}\n255\n"

# Stuff some pixels
var data: seq[uint8] = @[]

for j in countdown(image_height_px - 1, 0):
    for i in 0 ..< image_width_px:
        # generate gradient with RGB colors 
        var r = float(i) / float(image_width_px - 1)
        var g = float(j) / float(image_height_px - 1)
        var b = 0.25

        let red   = uint8(255.999 * r)
        let green = uint8(255.999 * g)
        let blue  = uint8(255.999 * b)

        var colorRGB = red;
        colorRGB     = rotateLeftBits(colorRGB, 8) + green;
        colorRGB     = rotateLeftBits(colorRGB, 8) + blue;
        
        # OMG! finally...
        echo &"{red} {green} {blue}"
        data.add(colorRGB)

# save it
# stbiw.writeBMP("OMG.png", image_width_px, image_height_px, 3, data)
