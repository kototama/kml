(def x 5)
(def add-five (fn (y) (+ x y)))
(print (quote (Expected 15)))
(print (add-five 10))

(def adder (fn (n) (fn (x) (+ x n))))
(def add-six (adder 6))
(print (quote (Expected 26)))
(print (add-six 20))

