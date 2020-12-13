
clear all

x0 = [0 1 0];

t1 = [1 0 0];
t2 = [0 1 0];
t3 = [0 0 1];

A = [1 1 -1;-1 -1 1;0 -1 1];

syms p1 p2 p3
X = [p1 p2 p3]

Ba = dotpetree(x0,A,7,'l1q1a.dot');
Bf = dotpetree([0 0 1],A,7,'l1q1f.dot');