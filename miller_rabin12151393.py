"""
miller-rabin prime test

Test if n is prime with error probability less than 2^(βπ ).

"""

import random
from exponentiation import exp

Prime = 0
Composite = 1


def miller_rabin(n, s):
    if n == 2:
        return Prime
    elif n % 2 == 0:
        return Composite

    for _ in range(s):
        a = random.randint(1, n-1)
        if test(a, n) == True:
            return Composite

    return Prime


def int_to_bin(num):
    return list(bin(num))[2:]
    #index 2λΆν° λ¬Έμμ΄λ―λ‘ ννκ° 0b010101110 μ΄λ°μμΌλ‘ λ€μ΄μμ κ·Έλμ 2


def test(a, n):
    s=n-1
    t = 0
    while(s%2==0): #2λ‘ λμ΄μ λλμ΄μ§μ§ μμ λκΉμ§
        s //= 2
        t+=1
    u=s # a=4 u=5 t=4
    x=exp(a,u,n)

    for i in range(t): #NSR Test
        xx=exp(x,2,n)
        if(xx==1):
            if(exp(x,1,n)!=1 and exp(x,1,n) !=n-1):
                return True # νμ€ν ν©μ±μ
        x=xx
    if(xx!=1): # Fermat Test
        return True
    return False

if __name__ == "__main__":

    primes = [
        39402006196394479212279040100143613805079739270465446667948293404245721771496870329047266088258938001861606973112319,
        6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151,
        443372888629441, 561
    ]

    for p in primes:
        result = miller_rabin(p, 20)
        if result == Prime:
            print("Prime")
        elif result == Composite:
            print("Composite")
        else:
            print("Undefined")
