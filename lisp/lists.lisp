
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

(define (maplist proc list)
	(if list
            (cons (proc list)
		   (maplist proc (cdr list)))))

(define (nth n list)
    (cond 
      ((< n 0) nil)
      ((null list) nil)
      (t
       (if (= n 0)
           (car list)
           (nth (1- n) (cdr list))))))


