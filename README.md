# Pollard's rho algorithm for DLP

Project contatins the following libraries

**impl/long_arithmetic**        
 - Implementation of long integer arithmetic using GMP

**impl/extended_euclidean**     
 - Extended Euclidean Algorithm for finding solution of ax + by = gcd(a, b) 
 - Implementation of finding solution of an equation A = Bx (mod n) using extended euclidean algorithm

**impl/elliptic_curve**         
 - Implementation of finite field and elliptic curve operations

**impl/discrete_logarithm** 
 - Pollard's rho method for finding discrete logarithm 
 - Tonnelli-Shanks algorithm for finding square root of A modulo prime number 

**test/main.cpp**
- main test file contains three checkes of finding DL in elliptic curve group with following parametrs

$p = 1099511627791, a = 490064540513, b = 170079681745, q = 1099513257113$

$p = 281474976710677, a = 187997080572537, b = 198915293914922, q = 281474987479363$

$p = 72057594037928017, a = 15222514519776677, b = 7110318376978981, q = 72057594089783747$

$p = \vert \mathbb{F}_p \vert, q = \vert E(\mathbb{F}_p) \vert, p, q$ -- prime numbers.
Elliptic curve equation: 
$$
    y^2 = x^3 + ax + b.
$$



