
;; some functions 
(define (1+ x) (+ x 1))

(define (sq x) (* x x))

;; list accessing functions 
(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))
(define (caaar x) (car (caar x)))
(define (caadr x) (car (cadr x)))
(define (cadar x) (car (cdar x)))
(define (caddr x) (car (cddr x)))
(define (cdaar x) (cdr (caar x)))
(define (cdadr x) (cdr (cadr x)))
(define (cddar x) (cdr (cdar x)))
(define (cdddr x) (cdr (cddr x)))

;; mapping functions 
(define (mapcar proc list)
	(if list
		(cons (proc (car list))
			  (mapcar proc (cdr list)))))

(define (1- x) (- x 1))

(define (mapcdr proc list)
	(if list
            (cons (proc list)
		   (mapcdr proc (cdr list)))))


(define (identity x) x)


