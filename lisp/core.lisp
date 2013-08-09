
;; basic stuff ....
(define (identity x) x)

(define (not x)
	(if x nil t))

(define null not)

(define (member obj list)
  (if list
      (let ((val (eql? obj (car list))))
        (if val
            val
            (member obj (cdr list))))))

(define (number? x)
    (let ((type (type-of x)))
      (member type '(int double))))

(define (string? x)
    (eq? (type-of x) 'string))

(define (symbol? x)
    (eq? (type-of x) 'symbol))




