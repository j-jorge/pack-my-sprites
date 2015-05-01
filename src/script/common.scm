;
; This file is part of Pack My Sprites.
;
; Pack My Sprites is free software: you can redistribute it and/or modify it
; under the terms of the GNU General Public License as published by
; the Free Software Foundation, version 3 of the License.
;
; Pack My Sprites is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with Pack My Sprites.  If not, see <http://www.gnu.org/licenses/>.
;

; function telling if a value is in a list
(define in-list
  (lambda (l e)
    (if (null? l)
        #f
        (if (= (car l) e)
            #t
            (in-list (cdr l) e)
            )
        )
    )
  )

; function merging two lists
(define (merge l1 l2)
  (if (null? l1)
      l2
      (if (null? l2)
          l1
          (cons (car l1)
                (cons (car l2)
                      (merge (cdr l1) (cdr l2))
                      )
                )
          )
      )
  ) ; define

; function to modify the position of a layer
(define set-layer-position
  (lambda (img layer x y)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let ( (layer_i (aref (cadr all_layers) layer)) )
        (gimp-layer-set-offsets layer_i x y)
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; function returning a list of items corresponding to a list of names
(define get-items-by-name
  (lambda (img items)
    (if (null? items)
        '()
        ( cons (car (gimp-image-get-layer-by-name img (car items) ))
               (get-items-by-name img (cdr items)) )
        ) ; if
    ) ; lambda
  ) ; define

; function returning the positions of a list of items and their parents.
(define get-item-hierarchy-positions
  (lambda (img items)
    (if (null? items)
        '()
        (cons (car (gimp-image-get-item-position img (car items)) )
              (let ( (parent (gimp-item-get-parent (car items))) )
                (if (= -1 (car parent))
                    ( get-item-hierarchy-positions img (cdr items) )
                    ( get-item-hierarchy-positions img
                                                   (merge (cdr items) parent) )
                    ) ; if
                ) ; let
              )
        ) ; if
    ) ; lambda
  ) ; define

; function returning a list of positions corresponding to a list of item names
(define get-item-positions
  (lambda (img items)
    (if (null? items)
        '()
        (cons (car (gimp-image-get-item-position img (car items)) )
              (get-item-positions img (cdr items) )
              )
        ) ; if
    ) ; lambda
  ) ; define

; function showing a set of layers. This function turns the groups visible if a
; provided layer is in a group.
(define show-branch
  (lambda (layers)
    (unless (null? layers)
            (let ( (item (car layers)) )
              (show-branch (cdr layers))
              (gimp-item-set-visible item TRUE)

              (let ( (parent (gimp-item-get-parent item)) )
                (if (<> -1 (car parent))
                    (show-branch (gimp-item-get-parent item))
                    ) ; if
                ) ; if
              ) ; let
            ) ; unless
    ) ; lambda
  ) ; show-branch

; function turning off the visibility of a set of items and their chidren.
; the argument is a list whose first item is the number of items and the second
; one is an array of items to hide.
(define hide-branch
  (lambda (layers)
    (let ( (count (car layers))
           (layer_array (cadr layers))
           )
      (let loop ((i 0))
        (unless (= i count)
                (let ( (item (aref layer_array i)) )
                  (if ( = TRUE (car (gimp-item-is-group item)) )
                      ( hide-branch (gimp-item-get-children item) )
                      ) ; if

                  ( gimp-item-set-visible item FALSE )

                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; hide-branch

; function showing a the children of a group
; \param layers the layers to show. For the layer groups all children are shown.
(define show-groups
  (lambda (layers)
    (unless (null? layers)
            (let ( (item (car layers)) )
              (show-groups (cdr layers))
              (gimp-item-set-visible item TRUE)

              (if ( = TRUE (car (gimp-item-is-group item)) )
                  ( show-groups
                    ( vector->list (cadr (gimp-item-get-children item)) ) )
                  ) ; if
              ) ; let
            ) ; unless
    ) ; lambda
  ) ; define

; function showing a set of layer, and hide the others
(define show-layers
  (lambda (img layers)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (let ( (layer_i (aref (cadr all_layers) i)) )
                  (if (in-list layers i)
                      (gimp-drawable-set-visible layer_i TRUE)
                      (gimp-drawable-set-visible layer_i FALSE))
                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; function turning off the visibility of a set of items and their chidren.
; the argument is a list whose first item is the number of items and the second
; one is an array of items to hide.
(define hide-branch
  (lambda (layers)
    (let ( (count (car layers))
           (layer_array (cadr layers))
           )
      (let loop ((i 0))
        (unless (= i count)
                (let ( (item (aref layer_array i)) )
                  (if ( = TRUE (car (gimp-item-is-group item)) )
                      ( hide-branch (gimp-item-get-children item) )
                      ) ; if

                  ( gimp-item-set-visible item FALSE )

                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; function showing all layers
(define show-all-layers
  (lambda (img)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (let ( (layer_i (aref (cadr all_layers) i)) )
                  (gimp-drawable-set-visible layer_i TRUE)
                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; function to shift the position of a layer and loop its content
(define priv-paste-shift-layer
  (lambda (layer x y w h)
    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (gimp-layer-set-offsets layer_copy x y)
      (gimp-floating-sel-anchor layer_copy)
      ); let

    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (if (> x 0)
          (gimp-layer-set-offsets layer_copy (- x w) y)
          (gimp-layer-set-offsets layer_copy (+ w x) y)
          ) ; if
      (gimp-floating-sel-anchor layer_copy)
      ); let

    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (if (> y 0)
          (gimp-layer-set-offsets layer_copy x (- y h))
          (gimp-layer-set-offsets layer_copy x (+ h y))
          ) ; if
      (gimp-floating-sel-anchor layer_copy)
      ); let

    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (if (> x 0)
          (if (> y 0)
              (gimp-layer-set-offsets layer_copy (- x w) (- y h))
              (gimp-layer-set-offsets layer_copy (- x w) (+ h y))
              ) ; if
          (if (> y 0)
              (gimp-layer-set-offsets layer_copy (+ w x) (- y h))
              (gimp-layer-set-offsets layer_copy (+ w x) (+ h y))
              ) ; if
          ) ; if
      (gimp-floating-sel-anchor layer_copy)
      ) ; let
    ) ; lambda
  ) ; define priv-paste-shift-layer

; function to shift the position of a list of layers and loop their content
(define shift-layers
  (lambda (img layers x y)
    (show-layers img layers)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (if (in-list layers i)
                    (let ( (layer_i (aref (cadr all_layers) i)) )
                      (let ( (w (car (gimp-drawable-width layer_i)))
                             (h (car (gimp-drawable-height layer_i)))
                             )
                        (gimp-edit-copy layer_i)
                        (gimp-edit-clear layer_i)
                        
                        (priv-paste-shift-layer layer_i x y w h)
                        ) ; let
                      ) ; let
                    ) ; if
                (loop (+ i 1))
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define shift-layers

; copy the currently visible data and paste it as the mask of the active layer
(define apply-visible-to-layer-mask
  (lambda (src dest)
    (gimp-edit-copy-visible src)

    (let ( (the_layer (car (gimp-image-get-active-layer dest))) )
      ( gimp-layer-add-mask
        the_layer
        (car (gimp-layer-create-mask the_layer 0)) )
      (gimp-layer-set-apply-mask the_layer TRUE)
      (gimp-layer-set-edit-mask the_layer TRUE)
      (gimp-edit-paste (car (gimp-layer-get-mask the_layer)) 1)
      (gimp-floating-sel-anchor (car (gimp-image-get-active-layer dest)))
      (gimp-layer-set-edit-mask the_layer FALSE)
      (gimp-layer-set-show-mask the_layer FALSE)
      ) ; let
    ) ; lambda
  ) ; define

; use a layer of the source image as a mask for a layer of the destination
(define set-layer-mask
  (lambda (src mask dest)
    (if (null? mask)
        ()
        (begin
          (show-layers src mask)
          (apply-visible-to-layer-mask src dest)
          ) ; begin
        ) ; if
    ) ; lambda
  ) ; define

; use a layer of the source image as a mask for a layer of the destination
(define set-layer-mask-with-groups
  (lambda (src mask dest)
    (if (null? mask)
        ()
        (begin
          (hide-branch (gimp-image-get-layers src))
          (show-branch (get-items-by-name src mask))
          (show-groups (get-items-by-name src mask))

          (apply-visible-to-layer-mask src dest)
          ) ; begin
        ) ; if
    ) ; lambda
  ) ; define

; change the opacity of a layer
(define set-layer-opacity
  (lambda (img layers opacity)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (let ( (layer_i (aref (cadr all_layers) i)) )
                  (if (in-list layers i)
                      (gimp-layer-set-opacity layer_i opacity))
                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; create the scaled frame of an animation with the current visible layers
(define create-layer-from-image
  (lambda (img x y w h the_image)
    (let ( (frame (car (begin
                         (gimp-edit-copy-visible img)
                         (gimp-edit-paste-as-new)
                         ) ; begin
                       ) ; car
                  ) ) ; frame
                                        ; create the resulting merged layer
      (let ( (the_layer (car
                         (gimp-layer-new-from-drawable
                          (car (gimp-image-get-active-drawable frame))
                          the_image
                          ) ; gimp-layer-new-from-drawable
                         ) ; car
                        ) ) ; the_layer
                                        ; resize the layer
        (gimp-image-add-layer the_image the_layer -1)
        (gimp-layer-scale-full the_layer w h TRUE INTERPOLATION-CUBIC)
        (gimp-layer-set-offsets the_layer x y)

	the_layer
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; create the scaled frame of an animation
(define create-layer
  (lambda (img layers x y w h the_image)
    ; select the visible layers
    (show-layers img layers)
    (create-layer-from-image img x y w h the_image)
    ) ; lambda
  ) ; define

; create the scaled sprite of an item
(define create-layer-crop-with-groups
  (lambda (img layers sx sy sw sh x y w h rotate the_image mask)
    ; select the visible layers
    (hide-branch (gimp-image-get-layers img))
    (show-branch (get-items-by-name img layers))
    (show-groups (get-items-by-name img layers))

    ( create-layer-crop-current-with-groups
      img sx sy sw sh x y w h rotate the_image mask )
    ) ; lambda
  ) ; define

; create the scaled sprite of an item
(define create-layer-crop
  (lambda (img layers sx sy sw sh x y w h rotate the_image mask)
    ; select the visible layers
    (show-layers img layers)
    (create-layer-crop-current img sx sy sw sh x y w h rotate the_image mask)
    ) ; lambda
  ) ; define

; create the scaled sprite of an item with the currently visible layers
(define crop-current-layer
  (lambda (frame sx sy sw sh x y w h rotate the_image)

    ; get the sub part of the image
    (gimp-image-crop frame sw sh sx sy)

    ; create the resulting merged layer
    (let ( (the_layer (car
                       (gimp-layer-new-from-drawable
                        (car (gimp-image-get-active-drawable frame))
                        the_image
                        ) ; gimp-layer-new-from-drawable
                       ) ; car
                      ) ) ; the_layer

      (gimp-image-add-layer the_image the_layer -1)

      ; resize the layer
      (gimp-layer-scale-full the_layer w h TRUE INTERPOLATION-CUBIC)

      (if (= 1 rotate)
          (gimp-item-transform-rotate-simple the_layer ROTATE-90 TRUE 0 0)
          ) ; if

      (gimp-layer-set-offsets the_layer x y)

      the_layer
      ) ; let
    ) ; lambda
  ) ; define

; create the scaled sprite of an item with the currently visible layers
(define create-layer-crop-current
  (lambda (img sx sy sw sh x y w h rotate the_image mask)

    (let ( (frame (car (begin
                         (gimp-edit-copy-visible img)
                         (gimp-edit-paste-as-new)
                         ) ; begin
                       ) ; car
                  ) ) ; frame

      ; turn the mask on
      (set-layer-mask img mask frame)

      (crop-current-layer frame sx sy sw sh x y w h rotate the_image)

      ) ; let
    ) ; lambda
  ) ; define

; create the scaled sprite of an item with the currently visible layers
(define create-layer-crop-current-with-groups
  (lambda (img sx sy sw sh x y w h rotate the_image mask)

    (let ( (frame (car (begin
                         (gimp-edit-copy-visible img)
                         (gimp-edit-paste-as-new)
                         ) ; begin
                       ) ; car
                  ) ) ; frame

      ; turn the mask on
      (set-layer-mask-with-groups img mask frame)

      (crop-current-layer frame sx sy sw sh x y w h rotate the_image)

      ) ; let
    ) ; lambda
  ) ; define

; Create a new image
(define new-image
  (lambda (width height)
    (let ( (dest_image (car (gimp-image-new width height RGB)))
           )

      (let ( (bk (car (gimp-layer-new dest_image width height
                                      1 "background" 100 0)))
             )
        (gimp-image-add-layer dest_image bk 0)
        )

      dest_image
      ) ; let
    ) ; lambda
  ) ; new-image

; Create a new image
(define new-image-max
  (lambda (width height)
    (let ( (dest_image (car (gimp-image-new width height RGB)))
           )
      dest_image
      ) ; let
    ) ; lambda
  ) ; new-image-max

; Save the resulting image
(define save-frames
  (lambda (name img)
    (gimp-image-merge-visible-layers img 1)
    (gimp-file-save 1 img (car (gimp-image-get-active-drawable img)) name name)
    ) ; lambda
  ) ; define

; Save the resulting image then exit the Gimp
(define save-frames-and-exit
  (lambda (name img)
    (save-frames name img)
    (gimp-quit 1)
    ) ; lambda
  ) ; define

