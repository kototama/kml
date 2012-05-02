KML - KISS My Lisp
=====================

WHAT?
-----

KML is an implementation in pure C99 of a minimal Lisp language.
Implemented for 32-bits systems it supports symbols, integers, a few primitives, anonymous functions,
closures, macros and garbage collection.
The implementation follows the [KISS](http://en.wikipedia.org/wiki/KISS_principle) principles
and is around 1400 lines of code.

WHY?
----

I wanted to better understand the foundations of Lisp by reading the [original McCarthy's Lisp paper](http://www-formal.stanford.edu/jmc/recursive.html)
before diving into something else (Haskell? [Kernel](http://axisofeval.blogspot.de/2011/08/praising-kernel.html)? [IO](http://www.iolanguage.com/)? Prolog?).

HOW?
----

The Lisp eval function is implemented in C. This is enough to have a full working Lisp.
Another approach would be to implement a subset of Lisp in C and recodes a second eval function for
a full Lisp with this subset.

The functions and special forms implemented by eval are:
cons, quote, car, cdr, atom, eq, def, fn, +, -, *, print, macro, eval and if.

NOTES
-----

If you have simple bug fixes you can send them but for more complex things I'm not interested
in maintaining this project. Just fork it!

Some errors checking is done but not everywhere.


THINGS I LEARNED
----------------

- LIST stands not for LISt Processing but for LISt Processor.
- Programming in C is as painful as it used to be. Clang makes its slighly better.
- It's hard to abstract patterns of code in C. You feel the pain, even on a small project like this.
- apply can be implemented as a normal function on top of eval.
- the numbers of primitives necessary to bootstrap a Lisp is not a magic property of
Lisp but depends on how the eval function is implemented.
- the fancy Lisp eval function that you see in papers or books is not enough to have a Lisp,
it lacks error checking!

TODO
----
- garbage collection has probably bugs
- better roots encoding for eval
- quote as '
- string literals and export read function
- parse lisp comments
- variadic arguments for functions and macros
- (list) function
- double/float numbers
- free allocated memory for tokens, errors and so on.
- ((funcreator) args) is not implemented [but ((fn (x) ...) args) is]
- bug: (equal (cons (quote (1 2)) 3) (quote (1 2 3)))

LICENSE
-------

GPL v3. Get in touch if you need another license.

CONTACT
-------
Email: kototamo gmail com

EXAMPLES
--------
    
    (def range
        (fn (n m)
            (if (eq n m)
                (cons m nil)
                (cons n (range (+ n 1) m)))))
    
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

    (def if-not
        (macro (test e1 e2)
               (cons (quote if)
                     (cons test
                           (cons e2
                                 (cons e1 nil))))))
