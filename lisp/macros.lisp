
(defmacro (or . exprs)
  (if exprs
	  (let ((gval (gensym)))
      `(let ((,gval ,(car exprs)))
         (if ,gval 
             ,gval
             (or @(cdr exprs)))))))

(defmacro (%and . exprs)
	(if (cdr exprs)
		`(if ,(car exprs)
			 (and @(cdr exprs)))
		(car exprs)))

(defmacro (and . exprs)
	(if exprs
		`(%and @exprs)
		t))

(defmacro (when test . body)
  `(if ,test
	   (begin @body)))

(defmacro (unless test . body)
  `(if (not ,test)
	   (begin @body)))

;; (cond (test expr) ...)
(defmacro (cond . clauses)
	(if clauses
		(let ((clause (car clauses)))
		  `(if ,(car clause)
			   ,(cadr clause)
			   (cond @(cdr clauses))))))

(defmacro (push val place)
	`(set! ,place (cons ,val ,place)))

(defmacro (pop val place)
	(let ((gx (gensym)))
	  `(let ((,gx (car ,place)))
		 (set! ,place (cdr ,place))
		 ,gx)))
