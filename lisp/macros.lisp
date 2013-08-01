
(defmacro (or . exprs)
  (if exprs
      `(let ((val ,(car exprs)))
         (if val 
             val
             (or @(cdr exprs))))))

(defmacro (%and . exprs)
	(if (cdr exprs)
		`(if ,(car exprs)
			 (and @(cdr exprs)))
		(car exprs)))

(defmacro (and . exprs)
	(if exprs
		`(%and @exprs)
		t))

