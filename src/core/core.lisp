(def null
    (fn (x)
        (if (atom x)
            (if (eq x nil)
                t
                nil)
            nil)))

(def not null)

(def map
    (fn (f coll)
        (if (null coll)
            nil
            (cons (f (car coll)) (map f (cdr coll))))))

(def reduce
    (fn (f acc coll)
        (if (null coll)
            acc
            (reduce f (f acc (car coll)) (cdr coll)))))

(def reverse
    (fn (coll)
     (reduce (fn (acc x) (cons x acc)) nil coll)))

(def subst
    (fn (x y z)
        (if (atom z)
            (if (eq x z)
                y
                z)
            (cons (subst x y (car z))
                  (subst x y (cdr z))))))

(def equal
    (fn (x y)
        (if (atom x)
            (if (atom y)
                (if (eq x y)
                    t
                    nil)
                nil)
            (if (equal (car x) (car y))
                (if (equal (cdr x) (cdr y))
                    t
                    nil)
                nil))))

(def append
    (fn (x y)
        (if (null x)
            y
            (cons (car x)
                  (append (cdr x) y)))))

(def range
    (fn (n m)
        (if (eq n m)
            (cons m nil)
            (cons n (range (+ n 1) m)))))

(def if-not
    (macro (test e1 e2)
           (cons (quote if)
                 (cons test
                       (cons e2
                             (cons e1 nil))))))
(def quote-list
    (fn (m)
        (if (null m)
            nil
            (cons (cons (quote quote)
                        (cons (car m) nil))
                  (quote-list (cdr m))))))

(def apply
    (fn (f args)
        (eval (cons f (quote-list args)))))




