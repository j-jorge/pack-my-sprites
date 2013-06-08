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

; \brief Displays the items of a list on a single line, separated by spaces..
; \param l The list to display
;
(define display-list
  (lambda (l)
    (unless (null? l)
            (display (car l))
            (unless (null? (cdr l))
                    (display " ")
                    (display-list (cdr l))
                    ) ; unless
            ) ; unless
    ) ; lambda
  ) ; define display-list

; \brief output a line of information.
; \param l The list of information to display.
;
(define output-line
  (lambda (l)
    (display "|- ")
    (display-list l)
    (display "\n")
    ) ; lambda
  ) ; define output-line

; \brief Display some informations about a given image.
; \param image The image.
;
; This function displays, on a single line, the version of GIMP, followed by the
; dimensions of the image.
;
(define print-global-info
  (lambda (image)

    (output-line (list (car (gimp-version))
                       (car (gimp-image-width image))
                       (car (gimp-image-height image)) ) )
    ) ; lambda
  ) ; define print-global-info

; \brief Displayes some information about a given layer.
; \param layer The layer.
; 
; The function displays a line containing the dimensions of the layer, followed
; by its offset and its name.
;
(define print-layer-info
  (lambda (layer)
    (let ( (offset (gimp-drawable-offsets layer)) )
      (output-line (list (car (gimp-drawable-width layer))
                         (car (gimp-drawable-height layer))
                         (car offset)
                         (cadr offset)
                         (car (gimp-drawable-get-name layer)) ) )
      ) ; let
    ) ; lambda
  ) ; define

; \brief Calls print-layers-info for a given array of layers.
; \param l The layer array.
;
(define print-layer-array-info
  (lambda (l)
    (print-layers-info 0 (car l) (cadr l))
    ) ; lambda
  ) ; define

; \brief Calls print-layer-info for each layer of a given array starting from a
;        given index.
; \param i The index of the layer from which we start the calls.
; \param count The number of layers in the array.
; \param layers The layers.
;
(define print-layers-info
  (lambda (i count layers)
    (unless (= i count)
            (let ( (layer (aref layers i)) )
              (print-layer-info layer)
              (print-layers-info (+ i 1) count layers)

              (if (= TRUE (car (gimp-item-is-group layer)))
                  (print-layer-array-info (gimp-item-get-children layer))
                  ) ; if
              ) ; let
            ) ; unless
    ) ; lambda
  ) ; define

; Displays some information about a given file.
;
; \param file_name The path to the file to load.
;
(define xcfinfo
  (lambda (file_name)
    (display "xcfinfo for ")
    (display file_name)
    (display "\n")

    (let ( (img (car (gimp-file-load 1 file_name file_name) ) ) )
      (print-global-info img)
      (print-layer-array-info (gimp-image-get-layers img))
      ) ; let
    ) ; lambda
  ) ; define
