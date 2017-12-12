**Pack My Sprites** is a collection of command line tools for creating
sprite sheets.

The purpose of these tools is to reduce the time required to integrate
graphic updates into the final product. In order to achieve this goal,
Pack My Sprites automates the generation of the sprites and the
atlases in an way that can be easily integrated in the build process
of the product.

There are currently two tools to generate a sprite sheet. The first
one is `auto-pack-my-sprites`. As simple as it can be, it takes a list
of images as its arguments and produces a sprite sheet with all of
them.

The second tool is `pack-my-sprites`; which allows to generate the
sprites by picking specified layers from an XCF image made with the
[GIMP](http://www.gimp.org/) then to place these sprites in a sprite
sheet.

Both tools support the creation of sprite atlases in CSS and Zwoptex
formats.

# License

Pack My Sprites is written by Julien Jorge <julien.jorge@stuff-o-matic.com>.
It is hereby licensed under the terms of the version 3 of the GNU Affero
General Public Licence. See the file COPYING for details.

# Using `auto-pack-my-sprites`

Creating a sprite sheet with `auto-pack-my-sprites` cannot be simpler:

```sh
$ auto-pack-my-sprites sprites/*.png
```

This command will create a file named `sprite-sheet.png` with all the
PNG images from the `sprites` folder. The sprite sheet is limited to
1024 pixels both in width and height. If all the sprites cannot fit
in the same sheet then a series of numbered output files will be
created: `sprite-sheet-1.png`, `sprite-sheet-2.png`, and so on. The
name of the output can be modified with the `--output` argument.

The sprites will be packed by leaving a margin of one pixel between
them. The width of the margin can be modified with the `--margin M`
argument, which sets the margin to `M` pixels.

Some sprites need to have their edges cloned around them into the
sprite sheet to avoid visual artifacts. This can be accomplished by
preceeding their names with the `--bleeding` argument:

```sh
$ auto-pack-my-sprites dry/*.png --bleeding bleed/*.png
```

The size of the output file can be controled with the `--size WxH`
argument to set the maximum width to `W` pixels and the maximum height
to `H` pixels.

By default the name of the sprites in the generated atlas will be the
name passed on the command line, path included. This behavior is
modified by the `--strip-paths` argument, which removes any folder from
the name of the sprites, and `--strip-extensions`, which removes the
extension from the name of the sprite..

All arguments of `pack-my-sprites` are supported by
`auto-pack-my-sprites`. They are listed in the following section.

# Using `pack-my-sprites`

The default behavior of `pack-my-sprites` is to generate the sprite
sheets from an input text file describing how to build the sprites.

The input file contains the name and the maximum size of the sprite
sheet, followed by a description of each sprite with a unique name,
the layers to use to build them, and the size to scale them to in the
the sheet. See [Input Format](#input-format) for details about the
syntax of the input file.

The output is a PNG file containing each sprite. If all the sprites
cannot fit in the same sheet then a series of numbered output files
will be created by appending the string `-N` to the provided output
name, where `N` ranges from 1 to the number of files required to pack
all the sprites.

The sprites may be rotated a quarter turn anticlockwise if it helps to
produce a better packing. This behavior can be prevented with the
`--no-rotation` argument, in which case no rotation will occur.

Transparent borders are automatically cropped out of the sprites if
the selected atlas format support it. This behavior can be prevented
with the `--no-crop` argument, in which case no cropping will occur.

When invoked with the `--format` argument, an atlas of the selected
type is also generated to describe the position and the size of the
sprites in the image. Valid values are:

* `css`, a css file with the same name than the sprite sheet will be
  created, containing a class for each sprite.

* `plist`, a property list XML file is generated for the sprite
  sheet. The generated file can be immediately used in
  [Cocos2d-X](http://www.cocos2d-x.org/) projects.

* `spine`, an atlas is generated as if it was created with
  [Spine](http://esotericsoftware.com/).
  
* `spritepos`, a plain text file with the same name than the sprite
  sheet will be created, containing a listing of all the sprites.

The software processes PNG, JPG, TGA, BMP, PCX, XBM and non-animated
GIF files internally. For other input formats, it relies upon the
`gimp-console` program for the generation of the sprite sheets. The
location of this program can be set with the `--gimp-console path`
argument.

## Input Format

The input file begins with the `sprite_sheet` keyword, followed by the
name of the sprite sheet and the maximum size of the output image,
according to the following syntax:

    sprite_sheet "name" width x height [margin M]

The `margin M` optional argument allows to define the spacing
between the sprites in the output. The default value is 1.

Following the header is a list of image files used to build the sprites:

    image_name_1 "file_name_1"
    image_name_2 "file_name_2"
    ...

Then come the sprites, defined by a name, a size, a source image, a
selection of layers and an optional mask. The name of the sprite and
the selection of layers can be prefixed by some properties to modify
how they are considered in the sprite sheet:

    [sprite_properties] "name" reference_box * scale_factor with image_name
      [layer_properties] layer_selection_1
      [layer_properties] layer_selection_2
      ...
      [mask "layer_name"]
      ;

The current single property that can be applied to the whole sprite is
`bleed`; it tells the program to duplicate the edges of the resulting
sprite outside its bounds (i.e. the topmost pixel row is duplicated
above the sprite, the rightmost pixel column on its right and so
on). The properties are optional and must be separated by spaces and
put between brackets, according to the following syntax:

    [ property_1 property_2 ... ]

The reference box can be the name of a layer from the source image, or
one of the following special values:

  - `autosize`
      The box is the smallest one containing all the selected layers.

  - `image_size`
      The box is the one of the canvas in the input image.

The scale factor is a positive value determining the size of the
sprite in the output. For example, a value of 0.5 will produce a
sprite half the size of the reference box.

The layer selection has different formats:

  - `"layer_name"`
      Selects the layer with the given name in the source image.

  - `glob "pattern"`
      Selects all layers whose name match the given pattern. The
      pattern can contain the following special characters:

    - `*`  Matches any sequence of symbols, empty sequences included.

    - `.`  Matches a single symbol.

    - `?`  Matches zero or one symbol.

  - `exclude "layer_name"`
      Forces the layer with the given name in the source image not to
      be selected. This is useful to remove a layer from a glob
      expression.

  - `exclude glob "pattern"`
      Forces the layers whose name match the given pattern not to be
      selected.

The layer name and the glob syntax can be prefixed by a list of
properties to change how the layers are used in the image. The current
only property is `hollow`; it tells the program not to consider the
size of the layer when computing the size of the sprite. The
properties are optional and must be separated by spaces and put
between brackets, according to the following syntax:

    [ property_1 property_2 ... ]

The optional `mask` command tells to use the layer with the given name
as a mask in the resulting sprite. The white parts of the layer will
be opaque in the resulting sprite, while the black parts will be fully
transparent. Intermediate grey values will be partially transparent.

# Examples

See the `example/` folder for sample projects.
