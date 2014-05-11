Pack My Sprites is a program that generates sprite sheets from a
collection of images made with the GIMP.

== DESCRIPTION ==

The purpose of the software is to reduce the time required to
integrate graphic updates into the final product. In order to achieve
this goal, Pack My Sprites automates the generation of the sprites and
helps its integration in the build process of the product.

The default behavior of the program is to parse an input text file to
generate some sprite sheets and a listing of the localization of their
sprites. The input file must describe each sprite with a unique name,
the layers to show to build them, and their final size in the sprite
sheet. The output is a PNG file containing each sprite and a text file
associating the position and the size of each sprite with its
name. See INPUT FORMAT for details about the syntax of the input file.

When invoked with the --no-spritepos argument, only the PNG file is
generated.

When invoked with the --plist argument, a property list XML file is
generated for each sprite sheet described in each input file. The
generated file can be immediately used in Cocos2d projects.

When invoked with the --makefile argument, the program generates a
makefile whose rules build the sprite sheets, with dependencies on the
XCF files and the input text file. No sprite sheet will be generated
when using this option.

The software relies upon the gimp-console(1) programs for the
generation of the sprite sheets.

== LICENSE ==

Pack My Sprites is written by Julien Jorge <julien.jorge@gamned.org>.
It is hereby licenced under the terms of the version 3 of the GNU
General Public Licence. See the file COPYING for details.

== INPUT FORMAT ==

The input files can contain one or several sprite sheets. Each one
begins with the sprite_sheet keyword, followed by the name of the
sprite sheet and the maximum size of the output image, according to
the following syntax:

  sprite_sheet "name" width x height [margin M] [order "O"]

The ``margin M'' optional argument allows to define the spacing
between the sprites in the output. The default value is 1.

The ``order "O"'' optional argument allows to force a given heuristic
for the placement of the sprites in the output. The default value is
"area", which considers the sprites in decreasing order of their
area. Other possible values are "height" to select the sprites by
decreasing order of their height or "none" to consider the sprites in
their declaration order.

Following the header is a list of XCF files used to build the sprites:

  image_name_1 "file_name_1"
  image_name_2 "file_name_2"
  ...

Then come the sprites, defined by a name, a size, a source image, a
selection of layers and an optional mask. The selec tion of layers can
be prefixed by some properties to apply to the selected layers:

  "name" reference_box * scale_factor with image_name
    [layer_properties] layer_selection_1
    [layer_properties] layer_selection_2
    ...
    [mask "layer_name"]
    ;

The reference box can be the name of a layer from the source image, or
one of the following special values:

  - autosize
      The box is the smallest one that contains all the selected layers.

  - image_size
      The box is the one of the canvas in the input image.

The scale factor is a positive value that determine the size of the
sprite in the output. For example, a value of 0.5 will produce a
sprite half the size of the reference box.

The layer selection has different formats:

  - ``layer_name''
      Selects the layer with the given name in the source image.

  - glob ``pattern''
      Selects all layers whose name match the given pattern. The
      pattern can contain the following special charaters:

        - *  Matches any sequence of symbols, empty sequences included.

        - .  Matches a single symbol.

        - ?  Matches zero or one symbol.

  - exclude ``layer_name''
      Forces the layer with the given name in the source image not to
      be selected. This is useful to remove a layer from a glob
      expression.

  - exclude glob ``pattern''
      Forces the layers whose name match the given pattern not to be
      selected.

The layer name and the glob syntax can be prefixed by a list of
properties to change how the layers are used in the image. The current
only property is ``hollow''; it tells the program not to consider the
size of the layer when computing the size of the sprite. The
properties are optional and must be separated by spaces and put
between brackets, according to the following syntax:

  [ property_1 property_2 ... ]

The optional mask command tells to use the layer with the given name
as a mask in the resulting sprite. The white parts of the layer will
be opaque in the resulting sprite, while the black parts will be fully
transparent. Intermediate grey values will be partially transparent.

== EXAMPLE ==

See the example/ folder for a sample script. You can process it with
the following command:

  ./bin/pack-my-sprites --scheme-directory=src/script/ example/bird.spritedesc
