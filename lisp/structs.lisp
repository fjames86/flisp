
(define *struct-definitions* nil)

(defmacro (defstruct name . slots)
	`(push (cons ',name ',slots) *struct-definitions*))

(define (make-instance struct-type)
	(let ((s (assoc struct-type *struct-definitions*)))
	  (if s
		  (make-array (length (cdr s)))
		  (error "No such struct" "MAKE-INSTANCE"))))

(define (list-position list val)
	(cond
	  ((null list)
	   (error "List does not have that item" "LIST-POSITION"))
	  ((eql? (car list) val) 0)
	  (t (1+ (list-position (cdr list) val)))))


(define (struct-slot struct slot)
	(let ((s (assoc struct *struct-definitions*)))
	   (if s
		   (aref struct (list-position (cdr s) slot))
		   (error "Struct does not have that slot" "STRUCT-SLOT"))))

(define (set-struct-slot! struct slot val)
	(let ((s (assoc struct *struct-definitions*)))
	  (if s
		  (set-aref! struct (list-position (cdr s) slot))
		  (error "Struct does not have that slot" "SET-STRUCT-SLOT!"))))

