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
; dimensions of the image and the number of layers.
;
(define print-global-info
  (lambda (image)

    (output-line (list (car (gimp-version))
                       (car (gimp-drawable-width image))
                       (car (gimp-drawable-height image))
                       (car (gimp-image-get-layers image)) ) )
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

; \brief Calls print-layer-info for each layer of a given array starting from a
;        given index.
; \param i The index of the layer from which we start the calls.
; \param count The number of layers in the array.
; \param layers The layers.
;
(define print-layers-info
  (lambda (i count layers)
    (unless (= i count)
            (print-layer-info (aref layers i))
            (print-layers-info (+ i 1) count layers)
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

      (let ( (l (gimp-image-get-layers img) ) )

        (print-layers-info 0 (car l) (cadr l))
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define
